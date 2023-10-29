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
#include "cdrawRenderer_vk/_h/cdrawRenderer_vk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cdraw/cdrawCore/cdrawUtility.h"


/******************************************************************************
* Private implementations.
******************************************************************************/

void cdrawVkInstanceRefresh(VkInstance const instance,
	cdrawVkInstanceFuncTable* const f);

void cdrawVkDeviceRefresh(VkDevice const device,
	cdrawVkDeviceFuncTable* const f);


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

static VkFormat cdrawVkFormatDepthStencil(bool const useDepthHiPrec, bool const useStencil)
{
	// depth/stencil buffer: can change bit size of depth and add _S8_UINT for stencil
	VkFormat format = VK_FORMAT_D16_UNORM;
	if (useStencil)
		format = useDepthHiPrec ? VK_FORMAT_D32_SFLOAT_S8_UINT : VK_FORMAT_D16_UNORM_S8_UINT;
	else if (useDepthHiPrec)
		format = VK_FORMAT_D32_SFLOAT;
	return format;
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
			VkFormat const imageFormat_depth = cdrawVkFormatDepthStencil(useDepthHiPrec, useStencil);

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
						result = vkQueueWaitIdle(queue);
						cdraw_assert(result == VK_SUCCESS);

						if (cdrawRendererFreeCommandBuffers_vk(&cmdBuf_depth, 1, cmdPool, device))
						{
							VkComponentMapping const imageComponent_depth = { VK_COMPONENT_SWIZZLE_IDENTITY };
							imageViewCreateInfo = cdrawVkImageViewCreateInfoCtor(image_depth, VK_IMAGE_VIEW_TYPE_2D,
								imageFormat_depth, imageComponent_depth, imageSubResourceRange);
							result = vkCreateImageView(device, &imageViewCreateInfo, alloc_opt, &imageView_depth);
							if (imageView_depth)
								cdraw_assert(result == VK_SUCCESS);
							depthImage_out_opt->image = image_depth;
						}
					}
				}
			}

			// set handles
			if (!cdrawVkImageCtor(depthImage_out_opt, "cdraw Present Depth", image_depth, imageMem_depth, imageView_depth))
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
	vkDeviceWaitIdle(r->logicalDevice.logicalDevice);
	for (idx = 0; idx < nSwapchains; ++idx)
	{
		result = vkAcquireNextImageKHR(r->logicalDevice.logicalDevice, swapchains[idx], UINT64_MAX, r->semaphore, VK_NULL_HANDLE, &imageIndices[idx]);
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
		vkDeviceWaitIdle(r->logicalDevice.logicalDevice);

		// release old swapchain if the window size was valid
		if (w_old && h_old)
			result = cdrawRendererDestroySwapchain_vk(&r->swapchain, &r->queue_graphics,
				r->imageView_present, buffer_len(r->imageView_present), &r->depthImage_present, r->logicalDevice.logicalDevice, &r->allocator.allocationCallbacks);

		// create new swapchain if the new window size is valid
		if (w_new && h_new)
			result = cdrawRendererCreateSwapchain_vk(&r->swapchain, &r->queue_graphics,
				r->imageView_present, buffer_len(r->imageView_present), &r->depthImage_present,
				r->logicalDevice.logicalDevice, r->surface, r->cmdPool, r->logicalDevice.physicalDevice.physicalDevice, r->logicalDevice.queueFamilyIdx_graphics, &r->allocator.allocationCallbacks);
	}
	return result;
}


/// <summary>
/// Vulkan convenience function table.
/// </summary>
static cdrawRendererFuncTable gRendererFuncTable_vk;


bool cdrawVkDebugCreateDebugReport(VkDebugReportCallbackEXT* const debugReport_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt,
	cdrawVkInstanceDebugFuncTable const* const f);

bool cdrawVkDebugDestroyDebugReport(VkDebugReportCallbackEXT* const debugReport_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt,
	cdrawVkInstanceDebugFuncTable const* const f);

bool cdrawVkDebugCreateDebugUtilsMessenger(VkDebugUtilsMessengerEXT* const debugUtilsMsg_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt,
	cdrawVkInstanceDebugFuncTable const* const f);

