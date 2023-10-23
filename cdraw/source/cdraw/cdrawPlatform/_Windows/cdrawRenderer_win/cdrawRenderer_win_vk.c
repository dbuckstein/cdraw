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

#include "cdraw/cdrawCore/cdrawUtility.h"


/******************************************************************************
* Internal interfaces.
******************************************************************************/

/// <summary>
/// Complete description of Vulkan physical device.
/// </summary>
typedef struct cdrawVkPhysicalDevice
{
	/// <summary>
	/// Device properties.
	/// </summary>
	VkPhysicalDeviceProperties deviceProp;

	/// <summary>
	/// Device memory properties.
	/// </summary>
	VkPhysicalDeviceMemoryProperties deviceMemProp;

	/// <summary>
	/// Queue family selected for graphics.
	/// </summary>
	VkQueueFamilyProperties queueFamilyProp_graphics;

	/// <summary>
	/// Device features.
	/// </summary>
	VkPhysicalDeviceFeatures deviceFeat;

	/// <summary>
	/// Device features actually requested/used.
	/// </summary>
	VkPhysicalDeviceFeatures deviceFeatUse;
} cdrawVkPhysicalDevice;

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

	/// <summary>
	/// Description of physical device used.
	/// </summary>
	cdrawVkPhysicalDevice physicalDevice;
} cdrawRenderer_win_vk;


int32_t cdrawRendererPrintLayer_vk(VkLayerProperties const* const layerProp, uint32_t const index, cstrk_t const prefix);
int32_t cdrawRendererPrintExt_vk(VkExtensionProperties const* const extProp, uint32_t const index, cstrk_t const prefix);
int32_t cdrawRendererPrintPhysicalDevice_vk(VkPhysicalDeviceProperties const* const physicalDeviceProp, uint32_t const index, cstrk_t const prefix);
int32_t cdrawRendererPrintQueueFamily_vk(VkQueueFamilyProperties const* const queueFamilyProp, uint32_t const index, cstrk_t const prefix);
int32_t cdrawRendererPrintMemoryType_vk(VkMemoryType const* const memoryType, uint32_t const index, cstrk_t const prefix);
int32_t cdrawRendererPrintMemoryHeap_vk(VkMemoryHeap const* const memoryHeap, uint32_t const index, cstrk_t const prefix);


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
		printf("\n cdraw Debug INFO (%s, #%d): %s", pLayerPrefix, messageCode, pMessage);
	else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
		printf("\n cdraw Debug WARNING (%s, #%d): %s", pLayerPrefix, messageCode, pMessage);
	else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
		printf("\n cdraw Debug PERFORMANCE WARNING (%s, #%d): %s", pLayerPrefix, messageCode, pMessage);
	else if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
		printf("\n cdraw Debug ERROR (%s, #%d): %s", pLayerPrefix, messageCode, pMessage);
	else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
		printf("\n cdraw Debug DEBUG (%s, #%d): %s", pLayerPrefix, messageCode, pMessage);
	// not handled
	else
		return VK_FALSE;

	// handled, but should still return false according to spec
	return VK_SUCCESS;
}


/*
* Singh, c.3 - translated to C
*/
static bool cdrawRendererInternalCreateInstance_win_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	VkResult result = VK_SUCCESS;
	cdraw_assert(p_renderer && !p_renderer->inst);

	// print prefixes
	cstrk_t const pref1 = "\t    ", pref1A = "\t -> ", pref2 = "\t\t    ", pref2A = "\t\t -> ", pref3 = "\t\t\t    ", pref3A = "\t\t\t -> ";

	// instance layers
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

	// instance extensions
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

	printf("\n Creating Vulkan instance...");

	// get available instance layers; may be zero
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

				// get available instance extensions related to this layer; may be zero
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
		{
			instLayerName[nInstLayerEnabled++] = name;
			printf("\n\t Additional layer: \"%s\"", name);
		}
	}
	cdraw_assert(nInstLayerEnabled == cdrawUtilityPtrCount(instLayerName, instLayerName_baseLen));

	// copy required extensions to final list, confirm count
	for (ext = 0; ext < instExtName_require_count; ++ext)
	{
		name = instExtName_require[ext];
		if (cdrawUtilityStrFind(name, instExtName, nInstExtEnabled) < 0)
		{
			instExtName[nInstExtEnabled++] = name;
			printf("\n\t\t Additional extension: \"%s\"", name);
		}
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
		{
			cdraw_assert(p_renderer->inst);
		}
		else
			result = VK_INCOMPLETE;
	}

	// done
	if (result != VK_SUCCESS)
	{
		printf("\n Vulkan instance creation failed.");
		return false;
	}
	printf("\n Vulkan instance creation succeeded.");
	return true;
}

