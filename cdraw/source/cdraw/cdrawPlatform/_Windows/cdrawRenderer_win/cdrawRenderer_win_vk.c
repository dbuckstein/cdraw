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

#include "cdraw/cdrawPlatform/cdrawRenderer.h"
#if CDRAW_TARGET_WINDOWS
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"

#include "cdraw/cdrawCore/cdrawUtility.h"


/******************************************************************************
* Internal interfaces.
******************************************************************************/

/// <summary>
/// Vulkan renderer data for Windows platform.
/// </summary>
typedef struct cdrawRenderer_win_vk
{
	/// <summary>
	/// Vulkan instance.
	/// </summary>
	VkInstance inst;

	/// <summary>
	/// Vulkan logical device.
	/// </summary>
	VkDevice device;

	/// <summary>
	/// Vulkan presentation surface.
	/// </summary>
	VkSurfaceKHR surface;

	/// <summary>
	/// Vulkan allocation callbacks for memory management.
	/// </summary>
	VkAllocationCallbacks alloc;

#if CDRAW_DEBUG
	/// <summary>
	/// Debug report extension.
	/// </summary>
	VkDebugReportCallbackEXT debug;
#endif // #if CDRAW_DEBUG
} cdrawRenderer_win_vk;


int32_t cdrawRendererPrintLayer_vk(VkLayerProperties const* const layerProp, uint32_t const index, cstrk_t const prefix);
int32_t cdrawRendererPrintExt_vk(VkExtensionProperties const* const extProp, uint32_t const index, cstrk_t const prefix);
int32_t cdrawRendererEnumDeviceID_vk(uint32_t const id);


static bool cdrawRendererInternalAllocUse_vk(cdrawRenderer_win_vk* const p_renderer)
{
	if (!p_renderer->alloc.pUserData)
		return false;
	return true;
}

static bool cdrawRendererInternalAllocInit_vk(cdrawRenderer_win_vk* const p_renderer)
{
	//p_renderer->alloc.pUserData = malloc(0);
	if (!cdrawRendererInternalAllocUse_vk(p_renderer))
		return false;

	// ****TO-DO: set callbacks

	return true;
}

static bool cdrawRendererInternalAllocClean_vk(cdrawRenderer_win_vk* const p_renderer)
{
	if (!cdrawRendererInternalAllocUse_vk(p_renderer))
		return false;
	free(p_renderer->alloc.pUserData);
	memset(&p_renderer->alloc, 0, sizeof(p_renderer->alloc));
	return true;
}


//PFN_vkDebugReportCallbackEXT
static VkBool32 cdrawRendererInternalDebugCallback_vk(
	VkDebugReportFlagsEXT const flags,
	VkDebugReportObjectTypeEXT const objectType,
	uint64_t const object,
	size_t const location,
	int32_t const messageCode,
	cstrk_t const pLayerPrefix,
	cstrk_t const pMessage,
	ptr_t const pUserData)
{
	// print debug info
	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
		printf("cdraw Debug INFO (%s, #%d): %s \n", pLayerPrefix, messageCode, pMessage);
	else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
		printf("cdraw Debug WARNING (%s, #%d): %s \n", pLayerPrefix, messageCode, pMessage);
	else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
		printf("cdraw Debug PERFORMANCE WARNING (%s, #%d): %s \n", pLayerPrefix, messageCode, pMessage);
	else if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
		printf("cdraw Debug ERROR (%s, #%d): %s \n", pLayerPrefix, messageCode, pMessage);
	else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
		printf("cdraw Debug DEBUG (%s, #%d): %s \n", pLayerPrefix, messageCode, pMessage);
	// not handled
	else
		return VK_FALSE;

	// handled, but should still return false according to spec
	return VK_SUCCESS;
}


