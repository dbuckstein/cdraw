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
* cdrawRendererDevice_vk.c
* Internal source for Vulkan device management.
*/

#include "_h/cdrawRenderer_vk.h"
#include "_h/cdrawRenderer_vk/cdrawRendererDevice_vk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cdraw/cdrawCore/cdrawUtility.h"


/******************************************************************************
* Internal implementations.
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
* Private implementations.
******************************************************************************/

/// <summary>
/// Constructor for Vulkan logical device descriptor.
/// </summary>
/// <param name="logicalDevice_out">Target logical device descriptor (non-null).</param>
/// <param name="name">Descriptor name.</param>
/// <param name="logicalDevice">Vulkan logical device handle.</param>
/// <returns>Success: <paramref name="logicalDevice_out"/>; Failure: <c>NULL</c>.</returns>
static cdrawVkLogicalDevice* cdrawVkLogicalDeviceCtor(cdrawVkLogicalDevice* const logicalDevice_out,
	label_t const name, VkDevice const logicalDevice, cdrawVkPhysicalDevice const* const physicalDevice);

/// <summary>
/// Destructor interface for Vulkan logical device descriptor.
/// </summary>
/// <param name="logicalDevice_out">Target logical device descriptor (non-null).</param>
static void cdrawVkLogicalDeviceDtor(cdrawVkLogicalDevice* const logicalDevice_out);

#if CDRAW_DEBUG
void cdrawVkDeviceDebugRefresh(VkDevice const device,
	cdrawVkDeviceDebugFuncTable* const f);
bool cdrawVkDeviceDebugFuncValid(
	cdrawVkDeviceDebugFuncTable const* const f);
#endif // #if CDRAW_DEBUG

void cdrawVkDeviceRefresh(VkDevice const device,
	cdrawVkDeviceFuncTable* const f)
{
	cdraw_assert(device && f);

#if CDRAW_DEBUG
	cdrawVkDeviceDebugRefresh(device, &f->f_debug);
#endif // #if CDRAW_DEBUG
}

static bool cdrawVkDeviceFuncValid(
	cdrawVkDeviceFuncTable const* const f)
{
	cdraw_assert(f);
	return (true
#if CDRAW_DEBUG
		&& cdrawVkDeviceDebugFuncValid(&f->f_debug)
#endif // #if CDRAW_DEBUG
		);
}

int32_t cdrawVkLayerPropertiesPrint(VkLayerProperties const* const layerProp, uint32_t const index, cstrk_t const prefix);
int32_t cdrawVkExtensionPropertiesPrint(VkExtensionProperties const* const extProp, uint32_t const index, cstrk_t const prefix);


/******************************************************************************
* Public implementations.
******************************************************************************/

cdrawVkLogicalDevice* cdrawVkLogicalDeviceCtor(cdrawVkLogicalDevice* const logicalDevice_out,
	label_t const name, VkDevice const logicalDevice, cdrawVkPhysicalDevice const* const physicalDevice)
{
	failassertret(logicalDevice_out && cdrawVkLogicalDeviceUnused(logicalDevice_out), NULL);
	failassertret(logicalDevice && physicalDevice && physicalDevice->physicalDevice, NULL);
	label_copy_safe(logicalDevice_out->name, name);
	logicalDevice_out->logicalDevice = logicalDevice;
	memcpy(&logicalDevice_out->physicalDevice, physicalDevice, sizeof(logicalDevice_out->physicalDevice));
	return logicalDevice_out;
}

void cdrawVkLogicalDeviceDtor(cdrawVkLogicalDevice* const logicalDevice_out)
{
	failassertret(logicalDevice_out);
	label_init(logicalDevice_out->name);
	logicalDevice_out->logicalDevice = VK_NULL_HANDLE;
}

bool cdrawVkLogicalDeviceValid(cdrawVkLogicalDevice const* const logicalDevice)
{
	cdraw_assert(logicalDevice);
	return (logicalDevice->logicalDevice && logicalDevice->physicalDevice.physicalDevice
		&& cdrawVkDeviceFuncValid(&logicalDevice->f));
}

bool cdrawVkLogicalDeviceUnused(cdrawVkLogicalDevice const* const logicalDevice)
{
	cdraw_assert(logicalDevice);
	return (!logicalDevice->logicalDevice && !logicalDevice->physicalDevice.physicalDevice);
}


cdrawVkCommandBuffer* cdrawVkCommandBufferCtor(cdrawVkCommandBuffer* const commandBuffer_out,
	label_t const name, VkCommandBuffer const commandBuffer)
{
	failassertret(commandBuffer_out && cdrawVkCommandBufferUnused(commandBuffer_out), NULL);
	failassertret(commandBuffer, NULL);
	label_copy_safe(commandBuffer_out->name, name);
	commandBuffer_out->commandBuffer = commandBuffer;
	return commandBuffer_out;
}

