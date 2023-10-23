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


result_t cdrawRendererCreate_win_vk(cdrawRenderer* const renderer, ptrk_t const p_data);
result_t cdrawRendererRelease_win_vk(cdrawRenderer* const renderer);
bool cdrawRendererPrint_vk(cdrawRenderer const* const renderer);


result_t cdrawRendererCreate(cdrawRenderer* const renderer, cdrawRenderAPI const renderAPI, ptrk_t const p_data_opt)
{
	result_init();
	asserterr_ptr(renderer, errcode_invalidarg);
	asserterr(renderer->p_renderer == NULL, errcode_invalidarg);
	asserterr(renderer->renderAPI == cdrawRenderAPI_none, errcode_invalidarg);
	if (renderAPI == cdrawRenderAPI_none)
		result_return();
	result_t result = 0;
	switch (renderAPI)
	{
	case cdrawRenderAPI_software:
		printf("\n cdrawRendererPrint: Software rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Vulkan:
		result = cdrawRendererCreate_win_vk(renderer, p_data_opt);
		break;
	case cdrawRenderAPI_OpenGL:
		printf("\n cdrawRendererPrint: OpenGL rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Direct3D:
		printf("\n cdrawRendererPrint: Direct3D rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Metal:
		printf("\n cdrawRendererPrint: Metal rendering is not supported by this platform.");
		break;
	}
	if (!result_isclean(result))
	{
		cdrawRendererRelease_win_vk(renderer);
		result_seterror(errcode_renderer_api);
		result_return();
	}
	renderer->renderAPI = renderAPI;
	result_return();
}

result_t cdrawRendererRelease(cdrawRenderer* const renderer)
{
	result_init();
	asserterr_ptr(renderer, errcode_invalidarg);
	if (renderer->renderAPI == cdrawRenderAPI_none)
		result_return();
	asserterr_ptr(renderer->p_renderer, errcode_invalidarg);
	result_t result = 0;
	switch (renderer->renderAPI)
	{
	case cdrawRenderAPI_software:
		printf("\n cdrawRendererPrint: Software rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Vulkan:
		result = cdrawRendererRelease_win_vk(renderer);
		break;
	case cdrawRenderAPI_OpenGL:
		printf("\n cdrawRendererPrint: OpenGL rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Direct3D:
		printf("\n cdrawRendererPrint: Direct3D rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Metal:
		printf("\n cdrawRendererPrint: Metal rendering is not supported by this platform.");
		break;
	}
	failret(result_isclean(result), result_seterror(errcode_renderer_api));
	renderer->renderAPI = cdrawRenderAPI_none;
	result_return();
}

result_t cdrawRendererPrint(cdrawRenderer const* const renderer)
{
	result_init();
	asserterr_ptr(renderer, errcode_invalidarg);
	if (renderer->renderAPI == cdrawRenderAPI_none)
	{
		printf("\n cdrawRendererPrint: No rendering in use.");
		result_return();
	}
	asserterr_ptr(renderer->p_renderer, errcode_invalidarg);
	bool result = false;
	switch (renderer->renderAPI)
	{
	case cdrawRenderAPI_software:
		printf("\n cdrawRendererPrint: Software rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Vulkan:
		result = cdrawRendererPrint_vk(renderer);
		break;
	case cdrawRenderAPI_OpenGL:
		printf("\n cdrawRendererPrint: OpenGL rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Direct3D:
		printf("\n cdrawRendererPrint: Direct3D rendering has not yet been implemented for this platform.");
		break;
	case cdrawRenderAPI_Metal:
		printf("\n cdrawRendererPrint: Metal rendering is not supported by this platform.");
		break;
	}
	failret(result, result_seterror(errcode_renderer_api));
	result_return();
}


#endif // #if !CDRAW_TARGET_WINDOWS