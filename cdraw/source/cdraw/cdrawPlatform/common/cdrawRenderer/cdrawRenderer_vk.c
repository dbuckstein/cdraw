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
* cdrawRenderer_vk.c
* Common implementation of modern Vulkan renderer management.
*/

#include "cdraw/cdrawPlatform/cdrawRenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vulkan/vulkan.h"

#include "cdraw/cdrawCore/cdrawUtility.h"


/******************************************************************************
* SECTION: Internal utility implementations.
******************************************************************************/

static int32_t cdrawRendererEnumVendorID_vk(uint32_t const id)
{
	switch (id)
	{
		// 0 - other
		// https://www.reddit.com/r/vulkan/comments/4ta9nj/is_there_a_comprehensive_list_of_the_names_and/ 
	case 0x1002: return 1; // AMD
	case 0x1010: return 2; // ImgTec
	case 0x10DE: return 3; // NVIDIA
	case 0x13B5: return 4; // ARM
	case 0x5143: return 5; // QualComm
	case 0x8086: return 6; // INTEL
		// 7 - null
	case 0x10000: return 8; // KHR
	case VK_VENDOR_ID_VIV: return 9;
	case VK_VENDOR_ID_VSI: return 10;
	case VK_VENDOR_ID_KAZAN: return 11;
	case VK_VENDOR_ID_CODEPLAY: return 12;
	case VK_VENDOR_ID_MESA: return 13;
	case VK_VENDOR_ID_POCL: return 14;
		// 15 - null
	}
	return -1;
}

static int32_t cdrawRendererPrintLayer_vk(VkLayerProperties const* const layerProp, uint32_t const index, cstrk_t const prefix)
{
	return printf("\n %s layerProp[%u] = { \"%s\" (%u.%u.%u; %u.%u.%u): \"%s\" }", prefix, index,
		layerProp->layerName,
		VK_VERSION_MAJOR(layerProp->specVersion), VK_VERSION_MINOR(layerProp->specVersion), VK_VERSION_PATCH(layerProp->specVersion),
		VK_VERSION_MAJOR(layerProp->implementationVersion), VK_VERSION_MINOR(layerProp->implementationVersion), VK_VERSION_PATCH(layerProp->implementationVersion),
		layerProp->description);
}

static int32_t cdrawRendererPrintExt_vk(VkExtensionProperties const* const extProp, uint32_t const index, cstrk_t const prefix)
{
	return printf("\n %s extensionProp[%u] = { \"%s\" (%u.%u.%u) }", prefix, index,
		extProp->extensionName,
		VK_VERSION_MAJOR(extProp->specVersion), VK_VERSION_MINOR(extProp->specVersion), VK_VERSION_PATCH(extProp->specVersion));
}

static int32_t cdrawRendererPrintPhysicalDevice_vk(VkPhysicalDeviceProperties const* const physicalDeviceProp, uint32_t const index, cstrk_t const prefix)
{
	cstrk_t const deviceType[] = { "other", "integrated gpu", "discrete gpu", "virtual gpu", "cpu" };
	cstrk_t const vendorIDs[] = {
		"other", "amd", "imgtec", "nvidia", "arm", "qualcomm", "intel", 0,
		"khr", "viv", "vsi", "kazan", "codeplay", "mesa", "pocl", 0,
	};
	uint32_t const vendor = cdrawRendererEnumVendorID_vk(physicalDeviceProp->vendorID);
	uint32_t const device = (physicalDeviceProp->deviceID);

	return printf("\n %s physicalDeviceProp[%u] = { \"%s\" [%s] (%u.%u.%u; %u.%u.%u; %s; %u) }", prefix, index,
		physicalDeviceProp->deviceName, deviceType[physicalDeviceProp->deviceType],
		VK_VERSION_MAJOR(physicalDeviceProp->apiVersion), VK_VERSION_MINOR(physicalDeviceProp->apiVersion), VK_VERSION_PATCH(physicalDeviceProp->apiVersion),
		VK_VERSION_MAJOR(physicalDeviceProp->driverVersion), VK_VERSION_MINOR(physicalDeviceProp->driverVersion), VK_VERSION_PATCH(physicalDeviceProp->driverVersion),
		vendorIDs[vendor], device);
}

static int32_t cdrawRendererPrintQueueFamily_vk(VkQueueFamilyProperties const* const queueFamilyProp, uint32_t const index, cstrk_t const prefix)
{
	cstrk_t const queueFlag[] = { "[graphics]", "[compute]", "[transfer]", "[sparsebind]", "[protected]" };
	return printf("\n %s queueFamilyProp[%u] = { [%s%s%s%s%s] (%u; %u; %u,%u,%u) }", prefix, index,
		(queueFamilyProp->queueFlags & VK_QUEUE_GRAPHICS_BIT) ? queueFlag[0] : "",
		(queueFamilyProp->queueFlags & VK_QUEUE_COMPUTE_BIT) ? queueFlag[1] : "",
		(queueFamilyProp->queueFlags & VK_QUEUE_TRANSFER_BIT) ? queueFlag[2] : "",
		(queueFamilyProp->queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) ? queueFlag[3] : "",
		(queueFamilyProp->queueFlags & VK_QUEUE_PROTECTED_BIT) ? queueFlag[4] : "",
		queueFamilyProp->queueCount, queueFamilyProp->timestampValidBits,
		queueFamilyProp->minImageTransferGranularity.width, queueFamilyProp->minImageTransferGranularity.height, queueFamilyProp->minImageTransferGranularity.depth);
}

static int32_t cdrawRendererPrintMemoryType_vk(VkMemoryType const* const memoryType, uint32_t const index, cstrk_t const prefix)
{
	cstrk_t const memoryTypeFlag[] = { "[device local]", "[host visible]", "[host coherent]", "[host cached]", "[lazy alloc]", "[protected]", "[device coherent AMD]", "[device uncached AMD]" };
	return printf("\n %s memoryType[%u] = { [%s%s%s%s%s%s%s%s] (%u) }", prefix, index,
		(memoryType->propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) ? memoryTypeFlag[0] : "",
		(memoryType->propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) ? memoryTypeFlag[1] : "",
		(memoryType->propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) ? memoryTypeFlag[2] : "",
		(memoryType->propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) ? memoryTypeFlag[3] : "",
		(memoryType->propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) ? memoryTypeFlag[4] : "",
		(memoryType->propertyFlags & VK_MEMORY_PROPERTY_PROTECTED_BIT) ? memoryTypeFlag[5] : "",
		(memoryType->propertyFlags & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD) ? memoryTypeFlag[6] : "",
		(memoryType->propertyFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD) ? memoryTypeFlag[7] : "",
		memoryType->heapIndex);
}

static int32_t cdrawRendererPrintMemoryHeap_vk(VkMemoryHeap const* const memoryHeap, uint32_t const index, cstrk_t const prefix)
{
	cstrk_t const memoryHeapFlag[] = { "[device local]", "[multi-instance]", "[multi-instance KHR]" };
	return printf("\n %s memoryHeap[%u] = { [%s%s%s] (%llu) }", prefix, index,
		(memoryHeap->flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) ? memoryHeapFlag[0] : "",
		(memoryHeap->flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT) ? memoryHeapFlag[1] : "",
		(memoryHeap->flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR) ? memoryHeapFlag[2] : "",
		memoryHeap->size);
}


/******************************************************************************
* SECTION: Memory management.
******************************************************************************/

/// <summary>
/// Framework-defined Vulkan allocator manager.
/// </summary>
typedef struct cdrawVkAllocator
{
	/// <summary>
	/// Counts for testing.
	/// </summary>
	uint32_t allocCount, reallocCount, freeCount, internalAllocCount, internalFreeCount;
} cdrawVkAllocator;


static ptr_t cdrawVkAllocatorAllocation(
	cdrawVkAllocator*			const pUserData,
	size_t						const size,
	size_t						const alignment,
	VkSystemAllocationScope		const allocationScope)
{
	ptr_t pMemory = NULL;
	// perform custom allocation here
	// MUST return pointer aligned by 'alignment' if allocation succeeded
	// MUST return NULL if allocation failed (later generates VK_ERROR_OUT_OF_HOST_MEMORY)
	if (pMemory)
		++pUserData->allocCount;
	return pMemory;
}

static void cdrawVkAllocatorFree(
	cdrawVkAllocator*			const pUserData,
	ptr_t						const pMemory)
{
	if (pMemory)
	{
		// perform custom free here
		++pUserData->freeCount;
	}
}

static ptr_t cdrawVkAllocatorReallocation(
	cdrawVkAllocator*			const pUserData,
	ptr_t						const pOriginal,
	size_t						const size,
	size_t						const alignment,
	VkSystemAllocationScope		const allocationScope)
{
	ptr_t pMemory = NULL;
	if (pOriginal == NULL)
	{
		pMemory = cdrawVkAllocatorAllocation(pUserData, size, alignment, allocationScope);
	}
	else if (size == 0)
	{
		cdrawVkAllocatorFree(pUserData, pOriginal);
	}
	else
	{
		// perform custom reallocation here
		// same return rules as allocation
		// if fail and original is valid, do not free it
		if (pMemory)
			++pUserData->reallocCount;
	}
	return pMemory;
}

