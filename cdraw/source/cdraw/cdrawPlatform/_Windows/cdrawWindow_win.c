/*
* Copyright 2023 Daniel S. Buckstein
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/

/*
* cdrawConsole_win.c
* Windows implementation of window management.
*/

#include "cdraw/cdrawPlatform/cdrawWindow.h"
#if CDRAW_TARGET_WINDOWS
#include <Windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include "cdraw/cdrawPlatform/cdrawInput.h"


/******************************************************************************
* Private/internal data structures and functions.
******************************************************************************/

/// <summary>
/// Alternative keycodes.
/// </summary>
static const label_long_t gKeyAlt = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0 1\'3457\"908=<_>\?)!@#$%^&*(;:,+./2abcdefghijklmnopqrstuvwxyz]|[6-~ABCDEFGHIJKLMNOPQRSTUVWXYZ}\\{`";


/// <summary>
/// Internal data for Windows platform.
/// </summary>
typedef struct cdrawWindow_win
{
	struct {
		HINSTANCE hInst;			// Internal instance handle.
		HWND hWnd;					// Internal window handle.
		HDC hDC;					// Internal device context handle.
	};
	RECT restoreArea;				// Non-full-screen area.
	BOOL buildState;				// Hot-build tracker.
	BOOL mouseState;				// Mouse hovering tracker.
	BOOL activeState;				// Activation state.
	TRACKMOUSEEVENT mouseTrack;		// Mouse tracking for entering/exiting window.
	ptrk_t pluginOwner;				// Original owner of plugin if we have one attached externally.
	cdrawPlugin pluginDebug;		// Debug plugin instance.
	hbitflag_t keyState;			// State tracking for ASCII keys, since release is not always correct.
} cdrawWindow_win;

typedef struct cdrawWindowPlatform_win
{
	WNDCLASSEXA info;									// Window class. 
	ATOM info_reg;										// Window class result.
	label_t name;										// Window class name.
	size_t windowCount;									// Number of windows registered.

	cstrk_t dir_build, dir_target, dir_sdk, tag_cfg;	// Environment strings.
	int16_t res_control_base;							// Window control mapping base.
	int8_t res_control_id;								// Window control mapping resource.
	int8_t res_dialog;									// Dialog box resource.
} cdrawWindowPlatform_win;
static cdrawWindowPlatform_win gWindowPlatform;			// Global platform instance.


enum
{
	cdrawWinCtrlMsg = WM_USER,
	cdrawWinCtrlMsg_load,
	cdrawWinCtrlMsg_reload,
	cdrawWinCtrlMsg_unload,
	cdrawWinCtrlMsg_debug,
	cdrawWinCtrlMsg_build,
	cdrawWinCtrlMsg_rebuild,
	cdrawWinCtrlMsg_copy,
	cdrawWinCtrlMsg_cmd,
};


#define p_window										((cdrawWindow_win*)window->p_window)
#define cdraw_window_valid()							cdraw_assert(window && p_window)
#define cdraw_window_cancallback()						(window->p_plugin && p_window->pluginOwner)
#define cdraw_window_ownsplugin()						(p_window->pluginOwner == p_window)
#define cdraw_window_callback(callback,...)				\
		if (cdraw_window_cancallback())					\
			callback(window->p_plugin, p_window->pluginOwner, __VA_ARGS__)


CDRAW_INL bool cdrawWindowInternalPlatformReady_win()
{
	return (gWindowPlatform.info.hInstance != NULL);
}

CDRAW_INL bool cdrawWindowInternalPlatformHandleValid_win(HINSTANCE const inst)
{
	return (gWindowPlatform.info.hInstance == inst)
		|| (!inst && gWindowPlatform.info.hInstance == GetModuleHandle(NULL));
}

CDRAW_INL bool cdrawWindowInternalInfoReady_win()
{
	return (gWindowPlatform.info_reg != 0);
}

CDRAW_INL bool cdrawWindowInternalInfoCreate_win()
{
	if (!cdrawWindowInternalPlatformReady_win())
		return false;
	if (cdrawWindowInternalInfoReady_win())
		return true;

	gWindowPlatform.info_reg = RegisterClassExA(&gWindowPlatform.info);
	return cdrawWindowInternalInfoReady_win();
}

CDRAW_INL bool cdrawWindowInternalInfoRelease_win()
{
	if (!cdrawWindowInternalPlatformReady_win())
		return false;
	if (!cdrawWindowInternalInfoReady_win())
		return true;

	BOOL const unregistered = UnregisterClassA(gWindowPlatform.info.lpszClassName, gWindowPlatform.info.hInstance);
	if (unregistered == 0)
		return false;
	gWindowPlatform.info_reg = 0;
	return true;
}

bool cdrawWindowInternalUnlockPDB_win(label_t const sdkDirStr, label_t const cfgDirStr, label_t const projName);

#if CDRAW_DEBUG
#define cfgStr "Debug"
#else // #if CDRAW_DEBUG
#define cfgStr "Release"
#endif // #else // #if CDRAW_DEBUG
static bool cdrawWindowInternalBuild(cdrawWindow* const window, bool const rebuild)
{
	bool status = true;
	if (!p_window->buildState)
	{
		p_window->buildState = 1;
		{
			byte_t bat[256] = { 0 };
			byte_t dir[256] = { 0 };
			byte_t cmd[1024] = { 0 };
			cstrk_t const statusText[2] = { "failed", "succeeded" };
			cstrk_t const proj = ("cdrawPlugin");
			cstrk_t const cfg = (cfgStr "|x" tokenstr(CDRAW_BITWIDTH));
			cstrk_t const sw = (rebuild ? "Rebuild" : "Build");
			cstrk_t const log = ("cdraw-build.txt");

			int16_t i = -1;
			int16_t const pdbrmAttempts = 127;

			// utility path
			sprintf(bat, "%s\\_dev\\Windows\\_build", gWindowPlatform.dir_sdk);

			// project path
			sprintf(dir, "%s\\project\\VisualStudio\\vcxproj\\%s\\%s.vcxproj", gWindowPlatform.dir_sdk, proj, proj);

			// call batch: call "BATCH" "DEVENV" "PROJ" "CFG" "LOG" "SWITCH" "STOPCOPY"
			//		batch: call "DEVENV" "PROJ" /SWITCH(b/rb) "CFG(cfg|arch)" /Out "LOG"
			sprintf(cmd, "call \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ", bat, gWindowPlatform.dir_build, dir, cfg, log, sw, "YES");

			// build
			printf("\n----------------------------------------------------------------\n");
			printf("cdraw Player: Debug info removal attempt...");
			printf("\n----------------------------------------------------------------\n");
			for (i = 0; i < pdbrmAttempts && !status; ++i)
				status = cdrawWindowInternalUnlockPDB_win(gWindowPlatform.dir_sdk, gWindowPlatform.tag_cfg, proj);
			printf("  Attempt %s (%u/%u). \n", statusText[status], i, pdbrmAttempts);
			if (status && cdrawWindowInternalUnlockPDB_win(gWindowPlatform.dir_sdk, gWindowPlatform.tag_cfg, proj))
				printf("  Debug info removal confirmed. \n");
			printf("\n----------------------------------------------------------------\n");
			printf("cdraw Player: Debug info removal complete.");
			printf("\n----------------------------------------------------------------\n");
			printf("\n----------------------------------------------------------------\n");
			printf("cdraw Player: Hotbuild [%s] -> [%s]...", sw, gWindowPlatform.tag_cfg);
			printf("\n----------------------------------------------------------------\n");
			printf("  Building, please wait... \n");
			status = (system(cmd) == 0);
			printf("  Build %s. \n", statusText[status]);
			printf("\n----------------------------------------------------------------\n");
			printf("cdraw Player: Hotbuild complete.");
			printf("\n----------------------------------------------------------------\n");

			// post copy message to window
			PostMessageA(p_window->hWnd, cdrawWinCtrlMsg_copy, 0, (LPARAM)status);
		}
		p_window->buildState = 0;
	}
	return status;
}