static bool cdrawRendererInternalReleaseInstance_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	cdraw_assert(p_renderer);
	if (!p_renderer->inst)
		return false;

	vkDestroyInstance(p_renderer->inst, alloc);
	p_renderer->inst = NULL;
	printf("\n Released Vulkan instance.");
	return true;
}

/*
* Singh, c.3 - translated to C
*/
static bool cdrawRendererInternalCreateDevice_win_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	VkResult result = VK_SUCCESS;
	cdraw_assert(p_renderer && p_renderer->inst && !p_renderer->device);

	// print prefixes
	cstrk_t const pref1 = "\t    ", pref1A = "\t -> ", pref2 = "\t\t    ", pref2A = "\t\t -> ", pref3 = "\t\t\t    ", pref3A = "\t\t\t -> ";

	// limits
	enum {
		device_queue_family_queues_max = 16,		// hard limit
		device_queue_family_compute_only_max = 8,	// hard limit
	};
	fp32_t queuePriority_graphics[device_queue_family_queues_max] = { 0 };

	// temporary, to describe each family here
	VkDeviceQueueCreateInfo const tmpQueueCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		NULL,
		0,
		-1,		// invalid family index
		0,		// invalid queue count
		NULL,	// invalid priority list
	};

	// device descriptors
	VkPhysicalDeviceType const physicalDeviceSelectType_require =
		(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
	int32_t physicalDeviceSelectIdx = -1;
	VkPhysicalDevice physicalDeviceSelect = NULL;
	VkPhysicalDeviceProperties physicalDeviceProp;

	// queue family descriptors
	VkQueueFlagBits const queueFamilySelectType_graphics_require =
		(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
	int32_t queueFamilySelectIdx_graphics = -1;

	// device layers (deprecated)
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
	uint32_t const deviceLayerName_baseLen = buffer_len(deviceLayerName);

	// device extensions
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
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		NULL
	};
	cstrk_t deviceExtName[buffer_len(deviceExtName_request) + buffer_len(deviceExtName_require)] = { NULL };
	uint32_t const deviceExtName_request_count = cdrawUtilityPtrCount(deviceExtName_request, buffer_len(deviceExtName_request));
	uint32_t const deviceExtName_require_count = cdrawUtilityPtrCount(deviceExtName_require, buffer_len(deviceExtName_require));
	uint32_t const deviceExtName_baseLen = buffer_len(deviceExtName);

	// iterators and counts
	uint32_t device, nPhysicalDevice;
	uint32_t layer, nDeviceLayer, nDeviceLayerEnabled = 0;
	uint32_t ext, nDeviceExt, nDeviceExtEnabled = 0;
	uint32_t family, nQueueFamily;
	uint32_t memory;
	VkPhysicalDevice* pPhysicalDevice;
	VkLayerProperties* pDeviceLayerProp;
	VkExtensionProperties* pDeviceExtProp;
	VkQueueFamilyProperties* pQueueFamilyProp;
	int32_t layerIdx, extIdx;
	cstrk_t name;

	printf("\n Creating Vulkan logical device...");

	// get physical devices
	result = vkEnumeratePhysicalDevices(p_renderer->inst, &nPhysicalDevice, NULL);
	if (result == VK_SUCCESS)
	{
		// should not be zero if we are to proceed
		cdraw_assert(nPhysicalDevice);
		pPhysicalDevice = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * nPhysicalDevice);
		if (pPhysicalDevice)
		{
			result = vkEnumeratePhysicalDevices(p_renderer->inst, &nPhysicalDevice, pPhysicalDevice);
			printf("\n\t pPhysicalDevice[%u]: { \"name\" [type] (apiVer; driverVer; vendorID; deviceID) }", nPhysicalDevice);
			for (device = 0; device < nPhysicalDevice; ++device)
			{
				// find most capable device (e.g. discrete GPU should be priority)
				vkGetPhysicalDeviceProperties(pPhysicalDevice[device], &physicalDeviceProp);
				if (flagcheckexcl(physicalDeviceProp.deviceType, physicalDeviceSelectType_require)
					&& physicalDeviceSelectIdx < 0)
				{
					physicalDeviceSelectIdx = device;
					cdrawRendererPrintPhysicalDevice_vk(&physicalDeviceProp, physicalDeviceSelectIdx, pref1A);
				}
				else
					cdrawRendererPrintPhysicalDevice_vk(&physicalDeviceProp, device, pref1);
			}

			// select physical device
			if (physicalDeviceSelectIdx >= 0)
			{
				physicalDeviceSelect = pPhysicalDevice[physicalDeviceSelectIdx];
			}

			free(pPhysicalDevice);
			pPhysicalDevice = NULL;
		}

		// setup logical device from selected physical device
		if (physicalDeviceSelect)
		{
			// device layers; may be zero (deprecated for use in logical device)
			result = vkEnumerateDeviceLayerProperties(physicalDeviceSelect, &nDeviceLayer, NULL);
			if (result == VK_SUCCESS && nDeviceLayer)
			{
				pDeviceLayerProp = (VkLayerProperties*)malloc(sizeof(VkLayerProperties) * nDeviceLayer);
				if (pDeviceLayerProp)
				{
					result = vkEnumerateDeviceLayerProperties(physicalDeviceSelect, &nDeviceLayer, pDeviceLayerProp);
					printf("\n\t\t pDeviceLayerProp[%u]: ", nDeviceLayer);
					for (layer = 0; layer < nDeviceLayer; ++layer)
					{
						// same logic as instance layers
						layerIdx = -1;
						name = pDeviceLayerProp[layer].layerName;
						if (cdrawUtilityStrFind(name, deviceLayerName, nDeviceLayerEnabled) < 0)
							if ((layerIdx = cdrawUtilityStrFind(name, deviceLayerName_request, deviceLayerName_request_count)) >= 0)
								deviceLayerName[nDeviceLayerEnabled++] = deviceLayerName_request[layerIdx];
						cdrawRendererPrintLayer_vk(&pDeviceLayerProp[layer], layer, (layerIdx >= 0 ? pref2A : pref2));

						// device extensions; may be zero
						result = vkEnumerateDeviceExtensionProperties(physicalDeviceSelect, name, &nDeviceExt, NULL);
						if (result == VK_SUCCESS && nDeviceExt)
						{
							pDeviceExtProp = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * nDeviceExt);
							if (pDeviceExtProp)
							{
								result = vkEnumerateDeviceExtensionProperties(physicalDeviceSelect, name, &nDeviceExt, pDeviceExtProp);
								printf("\n\t\t\t pDeviceExtProp[%u]:", nDeviceExt);
								for (ext = 0; ext < nDeviceExt; ++ext)
								{
									// same logic as instance extensions
									extIdx = -1;
									name = pDeviceExtProp[ext].extensionName;
									if (cdrawUtilityStrFind(name, deviceExtName, nDeviceExtEnabled) < 0)
										if ((extIdx = cdrawUtilityStrFind(name, deviceExtName_request, deviceExtName_request_count)) >= 0)
											deviceExtName[nDeviceExtEnabled++] = deviceExtName_request[extIdx];
									cdrawRendererPrintExt_vk(&pDeviceExtProp[ext], ext, (extIdx >= 0 ? pref3A : pref3));
								}
								free(pDeviceExtProp);
								pDeviceExtProp = NULL;
							}
						}
					}
					free(pDeviceLayerProp);
					pDeviceLayerProp = NULL;
				}
			}

			for (layer = 0; layer < deviceLayerName_require_count; ++layer)
			{
				name = deviceLayerName_require[layer];
				if (cdrawUtilityStrFind(name, deviceLayerName, nDeviceLayerEnabled) < 0)
				{
					deviceLayerName[nDeviceLayerEnabled++] = name;
					printf("\n\t\t Additional layer: \"%s\"", name);
				}
			}
			cdraw_assert(nDeviceLayerEnabled == cdrawUtilityPtrCount(deviceLayerName, deviceLayerName_baseLen));

			for (ext = 0; ext < deviceExtName_require_count; ++ext)
			{
				name = deviceExtName_require[ext];
				if (cdrawUtilityStrFind(name, deviceExtName, nDeviceExtEnabled) < 0)
				{
					deviceExtName[nDeviceExtEnabled++] = name;
					printf("\n\t\t\t Additional extension: \"%s\"", name);
				}
			}
			cdraw_assert(nDeviceExtEnabled == cdrawUtilityPtrCount(deviceExtName, deviceExtName_baseLen));

			// set up queue family info
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceSelect, &nQueueFamily, NULL);
			if (nQueueFamily)
			{
				pQueueFamilyProp = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * nQueueFamily);
				if (pQueueFamilyProp)
				{
					vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceSelect, &nQueueFamily, pQueueFamilyProp);
					printf("\n\t\t pQueueFamilyProp[%u]: { [flags] (count; timestamp valid bits; min image transfer granularity) }", nQueueFamily);
					for (family = 0; family < nQueueFamily; ++family)
					{
						// save best queue family supporting graphics and presentation
						if (flagcheckexcl(pQueueFamilyProp[family].queueFlags, queueFamilySelectType_graphics_require)
#if CDRAW_TARGET_WINDOWS
							&& vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDeviceSelect, family)
#endif // #if CDRAW_TARGET_WINDOWS
							&& queueFamilySelectIdx_graphics < 0)
						{
							queueFamilySelectIdx_graphics = family;
							cdrawRendererPrintQueueFamily_vk(&pQueueFamilyProp[family], queueFamilySelectIdx_graphics, pref2A);
						}
						else
							cdrawRendererPrintQueueFamily_vk(&pQueueFamilyProp[family], family, pref2);
					}

					if (queueFamilySelectIdx_graphics >= 0)
					{
						p_renderer->physicalDevice.queueFamilyProp_graphics = pQueueFamilyProp[family];
					}

					free(pQueueFamilyProp);
					pQueueFamilyProp = NULL;
				}
			}

			// device features
			{
				VkPhysicalDeviceFeatures
					* const deviceFeat = &p_renderer->physicalDevice.deviceFeat,
					* const deviceFeatUse = &p_renderer->physicalDevice.deviceFeatUse;
				cdraw_assert(deviceFeat && deviceFeatUse);

				vkGetPhysicalDeviceProperties(physicalDeviceSelect, &p_renderer->physicalDevice.deviceProp);
				vkGetPhysicalDeviceFeatures(physicalDeviceSelect, deviceFeat);
				memset(deviceFeatUse, 0, sizeof(*deviceFeatUse));
				deviceFeatUse->geometryShader = VK_TRUE;
				deviceFeatUse->tessellationShader = VK_TRUE;
				deviceFeatUse->multiDrawIndirect = deviceFeat->multiDrawIndirect;
				//deviceFeatUse->multiViewport = deviceFeat->multiViewport;
			}

			// device memory
			{
				VkPhysicalDeviceMemoryProperties
					* const deviceMemProp = &p_renderer->physicalDevice.deviceMemProp;
				cdraw_assert(deviceMemProp);

				vkGetPhysicalDeviceMemoryProperties(physicalDeviceSelect, deviceMemProp);
				printf("\n\t nMemoryType = %u: { [flags] (heap index) }", deviceMemProp->memoryTypeCount);
				for (memory = 0; memory < deviceMemProp->memoryTypeCount; ++memory)
					cdrawRendererPrintMemoryType_vk(&deviceMemProp->memoryTypes[memory], memory, pref1);
				printf("\n\t nMemoryHeap = %u: { [flags] (device size) }", deviceMemProp->memoryHeapCount);
				for (memory = 0; memory < deviceMemProp->memoryHeapCount; ++memory)
					cdrawRendererPrintMemoryHeap_vk(&deviceMemProp->memoryHeaps[memory], memory, pref1);
			}

			// FINAL CREATE LOGICAL DEVICE
			{
				// queue creation data for graphics
				VkDeviceQueueCreateInfo const queueCreateInfo_graphics = {
					VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					NULL,
					0,
					queueFamilySelectIdx_graphics,
					1,
					queuePriority_graphics,
				};

				// all queue creation data
				VkDeviceQueueCreateInfo const queueCreateInfo[] = {
					queueCreateInfo_graphics,
				};

				// device creation data
				VkDeviceCreateInfo const deviceCreateInfo = {
					VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
					NULL,
					0,
					buffer_len(queueCreateInfo),
					queueCreateInfo,
					nDeviceLayerEnabled,
					deviceLayerName,
					nDeviceExtEnabled,
					deviceExtName,
					&p_renderer->physicalDevice.deviceFeatUse,
				};

				// create device
				result = vkCreateDevice(physicalDeviceSelect, &deviceCreateInfo, alloc, &p_renderer->device);
				if (result == VK_SUCCESS)
				{
					cdraw_assert(p_renderer->device);
				}
				else
					result = VK_INCOMPLETE;
			}
		}
		else
			result = VK_INCOMPLETE;
	}

	// done
	if (result != VK_SUCCESS)
	{
		printf("\n Vulkan logical device creation failed.");
		return false;
	}
	printf("\n Vulkan logical device creation succeeded.");
	return true;
}

