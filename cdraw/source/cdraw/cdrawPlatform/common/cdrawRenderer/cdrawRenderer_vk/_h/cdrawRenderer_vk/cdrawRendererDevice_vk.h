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
* cdrawRendererDevice_vk.h
* Facade interface for Vulkan device management.
*/

#if ((!defined _CDRAW_RENDERER_DEVICE_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))
#define _CDRAW_RENDERER_DEVICE_VK_H_

#include "cdrawRendererInstance_vk.h"


/// <summary>
/// Get Vulkan device-dependent function address.
/// </summary>
#define cdrawVkGetDeviceProcAddr(name,f,device) if ((f->name = (PFN_##name)vkGetDeviceProcAddr(device, #name)) == NULL) cdraw_assert(f->name)


/// <summary>
/// Device-dependent extension function pointer storage.
/// </summary>
typedef struct cdrawVkDeviceFuncTable
{
#if CDRAW_DEBUG
	cdrawVkDeviceDebugFuncTable f_debug;
#endif // #if CDRAW_DEBUG
} cdrawVkDeviceFuncTable;


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
	VkPhysicalDevice physicalDevice;

	/// <summary>
	/// Device properties.
	/// </summary>
	VkPhysicalDeviceProperties physicalDeviceProp;

	/// <summary>
	/// Device memory properties.
	/// </summary>
	VkPhysicalDeviceMemoryProperties physicalDeviceMemProp;

	/// <summary>
	/// Device features.
	/// </summary>
	VkPhysicalDeviceFeatures physicalDeviceFeat;

	/// <summary>
	/// Device features actually requested/used.
	/// </summary>
	VkPhysicalDeviceFeatures physicalDeviceFeatUse;
} cdrawVkPhysicalDevice;


/// <summary>
/// Vulkan logical device descriptor.
/// </summary>
typedef struct cdrawVkLogicalDevice
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Vulkan logical device handle.
	/// </summary>
	VkDevice logicalDevice;

	/// <summary>
	/// Queue family selected for graphics.
	/// Should have additional one for dedicated compute.
	/// </summary>
	VkQueueFamilyProperties queueFamilyProp_graphics;

	/// <summary>
	/// Index of graphics queue family.
	/// Should have additional one for dedicated compute.
	/// </summary>
	uint32_t queueFamilyIdx_graphics;

	/// <summary>
	/// Maximum number of surfaces expected to use this device.
	/// </summary>
	uint32_t surfaceCountMax;

	/// <summary>
	/// Description of physical device used to create logical device.
	/// </summary>
	cdrawVkPhysicalDevice physicalDevice;

	/// <summary>
	/// Device function pointers.
	/// </summary>
	cdrawVkDeviceFuncTable f;
} cdrawVkLogicalDevice;


enum
{
	cdrawVkCommandBuffer_max = 4,
};


/// <summary>
/// Vulkan command pool descriptor.
/// </summary>
typedef struct cdrawVkCommandPool
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Vulkan command pool handle.
	/// </summary>
	VkCommandPool commandPool;
} cdrawVkCommandPool;

/// <summary>
/// Vulkan command buffer descriptor.
/// </summary>
typedef struct cdrawVkCommandBuffer
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Vulkan command buffer handle.
	/// </summary>
	VkCommandBuffer commandBuffer[cdrawVkCommandBuffer_max];

	/// <summary>
	/// Number of command buffers.
	/// </summary>
	uint32_t commandBufferCount;
} cdrawVkCommandBuffer;