static result_t __stdcall cdrawWindowInternalBuildThread(cdrawWindow* const window)
{
	return cdrawWindowInternalBuild(window, false);
}

static result_t __stdcall cdrawWindowInternalRebuildThread(cdrawWindow* const window)
{
	return cdrawWindowInternalBuild(window, true);
}

static bool cdrawWindowInternalCreateBuildWarning(cdrawWindow const* const window)
{
	if (p_window->buildState)
	{
		bool const hideCursor = (window->control & cdrawWindowControl_cursor_hide);

		// reveal cursor
		if (hideCursor)
			ShowCursor(TRUE);

		// warning
		MessageBoxA(p_window->hWnd, "Please wait for build operation to finish.", "cdraw Player Application: Building", (MB_OK | MB_ICONEXCLAMATION | MB_SETFOREGROUND));

		// reveal cursor
		if (hideCursor)
			ShowCursor(FALSE);

		// done
		return false;
	}
	return true;
}

static void cdrawWindowInternalCreatePluginWarning(cdrawWindow const* const window)
{
	bool const hideCursor = (window->control & cdrawWindowControl_cursor_hide);
	if (hideCursor)
		ShowCursor(TRUE);
	MessageBoxA(p_window->hWnd, "Window cannot complete operation while externally managed plugin is attached; detach first.", "cdraw Player Application: Plugin", (MB_OK | MB_ICONEXCLAMATION | MB_SETFOREGROUND));
	if (hideCursor)
		ShowCursor(FALSE);
}

static bool cdrawWindowInternalCopyBuild(cdrawWindow* const window)
{
	bool status = true;
	
	byte_t bat[256] = { 0 };
	byte_t cmd[1024] = { 0 };
	label_t const proj = "cdrawPlugin";

	// utility path
	sprintf(bat, "%s\\_dev\\Windows\\cdraw_copybuild", gWindowPlatform.dir_sdk);

	// call batch: call BATCH LIBPATH BINPATH FILE FILE DATAPATH
	//	assemble: bat cfg cfg proj proj proj cfg
	sprintf(cmd, "call \"%s\" \".\\lib%s\\\" \".\\bin%s\\%s\\\" \"%s.dll\" \"%s.dll\" \".\\bin%s\\data\\\" ", bat, gWindowPlatform.tag_cfg, gWindowPlatform.tag_cfg, proj, proj, proj, gWindowPlatform.tag_cfg);

	// call copy utility
	printf("\n----------------------------------------------------------------\n");
	printf("cdraw Player: Deferred plugin copy attempt...");
	printf("\n----------------------------------------------------------------\n");
	status = (system(cmd) == 0);
	printf("\n----------------------------------------------------------------\n");
	printf("cdraw Player: Plugin copy complete.");
	printf("\n----------------------------------------------------------------\n");

	// done
	return status;
}

static void cdrawWindowInternalCreateInfo(cdrawWindow* const window)
{
	bool const hideCursor = (window->control & cdrawWindowControl_cursor_hide);
	bool const enterCmd = (window->control & cdrawWindowControl_ESC_command);

	byte_t buffer[1024] = { 0 }, * bufferPtr = buffer;
	cstrk_t const info[] = {
		"cdraw Player Application: About",
		"C-based cross-platform lightweight rendering framework.",
		"Copyright 2023 Daniel S. Buckstein",
	};
	bufferPtr += sprintf(bufferPtr, "%s\n%s\n\n", info[1], info[2]);

	cdrawWindowControl const winCtrlAll =
		cdrawWindowControl_F1_info | cdrawWindowControl_F2_load | cdrawWindowControl_F3_reload | cdrawWindowControl_F4_unload |
		cdrawWindowControl_F5_debug | cdrawWindowControl_F6_build | cdrawWindowControl_F7_rebuild | cdrawWindowControl_F8_fullscreen |
		cdrawWindowControl_F9_user_1 | cdrawWindowControl_F10_user_2 | cdrawWindowControl_F11_user_3 | cdrawWindowControl_F12_user_cmd;
	cdrawWindowControl winCtrl = cdrawWindowControl_F1_info;
	uint32_t winCtrlCt = 0;
	cstrk_t const ctrlBtn[] = {
		"F1", "F2", "F3", "F4",
		"F5", "F6", "F7", "F8",
		"F9", "F10", "F11", "F12",
	};
	cstrk_t const ctrlCmd[] = {
		" or cmd \'cdrawinfo\'", " or cmd \'cdrawload\'", " or cmd \'cdrawreload\'", " or cmd \'cdrawunload\'",
		" or cmd \'cdrawdebug\'", " or cmd \'cdrawbuild\'", " or cmd \'cdrawrebuild\'", " or cmd \'cdrawfullscreen\'",
		" or cmd \'cdrawuser1\'", " or cmd \'cdrawuser2\'", " or cmd \'cdrawuser3\'", " or cmd \'cdrawusercmd\'",
	};
	cstrk_t const ctrlDesc[] = {
		"About dialog (you are here)",
		"Plugin menu dialog",
		"Reload current plugin",
		"Unload current plugin",
		"Load and run debug plugin",
		"Hot build debug plugin",
		"Hot rebuild debug plugin",
		"Toggle full-screen",
		"User 1",
		"User 2",
		"User 3",
		"User 4, no cmd; VS break",
	};

	// print window features
	if (window->control & winCtrlAll)
	{
		bufferPtr += sprintf(bufferPtr, "Window controls: \n");
		while (winCtrl < cdrawWindowControl_ESC_command)
		{
			if (window->control & winCtrl)
				bufferPtr += sprintf(bufferPtr, "  %s%s: %s \n", ctrlBtn[winCtrlCt],
					enterCmd ? ctrlCmd[winCtrlCt] : "", ctrlDesc[winCtrlCt]);
			winCtrl <<= 1;
			++winCtrlCt;
		}
		if (enterCmd)
		{
			bufferPtr += sprintf(bufferPtr, "  %s\n  %s\n",
				"ESC: Enter and send cmd to window or user 4",
				"cmd cdrawexit: Quit application");
		}
		bufferPtr += sprintf(bufferPtr, "\n");
	}
	else
		bufferPtr += sprintf(bufferPtr, "No window controls.\n\n");

	// print plugin info
	if (window->p_plugin)
	{
		bufferPtr += sprintf(bufferPtr, "Plugin: ");
		cdrawPluginInfoPrint(&window->p_plugin->info, &bufferPtr);
		bufferPtr += sprintf(bufferPtr, "\n");
	}
	else
		bufferPtr += sprintf(bufferPtr, "No plugin attached.\n\n");

	// reveal cursor
	if (hideCursor)
		ShowCursor(TRUE);

	// present message box
	MessageBoxA(p_window->hWnd, buffer, *info, (MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND));

	// hide cursor
	if (hideCursor)
		ShowCursor(FALSE);
}