static bool cdrawRendererInternalReleaseDevice_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	cdraw_assert(p_renderer);
	if (!p_renderer->device)
		return false;

	if (vkDeviceWaitIdle(p_renderer->device) != VK_SUCCESS)
		return false;

	memset(&p_renderer->physicalDevice, 0, sizeof(p_renderer->physicalDevice));
	vkDestroyDevice(p_renderer->device, alloc);
	p_renderer->device = NULL;
	printf("\n Released Vulkan logical device.");
	return true;
}

/*
* Singh, c.6 - translated to C
*/
static bool cdrawRendererInternalCreateSurface_win_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc, ptrk_t const p_data)
{
	VkResult result = VK_SUCCESS;
	cdraw_assert(p_renderer && p_renderer->inst && !p_renderer->surface && p_data);

	printf("\n Creating Vulkan presentation surface...");



	// done
	if (result != VK_SUCCESS)
	{
		printf("\n Vulkan presentation surface creation failed.");
		return false;
	}
	printf("\n Vulkan presentation surface creation succeeded.");
	return true;
}

static bool cdrawRendererInternalReleaseSurface_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	cdraw_assert(p_renderer && p_renderer->inst);
	if (!p_renderer->surface)
		return false;

	vkDestroySurfaceKHR(p_renderer->inst, p_renderer->surface, alloc);
	p_renderer->surface = NULL;
	printf("\n Released Vulkan presentation surface.");
	return true;
}