void cdrawVkCommandBufferDtor(cdrawVkCommandBuffer* const commandBuffer_out)
{
	failassertret(commandBuffer_out);
	label_init(commandBuffer_out->name);
	commandBuffer_out->commandBuffer = VK_NULL_HANDLE;
}

bool cdrawVkCommandBufferValid(cdrawVkCommandBuffer const* const commandBuffer)
{
	cdraw_assert(commandBuffer);
	return (commandBuffer->commandBuffer);
}

bool cdrawVkCommandBufferUnused(cdrawVkCommandBuffer const* const commandBuffer)
{
	cdraw_assert(commandBuffer);
	return (!commandBuffer->commandBuffer);
}


cdrawVkQueue* cdrawVkQueueCtor(cdrawVkQueue* const queue_out,
	label_t const name, VkQueue const queue)
{
	failassertret(queue_out && cdrawVkQueueUnused(queue_out), NULL);
	failassertret(queue, NULL);
	label_copy_safe(queue_out->name, name);
	queue_out->queue = queue;
	return queue_out;
}

void cdrawVkQueueDtor(cdrawVkQueue* const queue_out)
{
	failassertret(queue_out);
	label_init(queue_out->name);
	queue_out->queue = VK_NULL_HANDLE;
}

bool cdrawVkQueueValid(cdrawVkQueue const* const queue)
{
	cdraw_assert(queue);
	return (queue->queue);
}

bool cdrawVkQueueUnused(cdrawVkQueue const* const queue)
{
	cdraw_assert(queue);
	return (!queue->queue);
}


/******************************************************************************
* SECTION: Device management.
* Reference: Singh, c.3.
* Substantial improvements: translated to C and organized.
******************************************************************************/

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

bool cdrawVkPhysicalDeviceGetPlatformPresentationSupport(VkPhysicalDevice const physicalDevice, uint32_t const queueFamilyIndex);

