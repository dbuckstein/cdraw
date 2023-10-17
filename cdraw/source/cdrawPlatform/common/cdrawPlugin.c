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


CDRAW_INL void cdrawPluginInternalResetCallbacks(cdrawPlugin* const plugin)
{
	cdraw_assert(plugin);
	plugin->cb_load = cdrawPluginCallbackDefault;
	plugin->cb_hotload = cdrawPluginCallbackDefault;
	plugin->cb_pre_reload = cdrawPluginCallbackDefault;
	plugin->cb_reload = cdrawPluginCallbackDefault;
	plugin->cb_pre_unload = cdrawPluginCallbackDefault;
	plugin->cb_unload = cdrawPluginCallbackDefault;
	plugin->cb_win_attach = cdrawPluginCallbackDefault;
	plugin->cb_win_detach = cdrawPluginCallbackDefault;
	plugin->cb_win_activate = cdrawPluginCallbackDefault;
	plugin->cb_win_deactivate = cdrawPluginCallbackDefault;
	plugin->cb_win_resize = cdrawPluginCallbackDefault;
	plugin->cb_win_move = cdrawPluginCallbackDefault;
	plugin->cb_display = cdrawPluginCallbackDefault;
	plugin->cb_idle = cdrawPluginCallbackDefault;
	plugin->cb_virtkey_press = cdrawPluginCallbackDefault;
	plugin->cb_virtkey_hold = cdrawPluginCallbackDefault;
	plugin->cb_virtkey_release = cdrawPluginCallbackDefault;
	plugin->cb_key_press = cdrawPluginCallbackDefault;
	plugin->cb_key_hold = cdrawPluginCallbackDefault;
	plugin->cb_key_release = cdrawPluginCallbackDefault;
	plugin->cb_mouse_press = cdrawPluginCallbackDefault;
	plugin->cb_mouse_release = cdrawPluginCallbackDefault;
	plugin->cb_mouse_double = cdrawPluginCallbackDefault;
	plugin->cb_mouse_wheel = cdrawPluginCallbackDefault;
	plugin->cb_mouse_move = cdrawPluginCallbackDefault;
	plugin->cb_mouse_drag = cdrawPluginCallbackDefault;
	plugin->cb_mouse_enter = cdrawPluginCallbackDefault;
	plugin->cb_mouse_leave = cdrawPluginCallbackDefault;
	plugin->cb_user_1 = cdrawPluginCallbackDefault;
	plugin->cb_user_2 = cdrawPluginCallbackDefault;
	plugin->cb_user_3 = cdrawPluginCallbackDefault;
	plugin->cb_user_cmd = cdrawPluginCallbackDefault;
}


/******************************************************************************
* Public functions.
******************************************************************************/

result_t cdrawPluginInfoInit(cdrawPluginInfo* const pluginInfo, label_t const name, label_t const dylib, label_t const author, label_t const version, label_long_t const details)
{
	result_init();
	asserterr_ptr(pluginInfo, errcode_invalidarg);

	cdrawPluginInfo const def = { "cdraw Plugin (SDK)", "cdrawPlugin", "Daniel S. Buckstein", (__TIME__ "; " __DATE__), "Default plugin built and loaded with current SDK configuration." };
	strncpy(pluginInfo->name, label_valid(name) ? name : def.name, sizeof(def.name));
	strncpy(pluginInfo->dylib, label_valid(dylib) ? dylib : def.dylib, sizeof(def.dylib));
	strncpy(pluginInfo->author, label_valid(author) ? author : def.author, sizeof(def.author));
	strncpy(pluginInfo->version, label_valid(version) ? version : def.version, sizeof(def.version));
	strncpy(pluginInfo->details, label_valid(details) ? details : def.details, sizeof(def.details));

	result_return();
}

result_t cdrawPluginInfoInitDefault(cdrawPluginInfo* const pluginInfo, label_t const author, label_t const version)
{
	return cdrawPluginInfoInit(pluginInfo, 0, 0, author, version, 0);
}

result_t cdrawPluginInfoReset(cdrawPluginInfo* const pluginInfo)
{
	return cdrawPluginInfoInit(pluginInfo, 0, 0, 0, 0, 0);
}

result_t cdrawPluginInfoListLoad(cdrawPluginInfo** const pluginInfoList_out, size_t* const count_out, cstrk_t const listFile)
{
	result_init();
	asserterr(pluginInfoList_out && !*pluginInfoList_out, errcode_invalidarg);
	asserterr_ptr(count_out, errcode_invalidarg);
	asserterr_cstr(listFile, errcode_invalidarg);

	FILE* fp = fopen(listFile, "r");
	asserterr_ptr(fp, errcode_plugin_init);

	byte_t line[512] = { 0 }, * linePtr;
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

	cstr_t data;
	cdrawPluginInfo tmp = { 0 }, * const list = (cdrawPluginInfo*)malloc(sizeof(tmp) * count);
	asserterr_ptr(list, errcode_plugin_init);

	// parse entries
	count = 0;
	while (linePtr = fgets(line, sz, fp))
	{
		if (*(linePtr++) == '@')
		{
			// skip opening quote and copy until closing quote
			// name
			data = tmp.name;
			while (*(linePtr++) != '\"');
			while (*(linePtr) != '\"')
				*(data++) = *(linePtr++);
			++linePtr;

			// library file
			data = tmp.dylib;
			while (*(linePtr++) != '\"');
			while (*(linePtr) != '\"')
				*(data++) = *(linePtr++);
			++linePtr;

			// author
			data = tmp.author;
			while (*(linePtr++) != '\"');
			while (*(linePtr) != '\"')
				*(data++) = *(linePtr++);
			++linePtr;

			// version
			data = tmp.version;
			while (*(linePtr++) != '\"');
			while (*(linePtr) != '\"')
				*(data++) = *(linePtr++);
			++linePtr;

			// description
			data = tmp.details;
			while (*(linePtr++) != '\"');
			while (*(linePtr) != '\"')
				*(data++) = *(linePtr++);
			++linePtr;

			// copy
			list[count++] = tmp;
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

	result_return();
}

result_t cdrawPluginReset(cdrawPlugin* const plugin)
{
	result_init();
	asserterr(plugin && !plugin->p_handle, errcode_invalidarg);
	cdrawPluginInternalResetCallbacks(plugin);
	plugin->p_owner = NULL;
	plugin->p_data = NULL;
	plugin->i_list = -1;
	result_return();
}