#if CDRAW_DEBUG
/*
* Singh, c.4 - translated to C
*/
static bool cdrawRendererInternalCreateDebug_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	VkResult result = VK_SUCCESS;
	cdraw_assert(p_renderer && p_renderer->inst && !p_renderer->debug);

	// create debug report callback function pointer
	PFN_vkCreateDebugReportCallbackEXT const vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(p_renderer->inst, "vkCreateDebugReportCallbackEXT");
	VkDebugReportCallbackCreateInfoEXT const debugCreateInfo = {
		VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
		NULL,
		(VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT),
		cdrawRendererInternalDebugCallback_vk,
		NULL,
	};

	printf("\n Creating Vulkan debugging...");

	// create debugging callback
	cdraw_assert(vkCreateDebugReportCallbackEXT);
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
	if (result != VK_SUCCESS)
	{
		printf("\n Vulkan debugging creation failed.");
		return false;
	}
	printf("\n Vulkan debugging creation succeeded.");
	return true;
}

static bool cdrawRendererInternalReleaseDebug_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	cdraw_assert(p_renderer && p_renderer->inst);
	if (!p_renderer->debug)
		return false;
	{
		PFN_vkDestroyDebugReportCallbackEXT const vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(p_renderer->inst, "vkDestroyDebugReportCallbackEXT");
		cdraw_assert(vkDestroyDebugReportCallbackEXT);
		vkDestroyDebugReportCallbackEXT(p_renderer->inst, p_renderer->debug, alloc);
	}
	p_renderer->debug = NULL;
	printf("\n Released Vulkan debugging.");
	return true;
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

	// setup allocation callbacks
	VkAllocationCallbacks const* const alloc = cdrawRendererInternalAllocInit_vk(p_renderer) ? &p_renderer->alloc : NULL;

	// CREATE INSTANCE
	result = cdrawRendererInternalCreateInstance_win_vk(p_renderer, alloc);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE LOGICAL DEVICE
	result = cdrawRendererInternalCreateDevice_win_vk(p_renderer, alloc);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE PRESENTATION SURFACE
	result = cdrawRendererInternalCreateSurface_win_vk(p_renderer, alloc, p_data);
	failassertret(result, result_seterror(errcode_renderer_init));

#if CDRAW_DEBUG
	// CREATE DEBUGGING
	result = cdrawRendererInternalCreateDebug_vk(p_renderer, alloc);
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
	cdraw_assert(p_renderer);
	VkAllocationCallbacks const* const alloc = cdrawRendererInternalAllocUse_vk(p_renderer) ? &p_renderer->alloc : NULL;

#if CDRAW_DEBUG
	// destroy debug report callback function pointer
	cdrawRendererInternalReleaseDebug_vk(p_renderer, alloc);
#endif // #if CDRAW_DEBUG

	// presentation surface (requires instance)
	cdrawRendererInternalReleaseSurface_vk(p_renderer, alloc);

	// logical device (wait for it to finish work)
	cdrawRendererInternalReleaseDevice_vk(p_renderer, alloc);

	// finally, destroy instance
	cdrawRendererInternalReleaseInstance_vk(p_renderer, alloc);

	// other data
	cdrawRendererInternalAllocClean_vk(p_renderer);

	// all done
	free(renderer->p_renderer);
	renderer->p_renderer = NULL;
	result_return();
}


#endif // #if !CDRAW_TARGET_WINDOWS