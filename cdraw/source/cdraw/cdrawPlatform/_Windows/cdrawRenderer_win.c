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
* cdrawRenderer_win.c
* Windows implementation of renderer management.
*/

#include "cdraw/cdrawPlatform/cdrawRenderer.h"
#if CDRAW_TARGET_WINDOWS
#include <Windows.h>

#include <stdio.h>


result_t cdrawRendererCreate_vk(cdrawRenderer* const renderer, uint32_t const surfaceCountMax);
result_t cdrawRendererDestroy_vk(cdrawRenderer* const renderer);
result_t cdrawRendererRefresh_vk(cdrawRenderer const* const renderer);
result_t cdrawRendererRefreshAPI_vk();


result_t cdrawRendererCreate_gl(cdrawRenderer* const renderer, uint32_t const surfaceCountMax);
result_t cdrawRendererDestroy_gl(cdrawRenderer* const renderer);
result_t cdrawRendererRefresh_gl(cdrawRenderer const* const renderer);
result_t cdrawRendererRefreshAPI_gl();


result_t cdrawRendererCreate(cdrawRenderer* const renderer, cdrawRenderAPI const renderAPI, uint32_t const windowsAllowed)
{
	result_init();
	asserterr_ptr(renderer, errcode_invalidarg);
	asserterr(renderer->r == NULL, errcode_invalidarg);
	asserterr(renderer->renderAPI == cdrawRenderAPI_none, errcode_invalidarg);
	if (renderAPI == cdrawRenderAPI_none)
		result_return();
	result_t result = 0;
	switch (renderAPI)
	{
	case cdrawRenderAPI_software:
		printf("\n cdrawRendererCreate: Software rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Vulkan:
		result = cdrawRendererCreate_vk(renderer, windowsAllowed);
		break;
	case cdrawRenderAPI_OpenGL:
		result = cdrawRendererCreate_gl(renderer, windowsAllowed);
		break;
	case cdrawRenderAPI_Direct3D:
		printf("\n cdrawRendererCreate: Direct3D rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Metal:
		printf("\n cdrawRendererCreate: Metal rendering is not supported by this platform.");
		break;
	}
	if (!result_isclean(result))
	{
		cdrawRendererDestroy_vk(renderer);
		result_seterror(errcode_renderer_api);
		result_return();
	}
	renderer->renderAPI = renderAPI;
	result_return();
}

result_t cdrawRendererDestroy(cdrawRenderer* const renderer)
{
	result_init();
	asserterr_ptr(renderer, errcode_invalidarg);
	if (renderer->renderAPI == cdrawRenderAPI_none)
		result_return();
	asserterr_ptr(renderer->r, errcode_invalidarg);
	result_t result = 0;
	switch (renderer->renderAPI)
	{
	case cdrawRenderAPI_software:
		printf("\n cdrawRendererDestroy: Software rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Vulkan:
		result = cdrawRendererDestroy_vk(renderer);
		break;
	case cdrawRenderAPI_OpenGL:
		result = cdrawRendererDestroy_gl(renderer);
		break;
	case cdrawRenderAPI_Direct3D:
		printf("\n cdrawRendererDestroy: Direct3D rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Metal:
		printf("\n cdrawRendererDestroy: Metal rendering is not supported by this platform.");
		break;
	}
	failret(result_isclean(result), result_seterror(errcode_renderer_api));
	renderer->renderAPI = cdrawRenderAPI_none;
	result_return();
}

result_t cdrawRendererRefresh(cdrawRenderer const* const renderer)
{
	result_init();
	asserterr_ptr(renderer, errcode_invalidarg);
	if (renderer->renderAPI == cdrawRenderAPI_none)
		result_return();
	asserterr_ptr(renderer->r, errcode_invalidarg);
	result_t result = 0;
	switch (renderer->renderAPI)
	{
	case cdrawRenderAPI_software:
		printf("\n cdrawRendererDestroy: Software rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Vulkan:
		result = cdrawRendererRefresh_vk(renderer);
		break;
	case cdrawRenderAPI_OpenGL:
		result = cdrawRendererRefresh_gl(renderer);
		break;
	case cdrawRenderAPI_Direct3D:
		printf("\n cdrawRendererDestroy: Direct3D rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Metal:
		printf("\n cdrawRendererDestroy: Metal rendering is not supported by this platform.");
		break;
	}
	failret(result_isclean(result), result_seterror(errcode_renderer_api));
	result_return();
}

result_t cdrawRendererRefreshAPI(cdrawRenderAPI const renderAPI)
{
	result_init();
	if (renderAPI == cdrawRenderAPI_none)
		result_return();
	result_t result = 0;
	switch (renderAPI)
	{
	case cdrawRenderAPI_software:
		printf("\n cdrawRendererDestroy: Software rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Vulkan:
		result = cdrawRendererRefreshAPI_vk();
		break;
	case cdrawRenderAPI_OpenGL:
		result = cdrawRendererRefreshAPI_gl();
		break;
	case cdrawRenderAPI_Direct3D:
		printf("\n cdrawRendererDestroy: Direct3D rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Metal:
		printf("\n cdrawRendererDestroy: Metal rendering is not supported by this platform.");
		break;
	}
	failret(result_isclean(result), result_seterror(errcode_renderer_api));
	result_return();
}


#endif // #if !CDRAW_TARGET_WINDOWS