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
* cdrawPlugin_win.c
* Windows implementation of plugin management.
*/

#include "cdraw/cdrawPlatform/cdrawPlugin.h"
#if CDRAW_TARGET_WINDOWS
#include <Windows.h>

cstr_t cdrawPluginInternalDylibGetExt(cstr_t const ext_out, size_t const arrlen_ext)
{
	char const ext[] = ".dll", * src = ext;
	cstr_t dst = ext_out;
	cdraw_assert(ext_out && (arrlen_ext >= buffer_len(ext)));
	while (*src) *(dst++) = *(src++);
	*dst = 0;
	return ext_out;
}

ptr_t cdrawPluginInternalDylibLoad(cstrk_t const path)
{
	cdraw_assert(buffer_valid(path));
	HMODULE handle = LoadLibraryA(path);
	return (ptr_t)handle;
}

bool cdrawPluginInternalDylibUnload(ptr_t const handle)
{
	BOOL result = FreeLibrary((HMODULE)handle);
	return (result != 0);
}

ptr_t cdrawPluginInternalDylibGetSymbol(ptrk_t const handle, cstrk_t const name)
{
	cdraw_assert(handle && buffer_valid(name));
	FARPROC proc = GetProcAddress((HMODULE)handle, name);
	return (ptr_t)proc;
}


#endif // #if CDRAW_TARGET_WINDOWS


#if !CDRAW_TARGET_WINDOWS // ****TO-DO: MOVE TO NEW PLATFORM SOURCE
#include <dlfcn.h>

cstr_t cdrawPluginInternalDylibGetExt(cstr_t const ext_out, size_t const arrlen_ext)
{
	char const ext[] = ".so", * src = ext;
	cstr_t dst = ext_out;
	cdraw_assert(ext_out && (arrlen_ext >= buffer_len(ext)));
	while (*src) *(dst++) = *(src++);
	*dst = 0;
	return ext_out;
}

ptr_t cdrawPluginInternalDylibLoad(cstrk_t const path)
{
	cdraw_assert(buffer_valid(path));
	ptr_t handle = dlopen(path, RTLD_NOW);
	return handle;
}

bool cdrawPluginInternalDylibUnload(ptr_t const handle)
{
	int result = dlclose(dylib_handle);
	return (result == 0);
}

ptr_t cdrawPluginInternalDylibGetSymbol(ptrk_t const handle, cstrk_t const name)
{
	cdraw_assert(handle && buffer_valid(name));
	ptr_t proc = dlsym(handle, name);
	return proc;
}


#endif // #if !CDRAW_TARGET_WINDOWS