static LRESULT __stdcall cdrawWindowInternalEventProcessList(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	typedef struct cdrawWindowDialog
	{
		HWND box, text;
		cdrawPluginInfo* pluginInfo;
		size_t pluginInfoCount;
		cdrawWindowControl winCtrl;
	} cdrawWindowDialog;
	cdrawWindowDialog* dlg = (cdrawWindowDialog*)GetWindowLongPtrA(hDlg, GWLP_USERDATA);

	word_t const value = LOWORD(wParam), cmd = HIWORD(wParam);
	switch (message)
	{
		// used instead of WM_CREATE for dialog boxes
	case WM_INITDIALOG: {
		HINSTANCE const inst = (HINSTANCE)GetWindowLongPtrA(hDlg, GWLP_HINSTANCE);
		cdrawWindowControl const winCtrl = (cdrawWindowControl)lParam;
		byte_t caption[256] = { 0 };
		RECT rect = { 0 };
		HWND box = 0, text = 0;
		cdrawPluginInfo* pluginInfo = 0;
		size_t pluginInfoCount = 0, i = 0;
		int32_t pos = -1;

		GetWindowTextA(hDlg, caption, sizeof(caption));
		switch (winCtrl)
		{
		case cdrawWindowControl_F2_load: {
			// relative path to plugin info resource
			cstrk_t const cdraw_PLUGIN_INFO_PATH = "../../../../resource/_plugin/cdraw_plugin_info.txt";

			// load plugin data
			if (result_isclean(cdrawPluginInfoListLoad(&pluginInfo, &pluginInfoCount, cdraw_PLUGIN_INFO_PATH)) && pluginInfoCount)
			{
				int32_t w, h;

				// create plugin box
				strcat(caption, "Load Plugin");
				GetWindowRect(hDlg, &rect);
				box = CreateWindowExA(0, "LISTBOX", 0,
					(WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY),
					(8), (8), (w = (rect.right - rect.left - 304)), (h = (rect.bottom - rect.top - 96)),
					hDlg, 0, inst, NULL);
				SetFocus(box);

				// create info display box
				text = CreateWindowExA(0, "STATIC", 0,
					(WS_CHILD | WS_VISIBLE | SS_LEFT),
					(w + 16), (8), w, h,
					hDlg, 0, inst, NULL);
				SetWindowTextA(text, "Select plugin info to display.");

				// populate list
				for (i = 0; i < pluginInfoCount; ++i)
				{
					pos = (int32_t)SendMessageA(box, LB_ADDSTRING, 0, (LPARAM)pluginInfo[i].name);
					if (pos >= 0)
						SendMessageA(box, LB_SETITEMDATA, pos, (LPARAM)i);
				}
			}
			// if empty, display warning and close dialog before it appears
			else
			{
				MessageBoxA(hDlg, "No plugin information available.", "cdraw Player Application: Load Plugin Failed", (MB_OK | MB_ICONEXCLAMATION | MB_SETFOREGROUND));
				EndDialog(hDlg, value);
				return TRUE;
			}
		}	break;
		case cdrawWindowControl_ESC_command: {
			int32_t w, h;

			// create command box
			strcat(caption, "Enter Command");
			GetWindowRect(hDlg, &rect);
			box = CreateWindowExA(0, "EDIT", 0,
				(WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL),
				(8), (8), (w = (rect.right - rect.left - 32)), (h = (rect.bottom - rect.top - 96)),
				hDlg, 0, inst, NULL);
			SetFocus(box);

			// create info display box
			text = CreateWindowExA(0, "STATIC", 0,
				(WS_CHILD | WS_VISIBLE | SS_LEFT),
				(8), (h + 24), 256, 16,
				hDlg, 0, inst, NULL);
			SetWindowTextA(text, "Enter command.");
		}	break;
		}
		dlg = (cdrawWindowDialog*)malloc(sizeof(cdrawWindowDialog));
		if (dlg)
		{
			dlg->box = box;
			dlg->text = text;
			dlg->pluginInfo = pluginInfo;
			dlg->pluginInfoCount = pluginInfoCount;
			dlg->winCtrl = winCtrl;
			SetWindowTextA(hDlg, caption);
			SetWindowLongPtrA(hDlg, GWLP_USERDATA, (LONG_PTR)dlg);
			return TRUE;
		}
		return FALSE;
	}
	case WM_CLOSE: {
		if (dlg->winCtrl == cdrawWindowControl_F2_load)
			cdrawPluginInfoListRelease(&dlg->pluginInfo);
		free(dlg);
		EndDialog(hDlg, value);
		return TRUE;
	}
	case WM_COMMAND:
		switch (cmd)
		{
		case EN_CHANGE:
			if (dlg->winCtrl == cdrawWindowControl_ESC_command)
			{
				// toggle OK button
				if (SendMessageA(dlg->box, WM_GETTEXTLENGTH, 0, 0))
				{
					EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
					SetWindowTextA(dlg->text, "Press OK to send command.");
				}
				else
				{
					EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
					SetWindowTextA(dlg->text, "Enter command.");
				}
				return TRUE;
			}
			break;
		case LBN_SELCHANGE:
		case LBN_DBLCLK:
			if (dlg->winCtrl == cdrawWindowControl_F2_load)
			{
				cdrawPluginInfo const* info = 0;
				byte_t buf[512] = { 0 };
				int32_t i = -1;

				// display info in text box
				i = (int32_t)SendMessageA(dlg->box, LB_GETCURSEL, 0, 0);
				if (i >= 0)
				{
					i = (int32_t)SendMessageA(dlg->box, LB_GETITEMDATA, i, 0);
					info = dlg->pluginInfo + i;
					sprintf(buf, "Press OK to load plugin: \n\n\"%s\" \n  By %s \n  Ver. %s \n%s \n",
						info->name, info->author, info->version, info->details);
					SetWindowTextA(dlg->text, buf);

					// enable OK button
					EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
				}
				return TRUE;
			}
			break;
		case LBN_SELCANCEL: {
			SetWindowTextA(dlg->text, "Select plugin info to display.");
		}	break;

		default:
			switch (value)
			{
			case IDOK: {
				//	send command to parent window
				HWND hWnd = GetParent(hDlg);
				switch (dlg->winCtrl)
				{
				case cdrawWindowControl_F2_load: {
					//	send info structure at selected index
					cdrawPluginInfo const* info = 0;
					int32_t i = (int32_t)SendMessageA(dlg->box, LB_GETCURSEL, 0, 0);
					if (i >= 0)
					{
						i = (int32_t)SendMessageA(dlg->box, LB_GETITEMDATA, i, 0);
						info = dlg->pluginInfo + i;
						//SendMessageA(hWnd, cdrawWinCtrlMsg_unload, (WPARAM)true, 0);
						SendMessageA(hWnd, cdrawWinCtrlMsg_load, (WPARAM)i, (LPARAM)info);
					}
					else
					{
						MessageBoxA(hDlg, "Error: Invalid plugin info.", "cdraw Player Application: Load Plugin Failed", (MB_OK | MB_ICONHAND | MB_SETFOREGROUND));
					}
				}	break;
				case cdrawWindowControl_ESC_command: {
					//	send command string
					int32_t const i = GetWindowTextLengthA(dlg->box);
					if (i > 0)
					{
						cstr_t buf = (cstr_t)malloc((size_t)i + 1);
						if (buf)
						{
							buf[i] = 0;
							GetWindowTextA(dlg->box, buf, (i + 1));
							SendMessageA(hWnd, cdrawWinCtrlMsg_cmd, (WPARAM)i, (LPARAM)buf);
							free(buf);
						}
					}
				}	break;
				}
				// fall through to close
			}
			case IDCLOSE:
			case IDCANCEL:
				// exit dialog
				if (dlg->winCtrl == cdrawWindowControl_F2_load)
					cdrawPluginInfoListRelease(&dlg->pluginInfo);
				free(dlg);
				EndDialog(hDlg, value);
				return TRUE;
			}
			break;
		}
	}

	// do not pass unprocessed messages to DefDlgProc, recursive
	return FALSE;
}

