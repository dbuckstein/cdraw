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
* cdrawRenderer_win_gl.c
* Windows implementation of modern OpenGL renderer management.
*/

/*
*	Download latest version of GLEW (OpenGL Extension Wrangler):
*		-> http://glew.sourceforge.net/
*	Unzip versioned folder (e.g. "glew-2.1.0") into container directory at
*	system level: "C:/GLEW" (e.g. above, full path "C:/GLEW/glew-2.1.0").
*	After unzipping, run install utility:
*		-> "<framework root>/_dev/Windows/_glewlocate.bat"
*		-> Environment variables: GLEW_PATH
*	Other info about GL loaders and extension managers:
*		-> https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library
*/

#include "cdraw/cdrawPlatform/cdrawRenderer.h"
#if CDRAW_TARGET_WINDOWS
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GL/glew.h"
#include "GL/wglew.h"


/******************************************************************************
* Private implementations.
******************************************************************************/

enum
{
	cdrawMaxWindowsGL = 1,	// convenience: max number of windows for GL renderer
};


/// <summary>
/// OpenGL renderer data (Windows).
/// </summary>
typedef struct cdrawRenderer_win_gl
{
	/// <summary>
	/// OpenGL render context handle.
	/// </summary>
	HGLRC rc;

	/// <summary>
	/// Device context handle from window.
	/// </summary>
	HDC dc;

	/// <summary>
	/// OpenGL pixel format flag.
	/// </summary>
	int32_t pf;
} cdrawRenderer_win_gl;


static result_t cdrawRendererPrint_gl(cdrawRenderer_win_gl const* const r)
{
	// get strings
	cstrk_t const versionStr = glGetString(GL_VERSION);
	cstrk_t const shadingStr = glGetString(GL_SHADING_LANGUAGE_VERSION);
	cstrk_t const rendererStr = glGetString(GL_RENDERER);
	cstrk_t const vendorStr = glGetString(GL_VENDOR);
	bool const status = (versionStr != NULL);

	// display strings
	if (status)
		printf("\n OpenGL version: %s\n\t GLSL version: %s\n\t GPU renderer: %s\n\t GPU vendor:   %s", versionStr, shadingStr, rendererStr, vendorStr);
	else
		printf("\n OpenGL: Unable to determine info; ensure context is active and valid.");

	// done
	return 0;
}

#if CDRAW_DEBUG
bool cdrawRendererDisplayTest_gl();
#endif // #if CDRAW_DEBUG

static result_t cdrawRendererBeginDraw_gl(cdrawRenderer_win_gl* const r, uint32_t const windowIndex)
{
#if CDRAW_DEBUG
	// TEST: clear color and depth
	cdrawRendererDisplayTest_gl();
#endif // #if CDRAW_DEBUG

	// done
	return 0;
}

static result_t cdrawRendererEndDraw_gl(cdrawRenderer_win_gl* const r, uint32_t const windowIndex)
{
	// DO NOTHING

	// done
	return 0;
}

static result_t cdrawRendererDisplay_gl(cdrawRenderer_win_gl* const r)
{
	// swap buffers

	// done
	return 0;
}

static result_t cdrawRendererResize_gl(cdrawRenderer_win_gl* const r, uint32_t const windowIndex, uint32_t const w_old, uint32_t const h_old, uint32_t const w_new, uint32_t const h_new)
{
	// DO NOTHING, unless using framebuffer to manage display image targets (i.e. custom "swapchain" like VK)

	cdraw_assert(r);
	if ((w_old != w_new) || (h_old != h_new))
	{
		if (w_old && h_old)
		{
			// cleanup of old targets here
		}

		if (w_new && h_new)
		{
			// setup of new targets here
		}
	}

	// done
	return 0;
}

static result_t cdrawRendererAttachWindow_gl(cdrawRenderer_win_gl* const r, uint32_t const windowIndex, ptrk_t const p_data)
{
	// assign real context to window

	// done
	return 0;
}

static result_t cdrawRendererDetachWindow_gl(cdrawRenderer_win_gl* const r, uint32_t const windowIndex)
{
	// remove context from window

	// done
	return 0;
}

static result_t cdrawRendererWindowCountMax_gl(cdrawRenderer_win_gl const* const r, uint32_t* const count_out)
{
	cdraw_assert(r);
	cdraw_assert(count_out);
	*count_out = (r->rc) ? cdrawMaxWindowsGL : 0;
	return 0;
}


/// <summary>
/// OpenGL convenience function table.
/// </summary>
static cdrawRendererFuncTable gRendererFuncTable_gl;


/******************************************************************************
* Public implementations.
******************************************************************************/

result_t cdrawRendererCreate_gl(cdrawRenderer* const renderer, uint32_t const surfaceCountMax)
{
	result_init();
	size_t const dataSz = sizeof(cdrawRenderer_win_gl);
	cdrawRenderer_win_gl* r;
	asserterr(renderer && !renderer->r && !renderer->renderAPI, errcode_invalidarg);
	r = (cdrawRenderer_win_gl*)malloc(dataSz);
	asserterr_ptr(r, errcode_renderer_init);
	memset(r, 0, dataSz);

	// register dummy window class

	// create window to generate "false" context with pixel format describing capabilities

	// create window to generate "true" context with actual pixel format

	// clean up

	// all done
	renderer->r = r;
	renderer->f = &gRendererFuncTable_gl;
	result_return();
}

result_t cdrawRendererDestroy_gl(cdrawRenderer* const renderer)
{
	result_init();
	cdrawRenderer_win_gl* r;
	asserterr(renderer && renderer->r && renderer->renderAPI == cdrawRenderAPI_OpenGL, errcode_invalidarg);
	r = ((cdrawRenderer_win_gl*)renderer->r);

	// release context


	// all done
	free(renderer->r);
	renderer->r = NULL;
	renderer->f = NULL;
	result_return();
}

result_t cdrawRendererRefresh_gl(cdrawRenderer const* const renderer)
{
	result_init();
	cdrawRenderer_win_gl* r;
	asserterr(renderer && renderer->r && renderer->renderAPI == cdrawRenderAPI_OpenGL, errcode_invalidarg);
	r = ((cdrawRenderer_win_gl*)renderer->r);

	// DO NOTHING

	result_return();
}

result_t cdrawRendererRefreshAPI_gl()
{
	result_init();

	gRendererFuncTable_gl.cdrawRendererPrint = cdrawRendererPrint_gl;
	gRendererFuncTable_gl.cdrawRendererBeginDraw = cdrawRendererBeginDraw_gl;
	gRendererFuncTable_gl.cdrawRendererEndDraw = cdrawRendererEndDraw_gl;
	gRendererFuncTable_gl.cdrawRendererDisplay = cdrawRendererDisplay_gl;
	gRendererFuncTable_gl.cdrawRendererResize = cdrawRendererResize_gl;
	gRendererFuncTable_gl.cdrawRendererAttachWindow = cdrawRendererAttachWindow_gl;
	gRendererFuncTable_gl.cdrawRendererDetachWindow = cdrawRendererDetachWindow_gl;
	gRendererFuncTable_gl.cdrawRendererWindowsSupported = cdrawRendererWindowCountMax_gl;

	result_return();
}


#endif // #if !CDRAW_TARGET_WINDOWS