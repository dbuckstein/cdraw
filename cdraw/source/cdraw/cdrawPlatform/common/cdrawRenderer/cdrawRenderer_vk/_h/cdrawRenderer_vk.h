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
* cdrawRenderer_vk.h
* Root header for Vulkan renderer implementation.
*/

#ifndef _CDRAW_RENDERER_VK_H_
#define _CDRAW_RENDERER_VK_H_


#include "cdrawRenderer_vk/cdrawInstance_vk.h"
#include "cdrawRenderer_vk/cdrawDevice_vk.h"


#define cdrawGetInstanceProcAddr_vk(name,f,inst) if ((f->name = (PFN_##name)vkGetInstanceProcAddr(inst, #name)) == NULL) cdraw_assert(f->name)
#define cdrawGetDeviceProcAddr_vk(name,f,device) if ((f->name = (PFN_##name)vkGetDeviceProcAddr(device, #name)) == NULL) cdraw_assert(f->name)
#if CDRAW_DEBUG
/// <summary>
/// Internal function pointer storage.
/// </summary>
typedef struct cdrawRendererInternalFuncTable_vk_dbg
{
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
	PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;

	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
	PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
	PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
	PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
	PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT;
	PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT;
	PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT;
	PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT;
	PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
	PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
	PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;

	PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
	PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
	PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
	PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
	PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
} cdrawRendererInternalFuncTable_vk_dbg;
#endif // #if CDRAW_DEBUG


/// <summary>
/// Framework-defined Vulkan allocation manager descriptor.
/// </summary>
typedef struct cdrawVkAllocator
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Counts for testing.
	/// </summary>
	uint32_t allocCount, reallocCount, freeCount, internalAllocCount, internalFreeCount;
} cdrawVkAllocator;


/// <summary>
/// Vulkan image and dependencies descriptor.
/// </summary>
typedef struct cdrawVkImage
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Vulkan image handle.
	/// </summary>
	VkImage image;

	/// <summary>
	/// Vulkan device memory handle (memory allocated for image).
	/// </summary>
	VkDeviceMemory imageMem;

	/// <summary>
	/// Vulkan image view handle (exposure to app).
	/// </summary>
	VkImageView imageView;
} cdrawVkImage;


/// <summary>
/// Vulkan physical device and details descriptor.
/// </summary>
typedef struct cdrawVkPhysicalDevice
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

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

		/// <summary>
		/// Vulkan allocation callbacks for memory management.
		/// </summary>
		VkAllocationCallbacks alloc;

		/// <summary>
		/// Allocator data for callbacks.
		/// </summary>
		cdrawVkAllocator allocator;

#if CDRAW_DEBUG
		/// <summary>
		/// Debug report extension.
		/// </summary>
		VkDebugReportCallbackEXT debugReport;

		/// <summary>
		/// Debug messenger extension.
		/// </summary>
		VkDebugUtilsMessengerEXT debugMessenger;

		/// <summary>
		/// Internal fuction table.
		/// </summary>
		cdrawRendererInternalFuncTable_vk_dbg f;
#endif // #if CDRAW_DEBUG
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


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus



#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef _CDRAW_RENDERER_VK_H_