static void cdrawWindowInternalCreateDialog(cdrawWindow* const window, cdrawWindowControl const purpose)
{
	int8_t const dialogID = gWindowPlatform.res_dialog;
	bool const hideCursor = (window->control & cdrawWindowControl_cursor_hide);
	LPCSTR const dialogRes = MAKEINTRESOURCEA(dialogID);

	// reveal cursor
	if (hideCursor)
		ShowCursor(TRUE);

	// present dialog box
	DialogBoxParamA(gWindowPlatform.info.hInstance, dialogRes, p_window->hWnd, cdrawWindowInternalEventProcessList, (LPARAM)purpose);

	// hide cursor
	if (hideCursor)
		ShowCursor(FALSE);
}

static void cdrawWindowInternalToggleFullScreen(cdrawWindow* const window)
{
	MONITORINFO monitorArea = { sizeof(MONITORINFO) };
	RECT displayArea = { 0 };
	HWND hWnd = p_window->hWnd;
	dword_t style = (dword_t)GetWindowLongPtrA(hWnd, GWL_STYLE), styleEx = (dword_t)GetWindowLongPtrA(hWnd, GWL_EXSTYLE);

	// full-screen if original display area is not all zeros
	if (p_window->restoreArea.left | p_window->restoreArea.top | p_window->restoreArea.right | p_window->restoreArea.bottom)
	{
		// full-screen to windowed
		styleEx |= WS_EX_WINDOWEDGE;
		style |= WS_OVERLAPPEDWINDOW;
		displayArea = p_window->restoreArea;
		memset(&p_window->restoreArea, 0, sizeof(p_window->restoreArea));
	}
	else
	{
		// windowed to full-screen
		styleEx &= ~WS_EX_WINDOWEDGE;
		style &= ~WS_OVERLAPPEDWINDOW;
		GetWindowRect(hWnd, &p_window->restoreArea);
		GetMonitorInfoA(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &monitorArea);
		displayArea = monitorArea.rcMonitor;
		//GetWindowRect(GetDesktopWindow(), &displayArea);
		//AdjustWindowRectEx(&displayArea, style, false, styleEx);
	}

	// perform resize
	SetWindowLongPtrA(hWnd, GWL_EXSTYLE, styleEx);
	SetWindowLongPtrA(hWnd, GWL_STYLE, style);
	MoveWindow(hWnd, displayArea.left, displayArea.top, (displayArea.right - displayArea.left), (displayArea.bottom - displayArea.top), TRUE);
}

static void cdrawWindowInternalLockCursor(cdrawWindow* const window)
{
	if (window && p_window)
	{
		HWND const handle = p_window->hWnd;
		POINT corner_tl = { 0 }, corner_br = { 0 };
		RECT displayArea = { 0 };
		GetClientRect(handle, &displayArea);
		corner_tl.x = displayArea.left;
		corner_tl.y = displayArea.top;
		corner_br.x = displayArea.right;
		corner_br.y = displayArea.bottom;
		ClientToScreen(handle, &corner_tl);
		ClientToScreen(handle, &corner_br);
		displayArea.left = corner_tl.x;
		displayArea.top = corner_tl.y;
		displayArea.right = corner_br.x;
		displayArea.bottom = corner_br.y;
		ClipCursor(&displayArea);
	}
	else
	{
		ClipCursor(NULL);
	}
}

CDRAW_INL void cdrawWindowInternalHandleF1(cdrawWindow* const window)
{
	cdrawWindowInternalCreateInfo(window);
}

CDRAW_INL void cdrawWindowInternalHandleF2(cdrawWindow* const window)
{
	if (cdrawWindowInternalCreateBuildWarning(window))
		cdrawWindowInternalCreateDialog(window, cdrawWindowControl_F2_load);
}

CDRAW_INL void cdrawWindowInternalHandleF3(cdrawWindow* const window)
{
	if (cdrawWindowInternalCreateBuildWarning(window))
		SendMessageA(p_window->hWnd, cdrawWinCtrlMsg_reload, 0, 0);
}

CDRAW_INL void cdrawWindowInternalHandleF4(cdrawWindow* const window)
{
	if (cdrawWindowInternalCreateBuildWarning(window))
		SendMessageA(p_window->hWnd, cdrawWinCtrlMsg_unload, (WPARAM)true, 0);
}

CDRAW_INL void cdrawWindowInternalHandleF5(cdrawWindow* const window)
{
	if (cdrawWindowInternalCreateBuildWarning(window))
		SendMessageA(p_window->hWnd, cdrawWinCtrlMsg_debug, (WPARAM)true, 0);
}

CDRAW_INL void cdrawWindowInternalHandleF6(cdrawWindow* const window)
{
	if (cdrawWindowInternalCreateBuildWarning(window))
		SendMessageA(p_window->hWnd, cdrawWinCtrlMsg_build, 0, 0);
}

CDRAW_INL void cdrawWindowInternalHandleF7(cdrawWindow* const window)
{
	if (cdrawWindowInternalCreateBuildWarning(window))
		SendMessageA(p_window->hWnd, cdrawWinCtrlMsg_rebuild, 0, 0);
}

CDRAW_INL void cdrawWindowInternalHandleF8(cdrawWindow* const window)
{
	cdrawWindowInternalToggleFullScreen(window);
}

CDRAW_INL void cdrawWindowInternalHandleF9(cdrawWindow* const window)
{
	cdraw_window_callback(cdrawPluginCallOnUser1);
}

CDRAW_INL void cdrawWindowInternalHandleF10(cdrawWindow* const window)
{
	cdraw_window_callback(cdrawPluginCallOnUser2);
}

CDRAW_INL void cdrawWindowInternalHandleF11(cdrawWindow* const window)
{
	cdraw_window_callback(cdrawPluginCallOnUser3);
}

CDRAW_INL void cdrawWindowInternalHandleF12(cdrawWindow* const window)
{
	cdraw_window_callback(cdrawPluginCallOnUserCmd, 0, NULL);
}

CDRAW_INL void cdrawWindowInternalHandleESC(cdrawWindow* const window)
{
	cdrawWindowInternalCreateDialog(window, cdrawWindowControl_ESC_command);
}

