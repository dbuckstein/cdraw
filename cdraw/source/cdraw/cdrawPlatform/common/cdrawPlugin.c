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
* cdrawPlugin.c
* Common implementation of plugin management.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cdraw/cdrawPlatform/cdrawPlugin.h"


/******************************************************************************
* Private/internal functions.
******************************************************************************/

cstr_t cdrawPluginInternalDylibGetExt(cstr_t const ext_out, size_t const arrlen_ext);
ptr_t cdrawPluginInternalDylibLoad(cstrk_t const path);
bool cdrawPluginInternalDylibUnload(ptr_t const handle);
ptr_t cdrawPluginInternalDylibGetSymbol(ptrk_t const handle, cstrk_t const name);


result_t cdrawPluginCallbackDefault(ptr_t data, ...)
{
	result_init();
	result_return();
}


#define cbname_load_post			cb_load_post
#define cbname_hotload_pre			cb_hotload_pre
#define cbname_hotload_post			cb_hotload_post
#define cbname_reload_pre			cb_reload_pre
#define cbname_reload_post			cb_reload_post
#define cbname_unload_pre			cb_unload_pre
#define cbname_win_attach			cb_win_attach
#define cbname_win_detach			cb_win_detach
#define cbname_win_activate			cb_win_activate
#define cbname_win_deactivate		cb_win_deactivate
#define cbname_win_resize			cb_win_resize
#define cbname_win_move				cb_win_move
#define cbname_display				cb_display
#define cbname_idle					cb_idle
#define cbname_virtkey_press		cb_virtkey_press
#define cbname_virtkey_hold			cb_virtkey_hold
#define cbname_virtkey_release		cb_virtkey_release
#define cbname_key_press			cb_key_press
#define cbname_key_hold				cb_key_hold
#define cbname_key_release			cb_key_release
#define cbname_mouse_press			cb_mouse_press
#define cbname_mouse_release		cb_mouse_release
#define cbname_mouse_double			cb_mouse_double
#define cbname_mouse_wheel			cb_mouse_wheel
#define cbname_mouse_move			cb_mouse_move
#define cbname_mouse_drag			cb_mouse_drag
#define cbname_mouse_enter			cb_mouse_enter
#define cbname_mouse_leave			cb_mouse_leave
#define cbname_user_1				cb_user_1
#define cbname_user_2				cb_user_2
#define cbname_user_3				cb_user_3
#define cbname_user_cmd				cb_user_cmd
static label_t const gPluginCallbackHook[32] =
{
	tokenstr(cbname_load_post),
	tokenstr(cbname_hotload_pre),
	tokenstr(cbname_hotload_post),
	tokenstr(cbname_reload_pre),
	tokenstr(cbname_reload_post),
	tokenstr(cbname_unload_pre),
	tokenstr(cbname_win_attach),
	tokenstr(cbname_win_detach),
	tokenstr(cbname_win_activate),
	tokenstr(cbname_win_deactivate),
	tokenstr(cbname_win_resize),
	tokenstr(cbname_win_move),
	tokenstr(cbname_display),
	tokenstr(cbname_idle),
	tokenstr(cbname_virtkey_press),
	tokenstr(cbname_virtkey_hold),
	tokenstr(cbname_virtkey_release),
	tokenstr(cbname_key_press),
	tokenstr(cbname_key_hold),
	tokenstr(cbname_key_release),
	tokenstr(cbname_mouse_press),
	tokenstr(cbname_mouse_release),
	tokenstr(cbname_mouse_double),
	tokenstr(cbname_mouse_wheel),
	tokenstr(cbname_mouse_move),
	tokenstr(cbname_mouse_drag),
	tokenstr(cbname_mouse_enter),
	tokenstr(cbname_mouse_leave),
	tokenstr(cbname_user_1),
	tokenstr(cbname_user_2),
	tokenstr(cbname_user_3),
	tokenstr(cbname_user_cmd),
};

CDRAW_INL void cdrawPluginInfoInternalResetCallbackNames(cdrawPluginInfo* const pluginInfo)
{
	size_t i;
	size_t const n = buffer_len(pluginInfo->callbacks);
	for (i = 0; i < n; ++i)
		strncpy(pluginInfo->callbacks[i], gPluginCallbackHook[i], sizeof(*pluginInfo->callbacks));
}

