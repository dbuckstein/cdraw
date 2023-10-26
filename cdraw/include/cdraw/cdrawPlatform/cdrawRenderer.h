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

#include "cdraw/cdraw/cdrawConfig.h"

/******************************************************************************
* Public interfaces.
******************************************************************************/

enum
{
	errcode_renderer_init = errcode_CUSTOM_BEGIN,	// Failure with initialization.
	errcode_renderer_api,							// Failure with API call.
};


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
/// API-specific renderer handle.
/// </summary>
typedef ptr_t cdrawRendererHandle;

/// <summary>
/// Generic renderer function.
/// </summary>
typedef result_t(*cdrawRendererFunc)(cdrawRendererHandle r, ...);

/// <summary>
/// Portable renderer object.
/// </summary>
typedef struct cdrawRenderer
{
	/// <summary>
	/// Selected API internal renderer data for specific platform.
	/// </summary>
	cdrawRendererHandle r;
	
	/// <summary>
	/// Selected rendering API.
	/// </summary>
	cdrawRenderAPI renderAPI;

	/// <summary>
	/// Renderer-specific functions, set on initialization.
	/// The purpose of this "object-oriented" approach is to avoid having a switch wrapper for every single real-time method.
	/// The only methods that should be publicly fixed are those around creation or destruction, or non-real-time events.
	/// Anything that operates on overall renderer should pass its 'r' handle as the first argument.
	/// Individual renderer components will work similarly, taking the component's respective handle as the second argument.
	/// </summary>
	struct {
		cdrawRendererFunc cdrawRendererPrint;
		cdrawRendererFunc cdrawRendererDisplay;
		cdrawRendererFunc cdrawRendererResize;
	};
} cdrawRenderer;


/******************************************************************************
* Functions.
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Create and initialize internal resources for renderer of selected API.
	/// </summary>
	/// <param name="renderer">Target renderer.</param>
	/// <param name="renderAPI">Desired rendering API (must be supported on local platform).</param>
	/// <param name="p_data_opt">Optional pointer to platform data.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawRendererCreate(cdrawRenderer* const renderer, cdrawRenderAPI const renderAPI, ptrk_t const p_data_opt);

	/// <summary>
	/// Release renderer by cleaning up and destroying all of its internal resources.
	/// </summary>
	/// <param name="renderer">Target renderer.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawRendererDestroy(cdrawRenderer* const renderer);

	/// <summary>
	/// Print high-level information about renderer.
	/// </summary>
	/// <param name="renderer">Target renderer.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawRendererPrint(cdrawRenderer const* const renderer);

	/// <summary>
	/// Invoke renderer display.
	/// </summary>
	/// <param name="renderer">Target renderer.</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawRendererDisplay(cdrawRenderer const* const renderer);

	/// <summary>
	/// Invoke renderer resize; should destroy framebuffers before this and recreate them afterwards.
	/// </summary>
	/// <param name="renderer">Target renderer.</param>
	/// <param name="w_old">Old display width.</param>
	/// <param name="h_old">Old display height.</param>
	/// <param name="w_new">New display width.</param>
	/// <param name="h_new">New display height</param>
	/// <returns>Zero if success; Error code otherwise.</returns>
	result_t cdrawRendererResize(cdrawRenderer const* const renderer, uint32_t const w_old, uint32_t const h_old, uint32_t const w_new, uint32_t const h_new);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#include "_inl/cdrawRenderer.inl"


#endif // #ifndef _CDRAW_RENDERER_H_