static LRESULT __stdcall cdrawWindowInternalEvent_win(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cdrawWindow* window = (cdrawWindow*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
	switch (message)
	{
		// INITIAL WINDOW CREATION
	case WM_NCCREATE: {
		cdraw_assert(window == NULL);
		cdrawWindow_win* const window_platform = (cdrawWindow_win*)malloc(sizeof(cdrawWindow_win));
		if (window_platform)
		{
			window = (cdrawWindow*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
			p_window = window_platform;
			cdraw_window_valid();

			memset(p_window, 0, sizeof(cdrawWindow_win));
			p_window->hInst = gWindowPlatform.info.hInstance;
			p_window->hWnd = hWnd;
			p_window->hDC = GetDC(hWnd);
			p_window->mouseTrack.cbSize = sizeof(p_window->mouseTrack);
			p_window->mouseTrack.hwndTrack = hWnd;
			p_window->mouseTrack.dwFlags = (TME_LEAVE);
			cdraw_assert(p_window->hInst && p_window->hWnd && p_window->hDC);

			TrackMouseEvent(&p_window->mouseTrack);
			SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)window);
			++gWindowPlatform.windowCount;
		}
	}	break;
	
		// FINISH WINDOW CREATION
	case WM_CREATE: {
		// NOTHING ELSE TO DO - would set up rendering here if the window owned a context
	}	break;

		// WINDOW CLOSED
	case WM_CLOSE: {
		// recursively take down windows
		DestroyWindow(hWnd);
	}	break;
	
		// WINDOW DESTROYED - would clean up rendering here
	case WM_DESTROY: {
		cdraw_window_valid();
		SendMessageA(hWnd, cdrawWinCtrlMsg_unload, wParam, lParam);
		
		ReleaseDC(hWnd, p_window->hDC);
		free(p_window);
		p_window = NULL;

		// terminate loop if all windows closed
		--gWindowPlatform.windowCount;
		if (gWindowPlatform.windowCount <= 0)
			PostQuitMessage(0);
	}	break;

		// DISPLAY REFRESH
	case WM_PAINT: {
		PAINTSTRUCT paint;
		RECT area;
		HDC hDC;
		BOOL result = GetUpdateRect(hWnd, &area, 0);
		hDC = BeginPaint(hWnd, &paint);
		result = FillRect(hDC, &area, (HBRUSH)(COLOR_WINDOW));
		result = EndPaint(hWnd, &paint);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnDisplay);
	}	break;

		// MENU OR ACCELERATOR
	case WM_COMMAND: {
		word_t const value = LOWORD(wParam);
		word_t const base = gWindowPlatform.res_control_base;
		cdraw_window_valid();
		switch (value - base)
		{
		case 0: // F1: info dialog
			if (window->control & cdrawWindowControl_F1_info)
				cdrawWindowInternalHandleF1(window);
			break;
		case 1: // F2: load plugin dialog
			if (window->control & cdrawWindowControl_F2_load)
				cdrawWindowInternalHandleF2(window);
			break;
		case 2: // F3: reload plugin
			if (window->control & cdrawWindowControl_F3_reload)
				cdrawWindowInternalHandleF3(window);
			break;
		case 3: // F4: unload plugin
			if (window->control & cdrawWindowControl_F4_unload)
				cdrawWindowInternalHandleF4(window);
			break;
		case 4: // F5: debug plugin
			if (window->control & cdrawWindowControl_F5_debug)
				cdrawWindowInternalHandleF5(window);
			break;
		case 5: // F6: hot-build plugin
			if (window->control & cdrawWindowControl_F6_build)
				cdrawWindowInternalHandleF6(window);
			break;
		case 6: // F7: hot-rebuild plugin
			if (window->control & cdrawWindowControl_F7_rebuild)
				cdrawWindowInternalHandleF7(window);
			break;
		case 7: // F8: toggle full-screen
			if (window->control & cdrawWindowControl_F8_fullscreen)
				cdrawWindowInternalHandleF8(window);
			break;
		case 8: // F9: user 1
			if (window->control & cdrawWindowControl_F9_user_1)
				cdrawWindowInternalHandleF9(window);
			break;
		case 9: // F10: user 2
			if (window->control & cdrawWindowControl_F10_user_2)
				cdrawWindowInternalHandleF10(window);
			break;
		case 10: // F11: user 3
			if (window->control & cdrawWindowControl_F11_user_3)
				cdrawWindowInternalHandleF11(window);
			break;
		case 11: // F12: user 4
			if (window->control & cdrawWindowControl_F12_user_cmd)
				cdrawWindowInternalHandleF12(window);
			break;
		case 12: // ESC: command dialog
			if (window->control & cdrawWindowControl_ESC_command)
				cdrawWindowInternalHandleESC(window);
			break;
		}
	}	break;

		// WINDOW CHANGES ACTIVATION STATE
	case WM_ACTIVATE: {
		word_t const value = LOWORD(wParam);
		cdraw_window_valid();
		p_window->activeState = value;
		switch (value)
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE: {
			if (window->control & cdrawWindowControl_cursor_lock)
				cdrawWindowInternalLockCursor(window);
			if (!(window->control & cdrawWindowControl_active_unfocused))
				cdraw_window_callback(cdrawPluginCallOnWindowActivate, p_window);
		}	break;
		case WA_INACTIVE: {
			if (!(window->control & cdrawWindowControl_active_unfocused))
				cdraw_window_callback(cdrawPluginCallOnWindowDeactivate, p_window);
			if (window->control & cdrawWindowControl_cursor_lock)
				cdrawWindowInternalLockCursor(NULL);
		}	break;
		}
	}	break;

		// WINDOW MOVE
	case WM_MOVE: {
		int16_t const x = LOWORD(lParam);
		int16_t const y = HIWORD(lParam);
		cdraw_window_valid();
		window->pos_x = x;
		window->pos_y = y;
		if (window->control & cdrawWindowControl_cursor_lock)
			cdrawWindowInternalLockCursor(window);
		cdraw_window_callback(cdrawPluginCallOnWindowMove, x, y);
	}	break;

		// WINDOW RESIZE
	case WM_SIZE: {
		int16_t const w = LOWORD(lParam);
		int16_t const h = HIWORD(lParam);
		cdraw_window_valid();
		window->sz_w = w;
		window->sz_h = h;
		if (window->control & cdrawWindowControl_cursor_lock)
			cdrawWindowInternalLockCursor(window);
		cdraw_window_callback(cdrawPluginCallOnWindowResize, w, h);
	}	break;

		// VIRTUAL KEY DOWN
	case WM_KEYDOWN: {
		int16_t const virtkey = LOWORD(wParam);
		cdraw_window_valid();
		if (HIWORD(lParam) & KF_REPEAT)
		{
			cdraw_window_callback(cdrawPluginCallOnVirtkeyHold, virtkey);
		}
		else
		{
			cdraw_window_callback(cdrawPluginCallOnVirtkeyPress, virtkey);
		}
	}	break;

		// ASCII KEY DOWN - no up call
	case WM_CHAR: {
		int8_t const key = LOBYTE(LOWORD(wParam));
		cdraw_window_valid();
		if (HIWORD(lParam) & KF_REPEAT)
		{
			cdraw_window_callback(cdrawPluginCallOnKeyHold, key);
		}
		else
		{
			cdraw_window_callback(cdrawPluginCallOnKeyPress, key);
		}
		bitflagraise(p_window->keyState, key);
	}	break;

		// VIRTUAL KEY UP - determine ASCII equivalent
	case WM_KEYUP: {
		int16_t const virtkey = LOWORD(wParam);
		UINT const key_map = MapVirtualKeyA(virtkey, MAPVK_VK_TO_CHAR);
		int8_t key;
		cdraw_window_valid();
		bool const canCallback = cdraw_window_cancallback();
		if (canCallback)
			cdrawPluginCallOnVirtkeyRelease(window->p_plugin, p_window->pluginOwner, virtkey);
		if (key = LOBYTE(LOWORD(key_map)))
		{
			if (canCallback && bitflagcheck(p_window->keyState, key))
				cdrawPluginCallOnKeyRelease(window->p_plugin, p_window->pluginOwner, key);
			bitflaglower(p_window->keyState, key);
			if (key = gKeyAlt[key])
			{
				if (canCallback && bitflagcheck(p_window->keyState, key))
					cdrawPluginCallOnKeyRelease(window->p_plugin, p_window->pluginOwner, key);
				bitflaglower(p_window->keyState, key);
			}
		}
	}	break;

		// LEFT MOUSE PRESSED
	case WM_LBUTTONDOWN: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMousePress, mouse_left, x, y);
	}	break;

		// MIDDLE MOUSE PRESSED
	case WM_MBUTTONDOWN: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMousePress, mouse_middle, x, y);
	}	break;

		// RIGHT MOUSE PRESSED
	case WM_RBUTTONDOWN: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMousePress, mouse_right, x, y);
	}	break;

		// OTHER MOUSE PRESSED
	case WM_XBUTTONDOWN: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMousePress, mouse_right + HIWORD(wParam), x, y);
	}	break;

		// LEFT MOUSE DOUBLE
	case WM_LBUTTONDBLCLK: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMouseDouble, mouse_left, x, y);
	}	break;

		// MIDDLE MOUSE DOUBLE
	case WM_MBUTTONDBLCLK: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMouseDouble, mouse_middle, x, y);
	}	break;

		// RIGHT MOUSE DOUBLE
	case WM_RBUTTONDBLCLK: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMouseDouble, mouse_right, x, y);
	}	break;

		// OTHER MOUSE DOUBLE
	case WM_XBUTTONDBLCLK: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMouseDouble, mouse_right + HIWORD(wParam), x, y);
	}	break;

		// LEFT MOUSE RELEASE
	case WM_LBUTTONUP: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMouseRelease, mouse_left, x, y);
	}	break;

		// MIDDLE MOUSE RELEASE
	case WM_MBUTTONUP: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMouseRelease, mouse_middle, x, y);
	}	break;

		// RIGHT MOUSE RELEASE
	case WM_RBUTTONUP: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMouseRelease, mouse_right, x, y);
	}	break;

		// OTHER MOUSE RELEASE
	case WM_XBUTTONUP: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMouseRelease, mouse_right + HIWORD(wParam), x, y);
	}	break;

		// MOUSE WHEEL
	case WM_MOUSEWHEEL: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMouseWheel, GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA, x, y);
	}	break;

		// MOUSE MOVE / DRAG / ENTER
	case WM_MOUSEMOVE: {
		int32_t const x = GET_X_LPARAM(lParam);
		int32_t const y = GET_Y_LPARAM(lParam);
		cdraw_window_valid();
		if (cdraw_window_cancallback())
		{
			if (wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2))
				cdrawPluginCallOnMouseDrag(window->p_plugin, p_window->pluginOwner, x, y);
			else
				cdrawPluginCallOnMouseMove(window->p_plugin, p_window->pluginOwner, x, y);
			if (!p_window->mouseState)
				cdrawPluginCallOnMouseEnter(window->p_plugin, p_window->pluginOwner, x, y);
		}
		TrackMouseEvent(&p_window->mouseTrack);
		p_window->mouseState = true;
	}	break;

		// MOUSE LEAVE
	case WM_MOUSELEAVE: {
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(p_window->hWnd, &pos);
		cdraw_window_valid();
		cdraw_window_callback(cdrawPluginCallOnMouseLeave, pos.x, pos.y);
		p_window->mouseState = false;
	}	break;

		// hard-load plugin; unload first
	case cdrawWinCtrlMsg_load: {
		cdrawPluginInfo const* info = (cdrawPluginInfo*)lParam;
		int32_t const pluginID = (int32_t)wParam;
		cdraw_window_valid();
		SendMessageA(hWnd, cdrawWinCtrlMsg_unload, wParam, lParam);
		if (!window->p_plugin)
		{
			cdrawPluginReset(&p_window->pluginDebug);
			if (result_isclean(cdrawPluginLoad(&p_window->pluginDebug, info, pluginID, p_window)))
			{
				// clip cursor
				if (window->control & cdrawWindowControl_cursor_lock)
					cdrawWindowInternalLockCursor(window);

				window->p_plugin = &p_window->pluginDebug;
				p_window->pluginOwner = p_window;
				cdrawPluginCallOnWindowAttach(window->p_plugin, p_window, window->sz_w, window->sz_h, window->pos_x, window->pos_y, p_window);
				if (p_window->activeState || (window->control & cdrawWindowControl_active_unfocused))
					cdrawPluginCallOnWindowActivate(window->p_plugin, p_window, p_window);
			}
		}
	}	break;
		
		// reload plugin
	case cdrawWinCtrlMsg_reload: {
		cdraw_window_valid();
		if (window->p_plugin)
		{
			if (cdraw_window_ownsplugin())
			{
				if (result_isclean(cdrawPluginReload(window->p_plugin, p_window)))
				{
				}
			}
			else
			{
				cdrawWindowInternalCreatePluginWarning(window);
			}
		}
	}	break;

		// unload plugin
	case cdrawWinCtrlMsg_unload: {
		cdraw_window_valid();
		if (window->p_plugin)
		{
			if (cdraw_window_ownsplugin())
			{
				window->p_plugin->id = INT_MAX;
				cdrawPluginCallOnWindowDeactivate(window->p_plugin, p_window, p_window);
				cdrawPluginCallOnWindowDetach(window->p_plugin, p_window, p_window);
				if (result_isclean(cdrawPluginUnload(window->p_plugin, p_window)))
				{
					window->p_plugin = NULL;
					p_window->pluginOwner = NULL;
				}
			}
			else
			{
				cdrawWindowInternalCreatePluginWarning(window);
			}
		}
	}	break;

		// load debug plugin - unload first if not debug plugin
	case cdrawWinCtrlMsg_debug: {
		cdraw_window_valid();
		if (window->p_plugin)
		{
			ptrdiff_t const currentID = window->p_plugin->id;
			if (currentID >= 0)
				SendMessageA(hWnd, cdrawWinCtrlMsg_unload, wParam, lParam);
		}

		// load debug (default)
		if (!window->p_plugin)
		{
			cdrawPluginInfo info;
			cdrawPluginInfoReset(&info);
			SendMessageA(hWnd, cdrawWinCtrlMsg_load, (WPARAM)(INT_MAX), (LPARAM)(&info));
		}
		if (window->p_plugin)
			window->p_plugin->id = INT_MIN;
	}	break;

		// launch build thread
	case cdrawWinCtrlMsg_build: {
		CreateThread(0, 0, cdrawWindowInternalBuildThread, window, 0, 0);
	}	break;

		// launch rebuild thread
	case cdrawWinCtrlMsg_rebuild: {
		CreateThread(0, 0, cdrawWindowInternalRebuildThread, window, 0, 0);
	}	break;

		// perform reload and copy dylib
	case cdrawWinCtrlMsg_copy: {
		cdraw_window_valid();
		bool const success = (bool)lParam;
		if (success)
		{
			// hotload or load debug
			if (window->p_plugin && cdraw_window_ownsplugin() && (window->p_plugin->id < 0))
			{
				cdrawPluginUnload(window->p_plugin, p_window);
				cdrawWindowInternalCopyBuild(window);
				cdrawPluginLoad(window->p_plugin, &window->p_plugin->info, INT_MIN, p_window);
			}
			else
			{
				cdrawWindowInternalCopyBuild(window);
				SendMessageA(hWnd, cdrawWinCtrlMsg_debug, wParam, lParam);
			}
		}
	}	break;
		
		// capture and send command
	case cdrawWinCtrlMsg_cmd: {
		cstrk_t const cmd = (cstrk_t)lParam;
		int32_t const len = (int32_t)wParam;

		// process command locally first, sending only if not captured
		if (cmd && (len > 0))
		{
			cdraw_window_valid();
			cdraw_window_callback(cdrawPluginCallOnUserCmd, 1, &cmd);
			if (len < sizeof(label_t))
			{
				label_t cmd_short;
				int32_t i;
				for (i = 0; i < len; ++i)
					cmd_short[i] = tolower(cmd[i]);
				cmd_short[i] = 0;

				// quit app (in case of emergency)
				if (!strcmp(cmd_short, "cdrawexit"))
					PostQuitMessage(0);
				// info box
				else if (!strcmp(cmd_short, "cdrawinfo"))
					cdrawWindowInternalHandleF1(window);
				// load dialog
				else if (!strcmp(cmd_short, "cdrawload"))
					cdrawWindowInternalHandleF2(window);
				// reload
				else if (!strcmp(cmd_short, "cdrawreload"))
					cdrawWindowInternalHandleF3(window);
				// unload
				else if (!strcmp(cmd_short, "cdrawunload"))
					cdrawWindowInternalHandleF4(window);
				// debug
				else if (!strcmp(cmd_short, "cdrawdebug"))
					cdrawWindowInternalHandleF5(window);
				// build
				else if (!strcmp(cmd_short, "cdrawbuild"))
					cdrawWindowInternalHandleF6(window);
				// rebuild
				else if (!strcmp(cmd_short, "cdrawrebuild"))
					cdrawWindowInternalHandleF7(window);
				// fullscreen
				else if (!strcmp(cmd_short, "cdrawfullscreen"))
					cdrawWindowInternalHandleF8(window);
				// user1
				else if (!strcmp(cmd_short, "cdrawuser1"))
					cdrawWindowInternalHandleF9(window);
				// user2
				else if (!strcmp(cmd_short, "cdrawuser2"))
					cdrawWindowInternalHandleF10(window);
				// user3
				else if (!strcmp(cmd_short, "cdrawuser3"))
					cdrawWindowInternalHandleF11(window);
				// user4
				else if (!strcmp(cmd_short, "cdrawusercmd"))
					cdrawWindowInternalHandleF12(window);
			}
		}
	}	break;

		// NONE
	default:
		break;
	}
	return DefWindowProcA(hWnd, message, wParam, lParam);
}

