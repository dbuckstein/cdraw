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

#include "cdraw/cdraw/cdrawConfig.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	CDRAW_DYLIB_SYMBOL result_t cb_load_post(ptr_t* const data_inout);
	CDRAW_DYLIB_SYMBOL result_t cb_hotload_pre(ptr_t* const data_inout);
	CDRAW_DYLIB_SYMBOL result_t cb_hotload_post(ptr_t* const data_inout);
	CDRAW_DYLIB_SYMBOL result_t cb_reload_pre(ptr_t* const data_inout);
	CDRAW_DYLIB_SYMBOL result_t cb_reload_post(ptr_t* const data_inout);
	CDRAW_DYLIB_SYMBOL result_t cb_unload_pre(ptr_t* const data_inout);
	CDRAW_DYLIB_SYMBOL result_t cb_win_attach(ptr_t const data, int32_t const w, int32_t const h, int32_t const x, int32_t const y, ptrk_t const windowPlatform_opt);
	CDRAW_DYLIB_SYMBOL result_t cb_win_detach(ptr_t const data, ptrk_t const windowPlatform_opt);
	CDRAW_DYLIB_SYMBOL result_t cb_win_activate(ptr_t const data, ptrk_t const windowPlatform_opt);
	CDRAW_DYLIB_SYMBOL result_t cb_win_deactivate(ptr_t const data, ptrk_t const windowPlatform_opt);
	CDRAW_DYLIB_SYMBOL result_t cb_win_resize(ptr_t const data, int32_t const w, int32_t const h);
	CDRAW_DYLIB_SYMBOL result_t cb_win_move(ptr_t const data, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_display(ptr_t const data);
	CDRAW_DYLIB_SYMBOL result_t cb_idle(ptr_t const data);
	CDRAW_DYLIB_SYMBOL result_t cb_virtkey_press(ptr_t const data, int32_t const virtkey);
	CDRAW_DYLIB_SYMBOL result_t cb_virtkey_hold(ptr_t const data, int32_t const virtkey);
	CDRAW_DYLIB_SYMBOL result_t cb_virtkey_release(ptr_t const data, int32_t const virtkey);
	CDRAW_DYLIB_SYMBOL result_t cb_key_press(ptr_t const data, int32_t const key);
	CDRAW_DYLIB_SYMBOL result_t cb_key_hold(ptr_t const data, int32_t const key);
	CDRAW_DYLIB_SYMBOL result_t cb_key_release(ptr_t const data, int32_t const key);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_press(ptr_t const data, int32_t const btn, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_release(ptr_t const data, int32_t const btn, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_double(ptr_t const data, int32_t const btn, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_wheel(ptr_t const data, int32_t const delta, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_move(ptr_t const data, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_drag(ptr_t const data, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_enter(ptr_t const data, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_mouse_leave(ptr_t const data, int32_t const x, int32_t const y);
	CDRAW_DYLIB_SYMBOL result_t cb_user_1(ptr_t const data);
	CDRAW_DYLIB_SYMBOL result_t cb_user_2(ptr_t const data);
	CDRAW_DYLIB_SYMBOL result_t cb_user_3(ptr_t const data);
	CDRAW_DYLIB_SYMBOL result_t cb_user_cmd(ptr_t const data, int32_t const argc, cstrk_t const argv[]);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#include <stdio.h>


result_t cb_load_post(ptr_t* const data_inout)
{

	return printf("\n" __FUNCTION__ "(%p) -> %p", data_inout, *data_inout);
}

result_t cb_hotload_pre(ptr_t* const data_inout)
{

	return printf("\n" __FUNCTION__ "(%p) -> %p", data_inout, *data_inout);
}

result_t cb_hotload_post(ptr_t* const data_inout)
{

	return printf("\n" __FUNCTION__ "(%p) -> %p", data_inout, *data_inout);
}

result_t cb_reload_pre(ptr_t* const data_inout)
{

	return printf("\n" __FUNCTION__ "(%p) -> %p", data_inout, *data_inout);
}

result_t cb_reload_post(ptr_t* const data_inout)
{

	return printf("\n" __FUNCTION__ "(%p) -> %p", data_inout, *data_inout);
}

result_t cb_unload_pre(ptr_t* const data_inout)
{

	return printf("\n" __FUNCTION__ "(%p) -> %p", data_inout, *data_inout);
}

result_t cb_win_attach(ptr_t const data, int32_t const w, int32_t const h, int32_t const x, int32_t const y, ptrk_t const windowPlatform_opt)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d, %d, %d, %p)", data, w, h, x, y, windowPlatform_opt);
}

result_t cb_win_detach(ptr_t const data, ptrk_t const windowPlatform_opt)
{

	return printf("\n" __FUNCTION__ "(%p, %p)", data, windowPlatform_opt);
}

result_t cb_win_activate(ptr_t const data, ptrk_t const windowPlatform_opt)
{

	return printf("\n" __FUNCTION__ "(%p, %p)", data, windowPlatform_opt);
}

result_t cb_win_deactivate(ptr_t const data, ptrk_t const windowPlatform_opt)
{

	return printf("\n" __FUNCTION__ "(%p, %p)", data, windowPlatform_opt);
}

result_t cb_win_resize(ptr_t const data, int32_t const w, int32_t const h)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d)", data, w, h);
}

result_t cb_win_move(ptr_t const data, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d)", data, x, y);
}

result_t cb_display(ptr_t const data)
{

	return 0;// printf("\n" __FUNCTION__ "(%p)", data);
}

result_t cb_idle(ptr_t const data)
{

	return 0;// printf("\n" __FUNCTION__ "(%p)", data);
}

result_t cb_virtkey_press(ptr_t const data, int32_t const virtkey)
{

	return printf("\n" __FUNCTION__ "(%p, %d)", data, virtkey);
}

result_t cb_virtkey_hold(ptr_t const data, int32_t const virtkey)
{

	return 0;// printf("\n" __FUNCTION__ "(%p, %d)", data, virtkey);
}

result_t cb_virtkey_release(ptr_t const data, int32_t const virtkey)
{

	return printf("\n" __FUNCTION__ "(%p, %d)", data, virtkey);
}

result_t cb_key_press(ptr_t const data, int32_t const key)
{

	return printf("\n" __FUNCTION__ "(%p, %d)", data, key);
}

result_t cb_key_hold(ptr_t const data, int32_t const key)
{

	return 0;// printf("\n" __FUNCTION__ "(%p, %d)", data, key);
}

result_t cb_key_release(ptr_t const data, int32_t const key)
{

	return printf("\n" __FUNCTION__ "(%p, %d)", data, key);
}

result_t cb_mouse_press(ptr_t const data, int32_t const btn, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d, %d)", data, btn, x, y);
}

result_t cb_mouse_release(ptr_t const data, int32_t const btn, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d, %d)", data, btn, x, y);
}

result_t cb_mouse_double(ptr_t const data, int32_t const btn, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d, %d)", data, btn, x, y);
}

