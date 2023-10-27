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
* cdrawRenderer_win_vk.c
* Windows implementation of modern Vulkan renderer management.
*/

/*
*	Download and install Vulkan SDK for current platform: 
*		-> https://vulkan.lunarg.com/sdk/home#windows 
*		-> Environment variables: VK_SDK_PATH & VULKAN_SDK
* 
*	Primary references (chapters cited):
*		Graham Sellers, "Vulkan Programming Guide"
*		Parminder Singh, "Learning Vulkan"
*		Pawel Lapinski, "Vulkan Cookbook"
*/

#include "cdraw/cdrawPlatform/cdrawRenderer.h"
#if CDRAW_TARGET_WINDOWS
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"


/******************************************************************************
* SECTION: Surface management (Windows).
* Reference: Singh, c.6.
* Substantial improvements: translated to C and organized.
******************************************************************************/

bool cdrawRendererDestroySurface_vk(VkSurfaceKHR* const surface_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt);

#if CDRAW_TARGET_WINDOWS
static VkWin32SurfaceCreateInfoKHR cdrawVkSurfaceCreateInfoCtor(
	HINSTANCE const hInst,
	HWND const hWnd)
{
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = { 0 };
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = hInst;
	surfaceCreateInfo.hwnd = hWnd;
	return surfaceCreateInfo;
}
#endif // #if CDRAW_TARGET_WINDOWS
static bool cdrawRendererInternalCreateSurface_win_vk(VkSurfaceKHR* const surface_out,
	VkInstance const inst, ptrk_t const p_data, VkAllocationCallbacks const* const alloc_opt)
{
	VkResult result = VK_SUCCESS;
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	cdraw_assert(surface_out && !*surface_out);
	printf("\n Creating Vulkan presentation surface...");

	// FINAL CREATE SURFACE
	if (result == VK_SUCCESS)
	{
#if CDRAW_TARGET_WINDOWS
		// required leading members of "cdrawWindow_win" (cdrawWindow_win.c)
		struct {
			HINSTANCE hInst;
			HWND hWnd;
		} const* const data = p_data;

		// surface info
		VkWin32SurfaceCreateInfoKHR const surfaceCreateInfo = cdrawVkSurfaceCreateInfoCtor(
			data->hInst,
			data->hWnd);

		// create surface
		cdraw_assert(data->hInst && data->hWnd);
		result = vkCreateWin32SurfaceKHR(inst, &surfaceCreateInfo, alloc_opt, &surface);
#endif // #if CDRAW_TARGET_WINDOWS
		if (surface)
			cdraw_assert(result == VK_SUCCESS);
	}

	// set final outputs or clean up
	if (!surface || (result != VK_SUCCESS))
	{
		cdrawRendererDestroySurface_vk(&surface, inst, alloc_opt);
		printf("\n Vulkan presentation surface creation failed.");
		return false;
	}
	*surface_out = surface;
	cdraw_assert(*surface_out);
	printf("\n Vulkan presentation surface created.");
	return true;
}

bool cdrawRendererCreateSurface_vk(VkSurfaceKHR* const surface_out,
	VkInstance const inst, ptrk_t const p_data, VkAllocationCallbacks const* const alloc_opt)
{
	return cdrawRendererInternalCreateSurface_win_vk(surface_out, inst, p_data, alloc_opt);
}

cstrk_t cdrawRendererInternalPlatformSurfaceExtName_vk()
{
	cstrk_t result = NULL;
#if CDRAW_TARGET_WINDOWS
	result = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#endif // #if CDRAW_TARGET_WINDOWS
	return result;
}

bool cdrawRendererInternalPlatformQueueFamilySupportsPresentation_vk(VkPhysicalDevice const physicalDevice, uint32_t const queueFamilyIndex)
{
	VkBool32 result = VK_FALSE;
#if CDRAW_TARGET_WINDOWS
	result = vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex);
#endif // #if CDRAW_TARGET_WINDOWS
	return (result != 0);
}


#endif // #if !CDRAW_TARGET_WINDOWS