bool cdrawWindowInternalPlatformInit_win(HINSTANCE const inst,
	cstrk_t const dir_build, cstrk_t const dir_target, cstrk_t const dir_sdk, cstrk_t const tag_cfg,
	int16_t const res_control_base, int8_t const res_control_id, int8_t const res_dialog, int8_t const res_cursor, int8_t const res_icon)
{
	if (cdrawWindowInternalPlatformReady_win() || !inst)
		return false;

	HINSTANCE const hInst = (inst ? inst : GetModuleHandle(NULL));
	HINSTANCE const cursorInst = (res_cursor >= 0 ? hInst : NULL);
	HINSTANCE const iconInst = (res_icon >= 0 ? hInst : NULL);
	LPSTR const cursorStr = (cursorInst ? MAKEINTRESOURCEA(res_cursor) : MAKEINTRESOURCEA(32512)); // IDC_ARROW = 32512
	LPSTR const iconStr = (iconInst ? MAKEINTRESOURCEA(res_icon) : MAKEINTRESOURCEA(32517)); // IDI_WINLOGO = 32517

	label_t const windowPlatformName = "cdraw Window Platform";
	label_copy(gWindowPlatform.name, windowPlatformName);

	gWindowPlatform.dir_build = (buffer_valid(dir_build) ? dir_build : NULL);
	gWindowPlatform.dir_target = (buffer_valid(dir_target) ? dir_target : NULL);
	gWindowPlatform.dir_sdk = (buffer_valid(dir_sdk) ? dir_sdk : NULL);
	gWindowPlatform.tag_cfg = (buffer_valid(tag_cfg) ? tag_cfg : NULL);
	gWindowPlatform.res_control_base = res_control_base;
	gWindowPlatform.res_control_id = res_control_id;
	gWindowPlatform.res_dialog = res_dialog;

	gWindowPlatform.info.cbSize = sizeof(gWindowPlatform.info);
	gWindowPlatform.info.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS);
	gWindowPlatform.info.lpfnWndProc = cdrawWindowInternalEvent_win;
	gWindowPlatform.info.cbClsExtra = 0;
	gWindowPlatform.info.cbWndExtra = (int)sizeof(cdrawWindow*);
	gWindowPlatform.info.hInstance = hInst;
	gWindowPlatform.info.hIcon = LoadIconA(iconInst, iconStr);
	gWindowPlatform.info.hCursor = LoadCursorA(cursorInst, cursorStr);
	gWindowPlatform.info.hbrBackground = 0;
	gWindowPlatform.info.lpszMenuName = 0;
	gWindowPlatform.info.lpszClassName = gWindowPlatform.name;
	gWindowPlatform.info.hIconSm = gWindowPlatform.info.hIcon;

	return true;
}