CDRAW_INL void cdrawPluginInternalResetCallbacks(cdrawPluginCallback callback[], size_t const numCallbacks)
{
	cdraw_assert(callback);
	size_t i;
	for (i = 0; i < numCallbacks; ++i)
	{
		callback[i] = cdrawPluginCallbackDefault;
	}
}

CDRAW_INL void cdrawPluginInternalSetCallbacks(cdrawPluginCallback callback[], label_t const callbackNames[], ptr_t const handle, size_t const numCallbacks)
{
	cdraw_assert(callback && callbackNames);
	size_t i;
	ptr_t cb;
	for (i = 0; i < numCallbacks; ++i)
	{
		cb = cdrawPluginInternalDylibGetSymbol(handle, callbackNames[i]);
		callback[i] = cb ? cb : cdrawPluginCallbackDefault;
	}
}

CDRAW_INL cstr_t cdrawPluginInfoInternalRead(cstr_t const data_out, size_t const data_len, label_t data_alt_out_opt, cstr_t buffer, byte_t const delim)
{
	cdraw_assert(data_out && data_len && buffer_valid(buffer));
	cstr_t write = data_out;
	size_t count = data_len - 1, written = 0;

	// find open quote
	while (*buffer)
	{
		// open
		if (*(buffer++) == '\"')
		{
			// alt
			if (data_alt_out_opt && delim)
			{
				count = sizeof(label_t) - 1;
				write = data_alt_out_opt;
			}
			*write = 0;
			break;
		}
	}

	// open
	while (*buffer)
	{
		// closed
		if (*buffer == '\"')
		{
			*write = 0;
			return (++buffer);
		}
		// switch
		else if (*buffer == delim)
		{
			count = data_len - 1;
			write = data_out;
			*write = 0;
			written = 0;
		}
		// write
		else if (written < count)
		{
			*(write++) = *buffer;
			++written;
		}
		// fail
		else
		{
			break;
		}
		// next
		++buffer;
	}

	// fail
	return NULL;
}

CDRAW_INL ptr_t cdrawPluginInternalLoad(cdrawPlugin* const plugin, cdrawPluginInfo const* const pluginInfo)
{
	ptr_t handle = NULL;
	label_long_t path = "./cdrawPlugin/";
	label_t ext;
	failassertret(label_long_valid(pluginInfo->dylib), NULL);
	strcat(strcat(path, pluginInfo->dylib), cdrawPluginInternalDylibGetExt(ext, sizeof(ext)));
	handle = cdrawPluginInternalDylibLoad(path);
	failassertret(handle, NULL);
	cdrawPluginInternalSetCallbacks(plugin->cb, pluginInfo->callbacks, handle, buffer_len(plugin->cb));
	return handle;
}

CDRAW_INL bool cdrawPluginInternalUnload(cdrawPlugin* const plugin)
{
	cdrawPluginInternalResetCallbacks(plugin->cb, buffer_len(plugin->cb));
	return cdrawPluginInternalDylibUnload(plugin->p_handle);
}


/// <summary>
/// Call to safely invoke post-load callback with format: func(ptr_t* data_inout).
/// </summary>
/// <param name="plugin">Target plugin.</param>
/// <param name="caller">Pointer to caller of this function; must match owner.</param>
/// <returns>Zero if success; Error code otherwise.</returns>
static result_t cdrawPluginCallPostLoad(cdrawPlugin* const plugin, ptrk_t const caller);

/// <summary>
/// Call to safely invoke pre-hotload callback with format: func(ptr_t* data_inout).
/// </summary>
/// <param name="plugin">Target plugin.</param>
/// <param name="caller">Pointer to caller of this function; must match owner.</param>
/// <returns>Zero if success; Error code otherwise.</returns>
static result_t cdrawPluginCallPreHotload(cdrawPlugin* const plugin, ptrk_t const caller);

/// <summary>
/// Call to safely invoke post-hotload callback with format: func(ptr_t* data_inout).
/// </summary>
/// <param name="plugin">Target plugin.</param>
/// <param name="caller">Pointer to caller of this function; must match owner.</param>
/// <returns>Zero if success; Error code otherwise.</returns>
static result_t cdrawPluginCallPostHotload(cdrawPlugin* const plugin, ptrk_t const caller);

/// <summary>
/// Call to safely invoke pre-reload callback with format: func(ptr_t* data_inout).
/// </summary>
/// <param name="plugin">Target plugin.</param>
/// <param name="caller">Pointer to caller of this function; must match owner.</param>
/// <returns>Zero if success; Error code otherwise.</returns>
static result_t cdrawPluginCallPreReload(cdrawPlugin* const plugin, ptrk_t const caller);

