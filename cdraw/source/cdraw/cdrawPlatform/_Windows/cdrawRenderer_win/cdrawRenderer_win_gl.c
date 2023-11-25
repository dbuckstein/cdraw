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

	/// <summary>
	/// Setup flag.
	/// </summary>
	BOOL flag;
} cdrawRenderer_win_gl;


static void cdrawRendererInternalWindowSetPixelFormat_win_gl(cdrawRenderer_win_gl* const r)
{
	// describe built-in pixel format
	PIXELFORMATDESCRIPTOR pfd[1] = { 0 };
	pfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd->nVersion = 1;					// version
	pfd->dwFlags =
		PFD_DRAW_TO_WINDOW |			// window drawing
		PFD_SUPPORT_OPENGL |			// using opengl
		PFD_DOUBLEBUFFER |				// double buffer support
		PFD_GENERIC_ACCELERATED;		// hardware acceleration
	pfd->iPixelType = PFD_TYPE_RGBA;	// RGBA colour mode
	pfd->cColorBits = 32;				// **32 bit colour buffer
	pfd->cDepthBits = 24;				// **24 bit depth buffer
	pfd->cStencilBits = 8;				// **8 bit stencil buffer
	pfd->iLayerType = PFD_MAIN_PLANE;

	// result
	int32_t pixelFormat = 0;
	int32_t numFormats = 0;

	// reset
	r->flag = 0;

	// if platform repeat flag is raised, then the default format 
	//	has been previously used; select new pixel format using 
	//	WGL to get access to new features
	if (r->pf > 0)
	{
		// WGL pixel format descriptor
		const int32_t pixelAttribs[] = {
			// standard pixel format settings
			WGL_DRAW_TO_WINDOW_ARB,				GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,				GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,				GL_TRUE,
			WGL_ACCELERATION_ARB,				WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB,					WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,					040,
			WGL_DEPTH_BITS_ARB,					030,
			WGL_STENCIL_BITS_ARB,				010,

			// multi-sampling (MSAA) settings
			WGL_SAMPLE_BUFFERS_ARB,				GL_FALSE,
			WGL_SAMPLES_ARB,					000,

			// done
			0
		};

		// get pixel format function and use it to select pixel format
		// this is a good way to validate the address of the function
		PFNWGLCHOOSEPIXELFORMATARBPROC const wglChoosePixelFormat = wglChoosePixelFormatARB;
		if (wglChoosePixelFormat)
			wglChoosePixelFormat(r->dc, pixelAttribs, 0, 1, &pixelFormat, &numFormats);
	}

	// otherwise, use built-in default descriptor
	else
	{
		pixelFormat = ChoosePixelFormat(r->dc, pfd);
	}

	// after pixel format is selected, assign it to device context
	if (pixelFormat)
		r->flag = SetPixelFormat(r->dc, pixelFormat, pfd);

	// store
	r->pf = pixelFormat;
}

static LRESULT __stdcall cdrawRendererInternalWindowEvent_win_gl(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cdrawRenderer_win_gl* r = (cdrawRenderer_win_gl*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
	switch (message)
	{
	case WM_NCCREATE: {
		r = (cdrawRenderer_win_gl*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		r->dc = GetDC(hWnd);
		SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)r);
	}	break;
	case WM_CREATE: {
		cdrawRendererInternalWindowSetPixelFormat_win_gl(r);
	}	break;
	case WM_DESTROY: {
		// DO NOTHING
	}	break;
	case WM_NCDESTROY: {
		ReleaseDC(hWnd, r->dc);
	}	break;
	}
	return DefWindowProcA(hWnd, message, wParam, lParam);
}