static void cdrawVkAllocatorInternalAllocationNotification(
	cdrawVkAllocator*			const pUserData,
	size_t						const size,
	VkInternalAllocationType	const allocationType,
	VkSystemAllocationScope		const allocationScope)
{
	++pUserData->internalAllocCount;
}

static void cdrawVkAllocatorInternalFreeNotification(
	cdrawVkAllocator*			const pUserData,
	size_t						const size,
	VkInternalAllocationType	const allocationType,
	VkSystemAllocationScope		const allocationScope)
{
	++pUserData->internalFreeCount;
}

static VkAllocationCallbacks const* cdrawRendererInternalAllocUse_vk(VkAllocationCallbacks const* const alloc)
{
	cdraw_assert(alloc);
	return (alloc->pUserData ? alloc : NULL);
}

static VkAllocationCallbacks* cdrawRendererInternalAllocInit_vk(VkAllocationCallbacks* const alloc, cdrawVkAllocator* const allocator)
{
	cdraw_assert(alloc);
	//alloc->pUserData = allocator;
	if (!cdrawRendererInternalAllocUse_vk(alloc))
		return NULL;
	alloc->pfnAllocation = cdrawVkAllocatorAllocation;
	alloc->pfnReallocation = cdrawVkAllocatorReallocation;
	alloc->pfnFree = cdrawVkAllocatorFree;
	alloc->pfnInternalAllocation = cdrawVkAllocatorInternalAllocationNotification;
	alloc->pfnInternalFree = cdrawVkAllocatorInternalFreeNotification;
	return alloc;
}

static VkAllocationCallbacks* cdrawRendererInternalAllocClean_vk(VkAllocationCallbacks* const alloc)
{
	cdraw_assert(alloc);
	if (!cdrawRendererInternalAllocUse_vk(alloc))
		return NULL;
	alloc->pUserData = NULL;
	alloc->pfnAllocation = NULL;
	alloc->pfnReallocation = NULL;
	alloc->pfnFree = NULL;
	alloc->pfnInternalAllocation = NULL;
	alloc->pfnInternalFree = NULL;
	return alloc;
}


static VkMemoryAllocateInfo cdrawVkMemoryAllocateInfoCtor(
	VkDeviceSize const allocationSize,
	uint32_t const memoryTypeIndex)
{
	VkMemoryAllocateInfo memAllocInfo = { 0 };
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.allocationSize = allocationSize;
	memAllocInfo.memoryTypeIndex = memoryTypeIndex;
	return memAllocInfo;
}


/******************************************************************************
* SECTION: Debugging and validation layer.
* Reference: Singh, c.4.
* Substantial improvements: translated to C and organized.
******************************************************************************/

#if CDRAW_DEBUG
static bool cdrawRendererDestroyDebug_vk(VkDebugReportCallbackEXT* const debug_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt)
{
	cdraw_assert(debug_out);
	if (!*debug_out)
		return false;
	printf("\n Destroying Vulkan debugging...");

	cdraw_assert(inst);
	{
		PFN_vkDestroyDebugReportCallbackEXT const vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(inst, "vkDestroyDebugReportCallbackEXT");
		cdraw_assert(vkDestroyDebugReportCallbackEXT);
		vkDestroyDebugReportCallbackEXT(inst, *debug_out, alloc_opt);
	}

	printf("\n Vulkan debugging destroyed.");
	*debug_out = VK_NULL_HANDLE;
	return true;
}

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
	// same format as "PFN_vkDebugReportCallbackEXT"

	// the message contains general info
	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
		printf("\n\t cdrawVkDebugReport: INFORMATION (%s, #%d): %s", pLayerPrefix, messageCode, pMessage);
	// the message contains a warning
	else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
		printf("\n\t cdrawVkDebugReport: WARNING (%s, #%d): %s", pLayerPrefix, messageCode, pMessage);
	// the message contains a performance warning
	else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
		printf("\n\t cdrawVkDebugReport: PERFORMANCE WARNING (%s, #%d): %s", pLayerPrefix, messageCode, pMessage);
	// the message contains an error
	else if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
		printf("\n\t cdrawVkDebugReport: ERROR (%s, #%d): %s", pLayerPrefix, messageCode, pMessage);
	// the message pertains to debugging
	else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
		printf("\n\t cdrawVkDebugReport: DEBUG (%s, #%d): %s", pLayerPrefix, messageCode, pMessage);
	// not handled
	else
		return VK_FALSE;

	// handled, but should still return false according to spec
	return VK_SUCCESS;
}

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

static VkDebugReportCallbackCreateInfoEXT cdrawVkDebugReportCallbackCreateInfoCtorDefault()
{
	// all flags
	VkDebugReportFlagsEXT const flags =
		(VK_DEBUG_REPORT_INFORMATION_BIT_EXT
			| VK_DEBUG_REPORT_WARNING_BIT_EXT
			| VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
			| VK_DEBUG_REPORT_ERROR_BIT_EXT
			| VK_DEBUG_REPORT_DEBUG_BIT_EXT);
	return cdrawVkDebugReportCallbackCreateInfoCtor(
		flags,
		cdrawRendererInternalDebugCallback_vk,
		NULL);
}

static bool cdrawRendererCreateDebug_vk(VkDebugReportCallbackEXT* const debug_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt)
{
	VkResult result = VK_SUCCESS;
	VkDebugReportCallbackEXT debug = VK_NULL_HANDLE;
	cdraw_assert(debug_out && !*debug_out && inst);
	printf("\n Creating Vulkan debugging...");

	// FINAL CREATE DEBUGGING
	{
		// first need to get address of debug report callback function pointer
		PFN_vkCreateDebugReportCallbackEXT const vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(inst, "vkCreateDebugReportCallbackEXT");
		VkDebugReportCallbackCreateInfoEXT const debugCreateInfo = cdrawVkDebugReportCallbackCreateInfoCtorDefault();
		cdraw_assert(vkCreateDebugReportCallbackEXT);
		result = vkCreateDebugReportCallbackEXT(inst, &debugCreateInfo, alloc_opt, &debug);
		if (debug)
			cdraw_assert(result == VK_SUCCESS);
		
		//// custom message debug report: get function and call to add layer messages
		//{
		//	PFN_vkDebugReportMessageEXT const vkDebugReportMessageEXT = (PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(inst, "vkDebugReportMessageEXT");
		//	cdraw_assert(vkDebugReportMessageEXT);
		//}
	}

	// set final outputs or clean up
	if (!debug || (result != VK_SUCCESS))
	{
		cdrawRendererDestroyDebug_vk(&debug, inst, alloc_opt);
		printf("\n Vulkan debugging creation failed.");
		return false;
	}
	*debug_out = debug;
	cdraw_assert(*debug_out);
	printf("\n Vulkan debugging created.");
	return true;
}
#endif // #if CDRAW_DEBUG


/******************************************************************************
* SECTION: Instance management.
* Reference: Singh, c.3.
* Substantial improvements: translated to C and organized.
******************************************************************************/

static bool cdrawRendererDestroyInstance_vk(VkInstance* const inst_out,
	VkAllocationCallbacks const* const alloc_opt)
{
	cdraw_assert(inst_out);
	if (!*inst_out)
		return false;
	printf("\n Destroying Vulkan instance...");

	vkDestroyInstance(*inst_out, alloc_opt);

	printf("\n Vulkan instance destroyed.");
	*inst_out = VK_NULL_HANDLE;
	return true;
}

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
#if CDRAW_DEBUG
	VkDebugReportCallbackCreateInfoEXT const* const debugCreateInfo,
#endif // #if CDRAW_DEBUG
	VkApplicationInfo const* const appInfo,
	uint32_t const layerCount,
	cstrk_t const layerNames[/*layerCount*/],
	uint32_t const extCount,
	cstrk_t const extNames[/*extCount*/])
{
	VkInstanceCreateInfo instanceCreateInfo = { 0 };
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
#if CDRAW_DEBUG
	instanceCreateInfo.pNext = debugCreateInfo;
#endif // #if CDRAW_DEBUG
	instanceCreateInfo.pApplicationInfo = appInfo;
	instanceCreateInfo.enabledLayerCount = layerCount;
	instanceCreateInfo.ppEnabledLayerNames = layerNames;
	instanceCreateInfo.enabledExtensionCount = extCount;
	instanceCreateInfo.ppEnabledExtensionNames = extNames;
	return instanceCreateInfo;
}

cstrk_t cdrawRendererInternalPlatformSurfaceExtName_vk();

static bool cdrawRendererCreateInstance_vk(VkInstance* const inst_out,
	VkAllocationCallbacks const* const alloc_opt)
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
		cdrawRendererInternalPlatformSurfaceExtName_vk(),
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
	cdraw_assert(inst_out && !*inst_out);
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
#if CDRAW_DEBUG
		// add debug create info to show messages on instance create
		VkDebugReportCallbackCreateInfoEXT const debugCreateInfo = cdrawVkDebugReportCallbackCreateInfoCtorDefault();
