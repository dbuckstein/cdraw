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


/******************************************************************************
* Private/internal data structures and functions.
******************************************************************************/

typedef struct cdrawWindowPlatform_win
{
	WNDCLASSEXA info;
	ATOM info_reg;
	label_t name;
	size_t windowCount;

	cstrk_t dir_build, dir_target, dir_sdk, tag_cfg;
	int16_t res_control_base;
	int8_t res_control_id;
	int8_t res_dialog;
} cdrawWindowPlatform_win;
static cdrawWindowPlatform_win gWindowPlatform;


CDRAW_INL bool cdrawWindowPlatformIsInit_win()
{
	return (gWindowPlatform.info.hInstance != NULL);
}

CDRAW_INL bool cdrawWindowPlatformIsHandleValid_win(HINSTANCE const inst)
{
	return (gWindowPlatform.info.hInstance == inst)
		|| (!inst && gWindowPlatform.info.hInstance == GetModuleHandle(NULL));
}

CDRAW_INL bool cdrawWindowInternalInfoCreate_win()
{
	if (!cdrawWindowPlatformIsInit_win())
		return false;

	gWindowPlatform.info_reg = RegisterClassExA(&gWindowPlatform.info);
	return (gWindowPlatform.info_reg != 0);
}

CDRAW_INL bool cdrawWindowInternalInfoRelease_win()
{
	if (!cdrawWindowPlatformIsInit_win())
		return false;
	if (gWindowPlatform.info_reg == 0)
		return true;
	BOOL const unregistered = UnregisterClassA(gWindowPlatform.info.lpszClassName, gWindowPlatform.info.hInstance);
	return (unregistered != 0);
}

result_t cdrawWindowInternalUnlockPDB_win(label_t const sdkDirStr, label_t const cfgDirStr, label_t const projName);

static LRESULT __stdcall cdrawWindowInternalEvent_win(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcA(hWnd, message, wParam, lParam);
}

bool cdrawWindowInternalPlatformInit_win(HINSTANCE const inst,
	cstrk_t const dir_build, cstrk_t const dir_target, cstrk_t const dir_sdk, cstrk_t const tag_cfg,
	int16_t const res_control_base, int8_t const res_control_id, int8_t const res_dialog, int8_t const res_cursor, int8_t const res_icon)
{
	if (cdrawWindowPlatformIsInit_win() || !inst)
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
	gWindowPlatform.info.cbWndExtra = (int)sizeof(ptr_t);
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
	if (!cdrawWindowPlatformIsInit_win() || !cdrawWindowPlatformIsHandleValid_win(inst))
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

result_t cdrawWindowCreate(cdrawWindow* const window, label_t const windowName, int16_t const windowPosX, int16_t const windowPosY, int16_t const windowSzW, int16_t const windowSzH, bool const fullScreen, cdrawWindowControl const control)
{
	result_init();

	result_return();
}

result_t cdrawWindowRelease(cdrawWindow* const window)
{
	result_init();

	result_return();
}

result_t cdrawWindowLoop(cdrawWindow* const window)
{
	result_init();

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