static void cdrawRendererInternalWindowClassInit_win_gl(WNDCLASSA* const wndClass, cstrk_t const name)
{
	wndClass->style = 0;
	wndClass->lpfnWndProc = cdrawRendererInternalWindowEvent_win_gl;
	wndClass->cbClsExtra = 0;
	wndClass->cbWndExtra = (int)sizeof(cdrawRenderer_win_gl*);
	wndClass->hInstance = GetModuleHandle(NULL);
	wndClass->hIcon = NULL;
	wndClass->hCursor = NULL;
	wndClass->hbrBackground = 0;
	wndClass->lpszMenuName = 0;
	wndClass->lpszClassName = name;
}


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
	BOOL const result = SwapBuffers(r->dc);

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
	cdraw_assert(r);

#if CDRAW_TARGET_WINDOWS
	{
		// required leading members of "cdrawWindow_win" (cdrawWindow_win.c)
		struct {
			HINSTANCE hInst;
			HWND hWnd;
			HDC hDC;
		} const* const data = p_data;

		// activate context
		r->dc = data->hDC;
		wglMakeCurrent(r->dc, r->rc);
		wglSwapIntervalEXT(1);

		// attempt to set pixel format
		cdrawRendererInternalWindowSetPixelFormat_win_gl(r);
	}
#endif // #if CDRAW_TARGET_WINDOWS

	// done
	return 0;
}

static result_t cdrawRendererDetachWindow_gl(cdrawRenderer_win_gl* const r, uint32_t const windowIndex)
{
	// remove context from window
	wglSwapIntervalEXT(0);
	wglMakeCurrent(r->dc, NULL);
	r->dc = NULL;

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
	cdrawRenderer_win_gl tmpContextFalse = { 0 }, tmpContextTrue = { 0 };
	HWND hWndFalse = NULL, hWndTrue = NULL;
	cstrk_t const wndClassName = "cdrawRendererDummyWindow_gl";
	WNDCLASSA wndClass = { 0 };

	asserterr(renderer && !renderer->r && !renderer->renderAPI, errcode_invalidarg);
	r = (cdrawRenderer_win_gl*)malloc(dataSz);
	asserterr_ptr(r, errcode_renderer_init);
	memset(r, 0, dataSz);

	// register dummy window class
	cdrawRendererInternalWindowClassInit_win_gl(&wndClass, wndClassName);
	if (RegisterClassA(&wndClass))
	{
		// create window to generate "false" context with pixel format describing capabilities
		hWndFalse = CreateWindowA(wndClassName, NULL, 0, 0, 0, 0, 0, NULL, NULL, wndClass.hInstance, &tmpContextFalse);
		if (hWndFalse)
		{
			// create window to generate "true" context with actual pixel format
			tmpContextFalse.rc = wglCreateContext(tmpContextFalse.dc);
			if (tmpContextFalse.rc)
			{
				if (wglMakeCurrent(tmpContextFalse.dc, tmpContextFalse.rc))
				{
					// initialize extensions (GPU should do this automatically, but just in case)
					if (glewInit() == GLEW_OK)
					{
						// do it again with selected format
						tmpContextTrue.pf = tmpContextFalse.pf;
						hWndTrue = CreateWindowA(wndClassName, NULL, 0, 0, 0, 0, 0, NULL, NULL, wndClass.hInstance, &tmpContextTrue);
						if (hWndTrue)
						{
							tmpContextTrue.rc = wglCreateContext(tmpContextTrue.dc);
							if (tmpContextTrue.rc)
							{
								if (wglMakeCurrent(tmpContextTrue.dc, tmpContextTrue.rc))
								{
									// hand off to output so it is not deleted with window
									r->rc = tmpContextTrue.rc;
									tmpContextTrue.rc = NULL;
								}
							}

							// clean up true context window
							DestroyWindow(hWndTrue);
						}
					}
				}

				// clean up false context
				wglDeleteContext(tmpContextFalse.rc);
			}

			// clean up false context window
			DestroyWindow(hWndFalse);
		}

		// clean up dummy class
		UnregisterClassA(wndClassName, wndClass.hInstance);
	}

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
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(r->rc);

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