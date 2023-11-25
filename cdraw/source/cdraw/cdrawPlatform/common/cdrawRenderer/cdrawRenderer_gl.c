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
* cdrawRenderer_gl.c
* Common implementation of modern OpenGL renderer management.
*/

#include "cdraw/cdrawPlatform/cdrawRenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GL/glew.h"


#if CDRAW_DEBUG
bool cdrawRendererDisplayTest_gl()
{
	static fp32_t const s_clear = 0.5f;
	static fp32_t const clearColor[][4] = {
		{ 1.00f, 0.00f, 0.00f, 1.00f, },
		{ 0.75f, 0.25f, 0.00f, 1.00f, },
		{ 0.50f, 0.50f, 0.00f, 1.00f, },
		{ 0.25f, 0.75f, 0.00f, 1.00f, },
		{ 0.00f, 1.00f, 0.00f, 1.00f, },
		{ 0.00f, 0.75f, 0.25f, 1.00f, },
		{ 0.00f, 0.50f, 0.50f, 1.00f, },
		{ 0.00f, 0.25f, 0.75f, 1.00f, },
		{ 0.00f, 0.00f, 1.00f, 1.00f, },
		{ 0.25f, 0.00f, 0.75f, 1.00f, },
		{ 0.50f, 0.00f, 0.50f, 1.00f, },
		{ 0.75f, 0.00f, 0.25f, 1.00f, },
	};
	static uint32_t const rateToCount = 60 / buffer_len(clearColor);
	static fp64_t const clearDepth = 1.0;
	static int32_t const clearStencil = 0;
	static uint32_t clearColorIdx;

	// select clear values
	fp32_t clearColorValue[4];
	fp32_t const* const clearColorValuePtr = clearColor[((clearColorIdx++) / rateToCount) % buffer_len(clearColor)];
	cdraw_assert(clearColorValuePtr);
	clearColorValue[0] = clearColorValuePtr[0] * s_clear;
	clearColorValue[1] = clearColorValuePtr[1] * s_clear;
	clearColorValue[2] = clearColorValuePtr[2] * s_clear;
	clearColorValue[3] = clearColorValuePtr[3] * s_clear;

	// clear
	glClearColor(clearColorValue[0], clearColorValue[1], clearColorValue[2], clearColorValue[3]);
	glClearDepth(clearDepth);
	glClearStencil(clearStencil);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// done
	return true;
}
#endif // #if CDRAW_DEBUG
