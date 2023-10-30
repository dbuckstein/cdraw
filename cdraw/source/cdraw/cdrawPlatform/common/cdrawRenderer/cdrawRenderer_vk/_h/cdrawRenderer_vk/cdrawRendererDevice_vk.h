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
	VkCommandBuffer commandBuffer;
} cdrawVkCommandBuffer;


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
	/// Constructor for Vulkan command buffer descriptor.
	/// </summary>
	/// <param name="commandBuffer_out">Target command buffer descriptor (non-null).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="commandBuffer">Vulkan command buffer handle.</param>
	/// <returns>Success: <paramref name="commandBuffer_out"/>; Failure: <c>NULL</c>.</returns>
	cdrawVkCommandBuffer* cdrawVkCommandBufferCtor(cdrawVkCommandBuffer* const commandBuffer_out,
		label_t const name, VkCommandBuffer const commandBuffer);

	/// <summary>
	/// Destructor interface for Vulkan command buffer descriptor.
	/// </summary>
	/// <param name="commandBuffer_out">Target command buffer descriptor (non-null).</param>
	void cdrawVkCommandBufferDtor(cdrawVkCommandBuffer* const commandBuffer_out);

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


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if ((!defined _CDRAW_RENDERER_DEVICE_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))