bool cdrawVkDebugDestroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT* const debugUtilsMsg_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt,
	cdrawVkInstanceDebugFuncTable const* const f);


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
	VkAllocationCallbacks const* const alloc_opt = cdrawVkAllocatorInit(&r->allocator, "cdrawVkAllocator", false);

	// CREATE INSTANCE
	result = cdrawVkInstanceCreate(&r->instance, "cdrawVkInstance", alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

#if CDRAW_DEBUG
	//// CREATE DEBUG REPORT
	//result = cdrawVkDebugCreateDebugReport(&r->debug.debugReport, r->instance.instance, alloc_opt, &r->instance.f.f_debug);
	//failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE DEBUG MESSENGER
	result = cdrawVkDebugCreateDebugUtilsMessenger(&r->debug.debugMessenger, r->instance.instance, alloc_opt, &r->instance.f.f_debug);
	failassertret(result, result_seterror(errcode_renderer_init));
#endif // #if CDRAW_DEBUG

	// CREATE LOGICAL DEVICE
	result = cdrawVkLogicalDeviceCreate(&r->logicalDevice, "cdrawVkLogicalDevice", &r->instance, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE PRESENTATION SURFACE
	result = cdrawRendererCreateSurface_vk(&r->surface, r->instance.instance, p_data, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE COMMAND POOL
	result = cdrawRendererCreateCommandPool_vk(&r->cmdPool, r->logicalDevice.logicalDevice, r->logicalDevice.queueFamilyIdx_graphics, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE SEMAPHORES
	result = cdrawRendererCreateSemaphore_vk(&r->semaphore, r->logicalDevice.logicalDevice, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE SWAPCHAIN (may not be created due to surface size)
	result = cdrawRendererCreateSwapchain_vk(&r->swapchain, &r->queue_graphics,
		r->imageView_present, buffer_len(r->imageView_present), &r->depthImage_present,
		r->logicalDevice.logicalDevice, r->surface, r->cmdPool, r->logicalDevice.physicalDevice.physicalDevice, r->logicalDevice.queueFamilyIdx_graphics, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// all done
	renderer->r = r;
	renderer->f = &gRendererFuncTable_vk;
	result_return();
}

result_t cdrawRendererDestroy_vk(cdrawRenderer* const renderer)
{
	result_init();
	cdrawRenderer_vk* r;
	VkAllocationCallbacks const* alloc_opt;
	asserterr(renderer && renderer->r && renderer->renderAPI == cdrawRenderAPI_Vulkan, errcode_invalidarg);
	r = ((cdrawRenderer_vk*)renderer->r);
	alloc_opt = cdrawVkAllocatorUse(&r->allocator);

	// safety
	vkDeviceWaitIdle(r->logicalDevice.logicalDevice);

	// swapchain (requires device and surface; may not have been created due to surface size)
	cdrawRendererDestroySwapchain_vk(&r->swapchain, &r->queue_graphics,
		r->imageView_present, buffer_len(r->imageView_present), &r->depthImage_present, r->logicalDevice.logicalDevice, alloc_opt);

	// semaphores
	cdrawRendererDestroySemaphore_vk(&r->semaphore, r->logicalDevice.logicalDevice, alloc_opt);

	// command pool (requires device)
	cdrawRendererDestroyCommandPool_vk(&r->cmdPool, r->logicalDevice.logicalDevice, alloc_opt);

	// presentation surface (requires instance)
	cdrawRendererDestroySurface_vk(&r->surface, r->instance.instance, alloc_opt);

	// logical device (wait for it to finish work)
	cdrawVkLogicalDeviceDestroy(&r->logicalDevice, alloc_opt);

#if CDRAW_DEBUG
	// destroy debug messenger
	cdrawVkDebugDestroyDebugUtilsMessenger(&r->debug.debugMessenger, r->instance.instance, alloc_opt, &r->instance.f.f_debug);

	//// destroy debug report
	//cdrawVkDebugDestroyDebugReport(&r->debug.debugReport, r->instance.instance, alloc_opt, &r->instance.f.f_debug);
#endif // #if CDRAW_DEBUG

	// finally, destroy instance
	cdrawVkInstanceDestroy(&r->instance, alloc_opt);

	// clean allocation callbacks
	cdrawVkAllocatorReset(&r->allocator);

	// all done
	free(renderer->r);
	renderer->r = NULL;
	renderer->f = NULL;
	result_return();
}

result_t cdrawRendererRefresh_vk(cdrawRenderer const* const renderer)
{
	result_init();
	cdrawRenderer_vk* r;
	asserterr(renderer && renderer->r && renderer->renderAPI == cdrawRenderAPI_Vulkan, errcode_invalidarg);
	r = ((cdrawRenderer_vk*)renderer->r);

	cdrawVkInstanceRefresh(r->instance.instance, &r->instance.f);
	cdrawVkDeviceRefresh(r->logicalDevice.logicalDevice, &r->logicalDevice.f);

	result_return();
}

result_t cdrawRendererRefreshAPI_vk()
{
	result_init();

	gRendererFuncTable_vk.cdrawRendererPrint = cdrawRendererPrint_vk;
	gRendererFuncTable_vk.cdrawRendererDisplay = cdrawRendererDisplay_vk;
	gRendererFuncTable_vk.cdrawRendererResize = cdrawRendererResize_vk;

	result_return();
}