bool cdrawWindowInternalPlatformTerm_win(HINSTANCE const inst)
{
	if (!cdrawWindowInternalPlatformReady_win() || !cdrawWindowInternalPlatformHandleValid_win(inst))
		return false;
	if (!cdrawWindowInternalInfoRelease_win())
		return false;
	memset(&gWindowPlatform, 0, sizeof(gWindowPlatform));
	return true;
}

static result_t cdrawApplicationInternalStartSingleInstance(ptr_t* const handle_out, label_t const instanceName, int32_t* const index_out_opt)
{
	result_init();
	if (index_out_opt)
		*index_out_opt = -1;
	asserterr(handle_out && !*handle_out, errcode_invalidarg);
	asserterr_cstr(instanceName, errcode_invalidarg);

	// create mutex handle
	ptr_t const handle = CreateMutexA(0, TRUE, instanceName);
	asserterr_ptr(handle, errcode_window_init);

	// check if instance exists
	dword_t status = GetLastError();
	if (status == ERROR_ALREADY_EXISTS)
	{
		// current index is limit (1)
		if (index_out_opt)
			*index_out_opt = 1;

		// attempt close handle
		status = CloseHandle(handle);
		result_raisewarn(warncode_window_limit);
		result_return();
	}

	// set index
	if (index_out_opt)
		*index_out_opt = 0;

	// done
	*handle_out = handle;
	result_return();
}


/******************************************************************************
* Public functions.
******************************************************************************/

result_t cdrawApplicationStartSingleInstance(ptr_t* const p_handle_out, label_t const instanceName, int32_t* const index_out_opt)
{
	result_init();
	asserterr(p_handle_out && !*p_handle_out, errcode_invalidarg);
	asserterr_cstr(instanceName, errcode_invalidarg);

	result_t const init_internal = cdrawApplicationInternalStartSingleInstance(p_handle_out, instanceName, index_out_opt);
	if (result_getwarn(init_internal, warncode_window_limit))
	{
		HWND const existing = FindWindowA(0, instanceName);
		if (SetForegroundWindow(existing))
		{
			// switched to existing window
			// nothing more to do
		}
		else
		{
			// cannot switch to existing window
			// append warning
			result_seterror(errcode_window_init);
		}
	}
	result_return();
}

result_t cdrawApplicationStopSingleInstance(ptr_t* const p_handle, uint32_t* const available_out_opt)
{
	if (available_out_opt)
		*available_out_opt = 0;
	result_init();
	asserterr_ptrval(p_handle, errcode_invalidarg);

	// release and close mutex
	bool released;
	released = ReleaseMutex(*p_handle);
	released = released && CloseHandle(*p_handle);
	failret(released, result_seterror(errcode_window_init));

	// update count
	if (available_out_opt)
		*available_out_opt = 1;

	// done
	*p_handle = 0;
	result_return();
}

result_t cdrawApplicationStartMultipleInstance(ptr_t* const p_handle_out, label_t const instanceName, uint32_t const limit, int32_t* const index_out_opt)
{
	if (index_out_opt)
		*index_out_opt = -1;
	result_init();
	asserterr(p_handle_out && !*p_handle_out, errcode_invalidarg);
	asserterr_cstr(instanceName, errcode_invalidarg);
	asserterr(limit, errcode_invalidarg);

	// create semaphore handle, max count (signaled)
	ptr_t const handle = CreateSemaphoreA(0, limit, limit, instanceName);
	asserterr_ptr(handle, errcode_window_init);

	dword_t status = GetLastError();

	// get signal state, decrements count
	dword_t signal = WaitForSingleObject(handle, 0);

	bool const signaled = (signal == WAIT_OBJECT_0) || (signal == WAIT_TIMEOUT);
	if (!signaled)
	{
		status = CloseHandle(handle);
		failret(signaled, result_seterror(errcode_window_init));
	}

	// check if waited too long for new instance
	// this happens if count is zero (non-signaled) and timeout occurs
	if (signal == WAIT_TIMEOUT)
	{
		// current index is limit because a new instance should not start
		if (index_out_opt)
			*index_out_opt = limit;

		//status == ERROR_ALREADY_EXISTS
		// attempt close handle
		status = CloseHandle(handle);
		result_raisewarn(warncode_window_limit);
		result_return();
	}

	// semaphore has positive count (signaled)
	// (signal == WAIT_OBJECT_0)
	if (index_out_opt)
	{
		// to get current count, force semaphor change then undo
		signal = WaitForSingleObject(handle, 0);
		if (signal == WAIT_OBJECT_0)
		{
			// there are more instances available, revert and capture count
			// index will be two less than what we want relative to limit 
			//	because we have done two 'wait' operations
			status = ReleaseSemaphore(handle, 1, index_out_opt);
			*index_out_opt = limit - (*index_out_opt + 2);
		}
		else if (signal == WAIT_TIMEOUT)
		{
			// this is the last available instance
			// index is one less than count
			*index_out_opt = limit - 1;
		}
	}

	// done
	*p_handle_out = handle;
	result_return();
}

