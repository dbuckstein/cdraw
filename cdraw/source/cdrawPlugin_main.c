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
* cdrawPlugin_main.c
* Common entry point for plugin dynamic library via callbacks.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cdraw/cdrawPlatform/cdrawTimer.h"
#include "cdraw/cdrawPlatform/cdrawRenderer.h"

/******************************************************************************
* Interfaces.
******************************************************************************/

typedef struct cdrawTestPluginData
{
	cdrawTimer timer_sys, timer;
	cdrawRenderer renderer;
	bool activated;
} cdrawTestPluginData;


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	CDRAW_DYLIB_SYMBOL result_t cb_load_post(cdrawTestPluginData** const data_inout);
	CDRAW_DYLIB_SYMBOL result_t cb_hotload_pre(cdrawTestPluginData** const data_inout);
	CDRAW_DYLIB_SYMBOL result_t cb_hotload_post(cdrawTestPluginData** const data_inout);
	CDRAW_DYLIB_SYMBOL result_t cb_reload_pre(cdrawTestPluginData** const data_inout);
	CDRAW_DYLIB_SYMBOL result_t cb_reload_post(cdrawTestPluginData** const data_inout);
	CDRAW_DYLIB_SYMBOL result_t cb_unload_pre(cdrawTestPluginData** const data_inout);
	CDRAW_DYLIB_SYMBOL result_t cb_win_attach(cdrawTestPluginData* const data, int32_t const w, int32_t const h, int32_t const x, int32_t const y, ptrk_t const windowPlatform_opt);
	CDRAW_DYLIB_SYMBOL result_t cb_win_detach(cdrawTestPluginData* const data, ptrk_t const windowPlatform_opt);
	CDRAW_DYLIB_SYMBOL result_t cb_win_activate(cdrawTestPluginData* const data, ptrk_t const windowPlatform_opt);
	CDRAW_DYLIB_SYMBOL result_t cb_win_deactivate(cdrawTestPluginData* const data, ptrk_t const windowPlatform_opt);
	CDRAW_DYLIB_SYMBOL result_t cb_win_resize(cdrawTestPluginData* const data, int32_t const w, int32_t const h);
	CDRAW_DYLIB_SYMBOL result_t cb_win_move(cdrawTestPluginData* const data, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_display(cdrawTestPluginData* const data);
	CDRAW_DYLIB_SYMBOL result_t cb_idle(cdrawTestPluginData* const data);
	CDRAW_DYLIB_SYMBOL result_t cb_virtkey_press(cdrawTestPluginData* const data, int32_t const virtkey);
	CDRAW_DYLIB_SYMBOL result_t cb_virtkey_hold(cdrawTestPluginData* const data, int32_t const virtkey);
	CDRAW_DYLIB_SYMBOL result_t cb_virtkey_release(cdrawTestPluginData* const data, int32_t const virtkey);
	CDRAW_DYLIB_SYMBOL result_t cb_key_press(cdrawTestPluginData* const data, int32_t const key);
	CDRAW_DYLIB_SYMBOL result_t cb_key_hold(cdrawTestPluginData* const data, int32_t const key);
	CDRAW_DYLIB_SYMBOL result_t cb_key_release(cdrawTestPluginData* const data, int32_t const key);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_press(cdrawTestPluginData* const data, int32_t const btn, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_release(cdrawTestPluginData* const data, int32_t const btn, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_double(cdrawTestPluginData* const data, int32_t const btn, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_wheel(cdrawTestPluginData* const data, int32_t const delta, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_move(cdrawTestPluginData* const data, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_drag(cdrawTestPluginData* const data, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_enter(cdrawTestPluginData* const data, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_leave(cdrawTestPluginData* const data, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_user_1(cdrawTestPluginData* const data);
	CDRAW_DYLIB_SYMBOL result_t cb_user_2(cdrawTestPluginData* const data);
	CDRAW_DYLIB_SYMBOL result_t cb_user_3(cdrawTestPluginData* const data);
	CDRAW_DYLIB_SYMBOL result_t cb_user_cmd(cdrawTestPluginData* const data, int32_t const argc, cstrk_t const argv[]);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


/******************************************************************************
* Implementations.
******************************************************************************/

result_t cb_load_post(cdrawTestPluginData** const data_inout)
{
	cdraw_assert(data_inout && !*data_inout);
	size_t const dataSz = sizeof(cdrawTestPluginData);
	cdrawTestPluginData* const data = (cdrawTestPluginData*)malloc(dataSz);
	cdraw_assert(data);
	memset(data, 0, dataSz);

	cdrawTimerInitSystem(&data->timer_sys);
	cdrawTimerSet(&data->timer, &data->timer_sys, 60);

	*data_inout = data;
	return 0;// printf("\n" __FUNCTION__ "(%p) -> %p", data_inout, *data_inout);
}

result_t cb_hotload_pre(cdrawTestPluginData** const data_inout)
{

	return printf("\n" __FUNCTION__ "(%p) -> %p", data_inout, *data_inout);
}

result_t cb_hotload_post(cdrawTestPluginData** const data_inout)
{

	return printf("\n" __FUNCTION__ "(%p) -> %p", data_inout, *data_inout);
}

result_t cb_reload_pre(cdrawTestPluginData** const data_inout)
{

	return printf("\n" __FUNCTION__ "(%p) -> %p", data_inout, *data_inout);
}

result_t cb_reload_post(cdrawTestPluginData** const data_inout)
{

	return printf("\n" __FUNCTION__ "(%p) -> %p", data_inout, *data_inout);
}

result_t cb_unload_pre(cdrawTestPluginData** const data_inout)
{
	cdraw_assert(data_inout && *data_inout);
	cdrawTestPluginData* const data = *data_inout;
	cdraw_assert(data);
	free(data);
	*data_inout = NULL;
	return 0;// printf("\n" __FUNCTION__ "(%p) -> %p", data_inout, *data_inout);
}

result_t cb_win_attach(cdrawTestPluginData* const data, int32_t const w, int32_t const h, int32_t const x, int32_t const y, ptrk_t const windowPlatform_opt)
{
	cdraw_assert(data && windowPlatform_opt);
	cdrawRenderer* const renderer = &data->renderer;
	cdraw_assert(!renderer->r && !renderer->renderAPI);
	result_t result = cdrawRendererCreate(renderer, cdrawRenderAPI_Vulkan, windowPlatform_opt);
	cdrawRendererPrint(renderer);
	return result;// printf("\n" __FUNCTION__ "(%p, %d, %d, %d, %d, %p)", data, w, h, x, y, windowPlatform_opt);
}

result_t cb_win_detach(cdrawTestPluginData* const data, ptrk_t const windowPlatform_opt)
{
	cdraw_assert(data && windowPlatform_opt);
	cdrawRenderer* const renderer = &data->renderer;
	cdraw_assert(renderer->r && renderer->renderAPI);
	result_t result = cdrawRendererRelease(renderer);
	return result;// printf("\n" __FUNCTION__ "(%p, %p)", data, windowPlatform_opt);
}

result_t cb_win_activate(cdrawTestPluginData* const data, ptrk_t const windowPlatform_opt)
{
	cdraw_assert(data && windowPlatform_opt);
	data->activated = true;
	return 0;// printf("\n" __FUNCTION__ "(%p, %p)", data, windowPlatform_opt);
}

result_t cb_win_deactivate(cdrawTestPluginData* const data, ptrk_t const windowPlatform_opt)
{
	cdraw_assert(data && windowPlatform_opt);
	data->activated = false;
	return 0;// printf("\n" __FUNCTION__ "(%p, %p)", data, windowPlatform_opt);
}

result_t cb_win_resize(cdrawTestPluginData* const data, int32_t const w, int32_t const h)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d)", data, w, h);
}

result_t cb_win_move(cdrawTestPluginData* const data, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d)", data, x, y);
}

result_t cb_display(cdrawTestPluginData* const data)
{

	return 0;// printf("\n" __FUNCTION__ "(%p)", data);
}

result_t cb_idle(cdrawTestPluginData* const data)
{
	cdraw_assert(data);
	cdrawTimerStepSystem(&data->timer_sys);
	if (result_haswarns(cdrawTimerStepClip(&data->timer, data->timer_sys.state.dt)))
	{
		//stime_t t;
		//cdrawTimerStateGetElapsedTime(&data->timer.state, &t);
		//printf("\n" __FUNCTION__ ": t=%.3lf", t);
	
		//if (data->activated)
		//	cdrawRendererDisplay(&data->renderer);
	}
	return 0;// printf("\n" __FUNCTION__ "(%p)", data);
}

result_t cb_virtkey_press(cdrawTestPluginData* const data, int32_t const virtkey)
{

	return printf("\n" __FUNCTION__ "(%p, %d)", data, virtkey);
}

result_t cb_virtkey_hold(cdrawTestPluginData* const data, int32_t const virtkey)
{

	return 0;// printf("\n" __FUNCTION__ "(%p, %d)", data, virtkey);
}

result_t cb_virtkey_release(cdrawTestPluginData* const data, int32_t const virtkey)
{

	return printf("\n" __FUNCTION__ "(%p, %d)", data, virtkey);
}

result_t cb_key_press(cdrawTestPluginData* const data, int32_t const key)
{

	return printf("\n" __FUNCTION__ "(%p, %d)", data, key);
}

result_t cb_key_hold(cdrawTestPluginData* const data, int32_t const key)
{

	return 0;// printf("\n" __FUNCTION__ "(%p, %d)", data, key);
}

result_t cb_key_release(cdrawTestPluginData* const data, int32_t const key)
{

	return printf("\n" __FUNCTION__ "(%p, %d)", data, key);
}

result_t cb_mouse_press(cdrawTestPluginData* const data, int32_t const btn, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d, %d)", data, btn, x, y);
}

result_t cb_mouse_release(cdrawTestPluginData* const data, int32_t const btn, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d, %d)", data, btn, x, y);
}

result_t cb_mouse_double(cdrawTestPluginData* const data, int32_t const btn, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d, %d)", data, btn, x, y);
}

result_t cb_mouse_wheel(cdrawTestPluginData* const data, int32_t const delta, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d, %d)", data, delta, x, y);
}

result_t cb_mouse_move(cdrawTestPluginData* const data, int32_t const x, int32_t const y)
{

	return 0;// printf("\n" __FUNCTION__ "(%p, %d, %d)", data, x, y);
}

result_t cb_mouse_drag(cdrawTestPluginData* const data, int32_t const x, int32_t const y)
{

	return 0;// printf("\n" __FUNCTION__ "(%p, %d, %d)", data, x, y);
}

result_t cb_mouse_enter(cdrawTestPluginData* const data, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d)", data, x, y);
}

result_t cb_mouse_leave(cdrawTestPluginData* const data, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d)", data, x, y);
}

result_t cb_user_1(cdrawTestPluginData* const data)
{

	return printf("\n" __FUNCTION__ "(%p)", data);
}

result_t cb_user_2(cdrawTestPluginData* const data)
{

	return printf("\n" __FUNCTION__ "(%p)", data);
}

result_t cb_user_3(cdrawTestPluginData* const data)
{

	return printf("\n" __FUNCTION__ "(%p)", data);
}

result_t cb_user_cmd(cdrawTestPluginData* const data, int32_t const argc, cstrk_t const argv[])
{

	int32_t result = printf("\n" __FUNCTION__ "(%p, %d, %p) -> ", data, argc, argv);
	cstrk_t const* argv_itr = argv;
	cstrk_t const* const argv_end = argv + argc;
	while (argv_itr < argv_end)
		result += printf("%s; ", *(argv_itr++));
	return result;
}