static bool cdrawRendererInternalInitInstance_win_vk(cdrawRenderer_win_vk* const p_renderer)
{
	VkResult result = VK_SUCCESS;
	cdraw_assert(p_renderer && !p_renderer->inst);
	VkAllocationCallbacks const* const alloc = cdrawRendererInternalAllocInit_vk(p_renderer) ? &p_renderer->alloc : NULL;

	// print prefixes
	cstrk_t const pref1 = "\t   ", pref1A = "\t * ", pref2 = "\t\t   ", pref2A = "\t\t * ", pref3 = "\t\t\t   ", pref3A = "\t\t\t * ";

	// instance layers and extensions
	cstrk_t const instLayerName_request[] = {
#if CDRAW_DEBUG
		"VK_LAYER_KHRONOS_validation",
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_LUNARG_api_dump",
		//"VK_LAYER_LUNARG_monitor",
		//"VK_LAYER_LUNARG_screenshot",

		/*// included in standard validation (7):
		"VK_LAYER_GOOGLE_threading",
		"VK_LAYER_LUNARG_parameter_validation",
		"VK_LAYER_LUNARG_object_tracker",
		"VK_LAYER_LUNARG_image",
		"VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_LUNARG_swapchain",
		"VK_LAYER_GOOGLE_unique_objects",*/
#endif // #if CDRAW_DEBUG
		NULL
	};
	cstrk_t const instLayerName_require[] = {
		NULL
	};
	cstrk_t instLayerName[buffer_len(instLayerName_request) + buffer_len(instLayerName_require)] = { NULL };
	uint32_t const instLayerName_request_count = cdrawUtilityPtrCount(instLayerName_request, buffer_len(instLayerName_request));
	uint32_t const instLayerName_require_count = cdrawUtilityPtrCount(instLayerName_require, buffer_len(instLayerName_require));
	uint32_t const instLayerName_baseLen = buffer_len(instLayerName);

	cstrk_t const instExtName_request[] = {
#if CDRAW_DEBUG
		// included with KHR validation layer (3): 
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME,
#endif // #if CDRAW_DEBUG
		NULL
	};
	cstrk_t const instExtName_require[] = {
		VK_KHR_SURFACE_EXTENSION_NAME,
#if CDRAW_TARGET_WINDOWS
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif // #if CDRAW_TARGET_WINDOWS
		NULL
	};
	cstrk_t instExtName[buffer_len(instExtName_request) + buffer_len(instExtName_require)] = { NULL };
	uint32_t const instExtName_request_count = cdrawUtilityPtrCount(instExtName_request, buffer_len(instExtName_request));
	uint32_t const instExtName_require_count = cdrawUtilityPtrCount(instExtName_require, buffer_len(instExtName_require));
	uint32_t const instExtName_baseLen = buffer_len(instExtName);

	// iterators and counts
	uint32_t layer, nInstLayer, nInstLayerEnabled = 0;
	uint32_t ext, nInstExt, nInstExtEnabled = 0;
	VkLayerProperties* pInstLayerProp;
	VkExtensionProperties* pInstExtProp;
	int32_t layerIdx, extIdx;
	cstrk_t name;

	// get available instance layers
	result = vkEnumerateInstanceLayerProperties(&nInstLayer, NULL);
	if (result == VK_SUCCESS && nInstLayer)
	{
		pInstLayerProp = (VkLayerProperties*)malloc(sizeof(VkLayerProperties) * nInstLayer);
		if (pInstLayerProp)
		{
			result = vkEnumerateInstanceLayerProperties(&nInstLayer, pInstLayerProp);
			printf("\n\t pInstLayerProp[%u]: { \"layerName\" (specVer; implVer) \"description\" }", nInstLayer);
			for (layer = 0; layer < nInstLayer; ++layer)
			{
				// copy requested layers to final list if they are found
				layerIdx = -1;
				name = pInstLayerProp[layer].layerName;
				if (cdrawUtilityStrFind(name, instLayerName, nInstLayerEnabled) < 0)
					if ((layerIdx = cdrawUtilityStrFind(name, instLayerName_request, instLayerName_request_count)) >= 0)
						instLayerName[nInstLayerEnabled++] = instLayerName_request[layerIdx];
				cdrawRendererPrintLayer_vk(&pInstLayerProp[layer], layer, (layerIdx >= 0 ? pref1A : pref1));

				// get available instance extensions related to this layer
				result = vkEnumerateInstanceExtensionProperties(name, &nInstExt, NULL);
				if (result == VK_SUCCESS && nInstExt)
				{
					pInstExtProp = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * nInstExt);
					if (pInstExtProp)
					{
						result = vkEnumerateInstanceExtensionProperties(name, &nInstExt, pInstExtProp);
						printf("\n\t\t pInstExtProp[%u]: { \"extensionName\" (specVer) }", nInstExt);
						for (ext = 0; ext < nInstExt; ++ext)
						{
							// copy requested extensions to final list if they are found
							extIdx = -1;
							name = pInstExtProp[ext].extensionName;
							if (cdrawUtilityStrFind(name, instExtName, nInstExtEnabled) < 0)
								if ((extIdx = cdrawUtilityStrFind(name, instExtName_request, instExtName_request_count)) >= 0)
									instExtName[nInstExtEnabled++] = instExtName_request[extIdx];
							cdrawRendererPrintExt_vk(&pInstExtProp[ext], ext, (extIdx >= 0 ? pref2A : pref2));
						}
						free(pInstExtProp);
						pInstExtProp = NULL;
					}
				}
			}
			free(pInstLayerProp);
			pInstLayerProp = NULL;
		}
	}

	// copy required layers to final list, confirm count
	for (layer = 0; layer < instLayerName_require_count; ++layer)
	{
		name = instLayerName_require[layer];
		if (cdrawUtilityStrFind(name, instLayerName, nInstLayerEnabled) < 0)
			instLayerName[nInstLayerEnabled++] = instLayerName_require[layer];
	}
	cdraw_assert(nInstLayerEnabled == cdrawUtilityPtrCount(instLayerName, instLayerName_baseLen));

	// copy required extensions to final list, confirm count
	for (ext = 0; ext < instExtName_require_count; ++ext)
	{
		name = instExtName_require[layer];
		if (cdrawUtilityStrFind(name, instExtName, nInstExtEnabled) < 0)
			instExtName[nInstExtEnabled++] = instExtName_require[ext];
	}
	cdraw_assert(nInstExtEnabled == cdrawUtilityPtrCount(instExtName, instExtName_baseLen));

	// FINAL CREATE INSTANCE
	{
		// application info for instance
		VkApplicationInfo const appInfo = {
			VK_STRUCTURE_TYPE_APPLICATION_INFO,
			NULL,
			"cdraw Plugin Application",
			VK_MAKE_VERSION(0, 0, 1),
			"cdraw",
			VK_MAKE_VERSION(0, 0, 1),
			VK_API_VERSION_1_2,
		};

		// instance creation data
		VkInstanceCreateInfo const instCreateInfo = {
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			NULL,
			0,
			&appInfo,
			nInstLayerEnabled,
			instLayerName,
			nInstExtEnabled,
			instExtName,
		};

		// create instance
		result = vkCreateInstance(&instCreateInfo, alloc, &p_renderer->inst);
		if (result == VK_SUCCESS)
			cdraw_assert(p_renderer->inst);
		else
			result = VK_INCOMPLETE;
	}

	// done
	return (result == VK_SUCCESS);
}