/// <summary>
/// Call to safely invoke post-reload callback with format: func(ptr_t* data_inout).
/// </summary>
/// <param name="plugin">Target plugin.</param>
/// <param name="caller">Pointer to caller of this function; must match owner.</param>
/// <returns>Zero if success; Error code otherwise.</returns>
static result_t cdrawPluginCallPostReload(cdrawPlugin* const plugin, ptrk_t const caller);

/// <summary>
/// Call to safely invoke pre-unload callback with format: func(ptr_t* data_inout).
/// </summary>
/// <param name="plugin">Target plugin.</param>
/// <param name="caller">Pointer to caller of this function; must match owner.</param>
/// <returns>Zero if success; Error code otherwise.</returns>	
static result_t cdrawPluginCallPreUnload(cdrawPlugin* const plugin, ptrk_t const caller);


/******************************************************************************
* Public functions.
******************************************************************************/

result_t cdrawPluginInfoInit(cdrawPluginInfo* const pluginInfo, label_t const name, label_t const dylib, label_t const author, label_t const version, label_long_t const details, label_t const callbackNames[32])
{
	result_init();
	asserterr_ptr(pluginInfo, errcode_invalidarg);

	cdrawPluginInfo const def = { "cdraw Plugin (SDK)", "cdrawPlugin", "Daniel S. Buckstein", (__TIME__ "; " __DATE__), "Default plugin built and loaded with current SDK configuration." };
	strncpy(pluginInfo->name, label_valid(name) ? name : def.name, sizeof(def.name));
	strncpy(pluginInfo->dylib, label_valid(dylib) ? dylib : def.dylib, sizeof(def.dylib));
	strncpy(pluginInfo->author, label_valid(author) ? author : def.author, sizeof(def.author));
	strncpy(pluginInfo->version, label_valid(version) ? version : def.version, sizeof(def.version));
	strncpy(pluginInfo->details, label_long_valid(details) ? details : def.details, sizeof(def.details));
	if (callbackNames)
	{
		size_t i;
		size_t const n = buffer_len(pluginInfo->callbacks);
		for (i = 0; i < n; ++i)
			strncpy(pluginInfo->callbacks[i], label_valid(callbackNames[i]) ? callbackNames[i] : gPluginCallbackHook[i], sizeof(*pluginInfo->callbacks));
	}
	else
	{
		cdrawPluginInfoInternalResetCallbackNames(pluginInfo);
	}

	result_return();
}

result_t cdrawPluginInfoInitDefault(cdrawPluginInfo* const pluginInfo, label_t const author, label_t const version)
{
	return cdrawPluginInfoInit(pluginInfo, 0, 0, author, version, 0, 0);
}

result_t cdrawPluginInfoReset(cdrawPluginInfo* const pluginInfo)
{
	return cdrawPluginInfoInit(pluginInfo, 0, 0, 0, 0, 0, 0);
}

