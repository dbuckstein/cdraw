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
	errcode_plugin_callback,					// Failure invoking callback.
	errcode_plugin_CUSTOM_BEGIN					// Begin internal plugin error codes.
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
	/// <summary>
	/// List of user-defined named callback hooks.
	/// </summary>
	label_t callbacks[32];
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
	/// Info of plugin used.
	/// </summary>
	cdrawPluginInfo info;
	/// <summary>
	/// Index of plugin in managed list.
	/// </summary>
	ptrdiff_t id;
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

	union {
		/// <summary>
		/// Callback list.
		/// </summary>
		cdrawPluginCallback cb[32];
		struct
		{
			cdrawPluginCallback cb_load_post, cb_hotload_pre, cb_hotload_post;						// Loading/hotloading callbacks.
			cdrawPluginCallback cb_reload_pre, cb_reload_post, cb_unload_pre;						// Reloading/unloading callbacks.
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
	/// <param name="name">Short name of descriptor.</param>
	/// <param name="dylib">Short name of library file.</param>
	/// <param name="author">Short author string.</param>
	/// <param name="version">Short version string.</param>
	/// <param name="details">Longer details string.</param>
	/// <param name="callbackNames">List of callback names.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginInfoInit(cdrawPluginInfo* const pluginInfo, label_t const name, label_t const dylib, label_t const author, label_t const version, label_long_t const details, label_t const callbackNames[32]);

	/// <summary>
	/// Initialize default plugin info descriptor with basic info.
	/// </summary>
	/// <param name="pluginInfo">Plugin info descriptor.</param>
	/// <param name="author">Short author string.</param>
	/// <param name="version">Short version string.</param>
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
	/// <param name="count_out">Pointer to storage for number of info descriptors loaded.</param>
	/// <param name="listFile">Path to file.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginInfoListLoad(cdrawPluginInfo** const pluginInfoList_out, size_t* const count_out, cstrk_t const listFile);

	/// <summary>
	/// Release loaded list of plugin info descriptors.
	/// </summary>
	/// <param name="pluginInfoList">List of plugin info descriptors (points to valid descriptors).</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginInfoListRelease(cdrawPluginInfo** const pluginInfoList);

	/// <summary>
	/// Print info to string.
	/// </summary>
	/// <param name="pluginInfo">Plugin info descriptor.</param>
	/// <param name="stringPtr">Pointer to string to be concatenated with info (user must ensure buffer has enough space).</param>
	/// <returns></returns>
	result_t cdrawPluginInfoPrint(cdrawPluginInfo const* const pluginInfo, cstr_t* const stringPtr);

	/// <summary>
	/// Reset plugin descriptor.
	/// </summary>
	/// <param name="plugin">Plugin descriptor.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginReset(cdrawPlugin* const plugin);

	/// <summary>
	/// Load plugin dynamic library.
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="pluginInfo"></param>
	/// <param name="id"></param>
	/// <param name="owner_opt">Pointer to owner of plugin for control over its manipulation.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginLoad(cdrawPlugin* const plugin, cdrawPluginInfo const* const pluginInfo, ptrdiff_t const id, ptrk_t const owner_opt);

	/// <summary>
	/// Reload plugin dynamic library.
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginReload(cdrawPlugin* const plugin, ptrk_t const caller);

	/// <summary>
	/// Unload plugin dynamic library.
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawPluginUnload(cdrawPlugin* const plugin, ptrk_t const caller);

	/// <summary>
	/// Call to safely invoke window attach callback with format: func(ptr_t data, int32_t w, int32_t h, int32_t x, int32_t y).
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="w">Horizontal dimension (width).</param>
	/// <param name="h">Vertical dimension (height).</param>
	/// <param name="x">Horizontal position.</param>
	/// <param name="y">Vertical position.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnWindowAttach(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const w, int32_t const h, int32_t const x, int32_t const y);
	
	/// <summary>
	/// Call to safely invoke window detach callback with format: func(ptr_t data).
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnWindowDetach(cdrawPlugin const* const plugin, ptrk_t const caller);
	
	/// <summary>
	/// Call to safely invoke window activate callback with format: func(ptr_t data)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnWindowActivate(cdrawPlugin const* const plugin, ptrk_t const caller);
	
	/// <summary>
	/// Call to safely invoke window deactivate callback with format: func(ptr_t data)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnWindowDeactivate(cdrawPlugin const* const plugin, ptrk_t const caller);
	
	/// <summary>
	/// Call to safely invoke window resize callback with format: func(ptr_t data, int32_t w, int32_t h)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="w">Horizontal dimension (width).</param>
	/// <param name="h">Vertical dimension (height).</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnWindowResize(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const w, int32_t const h);
	
	/// <summary>
	/// Call to safely invoke window move callback with format: func(ptr_t data, int32_t x, int32_t y)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="x">Horizontal position.</param>
	/// <param name="y">Vertical position.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnWindowMove(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const x, int32_t const y);
	
	/// <summary>
	/// Call to safely invoke display callback with format: func(ptr_t data)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnDisplay(cdrawPlugin const* const plugin, ptrk_t const caller);
	
	/// <summary>
	/// Call to safely invoke idle callback with format: func(ptr_t data)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnIdle(cdrawPlugin const* const plugin, ptrk_t const caller);
	
	/// <summary>
	/// Call to safely invoke virtual key press callback with format: func(ptr_t data, int32_t virtkey)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="virtkey">Virtual key code.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnVirtkeyPress(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const virtkey);
	
	/// <summary>
	/// Call to safely invoke virtual key hold callback with format: func(ptr_t data, int32_t virtkey)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="virtkey">Virtual key code.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnVirtkeyHold(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const virtkey);
	
	/// <summary>
	/// Call to safely invoke virtual key release callback with format: func(ptr_t data, int32_t virtkey)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="virtkey">Virtual key code.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnVirtkeyRelease(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const virtkey);
	
	/// <summary>
	/// Call to safely invoke ASCII key press callback with format: func(ptr_t data, int32_t key)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="key">ASCII key code.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnKeyPress(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const key);
	
	/// <summary>
	/// Call to safely invoke ASCII key hold callback with format: func(ptr_t data, int32_t key)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="key">ASCII key code.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnKeyHold(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const key);
	
	/// <summary>
	/// Call to safely invoke ASCII key release callback with format: func(ptr_t data, int32_t key)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="key">ASCII key code.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnKeyRelease(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const key);
	
	/// <summary>
	/// Call to safely invoke mouse button press callback with format: func(ptr_t data, int32_t btn, int32_t x, int32_t y)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="btn">Mouse button.</param>
	/// <param name="x">Horizontal position.</param>
	/// <param name="y">Vertical position.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnMousePress(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const btn, int32_t const x, int32_t const y);
	
	/// <summary>
	/// Call to safely invoke mouse button release callback with format: func(ptr_t data, int32_t btn, int32_t x, int32_t y)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="btn">Mouse button.</param>
	/// <param name="x">Horizontal position.</param>
	/// <param name="y">Vertical position.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnMouseRelease(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const btn, int32_t const x, int32_t const y);
	
	/// <summary>
	/// Call to safely invoke mouse button double-click callback with format: func(ptr_t data, int32_t btn, int32_t x, int32_t y)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="btn">Mouse button.</param>
	/// <param name="x">Horizontal position.</param>
	/// <param name="y">Vertical position.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnMouseDouble(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const btn, int32_t const x, int32_t const y);
	
	/// <summary>
	/// Call to safely invoke mouse wheel roll callback with format: func(ptr_t data, int32_t delta, int32_t x, int32_t y)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="delta">Wheel delta.</param>
	/// <param name="x">Horizontal position.</param>
	/// <param name="y">Vertical position.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnMouseWheel(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const delta, int32_t const x, int32_t const y);
	
	/// <summary>
	/// Call to safely invoke mouse move (no buttons) callback with format: func(ptr_t data, int32_t x, int32_t y)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="x">Horizontal position.</param>
	/// <param name="y">Vertical position.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnMouseMove(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const x, int32_t const y);
	
	/// <summary>
	/// Call to safely invoke mouse drag (move with button pressed) callback with format: func(ptr_t data, int32_t x, int32_t y)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="x">Horizontal position.</param>
	/// <param name="y">Vertical position.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnMouseDrag(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const x, int32_t const y);
	
	/// <summary>
	/// Call to safely invoke mouse enter window callback with format: func(ptr_t data, int32_t x, int32_t y)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="x">Horizontal position.</param>
	/// <param name="y">Vertical position.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnMouseEnter(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const x, int32_t const y);
	
	/// <summary>
	/// Call to safely invoke mouse leave window callback with format: func(ptr_t data, int32_t x, int32_t y)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="x">Horizontal position.</param>
	/// <param name="y">Vertical position.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnMouseLeave(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const x, int32_t const y);
	
	/// <summary>
	/// Call to safely invoke first user custom callback with format: func(ptr_t data)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnUser1(cdrawPlugin const* const plugin, ptrk_t const caller);
	
	/// <summary>
	/// Call to safely invoke second user custom callback with format: func(ptr_t data)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnUser2(cdrawPlugin const* const plugin, ptrk_t const caller);
	
	/// <summary>
	/// Call to safely invoke third user custom callback with format: func(ptr_t data)
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnUser3(cdrawPlugin const* const plugin, ptrk_t const caller);
	
	/// <summary>
	/// Call to safely invoke user custom command callback with format: func(ptr_t data, int32_t argc, cstrk_t argv[])
	/// </summary>
	/// <param name="plugin">Target plugin.</param>
	/// <param name="caller">Pointer to caller of this function; must match owner.</param>
	/// <param name="argc">Command argument count.</param>
	/// <param name="argv">Command argument string array.</param>
	/// <returns>Zero if success, error code otherwise.</returns>	
	result_t cdrawPluginCallOnUserCmd(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const argc, cstrk_t const argv[]);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef _CDRAW_PLUGIN_H_