#endif // #if CDRAW_DEBUG

		// app descriptor
		VkApplicationInfo const appInfo = cdrawVkApplicationInfoCtor(
			"cdraw Plugin Application",
			VK_MAKE_VERSION(0, 0, 1),
			"cdraw",
			VK_MAKE_VERSION(0, 0, 1));

		// instance info
		VkInstanceCreateInfo const instCreateInfo = cdrawVkInstanceCreateInfoCtor(
#if CDRAW_DEBUG
			& debugCreateInfo,
#endif // #if CDRAW_DEBUG
			& appInfo,
			nInstLayerEnabled,
			instLayerName,
			nInstExtEnabled,
			instExtName);

		// create instance
		result = vkCreateInstance(&instCreateInfo, alloc_opt, &inst);
		if (inst)
			cdraw_assert(result == VK_SUCCESS);
	}

	// set final outputs or clean up
	if (!inst || (result != VK_SUCCESS))
	{
		cdrawRendererDestroyInstance_vk(inst_out, alloc_opt);
		printf("\n Vulkan instance creation failed.");
		return false;
	}
	*inst_out = inst;
	cdraw_assert(*inst_out);
	printf("\n Vulkan instance created.");
	return true;
}


/******************************************************************************
* SECTION: Physical device management.
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
	/// Should have additional one for dedicated compute.
	/// </summary>
	VkQueueFamilyProperties queueFamilyProp_graphics;

	/// <summary>
	/// Index of graphics queue family.
	/// Should have additional one for dedicated compute.
	/// </summary>
	int32_t queueFamilyIdx_graphics;
} cdrawVkPhysicalDevice;


/******************************************************************************
* SECTION: Device management.
* Reference: Singh, c.3.
* Substantial improvements: translated to C and organized.
******************************************************************************/

static bool cdrawRendererDestroyDevice_vk(VkDevice* const device_out, cdrawVkPhysicalDevice* const physicalDevice_out,
	VkAllocationCallbacks const* const alloc_opt)
{
	cdraw_assert(device_out);
	if (!*device_out)
		return false;
	printf("\n Destroying Vulkan logical device...");

	if (vkDeviceWaitIdle(*device_out) != VK_SUCCESS)
		return false;

	vkDestroyDevice(*device_out, alloc_opt);

	printf("\n Vulkan logical device destroyed.");
	cdraw_assert(physicalDevice_out);
	physicalDevice_out->device = VK_NULL_HANDLE;
	memset(physicalDevice_out, 0, sizeof(*physicalDevice_out));
	*device_out = VK_NULL_HANDLE;
	return true;
}

static VkDeviceQueueCreateInfo cdrawVkDeviceQueueCreateInfoCtor(
	uint32_t const queueFamilyIndex,
	uint32_t const queueCount,
	fp32_t const queuePriorities[/*queueCount*/])
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
	VkDeviceQueueCreateInfo const queueCreateInfo[/*queueCreateInfoCount*/],
	uint32_t const layerCount,
	cstrk_t const layerNames[/*layerCount*/],
	uint32_t const extCount,
	cstrk_t const extNames[/*extCount*/],
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

bool cdrawRendererInternalPlatformQueueFamilySupportsPresentation_vk(VkPhysicalDevice const physicalDevice, uint32_t const queueFamilyIndex);

static bool cdrawRendererCreateDevice_vk(VkDevice* const device_out, cdrawVkPhysicalDevice* const physicalDeviceDesc_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt)
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
	VkQueueFlagBits const queueFamilySelectType_graphics_require = (
		//| VK_QUEUE_COMPUTE_BIT // can be added later or make a new queue
		VK_QUEUE_GRAPHICS_BIT);
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
	cdraw_assert(device_out && !*device_out && physicalDeviceDesc_out && !physicalDeviceDesc_out->device && inst);
	printf("\n Creating Vulkan logical device...");

	// get physical devices
	result = vkEnumeratePhysicalDevices(inst, &nPhysicalDevice, NULL);
	if (result == VK_SUCCESS)
	{
		// should not be zero if we are to proceed
		cdraw_assert(nPhysicalDevice);
		pPhysicalDevice = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * nPhysicalDevice);
		if (pPhysicalDevice)
		{
			result = vkEnumeratePhysicalDevices(inst, &nPhysicalDevice, pPhysicalDevice);
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
						&& cdrawRendererInternalPlatformQueueFamilySupportsPresentation_vk(physicalDeviceSelect, familyItr)
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
					physicalDeviceDesc_out->queueFamilyProp_graphics = pQueueFamilyProp[queueFamilySelectIdx_graphics];
					physicalDeviceDesc_out->queueFamilyIdx_graphics = queueFamilySelectIdx_graphics;
				}

				free(pQueueFamilyProp);
				pQueueFamilyProp = NULL;
			}
		}

		// device features
		{
			VkPhysicalDeviceFeatures
				* const deviceFeat = &physicalDeviceDesc_out->deviceFeat,
				* const deviceFeatUse = &physicalDeviceDesc_out->deviceFeatUse;
			cdraw_assert(deviceFeat && deviceFeatUse);

			vkGetPhysicalDeviceProperties(physicalDeviceSelect, &physicalDeviceDesc_out->deviceProp);
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
				* const deviceMemProp = &physicalDeviceDesc_out->deviceMemProp;
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

	// FINAL CREATE LOGICAL DEVICE
	if (physicalDeviceSelect)
	{
		// device queue creation info, starting with graphics-capable
		VkDeviceQueueCreateInfo const queueCreateInfo[] = {
			cdrawVkDeviceQueueCreateInfoCtor(
				queueFamilySelectIdx_graphics,
				queueCount_graphics,
				queuePriority_graphics),
		};

		// device info
		VkDeviceCreateInfo const deviceCreateInfo = cdrawVkDeviceCreateInfoCtor(
			buffer_len(queueCreateInfo),
			queueCreateInfo,
			nDeviceLayerEnabled,
			deviceLayerName,
			nDeviceExtEnabled,
			deviceExtName,
			&physicalDeviceDesc_out->deviceFeatUse);

		// create device
		result = vkCreateDevice(physicalDeviceSelect, &deviceCreateInfo, alloc_opt, &device);
		if (device)
			cdraw_assert(result == VK_SUCCESS);
	}

	// set final outputs or clean up
	if (!device || !physicalDeviceSelect || (result != VK_SUCCESS))
	{
		cdrawRendererDestroyDevice_vk(&device, physicalDeviceDesc_out, alloc_opt);
		printf("\n Vulkan logical device creation failed.");
		return false;
	}
	*device_out = device;
	physicalDeviceDesc_out->device = physicalDeviceSelect;
	cdraw_assert(*device_out && physicalDeviceDesc_out->device);
	printf("\n Vulkan logical device created.");
	return true;
}


/******************************************************************************
* SECTION: Surface management (platform-independent).
* Reference: Singh, c.6.
* Substantial improvements: translated to C and organized.
******************************************************************************/

bool cdrawRendererDestroySurface_vk(VkSurfaceKHR* const surface_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt)
{
	cdraw_assert(surface_out);
	if (!*surface_out)
		return false;
	printf("\n Destroying Vulkan presentation surface...");

	cdraw_assert(inst);
	vkDestroySurfaceKHR(inst, *surface_out, alloc_opt);

	printf("\n Vulkan presentation surface destroyed.");
	*surface_out = VK_NULL_HANDLE;
	return true;
}

bool cdrawRendererCreateSurface_vk(VkSurfaceKHR* const surface_out,
	VkInstance const inst, ptrk_t const p_data, VkAllocationCallbacks const* const alloc_opt);


/******************************************************************************
* SECTION: Command buffer management.
* Reference: Singh, c.5.
* Substantial improvements: translated to C and organized.
******************************************************************************/

static bool cdrawRendererDestroyCommandPool_vk(VkCommandPool* const cmdPool_out,
	VkDevice const device, VkAllocationCallbacks const* const alloc_opt)
{
	cdraw_assert(cmdPool_out);
	if (!*cmdPool_out)
		return false;
	printf("\n Destroying Vulkan command pool...");

	cdraw_assert(cmdPool_out && *cmdPool_out && device);
	vkDestroyCommandPool(device, *cmdPool_out, alloc_opt);

	printf("\n Vulkan command pool destroyed.");
	*cmdPool_out = VK_NULL_HANDLE;
	return true;
}

static VkCommandPoolCreateInfo cdrawVkCommandPoolCreateInfoCtor(
	VkCommandPoolCreateFlags const flags,
	uint32_t const queueFamilyIndex)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = { 0 };
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = flags;
	commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
	return commandPoolCreateInfo;
}

