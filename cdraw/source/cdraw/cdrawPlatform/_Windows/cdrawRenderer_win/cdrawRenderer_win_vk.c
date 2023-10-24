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
	/// Device handle.
	/// </summary>
	VkPhysicalDevice device;

	/// <summary>
	/// Device properties.
	/// </summary>
	VkPhysicalDeviceProperties deviceProp;

	/// <summary>
	/// Device memory properties.
	/// </summary>
	VkPhysicalDeviceMemoryProperties deviceMemProp;

	/// <summary>
	/// Device features.
	/// </summary>
	VkPhysicalDeviceFeatures deviceFeat;

	/// <summary>
	/// Device features actually requested/used.
	/// </summary>
	VkPhysicalDeviceFeatures deviceFeatUse;

	/// <summary>
	/// Queue family selected for graphics.
	/// </summary>
	VkQueueFamilyProperties queueFamilyProp_graphics;

	/// <summary>
	/// Index of graphics queue family.
	/// </summary>
	int32_t queueFamilyIdx_graphics;
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
	/// Vulkan graphics/presentation queue.
	/// </summary>
	VkQueue queue;

	/// <summary>
	/// Vulkan swapchain.
	/// </summary>
	VkSwapchainKHR swapchain;

#if CDRAW_DEBUG
	/// <summary>
	/// Debug report extension.
	/// </summary>
	VkDebugReportCallbackEXT debug;
