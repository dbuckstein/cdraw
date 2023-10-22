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
* cdrawRenderer.h
* Main interface for renderer.
*/

#ifndef _CDRAW_RENDERER_H_
#define _CDRAW_RENDERER_H_

#include "cdrawWindow.h"

/******************************************************************************
* Public interfaces.
******************************************************************************/

/// <summary>
/// Rendering API choices.
/// </summary>
typedef enum cdrawRenderAPI
{
	cdrawRenderAPI_none,		// No rendering.
	cdrawRenderAPI_software,	// Custom software renderer.
	cdrawRenderAPI_Vulkan,		// Modern Vulkan renderer.
	cdrawRenderAPI_OpenGL,		// Modern OpenGL renderer.
	cdrawRenderAPI_Direct3D,	// Modern Direct3D renderer (Microsoft native).
	cdrawRenderAPI_Metal,		// Modern Metal renderer (Apple native).
} cdrawRenderAPI;


/// <summary>
/// Portable renderer handle.
/// </summary>
typedef struct cdrawRenderer
{
	/// <summary>
	/// Selected API renderer data.
	/// </summary>
	ptr_t p_renderer;
	/// <summary>
	/// Selected API renderer data for specific platform.
	/// </summary>
	ptr_t p_renderer_platform;
	/// <summary>
	/// Selected rendering API.
	/// </summary>
	cdrawRenderAPI renderAPI;
} cdrawRenderer;


/******************************************************************************
* Functions.
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus



#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef _CDRAW_RENDERER_H_