static bool cdrawRendererInternalInitDevice_vk(cdrawRenderer_win_vk* const p_renderer)
{
	VkResult result = VK_SUCCESS;
	cdraw_assert(p_renderer && p_renderer->inst);
	VkAllocationCallbacks const* const alloc = cdrawRendererInternalAllocUse_vk(p_renderer) ? &p_renderer->alloc : NULL;

	/*// device layers deprecated
	cstrk_t const deviceLayerName_request[] = {
#if CDRAW_DEBUG
		"VK_LAYER_KHRONOS_validation",
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_LUNARG_api_dump",
#endif // #if CDRAW_DEBUG
		NULL
	};
	cstrk_t const deviceLayerName_require[] = {
		NULL
	};
	cstrk_t deviceLayerName[buffer_len(deviceLayerName_request) + buffer_len(deviceLayerName_require)] = { NULL };
	uint32_t const deviceLayerName_request_count = cdrawUtilityPtrCount(deviceLayerName_request, buffer_len(deviceLayerName_request));
	uint32_t const deviceLayerName_require_count = cdrawUtilityPtrCount(deviceLayerName_require, buffer_len(deviceLayerName_require));
	uint32_t const deviceLayerName_baseLen = buffer_len(deviceLayerName);*/

	cstrk_t const deviceExtName_request[] = {
#if CDRAW_DEBUG
		// included with KHR validation layer (3): 
		VK_EXT_VALIDATION_CACHE_EXTENSION_NAME,
		VK_EXT_DEBUG_MARKER_EXTENSION_NAME,
		VK_EXT_TOOLING_INFO_EXTENSION_NAME,
#endif // #if CDRAW_DEBUG
		NULL
	};
	cstrk_t const deviceExtName_require[] = {
		NULL
	};
	cstrk_t deviceExtName[buffer_len(deviceExtName_request) + buffer_len(deviceExtName_require)] = { NULL };
	uint32_t const deviceExtName_request_count = cdrawUtilityPtrCount(deviceExtName_request, buffer_len(deviceExtName_request));
	uint32_t const deviceExtName_require_count = cdrawUtilityPtrCount(deviceExtName_require, buffer_len(deviceExtName_require));
	uint32_t const deviceExtName_baseLen = buffer_len(deviceExtName);

	// done
	return (result == VK_SUCCESS);
}

static bool cdrawRendererInternalInitSurface_win_vk(cdrawRenderer_win_vk* const p_renderer, ptrk_t const p_data)
{
	VkResult result = VK_SUCCESS;
	cdraw_assert(p_renderer && p_renderer->inst && p_data);
	VkAllocationCallbacks const* const alloc = cdrawRendererInternalAllocUse_vk(p_renderer) ? &p_renderer->alloc : NULL;

	// done
	return (result == VK_SUCCESS);
}