/// <summary>
/// Vulkan queue descriptor.
/// </summary>
typedef struct cdrawVkQueue
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Vulkan queue handle.
	/// </summary>
	VkQueue queue;
} cdrawVkQueue;


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Indicate whether descriptor is valid (set up correctly) and should not be modified.
	/// </summary>
	/// <param name="logicalDevice">Descriptor (non-null).</param>
	/// <returns>True if valid.</returns>
	bool cdrawVkLogicalDeviceValid(cdrawVkLogicalDevice const* const logicalDevice);

	/// <summary>
	/// Indicate whether descriptor is unused (not set up) and can be modified.
	/// </summary>
	/// <param name="logicalDevice">Descriptor (non-null).</param>
	/// <returns>True if unused.</returns>
	bool cdrawVkLogicalDeviceUnused(cdrawVkLogicalDevice const* const logicalDevice);

	/// <summary>
	/// Create logical device.
	/// </summary>
	/// <param name="logicalDevice_out">Target logical device descriptor (non-null and unused).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="instance">Instance descriptor.</param>
	/// <param name="surfaceCountMax">Application-defined maximum number of windows expected.</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkLogicalDeviceCreate(cdrawVkLogicalDevice* const logicalDevice_out,
		label_t const name, cdrawVkInstance const* const instance, uint32_t const surfaceCountMax, VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Destroy logical device.
	/// </summary>
	/// <param name="logicalDevice_out">Target logical device descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if destroyed.</returns>
	bool cdrawVkLogicalDeviceDestroy(cdrawVkLogicalDevice* const logicalDevice_out,
		VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Indicate whether descriptor is valid (set up correctly) and should not be modified.
	/// </summary>
	/// <param name="commandPool">Descriptor (non-null).</param>
	/// <returns>True if valid.</returns>
	bool cdrawVkCommandPoolValid(cdrawVkCommandPool const* const commandPool);

	/// <summary>
	/// Indicate whether descriptor is unused (not set up) and can be modified.
	/// </summary>
	/// <param name="commandPool">Descriptor (non-null).</param>
	/// <returns>True if unused.</returns>
	bool cdrawVkCommandPoolUnused(cdrawVkCommandPool const* const commandPool);

	/// <summary>
	/// Create command pool descriptor.
	/// </summary>
	/// <param name="commandPool_out">Target descriptor (non-null and unused).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkCommandPoolCreate(cdrawVkCommandPool* const commandPool_out,
		label_t const name, cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Destroy command pool.
	/// </summary>
	/// <param name="commandPool_out">Target descriptor (non-null and valid).</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if destroyed.</returns>
	bool cdrawVkCommandPoolDestroy(cdrawVkCommandPool* const commandPool_out,
		cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Indicate whether descriptor is valid (set up correctly) and should not be modified.
	/// </summary>
	/// <param name="commandBuffer">Descriptor (non-null).</param>
	/// <returns>True if valid.</returns>
	bool cdrawVkCommandBufferValid(cdrawVkCommandBuffer const* const commandBuffer);

	/// <summary>
	/// Indicate whether descriptor is unused (not set up) and can be modified.
	/// </summary>
	/// <param name="commandBuffer">Descriptor (non-null).</param>
	/// <returns>True if unused.</returns>
	bool cdrawVkCommandBufferUnused(cdrawVkCommandBuffer const* const commandBuffer);

	/// <summary>
	/// Allocate command buffer set.
	/// </summary>
	/// <param name="commandBuffer_out">Target descriptor (non-null and unused).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="commandBufferCount">Number of buffers to allocate.</param>
	/// <param name="commandPool">Command pool descriptor (non-null and valid).</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="commandBufferLevel">Select primary or secondary buffers.</param>
	/// <returns>True if allocated.</returns>
	bool cdrawVkCommandBufferAlloc(cdrawVkCommandBuffer* const commandBuffer_out,
		label_t const name, uint32_t const commandBufferCount, cdrawVkCommandPool const* const commandPool, cdrawVkLogicalDevice const* const logicalDevice, VkCommandBufferLevel const commandBufferLevel);

	/// <summary>
	/// Free command buffer set.
	/// </summary>
	/// <param name="commandBuffer_out">Target descriptor (non-null and valid).</param>
	/// <param name="commandPool">Command pool descriptor (non-null and valid).</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <returns>True if freed.</returns>
	bool cdrawVkCommandBufferFree(cdrawVkCommandBuffer* const commandBuffer_out,
		cdrawVkCommandPool const* const commandPool, cdrawVkLogicalDevice const* const logicalDevice);

	/// <summary>
	/// Construct command buffer begin info.
	/// </summary>
	/// <param name="usageFlags">Command buffer usage flags.</param>
	/// <param name="inheritanceInfo">Inheritance info (null for primary).</param>
	/// <returns>Info descriptor.</returns>
	VkCommandBufferBeginInfo cdrawVkCommandBufferBeginInfoCtor(
		VkCommandBufferUsageFlags const usageFlags,
		VkCommandBufferInheritanceInfo const* const inheritanceInfo);

	/// <summary>
	/// Constructor for Vulkan queue descriptor.
	/// </summary>
	/// <param name="queue_out">Target queue descriptor (non-null).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="queue">Vulkan queue handle.</param>
	/// <returns>Success: <paramref name="queue_out"/>; Failure: <c>NULL</c>.</returns>
	cdrawVkQueue* cdrawVkQueueCtor(cdrawVkQueue* const queue_out,
		label_t const name, VkQueue const queue);

	/// <summary>
	/// Destructor interface for Vulkan queue descriptor.
	/// </summary>
	/// <param name="queue_out">Target queue descriptor (non-null).</param>
	void cdrawVkQueueDtor(cdrawVkQueue* const queue_out);

	/// <summary>
	/// Indicate whether descriptor is valid (set up correctly) and should not be modified.
	/// </summary>
	/// <param name="queue">Descriptor (non-null).</param>
	/// <returns>True if valid.</returns>
	bool cdrawVkQueueValid(cdrawVkQueue const* const queue);

	/// <summary>
	/// Indicate whether descriptor is unused (not set up) and can be modified.
	/// </summary>
	/// <param name="queue">Descriptor (non-null).</param>
	/// <returns>True if unused.</returns>
	bool cdrawVkQueueUnused(cdrawVkQueue const* const queue);

	/// <summary>
	/// Construct info descriptor for queue submission.
	/// </summary>
	/// <param name="waitSemaphoreCount">Number of semaphores to wait on before submit.</param>
	/// <param name="waitSemaphores">Set of semaphores to wait on before submit.</param>
	/// <param name="waitDstStageMask">Set of pipeline stages (corresponding to semaphores) at which semaphores will wait.</param>
	/// <param name="commandBufferCount">Number of command buffers to submit.</param>
	/// <param name="commandBuffers">Command buffers to submit.</param>
	/// <param name="signalSemaphoreCount">Number of semaphores to signal on complete.</param>
	/// <param name="signalSemaphores">Set of semaphores to signal on complete.</param>
	/// <returns>Info descriptor.</returns>
	VkSubmitInfo cdrawVkSubmitInfoCtor(
		uint32_t const waitSemaphoreCount,
		VkSemaphore const waitSemaphores[/*waitSemaphoreCount*/],
		VkPipelineStageFlags const waitDstStageMask[/*waitSemaphoreCount*/],
		uint32_t const commandBufferCount,
		VkCommandBuffer const commandBuffers[/*commandBufferCount*/],
		uint32_t const signalSemaphoreCount,
		VkSemaphore const signalSemaphores[/*signalSemaphoreCount*/]);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if ((!defined _CDRAW_RENDERER_DEVICE_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))