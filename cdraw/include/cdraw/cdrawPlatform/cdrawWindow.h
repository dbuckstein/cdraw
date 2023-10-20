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


typedef struct cdrawWindow
{
	ptr_t p_handle;

	cdrawPlugin* p_plugin;
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
	/// <returns>Zero if success; error code otherwise.</returns>
	result_t cdrawApplicationStartSingleInstance(ptr_t* const p_handle_out, label_t const instanceName, int32_t* const index_out_opt);

	/// <summary>
	/// Stop single instance of application.
	/// </summary>
	/// <param name="p_handle">Pointer to handle initialized with start function.</param>
	/// <param name="available_out_opt">Optional pointer to store remaining available instances.</param>
	/// <returns>Zero if success; error code otherwise.</returns>
	result_t cdrawApplicationStopSingleInstance(ptr_t* const p_handle, uint32_t* const available_out_opt);

	/// <summary>
	/// Start instance of application limited by counter.
	/// </summary>
	/// <param name="p_handle_out">Pointer to unused handle.</param>
	/// <param name="instanceName">Name of application instance to track.</param>
	/// <param name="limit">Maximum instance count.</param>
	/// <param name="index_out_opt">Optional pointer to store index of this instance.</param>
	/// <returns>Zero if success; error code otherwise.</returns>
	result_t cdrawApplicationStartMultipleInstance(ptr_t* const p_handle_out, label_t const instanceName, uint32_t const limit, int32_t* const index_out_opt);

	/// <summary>
	/// Stop counted instance of application.
	/// </summary>
	/// <param name="p_handle">Pointer to handle initialized with start function.</param>
	/// <param name="available_out_opt">Optional pointer to store remaining available instances.</param>
	/// <returns>Zero if success; error code otherwise.</returns>
	result_t cdrawApplicationStopMultipleInstance(ptr_t* const p_handle, uint32_t* const available_out_opt);

	/// <summary>
	/// Attach plugin to window for automated event processing.
	/// </summary>
	/// <param name="window">Target window.</param>
	/// <param name="plugin">Plugin to attach.</param>
	/// <returns>Zero if success; error code otherwise.</returns>
	result_t cdrawWindowPluginAttach(cdrawWindow* const window, cdrawPlugin const* const plugin);

	/// <summary>
	/// Detach current (if any) plugin from window.
	/// </summary>
	/// <param name="window">Target window.</param>
	/// <returns>Zero if success; error code otherwise.</returns>
	result_t cdrawWindowPluginDetach(cdrawWindow* const window);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef _CDRAW_WINDOW_H_