#if CDRAW_DEBUG
static bool cdrawRendererInternalInitDebug_vk(cdrawRenderer_win_vk* const p_renderer)
{
	VkResult result = VK_SUCCESS;
	cdraw_assert(p_renderer && p_renderer->inst);
	VkAllocationCallbacks const* const alloc = cdrawRendererInternalAllocUse_vk(p_renderer) ? &p_renderer->alloc : NULL;

	// create debug report callback function pointer
	PFN_vkCreateDebugReportCallbackEXT const vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(p_renderer->inst, "vkCreateDebugReportCallbackEXT");
	cdraw_assert(vkCreateDebugReportCallbackEXT);
	VkDebugReportCallbackCreateInfoEXT const debugCreateInfo = {
		VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
		NULL,
		(VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT),
		cdrawRendererInternalDebugCallback_vk,
		NULL,
	};

	// create callback
	result = vkCreateDebugReportCallbackEXT(p_renderer->inst, &debugCreateInfo, alloc, &p_renderer->debug);
	if (result == VK_SUCCESS)
	{
		cdraw_assert(p_renderer->debug);

		// custom message debug report
		// get function and call to add layer messages
		PFN_vkDebugReportMessageEXT const vkDebugReportMessageEXT = (PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(p_renderer->inst, "vkDebugReportMessageEXT");
		cdraw_assert(vkDebugReportMessageEXT);
	}
	else
		result = VK_INCOMPLETE;

	// done
	return (result == VK_SUCCESS);
}
#endif // #if CDRAW_DEBUG


/******************************************************************************
* Implementations.
******************************************************************************/

result_t cdrawRendererCreate_win_vk(cdrawRenderer* const renderer, ptrk_t const p_data)
{
	result_init();
	bool result = false;
	size_t const dataSz = sizeof(cdrawRenderer_win_vk);
	cdrawRenderer_win_vk* const p_renderer = (cdrawRenderer_win_vk*)malloc(dataSz);
	asserterr_ptr(p_renderer, errcode_renderer_init);
	memset(p_renderer, 0, dataSz);

	// CREATE INSTANCE
	result = cdrawRendererInternalInitInstance_win_vk(p_renderer);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE LOGICAL DEVICE
	result = cdrawRendererInternalInitDevice_vk(p_renderer);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE PRESENTATION SURFACE
	result = cdrawRendererInternalInitSurface_win_vk(p_renderer, p_data);
	failassertret(result, result_seterror(errcode_renderer_init));

#if CDRAW_DEBUG
	// CREATE DEBUGGING
	result = cdrawRendererInternalInitDebug_vk(p_renderer);
	failassertret(result, result_seterror(errcode_renderer_init));
#endif // #if CDRAW_DEBUG

	// all done
	renderer->p_renderer = p_renderer;
	result_return();
}

result_t cdrawRendererRelease_win_vk(cdrawRenderer* const renderer)
{
	result_init();
	cdrawRenderer_win_vk* const p_renderer = ((cdrawRenderer_win_vk*)renderer->p_renderer);
	cdraw_assert(p_renderer && p_renderer->inst);
	VkAllocationCallbacks const* const alloc = cdrawRendererInternalAllocUse_vk(p_renderer) ? &p_renderer->alloc : NULL;

#if CDRAW_DEBUG
	// destroy debug report callback function pointer
	if (p_renderer->debug)
	{
		PFN_vkDestroyDebugReportCallbackEXT const vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(p_renderer->inst, "vkDestroyDebugReportCallbackEXT");
		cdraw_assert(vkDestroyDebugReportCallbackEXT);
		vkDestroyDebugReportCallbackEXT(p_renderer->inst, p_renderer->debug, alloc);
	}
#endif // #if CDRAW_DEBUG

	// presentation surface (requires instance)
	if (p_renderer->surface)
		vkDestroySurfaceKHR(p_renderer->inst, p_renderer->surface, alloc);

	// logical device (wait for it to finish work)
	if (p_renderer->device)
		if (vkDeviceWaitIdle(p_renderer->device) == VK_SUCCESS)
			vkDestroyDevice(p_renderer->device, alloc);

	// finally, destroy instance
	vkDestroyInstance(p_renderer->inst, alloc);
	p_renderer->inst = NULL;

	// other data
	cdrawRendererInternalAllocClean_vk(p_renderer);

	// all done
	free(renderer->p_renderer);
	renderer->p_renderer = NULL;
	result_return();
}


#endif // #if !CDRAW_TARGET_WINDOWS