bool cdrawVkLogicalDeviceCreate(cdrawVkLogicalDevice* const logicalDevice_out,
	label_t const name, cdrawVkInstance const* const instance, uint32_t const surfaceCountMax, VkAllocationCallbacks const* const alloc_opt)
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
	uint32_t queueFamilySelectIdx_graphics = uint32_invalid;
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
	cstrk_t nameCmp;

	VkResult result = VK_SUCCESS;
	VkDevice logicalDevice = VK_NULL_HANDLE;
	cdrawVkPhysicalDevice physicalDeviceSelect = { 0 };

	failassertret(logicalDevice_out && cdrawVkLogicalDeviceUnused(logicalDevice_out) && instance && cdrawVkInstanceValid(instance) && surfaceCountMax, false);
	printf("\n Creating Vulkan logical device \"%s\"...", name);

	// get physical devices
	result = vkEnumeratePhysicalDevices(instance->instance, &nPhysicalDevice, NULL);
	if (result == VK_SUCCESS)
	{
		// should not be zero if we are to proceed
		cdraw_assert(nPhysicalDevice);
		pPhysicalDevice = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * nPhysicalDevice);
		if (pPhysicalDevice)
		{
			result = vkEnumeratePhysicalDevices(instance->instance, &nPhysicalDevice, pPhysicalDevice);
			cdraw_assert(result == VK_SUCCESS);
			printf("\n\t pPhysicalDevice[%u]: { \"nameCmp\" [type] (apiVer; driverVer; vendorID; deviceID) }", nPhysicalDevice);
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
				physicalDeviceSelect.physicalDevice = pPhysicalDevice[physicalDeviceSelectIdx];
			}

			free(pPhysicalDevice);
			pPhysicalDevice = NULL;
		}
	}

	// setup logical device from selected physical device
	if (physicalDeviceSelect.physicalDevice)
	{
		// device layers; may be zero (deprecated for use in logical device)
		result = vkEnumerateDeviceLayerProperties(physicalDeviceSelect.physicalDevice, &nDeviceLayer, NULL);
		if (result == VK_SUCCESS && nDeviceLayer)
		{
			pDeviceLayerProp = (VkLayerProperties*)malloc(sizeof(VkLayerProperties) * nDeviceLayer);
			if (pDeviceLayerProp)
			{
				result = vkEnumerateDeviceLayerProperties(physicalDeviceSelect.physicalDevice, &nDeviceLayer, pDeviceLayerProp);
				cdraw_assert(result == VK_SUCCESS);
				printf("\n\t\t pDeviceLayerProp[%u]: ", nDeviceLayer);
				for (layerItr = 0; layerItr < nDeviceLayer; ++layerItr)
				{
					// same logic as instance layers
					layerIdx = -1;
					nameCmp = pDeviceLayerProp[layerItr].layerName;
					if (cdrawUtilityStrFind(nameCmp, deviceLayerName, nDeviceLayerEnabled) < 0)
						if ((layerIdx = cdrawUtilityStrFind(nameCmp, deviceLayerName_request, deviceLayerName_request_count)) >= 0)
							deviceLayerName[nDeviceLayerEnabled++] = deviceLayerName_request[layerIdx];
					cdrawVkLayerPropertiesPrint(&pDeviceLayerProp[layerItr], layerItr, (layerIdx >= 0 ? pref2A : pref2));

					// device extensions; may be zero
					result = vkEnumerateDeviceExtensionProperties(physicalDeviceSelect.physicalDevice, nameCmp, &nDeviceExt, NULL);
					if (result == VK_SUCCESS && nDeviceExt)
					{
						pDeviceExtProp = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * nDeviceExt);
						if (pDeviceExtProp)
						{
							result = vkEnumerateDeviceExtensionProperties(physicalDeviceSelect.physicalDevice, nameCmp, &nDeviceExt, pDeviceExtProp);
							cdraw_assert(result == VK_SUCCESS);
							printf("\n\t\t\t pDeviceExtProp[%u]:", nDeviceExt);
							for (extItr = 0; extItr < nDeviceExt; ++extItr)
							{
								// same logic as instance extensions
								extIdx = -1;
								nameCmp = pDeviceExtProp[extItr].extensionName;
								if (cdrawUtilityStrFind(nameCmp, deviceExtName, nDeviceExtEnabled) < 0)
									if ((extIdx = cdrawUtilityStrFind(nameCmp, deviceExtName_request, deviceExtName_request_count)) >= 0)
										deviceExtName[nDeviceExtEnabled++] = deviceExtName_request[extIdx];
								cdrawVkExtensionPropertiesPrint(&pDeviceExtProp[extItr], extItr, (extIdx >= 0 ? pref3A : pref3));
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
			nameCmp = deviceLayerName_require[layerItr];
			if (cdrawUtilityStrFind(nameCmp, deviceLayerName, nDeviceLayerEnabled) < 0)
			{
				deviceLayerName[nDeviceLayerEnabled++] = nameCmp;
				printf("\n\t\t Additional layer: \"%s\"", nameCmp);
			}
		}
		cdraw_assert(nDeviceLayerEnabled == cdrawUtilityPtrCount(deviceLayerName, deviceLayerName_baseLen));

		for (extItr = 0; extItr < deviceExtName_require_count; ++extItr)
		{
			nameCmp = deviceExtName_require[extItr];
			if (cdrawUtilityStrFind(nameCmp, deviceExtName, nDeviceExtEnabled) < 0)
			{
				deviceExtName[nDeviceExtEnabled++] = nameCmp;
				printf("\n\t\t\t Additional extension: \"%s\"", nameCmp);
			}
		}
		cdraw_assert(nDeviceExtEnabled == cdrawUtilityPtrCount(deviceExtName, deviceExtName_baseLen));

		// set up queue family info
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceSelect.physicalDevice, &nQueueFamily, NULL);
		if (nQueueFamily)
		{
			pQueueFamilyProp = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * nQueueFamily);
			if (pQueueFamilyProp)
			{
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDeviceSelect.physicalDevice, &nQueueFamily, pQueueFamilyProp);
				printf("\n\t\t pQueueFamilyProp[%u]: { [flags] (count; timestamp valid bits; min image transfer granularity) }", nQueueFamily);
				for (familyItr = 0; familyItr < nQueueFamily; ++familyItr)
				{
					// save best queue family supporting graphics and presentation
					if (flagcheckexcl(pQueueFamilyProp[familyItr].queueFlags, queueFamilySelectType_graphics_require)
						&& cdrawVkPhysicalDeviceGetPlatformPresentationSupport(physicalDeviceSelect.physicalDevice, familyItr)
						&& !uint32_valid(queueFamilySelectIdx_graphics))
					{
						queueFamilySelectIdx_graphics = familyItr;
						cdrawRendererPrintQueueFamily_vk(&pQueueFamilyProp[familyItr], queueFamilySelectIdx_graphics, pref2A);
					}
					else
						cdrawRendererPrintQueueFamily_vk(&pQueueFamilyProp[familyItr], familyItr, pref2);
				}

				if (uint32_valid(queueFamilySelectIdx_graphics))
				{
					logicalDevice_out->queueFamilyProp_graphics = pQueueFamilyProp[queueFamilySelectIdx_graphics];
					logicalDevice_out->queueFamilyIdx_graphics = queueFamilySelectIdx_graphics;
					logicalDevice_out->surfaceCountMax = surfaceCountMax;
				}

				free(pQueueFamilyProp);
				pQueueFamilyProp = NULL;
			}
		}

		// device features
		{
			VkPhysicalDeviceFeatures
				* const deviceFeat = &physicalDeviceSelect.physicalDeviceFeat,
				* const deviceFeatUse = &physicalDeviceSelect.physicalDeviceFeatUse;
			cdraw_assert(deviceFeat && deviceFeatUse);

			vkGetPhysicalDeviceProperties(physicalDeviceSelect.physicalDevice, &physicalDeviceSelect.physicalDeviceProp);
			vkGetPhysicalDeviceFeatures(physicalDeviceSelect.physicalDevice, deviceFeat);
			memset(deviceFeatUse, 0, sizeof(*deviceFeatUse));
			deviceFeatUse->geometryShader = VK_TRUE;
			deviceFeatUse->tessellationShader = VK_TRUE;
			deviceFeatUse->multiDrawIndirect = deviceFeat->multiDrawIndirect;
			//deviceFeatUse->multiViewport = deviceFeat->multiViewport;
		}

		// device memory
		{
			VkPhysicalDeviceMemoryProperties
				* const deviceMemProp = &physicalDeviceSelect.physicalDeviceMemProp;
			cdraw_assert(deviceMemProp);

			vkGetPhysicalDeviceMemoryProperties(physicalDeviceSelect.physicalDevice, deviceMemProp);
			printf("\n\t nMemoryType = %u: { [flags] (heap index) }", deviceMemProp->memoryTypeCount);
			for (memoryItr = 0; memoryItr < deviceMemProp->memoryTypeCount; ++memoryItr)
				cdrawRendererPrintMemoryType_vk(&deviceMemProp->memoryTypes[memoryItr], memoryItr, pref1);
			printf("\n\t nMemoryHeap = %u: { [flags] (device size) }", deviceMemProp->memoryHeapCount);
			for (memoryItr = 0; memoryItr < deviceMemProp->memoryHeapCount; ++memoryItr)
				cdrawRendererPrintMemoryHeap_vk(&deviceMemProp->memoryHeaps[memoryItr], memoryItr, pref1);
		}
	}

	// FINAL CREATE LOGICAL DEVICE
	if (physicalDeviceSelect.physicalDevice)
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
			&physicalDeviceSelect.physicalDeviceFeatUse);

		// create device
		result = vkCreateDevice(physicalDeviceSelect.physicalDevice, &deviceCreateInfo, alloc_opt, &logicalDevice);
		if (logicalDevice)
			cdraw_assert(result == VK_SUCCESS);
	}

	// set final outputs or clean up
	if (!logicalDevice || !physicalDeviceSelect.physicalDevice || (result != VK_SUCCESS))
	{
		cdrawVkLogicalDeviceDestroy(logicalDevice_out, alloc_opt);
		printf("\n Vulkan logical device \"%s\" creation failed.", name);
		return false;
	}
	cdrawVkLogicalDeviceCtor(logicalDevice_out, name, logicalDevice, &physicalDeviceSelect);
	cdrawVkDeviceRefresh(logicalDevice, &logicalDevice_out->f);
	cdraw_assert(cdrawVkLogicalDeviceValid(logicalDevice_out));
	printf("\n Vulkan logical device \"%s\" created.", name);
	return true;
}

bool cdrawVkLogicalDeviceDestroy(cdrawVkLogicalDevice* const logicalDevice_out,
	VkAllocationCallbacks const* const alloc_opt)
{
	failassertret(logicalDevice_out, false);
	if (cdrawVkLogicalDeviceUnused(logicalDevice_out))
		return true;

	printf("\n Destroying Vulkan logical device \"%s\"...", logicalDevice_out->name);
	//if (logicalDevice_out->logicalDevice)
	{
		if (vkDeviceWaitIdle(logicalDevice_out->logicalDevice) != VK_SUCCESS)
			return false;

		vkDestroyDevice(logicalDevice_out->logicalDevice, alloc_opt);
		logicalDevice_out->logicalDevice = VK_NULL_HANDLE;
	}
	//if (logicalDevice_out->physicalDevice.physicalDevice)
	{
		logicalDevice_out->physicalDevice.physicalDevice = VK_NULL_HANDLE;
		memset(&logicalDevice_out->physicalDevice, 0, sizeof(logicalDevice_out->physicalDevice));
	}
	printf("\n Vulkan logical device \"%s\" destroyed.", logicalDevice_out->name);
	label_init(logicalDevice_out->name);
	return true;
}
