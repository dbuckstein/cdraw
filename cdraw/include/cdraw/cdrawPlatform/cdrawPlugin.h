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
* cdrawPlugin.h
* Plugin management interface.
*/

#ifndef _CDRAW_PLUGIN_H_
#define _CDRAW_PLUGIN_H_


#include "cdraw/cdraw/cdrawConfig.h"


/******************************************************************************
* Public interfaces.
******************************************************************************/

enum
{
	errcode_plugin_init = errcode_CUSTOM_BEGIN,	// Failure with initialization.
};


/// <summary>
/// Short description of loadable plugin.
/// </summary>
typedef struct cdrawPluginInfo
{
	/// <summary>
	/// Short name of plugin.
	/// </summary>
	label_t name;
	/// <summary>
	/// Short name of dynamic library (no extension).
	/// </summary>
	label_t dylib;
	/// <summary>
	/// Short name of author/creator of plugin.
	/// </summary>
	label_t author;
	/// <summary>
	/// Short description of plugin.
	/// </summary>
	label_t version;
	/// <summary>
	/// Longer description of plugin (purpose/activities).
	/// </summary>
	label_long_t details;
} cdrawPluginInfo;


/// <summary>
/// Function pointer for plugin callback.
/// </summary>
typedef result_t(*cdrawPluginCallback)(ptr_t data, ...);


/// <summary>
/// Description of active plugin.
/// </summary>
typedef struct cdrawPlugin
{
	/// <summary>
	/// Pointer to handle of loaded plugin.
	/// </summary>
	ptr_t p_handle;
	/// <summary>
	/// Pointer to owner object responsible for loading and unloading plugin.
	/// </summary>
	ptrk_t p_owner;
	/// <summary>
	/// Pointer to user data consumed by plugin; allocation and deallocation may be managed by owner or plugin itself.
	/// </summary>
	ptr_t p_data;
	/// <summary>
	/// Index of plugin in managed list.
	/// </summary>
	ptrdiff_t i_list;
	/// <summary>
	/// Callback list.
	/// </summary>
	struct
	{
		cdrawPluginCallback cb_load, cb_hotload;												// Loading callbacks.
		cdrawPluginCallback cb_pre_reload, cb_reload;											// Reloading callbacks.
		cdrawPluginCallback cb_pre_unload, cb_unload;											// Unloading callbacks.
		cdrawPluginCallback cb_win_attach, cb_win_detach;										// Window attachment callbacks.
		cdrawPluginCallback cb_win_activate, cb_win_deactivate;									// Window activation callbacks.
		cdrawPluginCallback cb_win_resize, cb_win_move;											// Window shape callbacks.
		cdrawPluginCallback cb_display, cb_idle;												// Window display/idle callbacks.
		cdrawPluginCallback cb_virtkey_press, cb_virtkey_hold, cb_virtkey_release;				// Virtual key event callbacks.
		cdrawPluginCallback cb_key_press, cb_key_hold, cb_key_release;							// Character key event callbacks.
		cdrawPluginCallback cb_mouse_press, cb_mouse_release, cb_mouse_double, cb_mouse_wheel;	// Mouse button event callbacks.
		cdrawPluginCallback cb_mouse_move, cb_mouse_drag, cb_mouse_enter, cb_mouse_leave;		// Mouse motion event callbacks.
		cdrawPluginCallback cb_user_1, cb_user_2, cb_user_3, cb_user_cmd;						// User function callbacks (e.g. F9-11 and F12/ESC for command).
	};
} cdrawPlugin;


/******************************************************************************
* Functions.
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Initialize plugin info descriptor.
	/// </summary>
	/// <param name="pluginInfo">Plugin info descriptor.</param>
	/// <param name="name"></param>
	/// <param name="dylib"></param>
	/// <param name="author"></param>
	/// <param name="version"></param>
	/// <param name="details"></param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginInfoInit(cdrawPluginInfo* const pluginInfo, label_t const name, label_t const dylib, label_t const author, label_t const version, label_long_t const details);

	/// <summary>
	/// Initialize default plugin info descriptor with basic info.
	/// </summary>
	/// <param name="pluginInfo">Plugin info descriptor.</param>
	/// <param name="author"></param>
	/// <param name="version"></param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginInfoInitDefault(cdrawPluginInfo* const pluginInfo, label_t const author, label_t const version);

	/// <summary>
	/// Reset plugin info descriptor.
	/// </summary>
	/// <param name="pluginInfo">Plugin info descriptor.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginInfoReset(cdrawPluginInfo* const pluginInfo);

	/// <summary>
	/// Load list of plugin info descriptors from resource file.
	/// </summary>
	/// <param name="pluginInfoList_out">List of plugin info descriptors (points to null).</param>
	/// <param name="count_out"></param>
	/// <param name="listFile"></param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginInfoListLoad(cdrawPluginInfo** const pluginInfoList_out, size_t* const count_out, cstrk_t const listFile);

	/// <summary>
	/// Release loaded list of plugin info descriptors.
	/// </summary>
	/// <param name="pluginInfoList">List of plugin info descriptors (points to valid descriptors).</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginInfoListRelease(cdrawPluginInfo** const pluginInfoList);

	/// <summary>
	/// Reset plugin descriptor.
	/// </summary>
	/// <param name="plugin">Plugin descriptor.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginReset(cdrawPlugin* const plugin);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef _CDRAW_PLUGIN_H_