result_t cdrawPluginInfoListLoad(cdrawPluginInfo** const pluginInfoList_out, size_t* const count_out, cstrk_t const listFile)
{
	result_init();
	asserterr(pluginInfoList_out && !*pluginInfoList_out, errcode_invalidarg);
	asserterr_ptr(count_out, errcode_invalidarg);
	asserterr_cstr(listFile, errcode_invalidarg);

	FILE* fp = fopen(listFile, "r");
	asserterr_ptr(fp, errcode_plugin_init);

	byte_t line[2048] = { 0 }, * linePtr;
	size_t count = 0;
	int const sz = (int)sizeof(line);

	// get number of list items
	while (fgets(line, sz, fp))
		if (*line == '@')
			++count;

	if (!count)
	{
		fclose(fp);
		*count_out = 0;
		result_return();
	}

	// if found potential entries
	fp = freopen(listFile, "r", fp);
	asserterr_ptr(fp, errcode_plugin_init);

	cdrawPluginInfo tmp = { 0 }, * const list = (cdrawPluginInfo*)malloc(sizeof(tmp) * count);
	asserterr_ptr(list, errcode_plugin_init);

	label_t callbackHook, callbackName;
	size_t callbackIdx;
	size_t const callbackCt = buffer_len(tmp.callbacks);

	// parse entries
	count = 0;
	while (linePtr = fgets(line, sz, fp))
	{
		if (*(linePtr++) == '@')
		{
			// skip opening quote and copy until closing quote
			linePtr = cdrawPluginInfoInternalRead(tmp.name, sizeof(tmp.name), 0, linePtr, 0);
			if (!linePtr)
				continue;
			linePtr = cdrawPluginInfoInternalRead(tmp.dylib, sizeof(tmp.dylib), 0, linePtr, 0);
			if (!linePtr)
				continue;
			linePtr = cdrawPluginInfoInternalRead(tmp.author, sizeof(tmp.author), 0, linePtr, 0);
			if (!linePtr)
				continue;
			linePtr = cdrawPluginInfoInternalRead(tmp.version, sizeof(tmp.version), 0, linePtr, 0);
			if (!linePtr)
				continue;
			linePtr = cdrawPluginInfoInternalRead(tmp.details, sizeof(tmp.details), 0, linePtr, 0);
			if (!linePtr)
				continue;

			// optional callback name hooks
			// search until end of line
			while (*linePtr)
			{
				linePtr = cdrawPluginInfoInternalRead(callbackName, sizeof(callbackName), callbackHook, linePtr, ':');
				if (!linePtr || !*callbackName || !*callbackHook)
					break;

				for (callbackIdx = 0; callbackIdx < callbackCt; ++callbackIdx)
				{
					if (strncmp(callbackHook, gPluginCallbackHook[callbackIdx], sizeof(label_t)) == 0)
					{
						strncpy(tmp.callbacks[callbackIdx], callbackName, sizeof(label_t));
						break;
					}
				}
			}

			// copy
			cdrawPluginInfoInit(&list[count++], tmp.name, tmp.dylib, tmp.author, tmp.version, tmp.details, tmp.callbacks);
		}
	}

	// done
	fclose(fp);
	asserterr(count, errcode_plugin_init);
	*pluginInfoList_out = list;
	*count_out = count;
	result_return();
}

result_t cdrawPluginInfoListRelease(cdrawPluginInfo** const pluginInfoList)
{
	result_init();
	asserterr_ptrval(pluginInfoList, errcode_invalidarg);
	free(*pluginInfoList);
	*pluginInfoList = NULL;
	result_return();
}

result_t cdrawPluginInfoPrint(cdrawPluginInfo const* const pluginInfo, cstr_t* const stringPtr)
{
	result_init();
	asserterr_ptr(pluginInfo, errcode_invalidarg);
	asserterr_ptrval(stringPtr, errcode_invalidarg);
	*stringPtr += sprintf(*stringPtr,
		"\"%s\" \n  By: %s \n  Ver. %s \n%s \n",
		pluginInfo->name, pluginInfo->author, pluginInfo->version, pluginInfo->details);
	result_return();
}

result_t cdrawPluginReset(cdrawPlugin* const plugin)
{
	result_init();
	asserterr(plugin, errcode_invalidarg);
	cdrawPluginInfoReset(&plugin->info);
	plugin->id = -1;
	plugin->p_handle = NULL;
	plugin->p_owner = NULL;
	plugin->p_data = NULL;
	cdrawPluginInternalResetCallbacks(plugin->cb, buffer_len(plugin->cb));
	result_return();
}

result_t cdrawPluginLoad(cdrawPlugin* const plugin, cdrawPluginInfo const* const pluginInfo, ptrdiff_t const id, ptrk_t const owner_opt)
{
	result_init();
	asserterr(plugin && (plugin->id == -1) && !plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner, errcode_invalidarg);
	plugin->p_handle = cdrawPluginInternalLoad(plugin, pluginInfo);
	asserterr(plugin->p_handle, errcode_plugin_init);
	plugin->info = *pluginInfo;
	plugin->id = id;
	plugin->p_owner = owner_opt;
	if (id >= 0)
		cdrawPluginCallPostLoad(plugin, owner_opt);
	else
		cdrawPluginCallPostHotload(plugin, owner_opt);
	result_return();
}

result_t cdrawPluginReload(cdrawPlugin* const plugin, ptrk_t const caller)
{
	result_init();
	bool unloadResult = false;
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	cdrawPluginCallPreReload(plugin, caller);
	unloadResult = cdrawPluginInternalUnload(plugin);
	asserterr(unloadResult, errcode_plugin_init);
	plugin->p_handle = cdrawPluginInternalLoad(plugin, &plugin->info);
	asserterr(plugin->p_handle, errcode_plugin_init);
	cdrawPluginCallPostReload(plugin, caller);
	result_return();
}

