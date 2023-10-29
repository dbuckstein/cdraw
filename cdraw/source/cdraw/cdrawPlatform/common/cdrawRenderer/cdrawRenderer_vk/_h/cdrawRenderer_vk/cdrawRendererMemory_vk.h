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
* cdrawRendererMemory_vk.h
* Facade interface for Vulkan memory management.
*/

#if ((!defined _CDRAW_RENDERER_MEMORY_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))
#define _CDRAW_RENDERER_MEMORY_VK_H_


typedef struct cdrawVkAllocatorData
{
	/// <summary>
	/// Counts for testing.
	/// </summary>
	uint32_t allocCount, reallocCount, freeCount, internalAllocCount, internalFreeCount;
} cdrawVkAllocatorData;

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
	/// Vulkan allocation callbacks for memory management.
	/// </summary>
	VkAllocationCallbacks allocationCallbacks;

	/// <summary>
	/// User data to pass to allocator.
	/// </summary>
	cdrawVkAllocatorData allocatorData;
} cdrawVkAllocator;


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Get Vulkan allocation callbacks to use.
	/// </summary>
	/// <param name="allocator">Target allocator descriptor (non-null).</param>
	/// <returns>Pointer to allocation callbacks if in use; NULL if not in use.</returns>
	VkAllocationCallbacks const* cdrawVkAllocatorUse(cdrawVkAllocator const* const allocator);

	/// <summary>
	/// Initialize allocator descriptor.
	/// </summary>
	/// <param name="allocator_out">Target allocator descriptor (non-null).</param>
	/// <param name="name">Name of descriptor.</param>
	/// <param name="enable">Using flag, true to prepare for use.</param>
	/// <returns></returns>
	VkAllocationCallbacks* cdrawVkAllocatorInit(cdrawVkAllocator* const allocator_out,
		label_t const name, bool const enable);

	/// <summary>
	/// Reset allocator descriptor.
	/// </summary>
	/// <param name="allocator_out">Target allocator descriptor (non-null).</param>
	/// <returns>True if reset.</returns>
	bool cdrawVkAllocatorReset(cdrawVkAllocator* const allocator_out);

	/// <summary>
	/// Construct Vulkan memory allocate info structure.
	/// </summary>
	/// <param name="allocationSize">Size of memory to allocate.</param>
	/// <param name="memoryTypeIndex">Type of memory to allocate.</param>
	/// <returns>Vulkan memory allocate info structure.</returns>
	VkMemoryAllocateInfo cdrawVkMemoryAllocateInfoCtor(
		VkDeviceSize const allocationSize,
		uint32_t const memoryTypeIndex);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if ((!defined _CDRAW_RENDERER_MEMORY_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))