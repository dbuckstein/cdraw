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
* cdrawPlayer_WinMain.c
* Windows entry point for player application.
*/

#include "cdraw/cdraw/cdrawConfig.h"
#if CDRAW_USING_WINDOWS
#include <Windows.h>
#include "../cdrawPlayer.rc.h"


/******************************************************************************
* Private functions exposed locally.
******************************************************************************/

/// <summary>
/// Platform-independent entry point.
/// </summary>
/// <param name="cmd">Command line passed to application.</param>
/// <param name="option">Optional platform data.</param>
/// <returns>Zero if clean; window wparam on quit.</returns>
result_t cdrawPlayer_main(cstrk_t const cmd, ptr_t const data_opt);

/// <summary>
/// Platform-dependent windowing initialization (Windows).
/// This is private because using it in any other way could have unintended results.
/// </summary>
/// <param name="inst">Application handle.</param>
/// <param name="dir_build">Build tool directory.</param>
/// <param name="dir_target">Build target directory.</param>
/// <param name="dir_sdk">Framework directory.</param>
/// <param name="tag_cfg">Configuration summary.</param>
/// <param name="res_control_base">Accelerator table base.</param>
/// <param name="res_control_id">Accelerator table resource id.</param>
/// <param name="res_cursor">Cursor resource id.</param>
/// <param name="res_dialog">Dialog box resource id.</param>
/// <param name="res_icon">Icon resource id.</param>
/// <returns>True if windowing has been initialized.</returns>
bool cdrawWindowInternalPlatformInit_win(HINSTANCE const inst,
	cstrk_t const dir_build, cstrk_t const dir_target, cstrk_t const dir_sdk, cstrk_t const tag_cfg,
	int16_t const res_control_base, int8_t const res_control_id, int8_t const res_dialog, int8_t const res_cursor, int8_t const res_icon);

/// <summary>
/// Platform-dependent windowing termination (Windows).
/// This is private because using it in any other way could have unintended results.
/// </summary>
/// <param name="inst">Application handle.</param>
/// <returns>True if windowing has been terminated.</returns>
bool cdrawWindowInternalPlatformTerm_win(HINSTANCE const inst);


/******************************************************************************
* Platform entry point.
******************************************************************************/

int WINAPI WinMain(
	_In_		HINSTANCE 	hInstance,
	_In_opt_	HINSTANCE 	hPrevInstance,
	_In_		LPSTR 		lpCmdLine,
	_In_		int 		nCmdShow)
{
	int result = nCmdShow;
	cdrawWindowInternalPlatformInit_win(hInstance,
		cdraw_envstr_vsdevenv, cdraw_envstr_sln, cdraw_envstr_root, cdraw_envstr_cfg,
		ID_ACCEL_F1, IDR_ACCEL, IDD_DIALOGBAR, -1, IDI_ICON1);
	cdrawPlayer_main(lpCmdLine, &result);
	cdrawWindowInternalPlatformTerm_win(hInstance);
	return result;
}

#endif // #if CDRAW_USING_WINDOWS