result_t cb_mouse_wheel(ptr_t const data, int32_t const delta, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d, %d)", data, delta, x, y);
}

result_t cb_mouse_move(ptr_t const data, int32_t const x, int32_t const y)
{

	return 0;// printf("\n" __FUNCTION__ "(%p, %d, %d)", data, x, y);
}

result_t cb_mouse_drag(ptr_t const data, int32_t const x, int32_t const y)
{

	return 0;// printf("\n" __FUNCTION__ "(%p, %d, %d)", data, x, y);
}

result_t cb_mouse_enter(ptr_t const data, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d)", data, x, y);
}

result_t cb_mouse_leave(ptr_t const data, int32_t const x, int32_t const y)
{

	return printf("\n" __FUNCTION__ "(%p, %d, %d)", data, x, y);
}

result_t cb_user_1(ptr_t const data)
{

	return printf("\n" __FUNCTION__ "(%p)", data);
}

result_t cb_user_2(ptr_t const data)
{

	return printf("\n" __FUNCTION__ "(%p)", data);
}

result_t cb_user_3(ptr_t const data)
{

	return printf("\n" __FUNCTION__ "(%p)", data);
}

result_t cb_user_cmd(ptr_t const data, int32_t const argc, cstrk_t const argv[])
{

	int32_t result = printf("\n" __FUNCTION__ "(%p, %d, %p) -> ", data, argc, argv);
	cstrk_t const* argv_itr = argv;
	cstrk_t const* const argv_end = argv + argc;
	while (argv_itr < argv_end)
		result += printf("%s; ", *(argv_itr++));
	return result;
}
