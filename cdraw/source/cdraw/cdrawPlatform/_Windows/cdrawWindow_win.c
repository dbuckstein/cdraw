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
#include <stdio.h>
#include <stdlib.h>


/******************************************************************************
* Private/internal data structures and functions.
******************************************************************************/

typedef struct cdrawWindow_win
{
	HWND hWnd;						// Internal window handle.
	HDC hDC;						// Internal device context handle.
	RECT restoreArea;				// Non-full-screen area.
	BOOL buildState;				// Hot-build tracker.
	BOOL mouseState;				// Mouse hovering tracker.
	TRACKMOUSEEVENT mouseTrack;		// Mouse tracking for entering/exiting window.
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

result_t cdrawWindowInternalUnlockPDB_win(label_t const sdkDirStr, label_t const cfgDirStr, label_t const projName);

static LRESULT __stdcall cdrawWindowInternalEvent_win(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cdrawWindow* window = (cdrawWindow*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
	cdrawWindow_win* p_window;
	switch (message)
	{
	case WM_NCCREATE: {
		cdraw_assert(window == NULL);
		p_window = (cdrawWindow_win*)malloc(sizeof(cdrawWindow_win));
		if (p_window)
		{
			memset(p_window, 0, sizeof(cdrawWindow_win));
			p_window->hWnd = hWnd;
			p_window->hDC = GetDC(hWnd);
			p_window->mouseTrack.cbSize = sizeof(p_window->mouseTrack);
			p_window->mouseTrack.hwndTrack = hWnd;
			p_window->mouseTrack.dwFlags = (TME_LEAVE);
			TrackMouseEvent(&p_window->mouseTrack);

			window = (cdrawWindow*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
			SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)window);
			
			window->p_window = p_window;
			++gWindowPlatform.windowCount;
		}
	}	break;
	case WM_CREATE: {
		// NOTHING ELSE TO DO
	}	break;
	case WM_CLOSE: {
		// recursively take down windows
		DestroyWindow(hWnd);
	}	break;
	case WM_DESTROY: {
		if (window->p_plugin && window->p_plugin->p_owner == window)
			cdrawPluginUnload(window->p_plugin, window);
		p_window = (cdrawWindow_win*)window->p_window;
		if (p_window)
		{
			ReleaseDC(hWnd, p_window->hDC);
			free(p_window);
			window->p_window = NULL;
		}

		// terminate loop if all windows closed
		--gWindowPlatform.windowCount;
		if (gWindowPlatform.windowCount <= 0)
			PostQuitMessage(0);
	}	break;
	case WM_PAINT: {

	}	break;
	default:
		break;
	}
	return DefWindowProcA(hWnd, message, wParam, lParam);
}

static LRESULT CALLBACK cdrawWindowInternalEventProcessList(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	// do not pass unprocessed messages to DefDlgProc, recursive
	return FALSE;
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
	failassertret(handle, errcode_window_init);

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

static void cdrawWindowInternalLockCursor(cdrawWindow* const window)
{
	if (window && window->p_window)
	{
		cdrawWindow_win* const p_window = (cdrawWindow_win*)window->p_window;
		HWND const handle = p_window->hWnd;
		POINT corner_tl, corner_br;
		RECT displayArea;
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

static void cdrawWindowInternalToggleFullScreen(cdrawWindow* const window)
{
	MONITORINFO monitorArea = { sizeof(MONITORINFO) };
	RECT displayArea = { 0 };

	cdrawWindow_win* const p_window = (cdrawWindow_win*)window->p_window;
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

static void cdrawWindowInternalCreateDialog(cdrawWindow* const window, cdrawWindowControl const purpose)
{
	cdrawWindow_win* const p_window = (cdrawWindow_win*)window->p_window;
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
	failret(released, errcode_window_init);

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
	failassertret(handle, errcode_window_init);

	dword_t status = GetLastError();

	// get signal state, decrements count
	dword_t signal = WaitForSingleObject(handle, 0);

	bool const signaled = (signal == WAIT_OBJECT_0) || (signal == WAIT_TIMEOUT);
	if (!signaled)
	{
		status = CloseHandle(handle);
		failret(signaled, errcode_window_init);
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
	failret(released, errcode_window_init);

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
	asserterr(window && !window->p_window, errcode_invalidarg);
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
	failret(cdrawWindowInternalInfoReady_win(), errcode_window_init);

	handle = CreateWindowExA(
		styleEx, gWindowPlatform.info.lpszClassName, windowName, style,
		windowPosX, windowPosY, (displayArea.right - displayArea.left), (displayArea.bottom - displayArea.top),
		NULL, NULL, gWindowPlatform.info.hInstance, window);
	if (handle && !window->p_window)
	{
		// window created but not initialized correctly
		DestroyWindow(handle);
		handle = NULL;
	}
	if (gWindowPlatform.windowCount == 0)
		cdrawWindowInternalInfoRelease_win();
	failret(handle, errcode_window_init);

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

result_t cdrawWindowRelease(cdrawWindow* const window)
{
	result_init();
	asserterr(window, errcode_invalidarg);
	cdrawWindow_win* const p_window = (cdrawWindow_win*)window->p_window;
	if (p_window)
	{
		BOOL const destroyed = DestroyWindow(p_window->hWnd);
		failret(destroyed, errcode_window_init);
	}
	if (gWindowPlatform.windowCount == 0)
		cdrawWindowInternalInfoRelease_win();
	result_return();
}

result_t cdrawWindowLoop(ptr_t const data_opt)
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
	cdrawWindow_win* p_window;

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
			window = msg.hwnd ? (cdrawWindow*)GetWindowLongPtrA(msg.hwnd, GWLP_USERDATA) : NULL;
			p_window = window ? (cdrawWindow_win*)window->p_window : NULL;

			// idle callback
			if (window && window->p_plugin)
				result = cdrawPluginCallOnIdle(window->p_plugin, window->p_plugin->p_owner);

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

	// unload accelerator
	DestroyAcceleratorTable(hAccel);

	// set result
	if (data_opt)
		*((int32_t*)data_opt) = (int32_t)msg.wParam;
	result_inc(result);
	result_return();
}

result_t cdrawWindowPluginAttach(cdrawWindow* const window, cdrawPlugin const* const plugin)
{
	result_init();

	result_return();
}

result_t cdrawWindowPluginDetach(cdrawWindow* const window)
{
	result_init();

	result_return();
}


#endif // #if CDRAW_TARGET_WINDOWS