static bool cdrawRendererCreateCommandPool_vk(VkCommandPool* const cmdPool_out,
	VkDevice const device, int32_t const queueFamilyIdx_graphics, VkAllocationCallbacks const* const alloc_opt)
{
	VkResult result = VK_SUCCESS;
	VkCommandPool cmdPool = VK_NULL_HANDLE;
	cdraw_assert(cmdPool_out && !*cmdPool_out && device && queueFamilyIdx_graphics >= 0);
	printf("\n Creating Vulkan command pool...");

	// FINAL CREATE COMMAND POOL
	{
		// transient: will be changed frequently and have shorter lifespan
		// reset: buffers can be set individually via vkResetCommandBuffer or vkBeginCommandBuffer; 
		//	otherwise can only be reset in batch with vkResetCommandPool
		//		-> resetting is good if the same set of commands will be run repeatedly
		VkCommandPoolCreateFlags const cmdPoolFlags =
			(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		VkCommandPoolCreateInfo const cmdPoolCreateInfo = cdrawVkCommandPoolCreateInfoCtor(cmdPoolFlags, queueFamilyIdx_graphics);
		result = vkCreateCommandPool(device, &cmdPoolCreateInfo, alloc_opt, &cmdPool);
		if (cmdPool)
			cdraw_assert(result == VK_SUCCESS);
	}

	// set final outputs or clean up
	if (!cmdPool || (result != VK_SUCCESS))
	{
		cdrawRendererDestroyCommandPool_vk(&cmdPool, device, alloc_opt);
		printf("\n Vulkan command buffer pool failed.");
		return false;
	}
	*cmdPool_out = cmdPool;
	cdraw_assert(*cmdPool_out);
	printf("\n Vulkan command pool created.");
	return true;
}

static bool cdrawRendererFreeCommandBuffers_vk(VkCommandBuffer cmdBufs_out[], uint32_t const cmdBufCount,
	VkCommandPool const cmdPool, VkDevice const device)
{
	uint32_t idx;
	if (!cmdBufs_out || !cmdBufCount)
		return false;
	printf("\n Freeing Vulkan command buffers...");

	cdraw_assert(device && cmdPool);
	vkFreeCommandBuffers(device, cmdPool, cmdBufCount, cmdBufs_out);

	printf("\n Vulkan command buffers freed.");
	for (idx = 0; idx < cmdBufCount; ++idx)
		cmdBufs_out[idx] = VK_NULL_HANDLE;
	return true;
}

static VkCommandBufferAllocateInfo cdrawVkCommandBufferAllocateInfoCtor(
	VkCommandPool const commandPool,
	VkCommandBufferLevel const level,
	uint32_t const commandBufferCount)
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = { 0 };
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = level;
	commandBufferAllocateInfo.commandBufferCount = commandBufferCount;
	return commandBufferAllocateInfo;
}