result_t cdrawApplicationStopMultipleInstance(ptr_t* const p_handle, uint32_t* const available_out_opt)
{
	if (available_out_opt)
		*available_out_opt = 0;
	result_init();
	asserterr_ptrval(p_handle, errcode_invalidarg);

	// release and close semaphore, increasing count by 1 (signaled)
	bool released;
	released = ReleaseSemaphore(*p_handle, 1, available_out_opt);
	released = released && CloseHandle(*p_handle);
	failret(released, result_seterror(errcode_window_init));

	// update count
	if (available_out_opt)
		++(*available_out_opt);

	// done
	*p_handle = 0;
	result_return();
}

result_t cdrawWindowCreate(cdrawWindow* const window, label_t const windowName, int16_t const windowPosX, int16_t const windowPosY, int16_t const windowSzW, int16_t const windowSzH, bool const fullScreen, cdrawWindowControl const control, ptr_t const data_opt)
{
	result_init();
	asserterr(window && !p_window, errcode_invalidarg);
	asserterr(cdrawWindowInternalPlatformReady_win(), errcode_window_init);
	
	bool const hideCursor = flagcheck(control, cdrawWindowControl_cursor_hide);
	bool const lockCursor = flagcheck(control, cdrawWindowControl_cursor_lock);
	bool const showWindow = (data_opt ? *((int32_t*)data_opt) : SW_SHOWDEFAULT);

	HWND handle = NULL;
	RECT displayArea = { 0 };
	dword_t style = (WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | WS_POPUP), styleEx = (WS_EX_APPWINDOW);

	// normal windowed area
	displayArea.left = windowPosX;
	displayArea.top = windowPosY;
	displayArea.right = displayArea.left + windowSzW;
	displayArea.bottom = displayArea.top + windowSzH;
	styleEx |= WS_EX_WINDOWEDGE;
	style |= WS_OVERLAPPEDWINDOW;
	AdjustWindowRectEx(&displayArea, style, false, styleEx);

	if (gWindowPlatform.windowCount == 0)
		cdrawWindowInternalInfoCreate_win();
	failret(cdrawWindowInternalInfoReady_win(), result_seterror(errcode_window_init));

	handle = CreateWindowExA(
		styleEx, gWindowPlatform.info.lpszClassName, windowName, style,
		windowPosX, windowPosY, (displayArea.right - displayArea.left), (displayArea.bottom - displayArea.top),
		NULL, NULL, gWindowPlatform.info.hInstance, window);
	if (handle && !p_window)
	{
		// window created but not initialized correctly
		DestroyWindow(handle);
		handle = NULL;
	}
	if (gWindowPlatform.windowCount == 0)
		cdrawWindowInternalInfoRelease_win();
	failret(handle, result_seterror(errcode_window_init));

	ShowCursor(!hideCursor);
	ShowWindow(handle, showWindow);
	UpdateWindow(handle);
	if (lockCursor)
		cdrawWindowInternalLockCursor(window);
	if (fullScreen)
		cdrawWindowInternalToggleFullScreen(window);

	// done
	window->p_plugin = NULL;
	window->control = control;
	result_return();
}

result_t cdrawWindowDestroy(cdrawWindow* const window)
{
	result_init();
	asserterr(window, errcode_invalidarg);
	if (p_window)
	{
		BOOL const destroyed = DestroyWindow(p_window->hWnd);
		failret(destroyed, result_seterror(errcode_window_init));
	}
	if (gWindowPlatform.windowCount == 0)
		cdrawWindowInternalInfoRelease_win();
	result_return();
}

result_t cdrawWindowLoop(cdrawWindow* const window_opt, ptr_t const data_opt)
{
	// default data if not entered loop
	if (data_opt)
		*((int32_t*)data_opt) = 0;

	result_init();
	int16_t const accelBase = gWindowPlatform.res_control_base;
	int8_t const accelRes = gWindowPlatform.res_control_id;

	// load accelerator table
	HACCEL hAccel = LoadAcceleratorsA(gWindowPlatform.info.hInstance, MAKEINTRESOURCEA(accelRes));

	cdrawWindow* window;

	// message pump
	result_t result = 0;
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		// check for message
		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// check for accelerator key
			if (!TranslateAcceleratorA(msg.hwnd, hAccel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}
		}

		// if no message, idle
		else
		{
			// select window
			window = msg.hwnd ? (cdrawWindow*)GetWindowLongPtrA(msg.hwnd, GWLP_USERDATA) : window_opt;

			// idle callback
			if (window && cdraw_window_cancallback())
			{
				result = cdrawPluginCallOnIdle(window->p_plugin, p_window->pluginOwner);
				
				// if the result is positive, plugin threw a flag or warning
				if (result_haswarns(result))
				{
				}

				// if the result is negative, plugin threw a failure code
				// standalone window should close the window, which also unloads
				else if (result_haserror(result))
				{
					// send command to attempt to handle
					if (window && (window->control & cdrawWindowControl_ESC_command))
						cdrawWindowInternalCreateDialog(window, cdrawWindowControl_ESC_command);

					// auto-kill window
					else
						PostMessageA(msg.hwnd, WM_CLOSE, 0, 0);
				}

				// if result is zero, plugin idle succeeded, nothing happened
				// ...carry on
			}
		}
	}

	// unload accelerator
	DestroyAcceleratorTable(hAccel);

	// set result
	if (data_opt)
		*((int32_t*)data_opt) = (int32_t)msg.wParam;
	result_inc(result);
	result_return();
}

result_t cdrawWindowPluginAttach(cdrawWindow* const window, cdrawPlugin* const plugin)
{
	result_init();
	asserterr(window && p_window, errcode_invalidarg);
	asserterr(plugin && plugin->p_handle, errcode_invalidarg);
	failret(!window->p_plugin, result_seterror(errcode_window_init));
	window->p_plugin = plugin;
	p_window->pluginOwner = plugin->p_owner;
	cdraw_window_callback(cdrawPluginCallOnWindowAttach, window->sz_w, window->sz_h, window->pos_x, window->pos_y, p_window);
	result_return();
}

result_t cdrawWindowPluginDetach(cdrawWindow* const window)
{
	result_init();
	asserterr(window && p_window, errcode_invalidarg);
	failret(!cdraw_window_ownsplugin(), result_seterror(errcode_window_init));
	cdraw_window_callback(cdrawPluginCallOnWindowDetach, p_window);
	window->p_plugin = NULL;
	p_window->pluginOwner = NULL;
	result_return();
}

result_t cdrawWindowAttachToRenderer(cdrawWindow const* const window, cdrawRenderer const* const renderer, uint32_t const windowIndex)
{
	result_init();
	asserterr(window && p_window, errcode_invalidarg);
	asserterr(renderer && renderer->r, errcode_invalidarg);
	result_inc(cdrawRendererAttachWindow(renderer, windowIndex, p_window));
	result_return();
}


#endif // #if CDRAW_TARGET_WINDOWS