result_t cdrawPluginUnload(cdrawPlugin* const plugin, ptrk_t const caller)
{
	result_init();
	bool unloadResult = false;
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	if (plugin->id >= 0)
		cdrawPluginCallPreUnload(plugin, caller);
	else
		cdrawPluginCallPreHotload(plugin, caller);
	unloadResult = cdrawPluginInternalUnload(plugin);
	asserterr(unloadResult, errcode_plugin_init);
	cdrawPluginInfoReset(&plugin->info);
	plugin->id = -1;
	plugin->p_owner = NULL;
	plugin->p_handle = NULL;
	result_return();
}

result_t cdrawPluginCallPostLoad(cdrawPlugin* const plugin, ptrk_t const caller)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_load_post, errcode_plugin_callback);
	result_inc(plugin->cb_load_post(&plugin->p_data));
	result_return();
}

result_t cdrawPluginCallPreHotload(cdrawPlugin* const plugin, ptrk_t const caller)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_hotload_pre, errcode_plugin_callback);
	result_inc(plugin->cb_hotload_pre(&plugin->p_data));
	result_return();
}

result_t cdrawPluginCallPostHotload(cdrawPlugin* const plugin, ptrk_t const caller)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_hotload_post, errcode_plugin_callback);
	result_inc(plugin->cb_hotload_post(&plugin->p_data));
	result_return();
}

result_t cdrawPluginCallPreReload(cdrawPlugin* const plugin, ptrk_t const caller)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_reload_pre, errcode_plugin_callback);
	result_inc(plugin->cb_reload_pre(&plugin->p_data));
	result_return();
}

result_t cdrawPluginCallPostReload(cdrawPlugin* const plugin, ptrk_t const caller)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_reload_post, errcode_plugin_callback);
	result_inc(plugin->cb_reload_post(&plugin->p_data));
	result_return();
}

result_t cdrawPluginCallPreUnload(cdrawPlugin* const plugin, ptrk_t const caller)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_unload_pre, errcode_plugin_callback);
	result_inc(plugin->cb_unload_pre(&plugin->p_data));
	result_return();
}

result_t cdrawPluginCallOnWindowAttach(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const w, int32_t const h, int32_t const x, int32_t const y, ptrk_t const windowPlatform_opt)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_win_attach, errcode_plugin_callback);
	result_inc(plugin->cb_win_attach(plugin->p_data, w, h, x, y, windowPlatform_opt));
	result_return();
}

result_t cdrawPluginCallOnWindowDetach(cdrawPlugin const* const plugin, ptrk_t const caller, ptrk_t const windowPlatform_opt)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_win_detach, errcode_plugin_callback);
	result_inc(plugin->cb_win_detach(plugin->p_data, windowPlatform_opt));
	result_return();
}

result_t cdrawPluginCallOnWindowActivate(cdrawPlugin const* const plugin, ptrk_t const caller, ptrk_t const windowPlatform_opt)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_win_activate, errcode_plugin_callback);
	result_inc(plugin->cb_win_activate(plugin->p_data, windowPlatform_opt));
	result_return();
}

result_t cdrawPluginCallOnWindowDeactivate(cdrawPlugin const* const plugin, ptrk_t const caller, ptrk_t const windowPlatform_opt)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_win_deactivate, errcode_plugin_callback);
	result_inc(plugin->cb_win_deactivate(plugin->p_data, windowPlatform_opt));
	result_return();
}

result_t cdrawPluginCallOnWindowResize(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const w, int32_t const h)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_win_resize, errcode_plugin_callback);
	result_inc(plugin->cb_win_resize(plugin->p_data, w, h));
	result_return();
}

result_t cdrawPluginCallOnWindowMove(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const x, int32_t const y)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_win_move, errcode_plugin_callback);
	result_inc(plugin->cb_win_move(plugin->p_data, x, y));
	result_return();
}

result_t cdrawPluginCallOnDisplay(cdrawPlugin const* const plugin, ptrk_t const caller)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_display, errcode_plugin_callback);
	result_inc(plugin->cb_display(plugin->p_data));
	result_return();
}

result_t cdrawPluginCallOnIdle(cdrawPlugin const* const plugin, ptrk_t const caller)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_idle, errcode_plugin_callback);
	result_inc(plugin->cb_idle(plugin->p_data));
	result_return();
}

result_t cdrawPluginCallOnVirtkeyPress(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const virtkey)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_virtkey_press, errcode_plugin_callback);
	result_inc(plugin->cb_virtkey_press(plugin->p_data, virtkey));
	result_return();
}

