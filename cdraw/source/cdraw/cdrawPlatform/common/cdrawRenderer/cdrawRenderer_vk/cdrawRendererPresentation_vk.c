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
* cdrawRendererPresentation_vk.c
* Internal source for Vulkan presentation management.
*/

#include "_h/cdrawRenderer_vk.h"
#include "_h/cdrawRenderer_vk/cdrawRendererPresentation_vk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cdraw/cdrawCore/cdrawUtility.h"

/******************************************************************************
* Private implementations.
******************************************************************************/

/// <summary>
/// Destructor interface for Vulkan surface descriptor.
/// </summary>
/// <param name="surface_out">Target surface descriptor (non-null).</param>
static void cdrawVkSurfaceDtor(cdrawVkSurface* const surface_out);

/// <summary>
/// Constructor for Vulkan presentation descriptor.
/// </summary>
/// <param name="presentation_out">Target presentation descriptor (non-null).</param>
/// <param name="name">Descriptor name.</param>
/// <param name="swapchain">Vulkan swapchain handle.</param>
/// <param name="queue_graphics">Vulkan graphics queue handle.</param>
/// <returns>Success: <paramref name="presentation_out"/>; Failure: <c>NULL</c>.</returns>
static cdrawVkPresentation* cdrawVkPresentationCtor(cdrawVkPresentation* const presentation_out,
	label_t const name, VkSwapchainKHR const swapchain, VkQueue const queue_graphics);

/// <summary>
/// Destructor interface for Vulkan presentation descriptor.
/// </summary>
/// <param name="presentation_out">Target presentation descriptor (non-null).</param>
static void cdrawVkPresentationDtor(cdrawVkPresentation* const presentation_out);



/******************************************************************************
* Public implementations.
******************************************************************************/

cdrawVkSurface* cdrawVkSurfaceCtor(cdrawVkSurface* const surface_out,
	label_t const name, VkSurfaceKHR const surface)
{
	failassertret(surface_out && cdrawVkSurfaceUnused(surface_out), NULL);
	failassertret(surface, NULL);
	label_copy_safe(surface_out->name, name);
	surface_out->surface = surface;
	return surface_out;
}

void cdrawVkSurfaceDtor(cdrawVkSurface* const surface_out)
{
	failassertret(surface_out);
	label_init(surface_out->name);
	surface_out->surface = VK_NULL_HANDLE;
}

bool cdrawVkSurfaceValid(cdrawVkSurface const* const surface)
{
	cdraw_assert(surface);
	return (surface->surface);
}

bool cdrawVkSurfaceUnused(cdrawVkSurface const* const surface)
{
	cdraw_assert(surface);
	return (!surface->surface);
}


cdrawVkPresentation* cdrawVkPresentationCtor(cdrawVkPresentation* const presentation_out,
	label_t const name, VkSwapchainKHR const swapchain, VkQueue const queue_graphics)
{
	failassertret(presentation_out && cdrawVkPresentationUnused(presentation_out), NULL);
	failassertret(swapchain && queue_graphics, NULL);
	label_copy_safe(presentation_out->name, name);
	presentation_out->swapchain = swapchain;
	presentation_out->queue_graphics = queue_graphics;
	return presentation_out;
}

void cdrawVkPresentationDtor(cdrawVkPresentation* const presentation_out)
{
	failassertret(presentation_out);
	label_init(presentation_out->name);
	presentation_out->swapchain = VK_NULL_HANDLE;
	presentation_out->queue_graphics = VK_NULL_HANDLE;
}

bool cdrawVkPresentationValid(cdrawVkPresentation const* const presentation)
{
	cdraw_assert(presentation);
	return (presentation->swapchain && presentation->queue_graphics);
}

bool cdrawVkPresentationUnused(cdrawVkPresentation const* const presentation)
{
	cdraw_assert(presentation);
	return (!presentation->swapchain && !presentation->queue_graphics);
}


/******************************************************************************
* SECTION: Surface management (platform-independent).
* Reference: Singh, c.6.
* Substantial improvements: translated to C and organized.
******************************************************************************/