#endif // #if CDRAW_DEBUG

	/// <summary>
	/// Vulkan allocation callbacks for memory management.
	/// </summary>
	VkAllocationCallbacks alloc;

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
* Singh, c.3 - translated to C and organized
*/
static VkApplicationInfo cdrawVkApplicationInfoCtor(
	cstrk_t const appName,
	uint32_t const appVer,
	cstrk_t const engineName,
	uint32_t const engineVer)
{
	VkApplicationInfo applicationInfo = { 0 };
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = appName;
	applicationInfo.applicationVersion = appVer;
	applicationInfo.pEngineName = engineName;
	applicationInfo.engineVersion = engineVer;
	vkEnumerateInstanceVersion(&applicationInfo.apiVersion);
	return applicationInfo;
}
static VkInstanceCreateInfo cdrawVkInstanceCreateInfoCtor(
	VkApplicationInfo const* const appInfo,
	uint32_t const layerCount,
	cstrk_t const layerNames[],
	uint32_t const extCount,
	cstrk_t const extNames[])
{
	VkInstanceCreateInfo instanceCreateInfo = { 0 };
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = appInfo;
	instanceCreateInfo.enabledLayerCount = layerCount;
	instanceCreateInfo.ppEnabledLayerNames = layerNames;
	instanceCreateInfo.enabledExtensionCount = extCount;
	instanceCreateInfo.ppEnabledExtensionNames = extNames;
	return instanceCreateInfo;
}
static bool cdrawRendererInternalCreateInstance_win_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
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
	uint32_t nInstLayer = 0, nInstLayerEnabled = 0;
	uint32_t nInstExt = 0, nInstExtEnabled = 0;
	VkLayerProperties* pInstLayerProp = NULL;
	VkExtensionProperties* pInstExtProp = NULL;
	uint32_t layerItr, extItr;
	int32_t layerIdx, extIdx;
	cstrk_t name;

	VkResult result = VK_SUCCESS;
	VkInstance inst = VK_NULL_HANDLE;
	cdraw_assert(p_renderer);
	cdraw_assert(!p_renderer->inst);
	printf("\n Creating Vulkan instance...");

	// get available instance layers; may be zero
	result = vkEnumerateInstanceLayerProperties(&nInstLayer, NULL);
	if (result == VK_SUCCESS && nInstLayer)
	{
		pInstLayerProp = (VkLayerProperties*)malloc(sizeof(VkLayerProperties) * nInstLayer);
		if (pInstLayerProp)
		{
			result = vkEnumerateInstanceLayerProperties(&nInstLayer, pInstLayerProp);
			cdraw_assert(result == VK_SUCCESS);
			printf("\n\t pInstLayerProp[%u]: { \"layerName\" (specVer; implVer) \"description\" }", nInstLayer);
			for (layerItr = 0; layerItr < nInstLayer; ++layerItr)
			{
				// copy requested layers to final list if they are found
				layerIdx = -1;
				name = pInstLayerProp[layerItr].layerName;
				if (cdrawUtilityStrFind(name, instLayerName, nInstLayerEnabled) < 0)
					if ((layerIdx = cdrawUtilityStrFind(name, instLayerName_request, instLayerName_request_count)) >= 0)
						instLayerName[nInstLayerEnabled++] = instLayerName_request[layerIdx];
				cdrawRendererPrintLayer_vk(&pInstLayerProp[layerItr], layerItr, (layerIdx >= 0 ? pref1A : pref1));

				// get available instance extensions related to this layer; may be zero
				result = vkEnumerateInstanceExtensionProperties(name, &nInstExt, NULL);
				if (result == VK_SUCCESS && nInstExt)
				{
					pInstExtProp = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * nInstExt);
					if (pInstExtProp)
					{
						result = vkEnumerateInstanceExtensionProperties(name, &nInstExt, pInstExtProp);
						cdraw_assert(result == VK_SUCCESS);
						printf("\n\t\t pInstExtProp[%u]: { \"extensionName\" (specVer) }", nInstExt);
						for (extItr = 0; extItr < nInstExt; ++extItr)
						{
							// copy requested extensions to final list if they are found
							extIdx = -1;
							name = pInstExtProp[extItr].extensionName;
							if (cdrawUtilityStrFind(name, instExtName, nInstExtEnabled) < 0)
								if ((extIdx = cdrawUtilityStrFind(name, instExtName_request, instExtName_request_count)) >= 0)
									instExtName[nInstExtEnabled++] = instExtName_request[extItr];
							cdrawRendererPrintExt_vk(&pInstExtProp[extItr], extItr, (extIdx >= 0 ? pref2A : pref2));
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
	for (layerItr = 0; layerItr < instLayerName_require_count; ++layerItr)
	{
		name = instLayerName_require[layerItr];
		if (cdrawUtilityStrFind(name, instLayerName, nInstLayerEnabled) < 0)
		{
			instLayerName[nInstLayerEnabled++] = name;
			printf("\n\t Additional layer: \"%s\"", name);
		}
	}
	cdraw_assert(nInstLayerEnabled == cdrawUtilityPtrCount(instLayerName, instLayerName_baseLen));

	// copy required extensions to final list, confirm count
	for (extItr = 0; extItr < instExtName_require_count; ++extItr)
	{
		name = instExtName_require[extItr];
		if (cdrawUtilityStrFind(name, instExtName, nInstExtEnabled) < 0)
		{
			instExtName[nInstExtEnabled++] = name;
			printf("\n\t\t Additional extension: \"%s\"", name);
		}
	}
	cdraw_assert(nInstExtEnabled == cdrawUtilityPtrCount(instExtName, instExtName_baseLen));

	// FINAL CREATE INSTANCE
	if (result == VK_SUCCESS)
	{
		VkApplicationInfo const appInfo = cdrawVkApplicationInfoCtor(
			"cdraw Plugin Application",
			VK_MAKE_VERSION(0, 0, 1),
			"cdraw",
			VK_MAKE_VERSION(0, 0, 1));
		VkInstanceCreateInfo const instCreateInfo = cdrawVkInstanceCreateInfoCtor(
			&appInfo,
			nInstLayerEnabled,
			instLayerName,
			nInstExtEnabled,
			instExtName);
		result = vkCreateInstance(&instCreateInfo, alloc, &inst);
		if (inst)
		{
			cdraw_assert(result == VK_SUCCESS);
			p_renderer->inst = inst;
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
	cdraw_assert(p_renderer->inst);
	printf("\n Vulkan instance creation succeeded.");
	return true;
}

static bool cdrawRendererInternalReleaseInstance_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	cdraw_assert(p_renderer);
	if (!p_renderer->inst)
		return false;

	vkDestroyInstance(p_renderer->inst, alloc);

	printf("\n Released Vulkan instance.");
	p_renderer->inst = NULL;
	return true;
}

/*
* Singh, c.3 - translated to C and organized
*/
static VkDeviceQueueCreateInfo cdrawVkDeviceQueueCreateInfoCtor(
	uint32_t const queueFamilyIndex,
	uint32_t const queueCount,
	fp32_t const queuePriorities[])
{
	VkDeviceQueueCreateInfo deviceQueueCreateInfo = { 0 };
	deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
	deviceQueueCreateInfo.queueCount = queueCount;
	deviceQueueCreateInfo.pQueuePriorities = queuePriorities;
	return deviceQueueCreateInfo;
}
static VkDeviceCreateInfo cdrawVkDeviceCreateInfoCtor(
	uint32_t const queueCreateInfoCount,
	VkDeviceQueueCreateInfo const queueCreateInfo[],
	uint32_t const layerCount,
	cstrk_t const layerNames[],
	uint32_t const extCount,
	cstrk_t const extNames[],
	VkPhysicalDeviceFeatures const* const deviceFeat)
{
	VkDeviceCreateInfo deviceCreateInfo = { 0 };
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = queueCreateInfoCount;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfo;
	deviceCreateInfo.enabledLayerCount = layerCount;
	deviceCreateInfo.ppEnabledLayerNames = layerNames;
	deviceCreateInfo.enabledExtensionCount = extCount;
	deviceCreateInfo.ppEnabledExtensionNames = extNames;
	deviceCreateInfo.pEnabledFeatures = deviceFeat;
	return deviceCreateInfo;
}
static bool cdrawRendererInternalCreateDevice_win_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	// print prefixes
	cstrk_t const pref1 = "\t    ", pref1A = "\t -> ", pref2 = "\t\t    ", pref2A = "\t\t -> ", pref3 = "\t\t\t    ", pref3A = "\t\t\t -> ";

	// limits
	enum {
		device_queue_family_queues_max = 16,		// hard limit
		device_queue_family_compute_only_max = 8,	// hard limit
	};
	fp32_t queuePriority_graphics[device_queue_family_queues_max] = { 0 };

	// device descriptors
	VkPhysicalDeviceType const physicalDeviceSelectType_require =
		(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
	int32_t physicalDeviceSelectIdx = -1;
	VkPhysicalDeviceProperties physicalDeviceProp = { 0 };

	// queue family descriptors
	VkQueueFlagBits const queueFamilySelectType_graphics_require =
		(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
	int32_t queueFamilySelectIdx_graphics = -1;
	uint32_t const queueCount_graphics = 1;

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

	// iterators, lists and counts
	uint32_t nPhysicalDevice = 0;
	uint32_t nDeviceLayer = 0, nDeviceLayerEnabled = 0;
	uint32_t nDeviceExt = 0, nDeviceExtEnabled = 0;
	uint32_t nQueueFamily = 0;
	VkPhysicalDevice* pPhysicalDevice = NULL;
	VkLayerProperties* pDeviceLayerProp = NULL;
	VkExtensionProperties* pDeviceExtProp = NULL;
	VkQueueFamilyProperties* pQueueFamilyProp = NULL;
	uint32_t deviceItr, layerItr, extItr, familyItr, memoryItr;
	int32_t layerIdx, extIdx;
	cstrk_t name;

	VkResult result = VK_SUCCESS;
	VkDevice device = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDeviceSelect = VK_NULL_HANDLE;
	cdraw_assert(p_renderer && p_renderer->inst);
	cdraw_assert(!p_renderer->device && !p_renderer->physicalDevice.device);
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
			cdraw_assert(result == VK_SUCCESS);
			printf("\n\t pPhysicalDevice[%u]: { \"name\" [type] (apiVer; driverVer; vendorID; deviceID) }", nPhysicalDevice);
			for (deviceItr = 0; deviceItr < nPhysicalDevice; ++deviceItr)
			{
				// find most capable device (e.g. discrete GPU should be priority)
				vkGetPhysicalDeviceProperties(pPhysicalDevice[deviceItr], &physicalDeviceProp);
				if (flagcheckexcl(physicalDeviceProp.deviceType, physicalDeviceSelectType_require)
					&& physicalDeviceSelectIdx < 0)
				{
					physicalDeviceSelectIdx = deviceItr;
					cdrawRendererPrintPhysicalDevice_vk(&physicalDeviceProp, physicalDeviceSelectIdx, pref1A);
				}
				else
					cdrawRendererPrintPhysicalDevice_vk(&physicalDeviceProp, deviceItr, pref1);
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
					cdraw_assert(result == VK_SUCCESS);
					printf("\n\t\t pDeviceLayerProp[%u]: ", nDeviceLayer);
					for (layerItr = 0; layerItr < nDeviceLayer; ++layerItr)
					{
						// same logic as instance layers
						layerIdx = -1;
						name = pDeviceLayerProp[layerItr].layerName;
						if (cdrawUtilityStrFind(name, deviceLayerName, nDeviceLayerEnabled) < 0)
							if ((layerIdx = cdrawUtilityStrFind(name, deviceLayerName_request, deviceLayerName_request_count)) >= 0)
								deviceLayerName[nDeviceLayerEnabled++] = deviceLayerName_request[layerIdx];
						cdrawRendererPrintLayer_vk(&pDeviceLayerProp[layerItr], layerItr, (layerIdx >= 0 ? pref2A : pref2));

						// device extensions; may be zero
						result = vkEnumerateDeviceExtensionProperties(physicalDeviceSelect, name, &nDeviceExt, NULL);
						if (result == VK_SUCCESS && nDeviceExt)
						{
							pDeviceExtProp = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * nDeviceExt);
							if (pDeviceExtProp)
							{
								result = vkEnumerateDeviceExtensionProperties(physicalDeviceSelect, name, &nDeviceExt, pDeviceExtProp);
								cdraw_assert(result == VK_SUCCESS);
								printf("\n\t\t\t pDeviceExtProp[%u]:", nDeviceExt);
								for (extItr = 0; extItr < nDeviceExt; ++extItr)
								{
									// same logic as instance extensions
									extIdx = -1;
									name = pDeviceExtProp[extItr].extensionName;
									if (cdrawUtilityStrFind(name, deviceExtName, nDeviceExtEnabled) < 0)
										if ((extIdx = cdrawUtilityStrFind(name, deviceExtName_request, deviceExtName_request_count)) >= 0)
											deviceExtName[nDeviceExtEnabled++] = deviceExtName_request[extIdx];
									cdrawRendererPrintExt_vk(&pDeviceExtProp[extItr], extItr, (extIdx >= 0 ? pref3A : pref3));
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

			for (layerItr = 0; layerItr < deviceLayerName_require_count; ++layerItr)
			{
				name = deviceLayerName_require[layerItr];
				if (cdrawUtilityStrFind(name, deviceLayerName, nDeviceLayerEnabled) < 0)
				{
					deviceLayerName[nDeviceLayerEnabled++] = name;
					printf("\n\t\t Additional layer: \"%s\"", name);
				}
			}
			cdraw_assert(nDeviceLayerEnabled == cdrawUtilityPtrCount(deviceLayerName, deviceLayerName_baseLen));

			for (extItr = 0; extItr < deviceExtName_require_count; ++extItr)
			{
				name = deviceExtName_require[extItr];
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
					for (familyItr = 0; familyItr < nQueueFamily; ++familyItr)
					{
						// save best queue family supporting graphics and presentation
						if (flagcheckexcl(pQueueFamilyProp[familyItr].queueFlags, queueFamilySelectType_graphics_require)
#if CDRAW_TARGET_WINDOWS
							&& vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDeviceSelect, familyItr)
#endif // #if CDRAW_TARGET_WINDOWS
							&& queueFamilySelectIdx_graphics < 0)
						{
							queueFamilySelectIdx_graphics = familyItr;
							cdrawRendererPrintQueueFamily_vk(&pQueueFamilyProp[familyItr], queueFamilySelectIdx_graphics, pref2A);
						}
						else
							cdrawRendererPrintQueueFamily_vk(&pQueueFamilyProp[familyItr], familyItr, pref2);
					}

					if (queueFamilySelectIdx_graphics >= 0)
					{
						p_renderer->physicalDevice.queueFamilyProp_graphics = pQueueFamilyProp[queueFamilySelectIdx_graphics];
						p_renderer->physicalDevice.queueFamilyIdx_graphics = queueFamilySelectIdx_graphics;
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
				for (memoryItr = 0; memoryItr < deviceMemProp->memoryTypeCount; ++memoryItr)
					cdrawRendererPrintMemoryType_vk(&deviceMemProp->memoryTypes[memoryItr], memoryItr, pref1);
				printf("\n\t nMemoryHeap = %u: { [flags] (device size) }", deviceMemProp->memoryHeapCount);
				for (memoryItr = 0; memoryItr < deviceMemProp->memoryHeapCount; ++memoryItr)
					cdrawRendererPrintMemoryHeap_vk(&deviceMemProp->memoryHeaps[memoryItr], memoryItr, pref1);
			}
		}
		else
			result = VK_INCOMPLETE;
	}

	// FINAL CREATE LOGICAL DEVICE
	if (result == VK_SUCCESS)
	{
		VkDeviceQueueCreateInfo const queueCreateInfo[] = {
			cdrawVkDeviceQueueCreateInfoCtor(
				queueFamilySelectIdx_graphics,
				queueCount_graphics,
				queuePriority_graphics),
		};
		VkDeviceCreateInfo const deviceCreateInfo = cdrawVkDeviceCreateInfoCtor(
			buffer_len(queueCreateInfo),
			queueCreateInfo,
			nDeviceLayerEnabled,
			deviceLayerName,
			nDeviceExtEnabled,
			deviceExtName,
			&p_renderer->physicalDevice.deviceFeatUse);
		result = vkCreateDevice(physicalDeviceSelect, &deviceCreateInfo, alloc, &device);
		if (device)
		{
			cdraw_assert(result == VK_SUCCESS);
			p_renderer->device = device;
			p_renderer->physicalDevice.device = physicalDeviceSelect;
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
	cdraw_assert(p_renderer->device && p_renderer->physicalDevice.device);
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

	cdraw_assert(p_renderer->physicalDevice.device);
	vkDestroyDevice(p_renderer->device, alloc);

	printf("\n Released Vulkan logical device.");
	memset(&p_renderer->physicalDevice, 0, sizeof(p_renderer->physicalDevice));
	p_renderer->device = NULL;
	return true;
}

/*
* Singh, c.6 - translated to C and organized
*/
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
static bool cdrawRendererInternalCreateSurface_win_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc, ptrk_t const p_data)
{
	VkResult result = VK_SUCCESS;
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	cdraw_assert(p_renderer && p_renderer->inst && p_renderer->device && p_data);
	cdraw_assert(!p_renderer->surface);
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
		VkWin32SurfaceCreateInfoKHR const surfaceCreateInfo = cdrawVkSurfaceCreateInfoCtor(
			data->hInst,
			data->hWnd);
		cdraw_assert(data->hInst && data->hWnd);
		result = vkCreateWin32SurfaceKHR(p_renderer->inst, &surfaceCreateInfo, alloc, &surface);
#endif // #if CDRAW_TARGET_WINDOWS
		if (surface)
		{
			cdraw_assert(result == VK_SUCCESS);
			p_renderer->surface = surface;
		}
		else
			result = VK_INCOMPLETE;
	}

	// done
	if (result != VK_SUCCESS)
	{
		printf("\n Vulkan presentation surface creation failed.");
		return false;
	}
	cdraw_assert(p_renderer->surface);
	printf("\n Vulkan presentation surface creation succeeded.");
	return true;
}

static bool cdrawRendererInternalReleaseSurface_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	cdraw_assert(p_renderer);
	if (!p_renderer->surface)
		return false;

	cdraw_assert(p_renderer->inst);
	vkDestroySurfaceKHR(p_renderer->inst, p_renderer->surface, alloc);

	printf("\n Released Vulkan presentation surface.");
	p_renderer->surface = NULL;
	return true;
}

/*
* Singh, c.6 - translated to C and organized
*/
static VkSwapchainCreateInfoKHR cdrawVkSwapchainCreateInfoCtor(
	VkSurfaceKHR const surface,
	uint32_t const minImageCount,
	VkFormat const imageFormat,
	VkColorSpaceKHR const imageColorSpace,
	VkExtent2D const imageExtent,
	uint32_t const imageArrayLayers,
	VkImageUsageFlags const imageUsage,
	VkSharingMode const imageSharingMode,
	uint32_t const queueFamilyIndexCount,
	uint32_t const queueFamilyIndices[],
	VkSurfaceTransformFlagBitsKHR preTransform,
	VkCompositeAlphaFlagBitsKHR compositeAlpha,
	VkPresentModeKHR presentMode,
	VkBool32 const clipped,
	VkSwapchainKHR const oldSwapchain)
{
	VkSwapchainCreateInfoKHR swapchainCreateInfo = { 0 };
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = minImageCount;
	swapchainCreateInfo.imageFormat = imageFormat;
	swapchainCreateInfo.imageColorSpace = imageColorSpace;
	swapchainCreateInfo.imageExtent = imageExtent;
	swapchainCreateInfo.imageArrayLayers = imageArrayLayers;
	swapchainCreateInfo.imageUsage = imageUsage;
	swapchainCreateInfo.imageSharingMode = imageSharingMode;
	swapchainCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount;
	swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	swapchainCreateInfo.preTransform = preTransform;
	swapchainCreateInfo.compositeAlpha = compositeAlpha;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = clipped;
	swapchainCreateInfo.oldSwapchain = oldSwapchain;
	return swapchainCreateInfo;
}
static bool cdrawRendererInternalCreateSwapchain_win_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	// data
	VkBool32 supportsSurface = VK_FALSE;
	uint32_t nSurfaceFormat = 0;
	uint32_t nPresentMode = 0;
	VkSurfaceFormatKHR* pSurfaceFormat = NULL;
	VkPresentModeKHR* pPresentMode = NULL;
	VkSurfaceCapabilitiesKHR surfaceCapabilities = { 0 };

	// preferred format for swapchain if not supported
	VkSurfaceFormatKHR surfaceFormat = { VK_FORMAT_B8G8R8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR, presentModeRank;
	VkPresentModeKHR const pPresentModesRank[] = {
		VK_PRESENT_MODE_MAILBOX_KHR,
		VK_PRESENT_MODE_IMMEDIATE_KHR,
		VK_PRESENT_MODE_FIFO_RELAXED_KHR,
	};
	uint32_t const nPresentModesRank = buffer_len(pPresentModesRank);
	uint32_t presentModeItr, presentModeRankItr;

	uint32_t imageCount = 0, imageArrayLayers = 0;
	VkSurfaceTransformFlagBitsKHR preTransform = 0;
	VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	VkPhysicalDevice const physicalDevice = p_renderer->physicalDevice.device;
	int32_t const queueFamilyIdx_graphics = p_renderer->physicalDevice.queueFamilyIdx_graphics;

	VkResult result = VK_SUCCESS;
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	VkQueue queue = VK_NULL_HANDLE;
	cdraw_assert(p_renderer && p_renderer->device && p_renderer->surface);
	cdraw_assert(physicalDevice && queueFamilyIdx_graphics >= 0);
	cdraw_assert(!p_renderer->swapchain && !p_renderer->queue);
	printf("\n Creating Vulkan swapchain...");

	// search for queue that supports surface
	result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIdx_graphics, p_renderer->surface, &supportsSurface);
	if (supportsSurface)
	{
		cdraw_assert(result == VK_SUCCESS);

		// get device queue for presentation
		{
			uint32_t const queueIdx = 0;
			vkGetDeviceQueue(p_renderer->device, queueFamilyIdx_graphics, queueIdx, &queue);
			if (!queue)
				result = VK_INCOMPLETE;
		}
	}

	// got graphics/presentation queue
	if (queue)
	{
		// query surface capabilities
		result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, p_renderer->surface, &surfaceCapabilities);
		if (result == VK_SUCCESS)
		{
			// desired number of images in swapchain
			imageCount = surfaceCapabilities.minImageCount;
			imageArrayLayers = surfaceCapabilities.maxImageArrayLayers;
			preTransform = surfaceCapabilities.currentTransform;

			// query formats for swapchain images
			result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, p_renderer->surface, &nSurfaceFormat, NULL);
			if (result == VK_SUCCESS && nSurfaceFormat)
			{
				pSurfaceFormat = (VkSurfaceFormatKHR*)malloc(sizeof(VkSurfaceFormatKHR) * nSurfaceFormat);
				if (pSurfaceFormat)
				{
					result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, p_renderer->surface, &nSurfaceFormat, pSurfaceFormat);
					cdraw_assert(result == VK_SUCCESS);
					
					// choose first format that is not undefined
					if (nSurfaceFormat > 1 || pSurfaceFormat->format != VK_FORMAT_UNDEFINED)
					{
						surfaceFormat = *pSurfaceFormat;
					}
					free(pSurfaceFormat);
					pSurfaceFormat = NULL;
				}
			}

			// query presentation modes
			result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, p_renderer->surface, &nPresentMode, NULL);
			if (result == VK_SUCCESS && nPresentMode)
			{
				pPresentMode = (VkPresentModeKHR*)malloc(sizeof(VkPresentModeKHR) * nPresentMode);
				if (pPresentMode)
				{
					result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, p_renderer->surface, &nPresentMode, pPresentMode);
					cdraw_assert(result == VK_SUCCESS);

					// select present mode preference
					for (presentModeRankItr = 0; presentModeRankItr < nPresentModesRank; ++presentModeRankItr)
					{
						presentModeRank = pPresentModesRank[presentModeRankItr];
						for (presentModeItr = 0; presentModeItr < nPresentMode; ++presentModeItr)
						{
							if (pPresentMode[presentModeItr] == presentModeRank)
							{
								presentMode = presentModeRank;
								break;
							}
						}
						if (presentMode == presentModeRank)
							break;
					}
					free(pPresentMode);
					pPresentMode = NULL;
				}
			}
		}
	}

	// FINAL CREATE SWAPCHAIN
	if (result == VK_SUCCESS)
	{
		VkImageUsageFlags const imageUsage =
			(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
		VkSwapchainCreateInfoKHR const swapchainCreateInfo = cdrawVkSwapchainCreateInfoCtor(
			p_renderer->surface,
			min(imageCount, surfaceCapabilities.maxImageCount),
			surfaceFormat.format,
			surfaceFormat.colorSpace,
			surfaceCapabilities.currentExtent,
			min(imageArrayLayers, surfaceCapabilities.maxImageArrayLayers),
			flagcheck(imageUsage, surfaceCapabilities.supportedUsageFlags),
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			NULL,
			flagcheck(preTransform, surfaceCapabilities.supportedTransforms),
			flagcheck(compositeAlpha, surfaceCapabilities.supportedCompositeAlpha),
			presentMode,
			VK_TRUE,
			VK_NULL_HANDLE);
		result = vkCreateSwapchainKHR(p_renderer->device, &swapchainCreateInfo, alloc, &swapchain);
		if (swapchain)
		{
			cdraw_assert(result == VK_SUCCESS);
			p_renderer->swapchain = swapchain;
			p_renderer->queue = queue;
		}
		else
			result = VK_INCOMPLETE;
	}

	if (result != VK_SUCCESS)
	{
		printf("\n Vulkan swapchain creation failed.");
		return false;
	}
	cdraw_assert(p_renderer->swapchain && p_renderer->queue);
	printf("\n Vulkan swapchain creation succeeded.");
	return true;
}

static bool cdrawRendererInternalReleaseSwapchain_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	cdraw_assert(p_renderer);
	if (!p_renderer->swapchain)
		return false;

	cdraw_assert(p_renderer->queue);
	cdraw_assert(p_renderer->device);
	vkDestroySwapchainKHR(p_renderer->device, p_renderer->swapchain, alloc);
	
	printf("\n Released Vulkan swapchain.");
	p_renderer->swapchain = NULL;
	p_renderer->queue = NULL;
	return true;
}

#if CDRAW_DEBUG
/*
* Singh, c.4 - translated to C and organized
*/
static VkDebugReportCallbackCreateInfoEXT cdrawVkDebugReportCallbackCreateInfoCtor(
	VkDebugReportFlagsEXT const flags,
	PFN_vkDebugReportCallbackEXT const callback,
	ptr_t const data)
{
	VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo = { 0 };
	debugReportCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	debugReportCallbackCreateInfo.flags = flags;
	debugReportCallbackCreateInfo.pfnCallback = callback;
	debugReportCallbackCreateInfo.pUserData = data;
	return debugReportCallbackCreateInfo;
}
static bool cdrawRendererInternalCreateDebug_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	VkResult result = VK_SUCCESS;
	VkDebugReportCallbackEXT debug = VK_NULL_HANDLE;
	cdraw_assert(p_renderer && p_renderer->inst);
	cdraw_assert(!p_renderer->debug);
	printf("\n Creating Vulkan debugging...");

	// DEBUGGING
	{
		// first need to get address of debug report callback function pointer
		PFN_vkCreateDebugReportCallbackEXT const vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(p_renderer->inst, "vkCreateDebugReportCallbackEXT");
		VkDebugReportCallbackCreateInfoEXT const debugCreateInfo = cdrawVkDebugReportCallbackCreateInfoCtor(
			(VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT),
			cdrawRendererInternalDebugCallback_vk,
			NULL);
		cdraw_assert(vkCreateDebugReportCallbackEXT);
		result = vkCreateDebugReportCallbackEXT(p_renderer->inst, &debugCreateInfo, alloc, &debug);
		if (debug)
		{
			cdraw_assert(result == VK_SUCCESS);
			p_renderer->debug = debug;

			// custom message debug report
			// get function and call to add layer messages
			{
				PFN_vkDebugReportMessageEXT const vkDebugReportMessageEXT = (PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(p_renderer->inst, "vkDebugReportMessageEXT");
				cdraw_assert(vkDebugReportMessageEXT);
			}
		}
		else
			result = VK_INCOMPLETE;
	}

	// done
	if (result != VK_SUCCESS)
	{
		printf("\n Vulkan debugging creation failed.");
		return false;
	}
	cdraw_assert(p_renderer->debug);
	printf("\n Vulkan debugging creation succeeded.");
	return true;
}

static bool cdrawRendererInternalReleaseDebug_vk(cdrawRenderer_win_vk* const p_renderer, VkAllocationCallbacks const* const alloc)
{
	cdraw_assert(p_renderer);
	if (!p_renderer->debug)
		return false;

	cdraw_assert(p_renderer->inst);
	{
		PFN_vkDestroyDebugReportCallbackEXT const vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(p_renderer->inst, "vkDestroyDebugReportCallbackEXT");
		cdraw_assert(vkDestroyDebugReportCallbackEXT);
		vkDestroyDebugReportCallbackEXT(p_renderer->inst, p_renderer->debug, alloc);
	}

	printf("\n Released Vulkan debugging.");
	p_renderer->debug = NULL;
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

	// CREATE SWAPCHAIN
	result = cdrawRendererInternalCreateSwapchain_win_vk(p_renderer, alloc);
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

	// swapchain (requires device)
	cdrawRendererInternalReleaseSwapchain_vk(p_renderer, alloc);

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