result_t cdrawPluginCallOnVirtkeyHold(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const virtkey)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_virtkey_hold, errcode_plugin_callback);
	result_inc(plugin->cb_virtkey_hold(plugin->p_data, virtkey));
	result_return();
}

result_t cdrawPluginCallOnVirtkeyRelease(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const virtkey)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_virtkey_release, errcode_plugin_callback);
	result_inc(plugin->cb_virtkey_release(plugin->p_data, virtkey));
	result_return();
}

result_t cdrawPluginCallOnKeyPress(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const key)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_key_press, errcode_plugin_callback);
	result_inc(plugin->cb_key_press(plugin->p_data, key));
	result_return();
}

result_t cdrawPluginCallOnKeyHold(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const key)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_key_hold, errcode_plugin_callback);
	result_inc(plugin->cb_key_hold(plugin->p_data, key));
	result_return();
}

result_t cdrawPluginCallOnKeyRelease(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const key)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_key_release, errcode_plugin_callback);
	result_inc(plugin->cb_key_release(plugin->p_data, key));
	result_return();
}

result_t cdrawPluginCallOnMousePress(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const btn, int32_t const x, int32_t const y)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_mouse_press, errcode_plugin_callback);
	result_inc(plugin->cb_mouse_press(plugin->p_data, btn, x, y));
	result_return();
}

result_t cdrawPluginCallOnMouseRelease(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const btn, int32_t const x, int32_t const y)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_mouse_release, errcode_plugin_callback);
	result_inc(plugin->cb_mouse_release(plugin->p_data, btn, x, y));
	result_return();
}

result_t cdrawPluginCallOnMouseDouble(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const btn, int32_t const x, int32_t const y)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_mouse_double, errcode_plugin_callback);
	result_inc(plugin->cb_mouse_double(plugin->p_data, btn, x, y));
	result_return();
}

result_t cdrawPluginCallOnMouseWheel(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const delta, int32_t const x, int32_t const y)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_mouse_wheel, errcode_plugin_callback);
	result_inc(plugin->cb_mouse_wheel(plugin->p_data, delta, x, y));
	result_return();
}

result_t cdrawPluginCallOnMouseMove(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const x, int32_t const y)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_mouse_move, errcode_plugin_callback);
	result_inc(plugin->cb_mouse_move(plugin->p_data, x, y));
	result_return();
}

result_t cdrawPluginCallOnMouseDrag(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const x, int32_t const y)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_mouse_drag, errcode_plugin_callback);
	result_inc(plugin->cb_mouse_drag(plugin->p_data, x, y));
	result_return();
}

result_t cdrawPluginCallOnMouseEnter(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const x, int32_t const y)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_mouse_enter, errcode_plugin_callback);
	result_inc(plugin->cb_mouse_enter(plugin->p_data, x, y));
	result_return();
}

result_t cdrawPluginCallOnMouseLeave(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const x, int32_t const y)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_mouse_leave, errcode_plugin_callback);
	result_inc(plugin->cb_mouse_leave(plugin->p_data, x, y));
	result_return();
}

result_t cdrawPluginCallOnUser1(cdrawPlugin const* const plugin, ptrk_t const caller)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_user_1, errcode_plugin_callback);
	result_inc(plugin->cb_user_1(plugin->p_data));
	result_return();
}

result_t cdrawPluginCallOnUser2(cdrawPlugin const* const plugin, ptrk_t const caller)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_user_2, errcode_plugin_callback);
	result_inc(plugin->cb_user_2(plugin->p_data));
	result_return();
}

result_t cdrawPluginCallOnUser3(cdrawPlugin const* const plugin, ptrk_t const caller)
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_user_3, errcode_plugin_callback);
	result_inc(plugin->cb_user_3(plugin->p_data));
	result_return();
}

result_t cdrawPluginCallOnUserCmd(cdrawPlugin const* const plugin, ptrk_t const caller, int32_t const argc, cstrk_t const argv[])
{
	result_init();
	asserterr(plugin && (plugin->id != -1) && plugin->p_handle, errcode_invalidarg);
	asserterr(!plugin->p_owner || (plugin->p_owner == caller), errcode_invalidarg);
	asserterr_ptr(plugin->cb_user_cmd, errcode_plugin_callback);
	result_inc(plugin->cb_user_cmd(plugin->p_data, argc, argv));
	result_return();
}