bool cdrawVkSurfaceDestroy(cdrawVkSurface* const surface_out,
	cdrawVkInstance const* const instance, VkAllocationCallbacks const* const alloc_opt)
{
	failassertret(surface_out, false);
	if (cdrawVkSurfaceUnused(surface_out))
		return true;

	cdraw_assert(instance && cdrawVkInstanceValid(instance));
	printf("\n Destroying Vulkan surface \"%s\"...", surface_out->name);
	//if (surface_out->surface)
	{
		vkDestroySurfaceKHR(instance->instance, surface_out->surface, alloc_opt);
		surface_out->surface = VK_NULL_HANDLE;
	}
	printf("\n Vulkan surface \"%s\" destroyed.", surface_out->name);
	label_init(surface_out->name);
	return true;
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

	cdraw_assert(image && cmdBuf && uint32_valid(queueFamilyIndex));

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

bool cdrawVkPresentationCreate(cdrawVkPresentation* const presentation_out,
	label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkSurface const* const surface, cdrawVkCommandPool const* const commandPool, VkAllocationCallbacks const* const alloc_opt)
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
	bool const logicalDeviceValid = logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice);
	uint32_t const queueFamilyIdx_graphics = logicalDeviceValid ? logicalDevice->queueFamilyIdx_graphics : uint32_invalid;
	uint32_t const queueFamilyIndices[] = {
		queueFamilyIdx_graphics,
	};
	VkPhysicalDevice const physicalDevice = logicalDeviceValid ? logicalDevice->physicalDevice.physicalDevice : NULL;

	VkImageViewCreateInfo imageViewCreateInfo;
	VkImageView* const imageViews_out = presentation_out->imageView_present;
	cdrawVkImage* const depthImage_out_opt = &presentation_out->depthImage_present;

	VkResult result = VK_SUCCESS;
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	VkQueue queue = VK_NULL_HANDLE;

	cdraw_assert(presentation_out && cdrawVkPresentationUnused(presentation_out) && logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice) && surface && cdrawVkSurfaceValid(surface) && commandPool && cdrawVkCommandPoolValid(commandPool));
	printf("\n Creating Vulkan presentation \"%s\"...", name);

	// query surface capabilities
	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface->surface, &surfaceCapabilities);
	if (result == VK_SUCCESS)
	{
		// if surface has zero dimension, all of the following is pointless
		// still considered successful, just wait for resize later
		if (!surfaceCapabilities.currentExtent.width || !surfaceCapabilities.currentExtent.height)
		{
			printf("\n Vulkan presentation \"%s\" not created; surface \"%s\" has zero dimension.", name, surface->name);
			return true;
		}

		// desired number of images in swapchain
		imageCount = surfaceCapabilities.minImageCount + 1;
		imageArrayLayers = surfaceCapabilities.maxImageArrayLayers;
		preTransform = surfaceCapabilities.currentTransform;

		// query formats for swapchain images
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface->surface, &nSurfaceFormat, NULL);
		if (result == VK_SUCCESS && nSurfaceFormat)
		{
			pSurfaceFormat = (VkSurfaceFormatKHR*)malloc(sizeof(VkSurfaceFormatKHR) * nSurfaceFormat);
			if (pSurfaceFormat)
			{
				result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface->surface, &nSurfaceFormat, pSurfaceFormat);
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
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface->surface, &nPresentMode, NULL);
		if (result == VK_SUCCESS && nPresentMode)
		{
			pPresentMode = (VkPresentModeKHR*)malloc(sizeof(VkPresentModeKHR) * nPresentMode);
			if (pPresentMode)
			{
				result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface->surface, &nPresentMode, pPresentMode);
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
	result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIdx_graphics, surface->surface, &supportsSurface);
	if (supportsSurface)
	{
		cdraw_assert(result == VK_SUCCESS);

		// get device queue for presentation
		vkGetDeviceQueue(logicalDevice->logicalDevice, queueFamilyIdx_graphics, presentQueueIdx, &queue);
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
			surface->surface,
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
		result = vkCreateSwapchainKHR(logicalDevice->logicalDevice, &swapchainCreateInfo, alloc_opt, &swapchain);
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
		result = vkGetSwapchainImagesKHR(logicalDevice->logicalDevice, swapchain, &nSwapchainImage, NULL);
		if (result == VK_SUCCESS && nSwapchainImage)
		{
			pSwapchainImage = (VkImage*)malloc(sizeof(VkImage) * nSwapchainImage);
			if (pSwapchainImage)
			{
				result = vkGetSwapchainImagesKHR(logicalDevice->logicalDevice, swapchain, &nSwapchainImage, pSwapchainImage);
				cdraw_assert(result == VK_SUCCESS);

				// create image views
				cdraw_assert(nSwapchainImage <= buffer_len(presentation_out->imageView_present));
				for (idx = 0; idx < nSwapchainImage; ++idx)
				{
					cdraw_assert(pSwapchainImage[idx]);
					imageViewCreateInfo.image = pSwapchainImage[idx];
					result = vkCreateImageView(logicalDevice->logicalDevice, &imageViewCreateInfo, alloc_opt, &imageViews_out[idx]);
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
			result = vkCreateImage(logicalDevice->logicalDevice, &imageCreateInfo_depth, alloc_opt, &image_depth);
			if (image_depth)
				cdraw_assert(result == VK_SUCCESS);

			// allocate memory and create view for depth image
			if (image_depth)
			{
				int8_t memTypeIndex;
				VkMemoryRequirements memReq_depth;
				VkMemoryAllocateInfo memAllocInfo;
				vkGetImageMemoryRequirements(logicalDevice->logicalDevice, image_depth, &memReq_depth);
				memTypeIndex = cdrawUtilityLowestBit32(memReq_depth.memoryTypeBits);
				if (memTypeIndex >= 0)
				{
					memAllocInfo = cdrawVkMemoryAllocateInfoCtor(memReq_depth.size, memTypeIndex);
					result = vkAllocateMemory(logicalDevice->logicalDevice, &memAllocInfo, alloc_opt, &imageMem_depth);
					if (imageMem_depth)
					{
						cdraw_assert(result == VK_SUCCESS);
						result = vkBindImageMemory(logicalDevice->logicalDevice, image_depth, imageMem_depth, 0);
						cdraw_assert(result == VK_SUCCESS);
					}
				}
				if (imageMem_depth)
				{
					VkPipelineStageFlags const stage = (VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT);
					VkImageSubresourceRange imageSubResourceRange;
					VkCommandBufferBeginInfo cmdBeginInfo;
					VkSubmitInfo submitInfo;
					cdrawVkCommandBuffer commandBuffer_depth = { 0 };
					if (cdrawVkCommandBufferAlloc(&commandBuffer_depth, "commandBuffer_depth", 1, commandPool, logicalDevice, VK_COMMAND_BUFFER_LEVEL_PRIMARY))
					{
						VkCommandBuffer cmdBuf_depth = commandBuffer_depth.commandBuffer[0];
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

						if (cdrawVkCommandBufferFree(&commandBuffer_depth, commandPool, logicalDevice))
						{
							VkComponentMapping const imageComponent_depth = { VK_COMPONENT_SWIZZLE_IDENTITY };
							imageViewCreateInfo = cdrawVkImageViewCreateInfoCtor(image_depth, VK_IMAGE_VIEW_TYPE_2D,
								imageFormat_depth, imageComponent_depth, imageSubResourceRange);
							result = vkCreateImageView(logicalDevice->logicalDevice, &imageViewCreateInfo, alloc_opt, &imageView_depth);
							if (imageView_depth)
								cdraw_assert(result == VK_SUCCESS);
							depthImage_out_opt->image = image_depth;
						}
					}
				}
			}

			// set handles
			if (!cdrawVkImageCtor(depthImage_out_opt, "cdraw Present Depth", image_depth, imageMem_depth, imageView_depth))
				cdrawVkImageDestroy(depthImage_out_opt, logicalDevice->logicalDevice, alloc_opt);
		}
	}

	// set final outputs or clean up
	if (!swapchain || !queue || (result != VK_SUCCESS))
	{
		cdrawVkPresentationDestroy(presentation_out, logicalDevice, alloc_opt);
		printf("\n Vulkan presentation \"%s\" creation failed.", name);
		return false;
	}
	cdrawVkPresentationCtor(presentation_out, name, swapchain, queue);
	printf("\n Vulkan presentation \"%s\" created.", name);
	cdraw_assert(cdrawVkPresentationValid(presentation_out));
	return true;
}

bool cdrawVkPresentationDestroy(cdrawVkPresentation* const presentation_out,
	cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt)
{
	uint32_t idx;
	failassertret(presentation_out, false);
	if (cdrawVkPresentationUnused(presentation_out))
		return true;

	cdraw_assert(logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice));
	printf("\n Destroying Vulkan presentation \"%s\"...", presentation_out->name);

	cdrawVkImageDestroy(&presentation_out->depthImage_present, logicalDevice->logicalDevice, alloc_opt);

	for (idx = 0; idx < cdrawVkImagePresent_max; ++idx)
	{
		if (presentation_out->imageView_present[idx])
		{
			vkDestroyImageView(logicalDevice->logicalDevice, presentation_out->imageView_present[idx], alloc_opt);
			presentation_out->imageView_present[idx] = VK_NULL_HANDLE;
		}
	}

	//if (presentation_out->swapchain)
	{
		vkDestroySwapchainKHR(logicalDevice->logicalDevice, presentation_out->swapchain, alloc_opt);
		presentation_out->swapchain = VK_NULL_HANDLE;
		presentation_out->queue_graphics = VK_NULL_HANDLE;
	}

	printf("\n Vulkan presentation \"%s\" destroyed.", presentation_out->name);
	label_init(presentation_out->name);
	return true;
}
