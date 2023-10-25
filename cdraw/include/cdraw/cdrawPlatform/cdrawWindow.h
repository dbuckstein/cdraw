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
* cdrawWindow.h
* Window management interface.
*/

#ifndef _CDRAW_WINDOW_H_
#define _CDRAW_WINDOW_H_

#include "cdrawPlugin.h"

/******************************************************************************
* Public interfaces.
******************************************************************************/

enum
{
	warncode_window_limit,							// Warning that window limit has been reached.
	errcode_window_init = errcode_CUSTOM_BEGIN,		// Failure with initialization.
};


/// <summary>
/// Window control flags.
/// </summary>
typedef enum cdrawWindowControl
{
	cdrawWindowControl_none,						// All controls disabled.
	cdrawWindowControl_F1_info = 0x0001,			// Press F1 to open player info window.
	cdrawWindowControl_F2_load = 0x0002,			// Press F2 to open plugin management window.
	cdrawWindowControl_F3_reload = 0x0004,			// Press F3 to reload current plugin.
	cdrawWindowControl_F4_unload = 0x0008,			// Press F4 to unload current plugin.
	cdrawWindowControl_F5_debug = 0x0010,			// Press F5 to load and run debug plugin.
	cdrawWindowControl_F6_build = 0x0020,			// Press F6 to hot build debug plugin, and live swap if currently running.
	cdrawWindowControl_F7_rebuild = 0x0040,			// Press F7 to hot rebuild debug plugin, and live swap if currently running.
	cdrawWindowControl_F8_fullscreen = 0x0080,		// Press F8 to toggle full-screen.
	cdrawWindowControl_F9_user_1 = 0x0100,			// Press F9 to invoke plugin's user function (1)
	cdrawWindowControl_F10_user_2 = 0x0200,			// Press F10 to invoke plugin's user function (2)
	cdrawWindowControl_F11_user_3 = 0x0400,			// Press F11 to invoke plugin's user function (3).
	cdrawWindowControl_F12_user_cmd = 0x0800,		// Press F12 to invoke plugin's user command function (also triggers breakpoint in some IDEs).
	cdrawWindowControl_ESC_command = 0x1000,		// Press ESC to enter and process command before passing it to plugin's command user function.
	cdrawWindowControl_cursor_hide = 0x2000,		// Hide cursor.
	cdrawWindowControl_cursor_lock = 0x4000,		// Lock cursor to window area.
	cdrawWindowControl_active_unfocused = 0x8000,	// Keep window active if not in focus.
	cdrawWindowControl_all = 0xffff					// All controls enabled.
} cdrawWindowControl;

/// <summary>
/// Window descriptor.
/// </summary>
typedef struct cdrawWindow
{
	/// <summary>
	/// Internal window representation.
	/// </summary>
	ptr_t p_window;
	/// <summary>
	/// Plugin (can be attached via controls or manually).
	/// </summary>
	cdrawPlugin* p_plugin;
	/// <summary>
	/// Location of window on-screen.
	/// </summary>
	int16_t pos_x, pos_y;
	/// <summary>
	/// Dimensions of window on-screen.
	/// </summary>
	int16_t sz_w, sz_h;
	/// <summary>
	/// User controls.
	/// </summary>
	cdrawWindowControl control;
} cdrawWindow;


/******************************************************************************
* Functions.
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Start single instance of application, switching to existing instance if creation of new one is attempted.
	/// </summary>
	/// <param name="p_handle_out">Pointer to unused handle.</param>
	/// <param name="instanceName">Name of application instance to track.</param>
	/// <param name="index_out_opt">Optional pointer to store index of this instance.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawApplicationStartSingleInstance(ptr_t* const p_handle_out, label_t const instanceName, int32_t* const index_out_opt);

	/// <summary>
	/// Stop single instance of application.
	/// </summary>
	/// <param name="p_handle">Pointer to handle initialized with start function.</param>
	/// <param name="available_out_opt">Optional pointer to store remaining available instances.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawApplicationStopSingleInstance(ptr_t* const p_handle, uint32_t* const available_out_opt);

	/// <summary>
	/// Start instance of application limited by counter.
	/// </summary>
	/// <param name="p_handle_out">Pointer to unused handle.</param>
	/// <param name="instanceName">Name of application instance to track.</param>
	/// <param name="limit">Maximum instance count.</param>
	/// <param name="index_out_opt">Optional pointer to store index of this instance.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawApplicationStartMultipleInstance(ptr_t* const p_handle_out, label_t const instanceName, uint32_t const limit, int32_t* const index_out_opt);

	/// <summary>
	/// Stop counted instance of application.
	/// </summary>
	/// <param name="p_handle">Pointer to handle initialized with start function.</param>
	/// <param name="available_out_opt">Optional pointer to store remaining available instances.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawApplicationStopMultipleInstance(ptr_t* const p_handle, uint32_t* const available_out_opt);

	/// <summary>
	/// Create and initialize a window.
	/// </summary>
	/// <param name="window">Target window.</param>
	/// <param name="windowName">Name of window.</param>
	/// <param name="windowPosX">Horizontal position of window.</param>
	/// <param name="windowPosY">Vertical position of window.</param>
	/// <param name="windowSzW">Width of window.</param>
	/// <param name="windowSzH">Height of window.</param>
	/// <param name="fullScreen">Option to start in full-screen.</param>
	/// <param name="control">Set of window control options.</param>
	/// <param name="data_opt">Optional platform data.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawWindowCreate(cdrawWindow* const window, label_t const windowName, int16_t const windowPosX, int16_t const windowPosY, int16_t const windowSzW, int16_t const windowSzH, bool const fullScreen, cdrawWindowControl const control, ptr_t const data_opt);

	/// <summary>
	/// Destroy and release a window.
	/// </summary>
	/// <param name="window">Target window.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawWindowRelease(cdrawWindow* const window);

	/// <summary>
	/// Enter a window's main loop to handle events.
	/// </summary>
	/// <param name="window_opt">Optional target window to track; if not included, tracks all windows.</param>
	/// <param name="data_opt">Optional platform data.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawWindowLoop(cdrawWindow* const window_opt, ptr_t const data_opt);

	/// <summary>
	/// Attach plugin to window for automated event processing.
	/// </summary>
	/// <param name="window">Target window.</param>
	/// <param name="plugin">Plugin to attach.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawWindowPluginAttach(cdrawWindow* const window, cdrawPlugin* const plugin);

	/// <summary>
	/// Detach current (if any) plugin from window.
	/// </summary>
	/// <param name="window">Target window.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawWindowPluginDetach(cdrawWindow* const window);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef _CDRAW_WINDOW_H_