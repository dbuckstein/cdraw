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
* cdrawRendererMemory_vk.c
* Internal source for Vulkan memory management.
*/

#include "_h/cdrawRenderer_vk.h"
#include "_h/cdrawRenderer_vk/cdrawRendererMemory_vk.h"

#include <stdio.h>
#include <string.h>


/******************************************************************************
* SECTION: Synchronization utilities.
******************************************************************************/

bool cdrawRendererDestroySemaphore_vk(VkSemaphore* const semaphore_out,
	VkDevice const device, VkAllocationCallbacks const* const alloc_opt)
{
	cdraw_assert(semaphore_out);
	if (!*semaphore_out)
		return true;
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

bool cdrawRendererCreateSemaphore_vk(VkSemaphore* const semaphore_out,
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

bool cdrawRendererDestroyFence_vk(VkFence* const fence_out,
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

bool cdrawRendererCreateFence_vk(VkFence* const fence_out,
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
* Private implementations.
******************************************************************************/

static ptr_t cdrawVkAllocatorAllocation(
	cdrawVkAllocatorData* const pUserData,
	size_t const size,
	size_t const alignment,
	VkSystemAllocationScope const allocationScope)
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
	cdrawVkAllocatorData* const pUserData,
	ptr_t const pMemory)
{
	if (pMemory)
	{
		// perform custom free here
		++pUserData->freeCount;
	}
}

static ptr_t cdrawVkAllocatorReallocation(
	cdrawVkAllocatorData* const pUserData,
	ptr_t const pOriginal,
	size_t const size,
	size_t const alignment,
	VkSystemAllocationScope const allocationScope)
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
	cdrawVkAllocatorData* const pUserData,
	size_t const size,
	VkInternalAllocationType const allocationType,
	VkSystemAllocationScope const allocationScope)
{
	++pUserData->internalAllocCount;
}

static void cdrawVkAllocatorInternalFreeNotification(
	cdrawVkAllocatorData* const pUserData,
	size_t const size,
	VkInternalAllocationType const allocationType,
	VkSystemAllocationScope const allocationScope)
{
	++pUserData->internalFreeCount;
}


/******************************************************************************
* Public implementations.
******************************************************************************/

VkAllocationCallbacks const* cdrawVkAllocatorUse(cdrawVkAllocator const* const allocator)
{
	cdraw_assert(allocator);
	return (allocator->allocationCallbacks.pUserData ? &allocator->allocationCallbacks : NULL);
}

VkAllocationCallbacks const* cdrawVkAllocatorInit(cdrawVkAllocator* const allocator_out,
	label_t const name, bool const enable)
{
	cdraw_assert(allocator_out);
	label_copy_safe(allocator_out->name, name);

	if (enable)
		allocator_out->allocationCallbacks.pUserData = &allocator_out->allocatorData;

	if (cdrawVkAllocatorUse(allocator_out) == NULL)
		return NULL;

	allocator_out->allocationCallbacks.pfnAllocation = cdrawVkAllocatorAllocation;
	allocator_out->allocationCallbacks.pfnReallocation = cdrawVkAllocatorReallocation;
	allocator_out->allocationCallbacks.pfnFree = cdrawVkAllocatorFree;
	allocator_out->allocationCallbacks.pfnInternalAllocation = cdrawVkAllocatorInternalAllocationNotification;
	allocator_out->allocationCallbacks.pfnInternalFree = cdrawVkAllocatorInternalFreeNotification;

	memset(&allocator_out->allocatorData, 0, sizeof(allocator_out->allocatorData));

	return (&allocator_out->allocationCallbacks);
}

bool cdrawVkAllocatorReset(cdrawVkAllocator* const allocator_out)
{
	cdraw_assert(allocator_out);
	label_init(allocator_out->name);

	if (!cdrawVkAllocatorUse(allocator_out))
		return false;

	allocator_out->allocationCallbacks.pUserData = NULL;
	allocator_out->allocationCallbacks.pfnAllocation = NULL;
	allocator_out->allocationCallbacks.pfnReallocation = NULL;
	allocator_out->allocationCallbacks.pfnFree = NULL;
	allocator_out->allocationCallbacks.pfnInternalAllocation = NULL;
	allocator_out->allocationCallbacks.pfnInternalFree = NULL;

	memset(&allocator_out->allocatorData, 0, sizeof(allocator_out->allocatorData));

	return true;
}

VkMemoryAllocateInfo cdrawVkMemoryAllocateInfoCtor(
	VkDeviceSize const allocationSize,
	uint32_t const memoryTypeIndex)
{
	VkMemoryAllocateInfo memAllocInfo = { 0 };
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.allocationSize = allocationSize;
	memAllocInfo.memoryTypeIndex = memoryTypeIndex;
	return memAllocInfo;
}