static bool cdrawRendererAllocCommandBuffers_vk(VkCommandBuffer cmdBufs_out[], uint32_t const cmdBufCount,
	VkCommandPool const cmdPool, VkDevice const device)
{
	uint32_t idx;
	VkResult result = VK_SUCCESS;
	cdraw_assert(cmdBufs_out && cmdBufCount && cmdPool && device);
	for (idx = 0; idx < cmdBufCount; ++idx)
		cdraw_assert(!cmdBufs_out[idx]);
	printf("\n Allocating Vulkan command buffers...");

	// FINAL CREATE COMMAND BUFFER
	{
		// primary: can be submitted to queue
		// secondary: child of primary, cannot be submitted
		VkCommandBufferLevel const level = (VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		VkCommandBufferAllocateInfo const cmdBufAllocInfo = cdrawVkCommandBufferAllocateInfoCtor(cmdPool, level, cmdBufCount);
		result = vkAllocateCommandBuffers(device, &cmdBufAllocInfo, cmdBufs_out);
		if (result == VK_SUCCESS)
		{
			for (idx = 0; idx < cmdBufCount; ++idx)
				if (!cmdBufs_out[idx])
					break;
			cdraw_assert(idx == cmdBufCount);
		}
	}

	// set final outputs or clean up
	if (result != VK_SUCCESS)
	{
		cdrawRendererFreeCommandBuffers_vk(cmdBufs_out, cmdBufCount, cmdPool, device);
		printf("\n Vulkan command buffer allocation failed.");
		return false;
	}
	printf("\n Vulkan command buffer allocation succeeded.");
	return true;
}

static VkCommandBufferBeginInfo cdrawVkCommandBufferBeginInfoCtor(
	VkCommandBufferUsageFlags const usageFlags,
	VkCommandBufferInheritanceInfo const* const inheritanceInfo)
{
	VkCommandBufferBeginInfo commandBufferBeginInfo = { 0 };
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = usageFlags;
	commandBufferBeginInfo.pInheritanceInfo = inheritanceInfo; // ignored for primary buffers (Sellers, c.3)
	return commandBufferBeginInfo;
}

static VkSubmitInfo cdrawVkSubmitInfoCtor(
	uint32_t const waitSemaphoreCount,
	VkSemaphore const waitSemaphores[/*waitSemaphoreCount*/],
	VkPipelineStageFlags const* const pWaitDstStageMask,
	uint32_t const commandBufferCount,
	VkCommandBuffer const commandBuffers[/*commandBufferCount*/],
	uint32_t const signalSemaphoreCount,
	VkSemaphore const pSignalSemaphores[/*signalSemaphoreCount*/])
{
	VkSubmitInfo submitInfo = { 0 };
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = waitSemaphoreCount;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = pWaitDstStageMask;
	submitInfo.commandBufferCount = commandBufferCount;
	submitInfo.pCommandBuffers = commandBuffers;
	submitInfo.signalSemaphoreCount = signalSemaphoreCount;
	submitInfo.pSignalSemaphores = pSignalSemaphores;
	return submitInfo;
}

static VkRenderPassBeginInfo cdrawVkRenderPassBeginInfoCtor(
	VkRenderPass const renderPass,
	VkFramebuffer const framebuffer,
	VkRect2D const renderArea,
	uint32_t const clearValueCount,
	VkClearValue const clearValues[/*clearValueCount*/])
{
	VkRenderPassBeginInfo renderPassBeginInfo = { 0 };
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.framebuffer = framebuffer;
	renderPassBeginInfo.renderArea = renderArea;
	renderPassBeginInfo.clearValueCount = clearValueCount;
	renderPassBeginInfo.pClearValues = clearValues;
	return renderPassBeginInfo;
}


/******************************************************************************
* SECTION: Image management.
******************************************************************************/

/// <summary>
/// Description of Vulkan image and dependencies.
/// </summary>
typedef struct cdrawVkImage
{
	/// <summary>
	/// Image handle.
	/// </summary>
	VkImage image;

	/// <summary>
	/// Device memory allocated for image.
	/// </summary>
	VkDeviceMemory imageMem;

	/// <summary>
	/// Image view handle (exposure to app).
	/// </summary>
	VkImageView imageView;
} cdrawVkImage;

static cdrawVkImage* cdrawVkImageCtor(cdrawVkImage* const imageDesc_out,
	VkImage const image, VkDeviceMemory const imageMem, VkImageView const imageView)
{
	failassertret(imageDesc_out, NULL);
	failret(image && imageMem && imageView, NULL);
	imageDesc_out->image = image;
	imageDesc_out->imageMem = imageMem;
	imageDesc_out->imageView = imageView;
	return imageDesc_out;
}

static void cdrawVkImageDtor(cdrawVkImage* const imageDesc_out)
{
	failassertret(imageDesc_out);
	imageDesc_out->image = VK_NULL_HANDLE;
	imageDesc_out->imageMem = VK_NULL_HANDLE;
	imageDesc_out->imageView = VK_NULL_HANDLE;
}

static bool cdrawVkImageValid(cdrawVkImage const* const imageDesc)
{
	cdraw_assert(imageDesc);
	return (imageDesc->image && imageDesc->imageMem && imageDesc->imageView);
}

static bool cdrawVkImageUnused(cdrawVkImage const* const imageDesc)
{
	cdraw_assert(imageDesc);
	return (!imageDesc->image && !imageDesc->imageMem && !imageDesc->imageView);
}

static bool cdrawVkImageDestroy(cdrawVkImage* const imageDesc,
	VkDevice const device, VkAllocationCallbacks const* const alloc_opt)
{
	if (!imageDesc)
		return false;

	cdraw_assert(device);
	if (imageDesc->imageView)
	{
		vkDestroyImageView(device, imageDesc->imageView, alloc_opt);
		imageDesc->imageView = VK_NULL_HANDLE;
	}
	if (imageDesc->imageMem)
	{
		vkFreeMemory(device, imageDesc->imageMem, alloc_opt);
		imageDesc->imageMem = VK_NULL_HANDLE;
	}
	if (imageDesc->image)
	{
		vkDestroyImage(device, imageDesc->image, alloc_opt);
		imageDesc->image = VK_NULL_HANDLE;
	}
	return true;
}


static VkComponentMapping cdrawVkComponentMappingCtor(
	VkComponentSwizzle const r,
	VkComponentSwizzle const g,
	VkComponentSwizzle const b,
	VkComponentSwizzle const a)
{
	VkComponentMapping componentMapping;
	componentMapping.r = r;
	componentMapping.g = g;
	componentMapping.b = b;
	componentMapping.a = a;
	return componentMapping;
}

static VkComponentMapping cdrawVkComponentMappingCtorDefault()
{
	return cdrawVkComponentMappingCtor(VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);
}

static VkImageSubresourceRange cdrawVkImageSubresourceRangeCtor(
	VkImageAspectFlags const aspectMask,
	uint32_t const baseMipLevel,
	uint32_t const levelCount,
	uint32_t const baseArrayLayer,
	uint32_t const layerCount)
{
	VkImageSubresourceRange subresourceRange;
	subresourceRange.aspectMask = aspectMask;
	subresourceRange.baseMipLevel = baseMipLevel;
	subresourceRange.levelCount = levelCount;
	subresourceRange.baseArrayLayer = baseArrayLayer;
	subresourceRange.layerCount = layerCount;
	return subresourceRange;
}

static VkImageSubresourceRange cdrawVkImageSubresourceRangeCtorDefaultColor()
{
	VkImageAspectFlags const aspectFlags = (VK_IMAGE_ASPECT_COLOR_BIT);
	return cdrawVkImageSubresourceRangeCtor(aspectFlags, 0, 1, 0, 1);
}

static VkImageSubresourceRange cdrawVkImageSubresourceRangeCtorDefaultDepth()
{
	VkImageAspectFlags const aspectFlags = (VK_IMAGE_ASPECT_DEPTH_BIT);
	return cdrawVkImageSubresourceRangeCtor(aspectFlags, 0, 1, 0, 1);
}

static VkImageSubresourceRange cdrawVkImageSubresourceRangeCtorDefaultDepthStencil()
{
	VkImageAspectFlags const aspectFlags = (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);
	return cdrawVkImageSubresourceRangeCtor(aspectFlags, 0, 1, 0, 1);
}

static VkImageCreateInfo cdrawVkImageCreateInfoCtor(
	VkImageType	const imageType,
	VkFormat const format,
	VkExtent3D const extent,
	uint32_t const mipLevels,
	uint32_t const arrayLayers,
	VkSampleCountFlagBits const samples,
	VkImageTiling const tiling,
	VkImageUsageFlags const usage,
	VkSharingMode const sharingMode,
	uint32_t const queueFamilyIndexCount,
	uint32_t const queueFamilyIndices[/*queueFamilyIndexCount*/],
	VkImageLayout const initialLayout)
{
	VkImageCreateInfo imageCreateInfo = { 0 };
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = imageType;
	imageCreateInfo.format = format;
	imageCreateInfo.extent = extent;
	imageCreateInfo.mipLevels = mipLevels;
	imageCreateInfo.arrayLayers = arrayLayers;
	imageCreateInfo.samples = samples;
	imageCreateInfo.tiling = tiling;
	imageCreateInfo.usage = usage;
	imageCreateInfo.sharingMode = sharingMode;
	imageCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount;
	imageCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	imageCreateInfo.initialLayout = initialLayout;
	return imageCreateInfo;
}

static VkImageViewCreateInfo cdrawVkImageViewCreateInfoCtor(
	VkImage const image,
	VkImageViewType const viewType,
	VkFormat const format,
	VkComponentMapping const components,
	VkImageSubresourceRange const subresourceRange)
{
	VkImageViewCreateInfo imageViewCreateInfo = { 0 };
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.image = image;
	imageViewCreateInfo.viewType = viewType;
	imageViewCreateInfo.format = format;
	imageViewCreateInfo.components = components;
	imageViewCreateInfo.subresourceRange = subresourceRange;
	return imageViewCreateInfo;
}

static VkImageMemoryBarrier cdrawVkImageMemoryBarrierCtor(
	VkAccessFlags const srcAccessMask,
	VkAccessFlags const dstAccessMask,
	VkImageLayout const oldLayout,
	VkImageLayout const newLayout,
	uint32_t const srcQueueFamilyIndex,
	uint32_t const dstQueueFamilyIndex,
	VkImage const image,
	VkImageSubresourceRange const subresourceRange)
{
	VkImageMemoryBarrier imageMemoryBarrier = { 0 };
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.srcAccessMask = srcAccessMask;
	imageMemoryBarrier.dstAccessMask = dstAccessMask;
	imageMemoryBarrier.oldLayout = oldLayout;
	imageMemoryBarrier.newLayout = newLayout;
	imageMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex;
	imageMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
	imageMemoryBarrier.image = image;
	imageMemoryBarrier.subresourceRange = subresourceRange;
	return imageMemoryBarrier;
}

static void cdrawRendererCmdImageSetLayout_vk(VkImage const image,
	VkCommandBuffer const cmdBuf,
	uint32_t const queueFamilyIndex,
	VkAccessFlags const srcAccessMask, VkAccessFlags const dstAccessMask,
	VkPipelineStageFlags const srcStageMask, VkPipelineStageFlags const dstStageMask,
	VkImageLayout const oldLayout, VkImageLayout const newLayout,
	VkImageSubresourceRange const imageSubResourceRange)
{
	VkImageMemoryBarrier imageMemBarrier;

	cdraw_assert(image && cmdBuf && queueFamilyIndex >= 0);

	imageMemBarrier = cdrawVkImageMemoryBarrierCtor(srcAccessMask, dstAccessMask,
		oldLayout, newLayout, queueFamilyIndex, queueFamilyIndex, image, imageSubResourceRange);
	
	// safety
	if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		imageMemBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	// safety: ensure operations on image are completed
	// (Singh, c.6)
	switch (newLayout)
	{
		// image is destination of command
	case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
	case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
		imageMemBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		break;

		// image is read-only resource for shaders
	case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
		imageMemBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imageMemBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		break;

		// image is for framebuffer color
	case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
		imageMemBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		break;

		// image is for framebuffer depth/stencil
	case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
		imageMemBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		break;
	}

	// record command
	vkCmdPipelineBarrier(cmdBuf, srcStageMask, dstStageMask,
		0, 0, NULL, 0, NULL, 1, &imageMemBarrier);
}

static void cdrawVkFormatDepthStencil(
	VkFormat* const imageFormat_out, VkFormat* const imageViewFormat_out,
	bool const useDepthHiPrec, bool const useStencil)
{
	// depth/stencil buffer: can change bit size of depth and add _S8_UINT for stencil
	cdraw_assert(imageFormat_out && imageViewFormat_out);
	if (useStencil)
	{
		*imageFormat_out = useDepthHiPrec ? VK_FORMAT_D24_UNORM_S8_UINT : VK_FORMAT_D16_UNORM_S8_UINT;
		*imageViewFormat_out = VK_FORMAT_D32_SFLOAT_S8_UINT;
	}
	else
	{
		*imageFormat_out = useDepthHiPrec ? VK_FORMAT_D32_SFLOAT : VK_FORMAT_D16_UNORM;
		*imageViewFormat_out = VK_FORMAT_D32_SFLOAT;
	}
}


/******************************************************************************
* SECTION: Swapchain management.
* Reference: Singh, c.6.
* Substantial improvements: translated to C and organized.
******************************************************************************/

static bool cdrawRendererDestroySwapchain_vk(VkSwapchainKHR* const swapchain_out, VkQueue* const queue_out,
	VkImageView imageViews_out[], uint32_t const imageViewCount, cdrawVkImage* const depthImage_out_opt,
	VkDevice const device, VkAllocationCallbacks const* const alloc_opt)
{
	uint32_t idx;
	cdraw_assert(swapchain_out);
	if (!*swapchain_out)
		return false;
	printf("\n Destroying Vulkan swapchain...");

	cdrawVkImageDestroy(depthImage_out_opt, device, alloc_opt);

	cdraw_assert(imageViews_out);
	for (idx = 0; idx < imageViewCount; ++idx)
		if (imageViews_out[idx])
			vkDestroyImageView(device, imageViews_out[idx], alloc_opt);

	cdraw_assert(queue_out && *queue_out && device);
	vkDestroySwapchainKHR(device, *swapchain_out, alloc_opt);

	printf("\n Vulkan swapchain destroyed.");
	*swapchain_out = VK_NULL_HANDLE;
	*queue_out = VK_NULL_HANDLE;
	for (idx = 0; idx < imageViewCount; ++idx)
		imageViews_out[idx] = VK_NULL_HANDLE;
	return true;
}

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
	uint32_t const queueFamilyIndices[/*queueFamilyIndexCount*/],
	VkSurfaceTransformFlagBitsKHR const preTransform,
	VkCompositeAlphaFlagBitsKHR const compositeAlpha,
	VkPresentModeKHR const presentMode,
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

static bool cdrawRendererCreateSwapchain_vk(VkSwapchainKHR* const swapchain_out, VkQueue* const queue_out,
	VkImageView imageViews_out[], uint32_t const imageViewCount, cdrawVkImage* const depthImage_out_opt,
	VkDevice const device, VkSurfaceKHR const surface, VkCommandPool const cmdPool, VkPhysicalDevice const physicalDevice, int32_t const queueFamilyIdx_graphics, VkAllocationCallbacks const* const alloc_opt)
{
	uint32_t idx;

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

	uint32_t nSwapchainImage = 0;
	VkImage* pSwapchainImage = NULL;
	uint32_t const presentQueueIdx = 0;

	// queue families
	uint32_t const queueFamilyIndices[] = {
		queueFamilyIdx_graphics,
	};

	VkImageViewCreateInfo imageViewCreateInfo;

	VkResult result = VK_SUCCESS;
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	VkQueue queue = VK_NULL_HANDLE;
	cdraw_assert(swapchain_out && !*swapchain_out && queue_out && !*queue_out && imageViews_out && imageViewCount && device && surface && physicalDevice && queueFamilyIdx_graphics >= 0);
	for (idx = 0; idx < imageViewCount; ++idx)
		cdraw_assert(!imageViews_out[idx]);
	printf("\n Creating Vulkan swapchain...");

	// query surface capabilities
	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
	if (result == VK_SUCCESS)
	{
		// if surface has zero dimension, all of the following is pointless
		// still considered successful, just wait for resize later
		if (!surfaceCapabilities.currentExtent.width || !surfaceCapabilities.currentExtent.height)
		{
			cdraw_assert(!*swapchain_out && !*queue_out);
			printf("\n Vulkan swapchain not created; surface has zero dimension.");
			return true;
		}

		// desired number of images in swapchain
		imageCount = surfaceCapabilities.minImageCount + 1;
		imageArrayLayers = surfaceCapabilities.maxImageArrayLayers;
		preTransform = surfaceCapabilities.currentTransform;

		// query formats for swapchain images
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &nSurfaceFormat, NULL);
		if (result == VK_SUCCESS && nSurfaceFormat)
		{
			pSurfaceFormat = (VkSurfaceFormatKHR*)malloc(sizeof(VkSurfaceFormatKHR) * nSurfaceFormat);
			if (pSurfaceFormat)
			{
				result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &nSurfaceFormat, pSurfaceFormat);
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
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &nPresentMode, NULL);
		if (result == VK_SUCCESS && nPresentMode)
		{
			pPresentMode = (VkPresentModeKHR*)malloc(sizeof(VkPresentModeKHR) * nPresentMode);
			if (pPresentMode)
			{
				result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &nPresentMode, pPresentMode);
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

	// search for queue that supports surface
	result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIdx_graphics, surface, &supportsSurface);
	if (supportsSurface)
	{
		cdraw_assert(result == VK_SUCCESS);

		// get device queue for presentation
		vkGetDeviceQueue(device, queueFamilyIdx_graphics, presentQueueIdx, &queue);
	}

	// FINAL CREATE SWAPCHAIN
	if (queue)
	{
		// how can the images be used
		VkImageUsageFlags const imageUsage =
#if CDRAW_DEBUG
		(VK_BUFFER_USAGE_TRANSFER_DST_BIT) |
#endif // #if CDRAW_DEBUG
			(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);

		// image sharing mode
		VkSharingMode const imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

		// creation info
		VkSwapchainCreateInfoKHR const swapchainCreateInfo = cdrawVkSwapchainCreateInfoCtor(
			surface,
			gClamp(imageCount, surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount),
			surfaceFormat.format,
			surfaceFormat.colorSpace,
			surfaceCapabilities.currentExtent,
			min(imageArrayLayers, surfaceCapabilities.maxImageArrayLayers),
			flagcheck(imageUsage, surfaceCapabilities.supportedUsageFlags),
			imageSharingMode,
			buffer_len(queueFamilyIndices),
			queueFamilyIndices,
			flagcheck(preTransform, surfaceCapabilities.supportedTransforms),
			flagcheck(compositeAlpha, surfaceCapabilities.supportedCompositeAlpha),
			presentMode,
			VK_TRUE,
			VK_NULL_HANDLE);

		// create swapchain
		result = vkCreateSwapchainKHR(device, &swapchainCreateInfo, alloc_opt, &swapchain);
		if (swapchain)
			cdraw_assert(result == VK_SUCCESS);
	}

	// next, get images
	if (swapchain)
	{
		// reusable image view descriptor
		imageViewCreateInfo = cdrawVkImageViewCreateInfoCtor(
			VK_NULL_HANDLE, // updated per image below
			VK_IMAGE_VIEW_TYPE_2D,
			surfaceFormat.format,
			cdrawVkComponentMappingCtorDefault(),
			cdrawVkImageSubresourceRangeCtorDefaultColor()
		);

		// get count and allocate for color images
		result = vkGetSwapchainImagesKHR(device, swapchain, &nSwapchainImage, NULL);
		if (result == VK_SUCCESS && nSwapchainImage)
		{
			pSwapchainImage = (VkImage*)malloc(sizeof(VkImage) * nSwapchainImage);
			if (pSwapchainImage)
			{
				result = vkGetSwapchainImagesKHR(device, swapchain, &nSwapchainImage, pSwapchainImage);
				cdraw_assert(result == VK_SUCCESS);

				// create image views
				cdraw_assert(nSwapchainImage <= imageViewCount);
				for (idx = 0; idx < nSwapchainImage; ++idx)
				{
					cdraw_assert(pSwapchainImage[idx]);
					imageViewCreateInfo.image = pSwapchainImage[idx];
					result = vkCreateImageView(device, &imageViewCreateInfo, alloc_opt, &imageViews_out[idx]);
					if (imageViews_out[idx])
						cdraw_assert(result == VK_SUCCESS);
					else
						break;
				}
				cdraw_assert(idx == nSwapchainImage);

				free(pSwapchainImage);
				pSwapchainImage = NULL;
			}
		}

		// depth image if requested
		if (depthImage_out_opt)
		{
			VkImage image_depth = VK_NULL_HANDLE;
			VkImageView imageView_depth = VK_NULL_HANDLE;
			VkDeviceMemory imageMem_depth = VK_NULL_HANDLE;

			bool const useDepthHiPrec = false, useStencil = false;
			VkFormat imageFormat_depth, imageViewFormat_depth;

			VkExtent3D const imageExtent_depth = {
				surfaceCapabilities.currentExtent.width,
				surfaceCapabilities.currentExtent.height,
				1 }; // depth extent for 2D
			VkImageTiling imageTiling_depth = (VK_IMAGE_TILING_OPTIMAL);
			VkImageUsageFlags const imageUsage_depth =
				(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
			VkImageCreateInfo imageCreateInfo_depth;
			VkFormatProperties formatProperties;

			// validate
			cdraw_assert(cdrawVkImageUnused(depthImage_out_opt));
			cdraw_assert(cmdPool);

			cdrawVkFormatDepthStencil(&imageFormat_depth, &imageViewFormat_depth, useDepthHiPrec, useStencil);
			imageCreateInfo_depth = cdrawVkImageCreateInfoCtor(
				VK_IMAGE_TYPE_2D,
				imageFormat_depth,
				imageExtent_depth,
				1,
				1,
				VK_SAMPLE_COUNT_1_BIT,
				imageTiling_depth, // validate below
				imageUsage_depth,
				VK_SHARING_MODE_EXCLUSIVE,
				buffer_len(queueFamilyIndices), queueFamilyIndices, VK_IMAGE_LAYOUT_UNDEFINED);

			// confirm support
			vkGetPhysicalDeviceFormatProperties(physicalDevice, imageFormat_depth, &formatProperties);
			cdraw_assert(VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
				& (formatProperties.optimalTilingFeatures | formatProperties.linearTilingFeatures));
			if (!(VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT & formatProperties.optimalTilingFeatures))
				imageCreateInfo_depth.tiling = VK_IMAGE_TILING_LINEAR;

			// create image
			result = vkCreateImage(device, &imageCreateInfo_depth, alloc_opt, &image_depth);
			if (image_depth)
				cdraw_assert(result == VK_SUCCESS);

			// allocate memory and create view for depth image
			if (image_depth)
			{
				int8_t memTypeIndex;
				VkMemoryRequirements memReq_depth;
				VkMemoryAllocateInfo memAllocInfo;
				vkGetImageMemoryRequirements(device, image_depth, &memReq_depth);
				memTypeIndex = cdrawUtilityLowestBit32(memReq_depth.memoryTypeBits);
				if (memTypeIndex >= 0)
				{
					memAllocInfo = cdrawVkMemoryAllocateInfoCtor(memReq_depth.size, memTypeIndex);
					result = vkAllocateMemory(device, &memAllocInfo, alloc_opt, &imageMem_depth);
					if (imageMem_depth)
					{
						cdraw_assert(result == VK_SUCCESS);
						result = vkBindImageMemory(device, image_depth, imageMem_depth, 0);
						cdraw_assert(result == VK_SUCCESS);
					}
				}
				if (imageMem_depth)
				{
					VkPipelineStageFlags const stage = (VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT);
					VkImageSubresourceRange imageSubResourceRange;
					VkCommandBufferBeginInfo cmdBeginInfo;
					VkSubmitInfo submitInfo;
					VkCommandBuffer cmdBuf_depth = VK_NULL_HANDLE;
					if (cdrawRendererAllocCommandBuffers_vk(&cmdBuf_depth, 1, cmdPool, device))
					{
						cmdBeginInfo = cdrawVkCommandBufferBeginInfoCtor(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, NULL);
						result = vkBeginCommandBuffer(cmdBuf_depth, &cmdBeginInfo);
						cdraw_assert(result == VK_SUCCESS);

						imageSubResourceRange = useStencil
							? cdrawVkImageSubresourceRangeCtorDefaultDepthStencil()
							: cdrawVkImageSubresourceRangeCtorDefaultDepth();
						cdrawRendererCmdImageSetLayout_vk(image_depth, cmdBuf_depth, queueFamilyIdx_graphics, 0, 0, stage, stage,
							VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, imageSubResourceRange);
						submitInfo = cdrawVkSubmitInfoCtor(0, NULL, NULL, 1, &cmdBuf_depth, 0, NULL);
						result = vkEndCommandBuffer(cmdBuf_depth);
						cdraw_assert(result == VK_SUCCESS);
						
						result = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
						cdraw_assert(result == VK_SUCCESS);
						
						if (cdrawRendererFreeCommandBuffers_vk(&cmdBuf_depth, 1, cmdPool, device))
						{
							VkComponentMapping const imageComponent_depth = { VK_COMPONENT_SWIZZLE_IDENTITY };
							imageViewCreateInfo = cdrawVkImageViewCreateInfoCtor(image_depth, VK_IMAGE_VIEW_TYPE_2D,
								imageViewFormat_depth, imageComponent_depth, imageSubResourceRange);
							result = vkCreateImageView(device, &imageViewCreateInfo, alloc_opt, &imageView_depth);
							if (imageView_depth)
								cdraw_assert(result == VK_SUCCESS);
							depthImage_out_opt->image = image_depth;
						}
					}
				}
			}

			// set handles
			if (!cdrawVkImageCtor(depthImage_out_opt, image_depth, imageMem_depth, imageView_depth))
				cdrawVkImageDestroy(depthImage_out_opt, device, alloc_opt);
		}
	}

	// set final outputs or clean up
	if (result != VK_SUCCESS)
	{
		cdrawRendererDestroySwapchain_vk(&swapchain, &queue, imageViews_out, imageViewCount, depthImage_out_opt, device, alloc_opt);
		printf("\n Vulkan swapchain creation failed.");
		return false;
	}
	*swapchain_out = swapchain;
	*queue_out = queue;
	cdraw_assert(*swapchain_out && *queue_out);
	printf("\n Vulkan swapchain created.");
	return true;
}


/******************************************************************************
* SECTION: Synchronization utilities.
******************************************************************************/

static bool cdrawRendererDestroySemaphore_vk(VkSemaphore* const semaphore_out,
	VkDevice const device, VkAllocationCallbacks const* const alloc_opt)
{
	cdraw_assert(semaphore_out);
	if (!*semaphore_out)
		return false;
	printf("\n Destroying Vulkan semaphore...");

	cdraw_assert(device);
	vkDestroySemaphore(device, *semaphore_out, alloc_opt);

	printf("\n Vulkan semaphore destroyed.");
	*semaphore_out = VK_NULL_HANDLE;
	return true;
}

static VkSemaphoreCreateInfo cdrawVkSemaphoreCreateInfoCtorDefault()
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = { 0 };
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	return semaphoreCreateInfo;
}

static bool cdrawRendererCreateSemaphore_vk(VkSemaphore* const semaphore_out,
	VkDevice const device, VkAllocationCallbacks const* const alloc_opt)
{
	VkResult result = VK_SUCCESS;
	VkSemaphore semaphore = VK_NULL_HANDLE;
	cdraw_assert(semaphore_out && !*semaphore_out && device);
	printf("\n Creating Vulkan semaphore...");

	// FINAL CREATE SEMAPHORE
	{
		VkSemaphoreCreateInfo const semaphoreCreateInfo = cdrawVkSemaphoreCreateInfoCtorDefault();
		result = vkCreateSemaphore(device, &semaphoreCreateInfo, alloc_opt, &semaphore);
		if (semaphore)
			cdraw_assert(result == VK_SUCCESS);
	}

	// set final outputs or clean up
	if (result != VK_SUCCESS)
	{
		cdrawRendererDestroySemaphore_vk(&semaphore, device, alloc_opt);
		printf("\n Vulkan semaphore creation failed.");
		return false;
	}
	*semaphore_out = semaphore;
	cdraw_assert(*semaphore_out);
	printf("\n Vulkan semaphore created.");
	return true;
}

static bool cdrawRendererDestroyFence_vk(VkFence* const fence_out,
	VkDevice const device, VkAllocationCallbacks const* const alloc_opt)
{
	cdraw_assert(fence_out);
	if (!*fence_out)
		return false;
	printf("\n Destroying Vulkan fence...");

	cdraw_assert(device);
	vkDestroyFence(device, *fence_out, alloc_opt);

	printf("\n Vulkan fence destroyed.");
	*fence_out = VK_NULL_HANDLE;
	return true;
}

static VkFenceCreateInfo cdrawVkFenceCreateInfoCtorDefault()
{
	VkFenceCreateInfo fenceCreateInfo = { 0 };
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	return fenceCreateInfo;
}

static bool cdrawRendererCreateFence_vk(VkFence* const fence_out,
	VkDevice const device, VkAllocationCallbacks const* const alloc_opt)
{
	VkResult result = VK_SUCCESS;
	VkFence fence = VK_NULL_HANDLE;
	cdraw_assert(fence_out && !*fence_out && device);
	printf("\n Creating Vulkan fence...");

	// FINAL CREATE FENCE
	{
		VkFenceCreateInfo const fenceCreateInfo = cdrawVkFenceCreateInfoCtorDefault();
		result = vkCreateFence(device, &fenceCreateInfo, alloc_opt, &fence);
		if (fence)
			cdraw_assert(result == VK_SUCCESS);
	}

	// set final outputs or clean up
	if (result != VK_SUCCESS)
	{
		cdrawRendererDestroyFence_vk(&fence, device, alloc_opt);
		printf("\n Vulkan fence creation failed.");
		return false;
	}
	*fence_out = fence;
	cdraw_assert(*fence_out);
	printf("\n Vulkan fence created.");
	return true;
}


/******************************************************************************
* Internal interfaces.
******************************************************************************/

enum
{
	cdrawVkImagePresent_max = 4,	// convenience: max number of presentation images
	cdrawVkCmdBufPresent_max = 1,	// convenience: max number of presentation command buffers
};

/// <summary>
/// Vulkan renderer data (platform-agnostic).
/// </summary>
typedef struct cdrawRenderer_vk
{
	/// <summary>
	/// Vulkan handles and data related to instance or global usage.
	/// </summary>
	struct {
		/// <summary>
		/// Vulkan instance.
		/// </summary>
		VkInstance inst;

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
		/// Allocator data for callbacks.
		/// </summary>
		cdrawVkAllocator allocator;
	};

	/// <summary>
	/// Vulkan handles and data related to device management.
	/// </summary>
	struct {
		/// <summary>
		/// Vulkan logical device.
		/// </summary>
		VkDevice device;

		/// <summary>
		/// Description of physical device used.
		/// </summary>
		cdrawVkPhysicalDevice physicalDevice;
	};

	/// <summary>
	/// Vulkan handles and data related to presentation.
	/// </summary>
	struct {
		/// <summary>
		/// Vulkan presentation surface.
		/// </summary>
		VkSurfaceKHR surface;

		/// <summary>
		/// Vulkan swapchain.
		/// </summary>
		VkSwapchainKHR swapchain;

		/// <summary>
		/// Presentation semaphore.
		/// </summary>
		VkSemaphore semaphore;

		/// <summary>
		/// Vulkan graphics/presentation queue.
		/// Should have one for each swapchain image to avoid locking.
		/// </summary>
		VkQueue queue_graphics;

		/// <summary>
		/// Color image view resources associated with swapchain images.
		/// Images themselves are not needed as they are owned by the swapchain; can query later.
		/// </summary>
		VkImageView imageView_present[cdrawVkImagePresent_max];

		/// <summary>
		/// Depth image for presentation.
		/// </summary>
		cdrawVkImage depthImage_present;
	};

	/// <summary>
	/// Vulkan handles related to commands.
	/// </summary>
	struct {
		/// <summary>
		/// Vulkan command pool.
		/// </summary>
		VkCommandPool cmdPool;

		/// <summary>
		/// Vulkan command buffers.
		/// Should have one for each image to be processed.
		/// </summary>
		VkCommandBuffer cmdBuf[cdrawVkCmdBufPresent_max];
	};
} cdrawRenderer_vk;


/******************************************************************************
* Private implementations.
******************************************************************************/

static result_t cdrawRendererPrint_vk(cdrawRenderer_vk const* const r)
{
	VkResult result = VK_SUCCESS;
	uint32_t apiVersion = 0;
	cdraw_assert(r);

	result = vkEnumerateInstanceVersion(&apiVersion);
	printf("\n Vulkan API: %u.%u.%u", VK_API_VERSION_MAJOR(apiVersion), VK_API_VERSION_MINOR(apiVersion), VK_API_VERSION_PATCH(apiVersion));
	return result;
}


#if CDRAW_DEBUG
static bool cdrawRendererDisplayTest_vk(cdrawRenderer_vk const* const r)
{
	/*
	* ****TO-DO:
	*	-> DRAWING:
	*		0) first time - use/make command buffer
	*		1) begin recording
	*		2) call draw commands to build buffer: 
	*			begin render pass
	*			bind pipeline
	*			bind descriptors
	*			bind buffers
	*			set viewport
	*			set scissor
	*			draw
	*			end render pass
	*		3) end recording
	*		4) submit to queue
	*		5) queue wait idle
	*		6) reset to reuse
	*	-> assumptions/predictions: 
	*		create render pass
	*		create set of "present" framebuffers that will manage how final presentation works
	*		attach color (and possibly depth/stencil) images from swapchain
	*/

	VkResult result = VK_SUCCESS;
	//VkCommandBuffer const cmdBuf = r->cmdBuf[0];
	//VkCommandBufferBeginInfo const cmdBufBeginInfo = cdrawVkCommandBufferBeginInfoCtor(0, NULL);
	//VkRenderPassBeginInfo const renderPassBeginInfo = cdrawVkRenderPassBeginInfoCtor();
	////VkSemaphore const semaphores_signal[] = {
	////	r->semaphore,
	////};
	//VkSubmitInfo const submitInfo[] = {
	//	cdrawVkSubmitInfoCtor(0, NULL, NULL, 1, &cmdBuf, 0, NULL),
	//};
	//result = vkBeginCommandBuffer(cmdBuf, &cmdBufBeginInfo);
	//vkCmdBeginRenderPass(cmdBuf, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	//// ...
	//vkCmdEndRenderPass(cmdBuf);
	//result = vkEndCommandBuffer(cmdBuf);
	//result = vkQueueSubmit(r->queue, buffer_len(submitInfo), submitInfo, VK_NULL_HANDLE);
	//result = vkQueueWaitIdle(r->queue);
	////vkResetCommandBuffer(cmdBuf, 0);

	return (result == VK_SUCCESS);
}
#endif // #if CDRAW_DEBUG

static VkPresentInfoKHR cdrawVkPresentInfoCtor(
	uint32_t const waitSemaphoreCount,
	VkSemaphore const waitSemaphores[/*waitSemaphoreCount*/],
	uint32_t const swapchainCount,
	VkSwapchainKHR const swapchains[/*swapchainCount*/],
	uint32_t const imageIndices[/*swapchainCount*/],
	VkResult results_out_opt[/*swapchainCount*/])
{
	VkPresentInfoKHR presentInfo = { 0 };
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = waitSemaphoreCount;
	presentInfo.pWaitSemaphores = waitSemaphores;
	presentInfo.swapchainCount = swapchainCount;
	presentInfo.pSwapchains = swapchains;
	presentInfo.pImageIndices = imageIndices;
	presentInfo.pResults = results_out_opt;
	return presentInfo;
}

static result_t cdrawRendererDisplay_vk(cdrawRenderer_vk const* const r)
{
	uint32_t idx;
	VkResult result = VK_SUCCESS;
	cdraw_assert(r);
	if (!r->swapchain || !r->queue_graphics)
		return result;

	VkSemaphore const semaphores[] = {
		r->semaphore,
	};
	VkSwapchainKHR const swapchains[] = {
		r->swapchain,
	};
	VkResult result_swapchain[buffer_len(swapchains)] = { VK_INCOMPLETE };
	uint32_t const nSwapchains = buffer_len(swapchains);
	uint32_t imageIndices[buffer_len(swapchains)];
	VkPresentInfoKHR const presentInfo = cdrawVkPresentInfoCtor(buffer_len(semaphores), semaphores, nSwapchains, swapchains, imageIndices, result_swapchain);

#if CDRAW_DEBUG
	cdrawRendererDisplayTest_vk(r);
#endif // #if CDRAW_DEBUG

	// wait until idle before drawing
	vkDeviceWaitIdle(r->device);
	for (idx = 0; idx < nSwapchains; ++idx)
	{
		result = vkAcquireNextImageKHR(r->device, swapchains[idx], UINT64_MAX, r->semaphore, VK_NULL_HANDLE, &imageIndices[idx]);
		if (result != VK_SUCCESS)
			break;
	}
	if (result == VK_SUCCESS)
	{
		result = vkQueuePresentKHR(r->queue_graphics, &presentInfo);
		if (result != VK_SUCCESS)
		{
			// handle display error
			// NOTE: https://github.com/KhronosGroup/Vulkan-ValidationLayers/issues/4422#issuecomment-1256257653 
			//	-> getting validation error about image format being "undefined" - need actual commands from render pass to set it
		}
	}
	return result;
}


static result_t cdrawRendererResize_vk(cdrawRenderer_vk* const r, uint32_t const w_old, uint32_t const h_old, uint32_t const w_new, uint32_t const h_new)
{
	VkResult result = VK_SUCCESS;
	cdraw_assert(r);
	if ((w_old != w_new) && (h_old != h_new))
	{
		vkDeviceWaitIdle(r->device);

		// release old swapchain if the window size was valid
		if (w_old && h_old)
			result = cdrawRendererDestroySwapchain_vk(&r->swapchain, &r->queue_graphics,
				r->imageView_present, buffer_len(r->imageView_present), &r->depthImage_present, r->device, &r->alloc);

		// create new swapchain if the new window size is valid
		if (w_new && h_new)
			result = cdrawRendererCreateSwapchain_vk(&r->swapchain, &r->queue_graphics,
				r->imageView_present, buffer_len(r->imageView_present), &r->depthImage_present,
				r->device, r->surface, r->cmdPool, r->physicalDevice.device, r->physicalDevice.queueFamilyIdx_graphics, &r->alloc);
	}
	return result;
}


/******************************************************************************
* Public implementations.
******************************************************************************/

result_t cdrawRendererCreate_vk(cdrawRenderer* const renderer, ptrk_t const p_data)
{
	result_init();
	bool result = false;
	size_t const dataSz = sizeof(cdrawRenderer_vk);
	cdrawRenderer_vk* r;
	asserterr(renderer && !renderer->r && !renderer->renderAPI, errcode_invalidarg);
	r = (cdrawRenderer_vk*)malloc(dataSz);
	asserterr_ptr(r, errcode_renderer_init);
	memset(r, 0, dataSz);

	// setup allocation callbacks
	VkAllocationCallbacks const* const alloc_opt = cdrawRendererInternalAllocInit_vk(&r->alloc, &r->allocator);

	// CREATE INSTANCE
	result = cdrawRendererCreateInstance_vk(&r->inst, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

#if CDRAW_DEBUG
	// CREATE DEBUGGING
	result = cdrawRendererCreateDebug_vk(&r->debug, r->inst, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));
#endif // #if CDRAW_DEBUG

	// CREATE LOGICAL DEVICE
	result = cdrawRendererCreateDevice_vk(&r->device, &r->physicalDevice, r->inst, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE PRESENTATION SURFACE
	result = cdrawRendererCreateSurface_vk(&r->surface, r->inst, p_data, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE COMMAND POOL
	result = cdrawRendererCreateCommandPool_vk(&r->cmdPool, r->device, r->physicalDevice.queueFamilyIdx_graphics, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// ALLOCATE COMMAND BUFFERS
	result = cdrawRendererAllocCommandBuffers_vk(r->cmdBuf, buffer_len(r->cmdBuf), r->cmdPool, r->device);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE SEMAPHORES
	result = cdrawRendererCreateSemaphore_vk(&r->semaphore, r->device, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE SWAPCHAIN (may not be created due to surface size)
	result = cdrawRendererCreateSwapchain_vk(&r->swapchain, &r->queue_graphics,
		r->imageView_present, buffer_len(r->imageView_present), &r->depthImage_present,
		r->device, r->surface, r->cmdPool, r->physicalDevice.device, r->physicalDevice.queueFamilyIdx_graphics, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// setup functions for renderer and components
	{
		renderer->cdrawRendererPrint = cdrawRendererPrint_vk;
		renderer->cdrawRendererDisplay = cdrawRendererDisplay_vk;
		renderer->cdrawRendererResize = cdrawRendererResize_vk;
	}

	// all done
	renderer->r = r;
	result_return();
}

result_t cdrawRendererDestroy_vk(cdrawRenderer* const renderer)
{
	result_init();
	cdrawRenderer_vk* r;
	VkAllocationCallbacks const* alloc_opt;
	asserterr(renderer && renderer->r && renderer->renderAPI == cdrawRenderAPI_Vulkan, errcode_invalidarg);
	r = ((cdrawRenderer_vk*)renderer->r);
	alloc_opt = cdrawRendererInternalAllocUse_vk(&r->alloc);

	// safety
	vkDeviceWaitIdle(r->device);

	// swapchain (requires device and surface; may not have been created due to surface size)
	cdrawRendererDestroySwapchain_vk(&r->swapchain, &r->queue_graphics,
		r->imageView_present, buffer_len(r->imageView_present), &r->depthImage_present, r->device, alloc_opt);

	// semaphores
	cdrawRendererDestroySemaphore_vk(&r->semaphore, r->device, alloc_opt);

	// command buffers (requires command pool and device)
	cdrawRendererFreeCommandBuffers_vk(r->cmdBuf, buffer_len(r->cmdBuf), r->cmdPool, r->device);

	// command pool (requires device)
	cdrawRendererDestroyCommandPool_vk(&r->cmdPool, r->device, alloc_opt);

	// presentation surface (requires instance)
	cdrawRendererDestroySurface_vk(&r->surface, r->inst, alloc_opt);

	// logical device (wait for it to finish work)
	cdrawRendererDestroyDevice_vk(&r->device, &r->physicalDevice, alloc_opt);

#if CDRAW_DEBUG
	// destroy debug report callback function pointer
	cdrawRendererDestroyDebug_vk(&r->debug, r->inst, alloc_opt);
#endif // #if CDRAW_DEBUG

	// finally, destroy instance
	cdrawRendererDestroyInstance_vk(&r->inst, alloc_opt);

	// clean allocation callbacks
	cdrawRendererInternalAllocClean_vk(&r->alloc);

	// all done
	free(renderer->r);
	renderer->r = NULL;
	{
		renderer->cdrawRendererPrint = NULL;
		renderer->cdrawRendererDisplay = NULL;
		renderer->cdrawRendererResize = NULL;
	}
	result_return();
}
