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

/******************************************************************************
* Private implementations.
******************************************************************************/

/// <summary>
/// Constructor for Vulkan surface descriptor.
/// </summary>
/// <param name="surface_out">Target surface descriptor (non-null).</param>
/// <param name="name">Descriptor name.</param>
/// <param name="surface">Vulkan surface handle.</param>
/// <returns>Success: <paramref name="surface_out"/>; Failure: <c>NULL</c>.</returns>
static cdrawVkSurface* cdrawVkSurfaceCtor(cdrawVkSurface* const surface_out,
	label_t const name, VkSurfaceKHR const surface);

/// <summary>
/// Destructor interface for Vulkan surface descriptor.
/// </summary>
/// <param name="surface_out">Target surface descriptor (non-null).</param>
static void cdrawVkSurfaceDtor(cdrawVkSurface* const surface_out);

/// <summary>
/// Constructor for Vulkan swapchain descriptor.
/// </summary>
/// <param name="swapchain_out">Target swapchain descriptor (non-null).</param>
/// <param name="name">Descriptor name.</param>
/// <param name="swapchain">Vulkan swapchain handle.</param>
/// <returns>Success: <paramref name="swapchain_out"/>; Failure: <c>NULL</c>.</returns>
static cdrawVkSwapchain* cdrawVkSwapchainCtor(cdrawVkSwapchain* const swapchain_out,
	label_t const name, VkSwapchainKHR const swapchain,
	uint32_t const imageCount, VkImageView const imageView[/*imageCount*/]);

/// <summary>
/// Destructor interface for Vulkan swapchain descriptor.
/// </summary>
/// <param name="swapchain_out">Target swapchain descriptor (non-null).</param>
static void cdrawVkSwapchainDtor(cdrawVkSwapchain* const swapchain_out);

/// <summary>
/// Constructor for Vulkan presentation descriptor.
/// </summary>
/// <param name="presentation_out">Target presentation descriptor (non-null).</param>
/// <param name="name">Descriptor name.</param>
/// <param name="swapchain">Vulkan swapchain handle.</param>
/// <returns>Success: <paramref name="presentation_out"/>; Failure: <c>NULL</c>.</returns>
static cdrawVkPresentation* cdrawVkPresentationCtor(cdrawVkPresentation* const presentation_out,
	label_t const name, cdrawVkSwapchain const* const swapchain);

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


cdrawVkSwapchain* cdrawVkSwapchainCtor(cdrawVkSwapchain* const swapchain_out,
	label_t const name, VkSwapchainKHR const swapchain,
	uint32_t const imageCount, VkImageView const imageView[/*imageCount*/])
{
	uint32_t idx;
	failassertret(swapchain_out && cdrawVkSwapchainUnused(swapchain_out), NULL);
	failassertret(swapchain && gCount(imageCount, cdrawVkSwapchainImage_max), NULL);
	label_copy_safe(swapchain_out->name, name);
	swapchain_out->swapchain = swapchain;
	swapchain_out->imageCount = imageCount;
	for (idx = 0; idx < swapchain_out->imageCount; ++idx)
	{
		failassertret(imageView[idx], NULL);
		swapchain_out->imageView[idx] = imageView[idx];
	}
	return swapchain_out;
}

void cdrawVkSwapchainDtor(cdrawVkSwapchain* const swapchain_out)
{
	uint32_t idx;
	failassertret(swapchain_out);
	label_init(swapchain_out->name);
	swapchain_out->swapchain = VK_NULL_HANDLE;
	for (idx = 0; idx < swapchain_out->imageCount; ++idx)
		swapchain_out->imageView[idx] = VK_NULL_HANDLE;
	swapchain_out->imageCount = 0;
}

bool cdrawVkSwapchainValid(cdrawVkSwapchain const* const swapchain)
{
	uint32_t idx;
	cdraw_assert(swapchain);
	if (!swapchain->swapchain || !swapchain->imageCount)
		return false;
	for (idx = 0; idx < swapchain->imageCount; ++idx)
		if (!swapchain->imageView[idx])
			return false;
	return true;
}

bool cdrawVkSwapchainUnused(cdrawVkSwapchain const* const swapchain)
{
	uint32_t idx;
	cdraw_assert(swapchain);
	if (swapchain->swapchain || swapchain->imageCount)
		return false;
	for (idx = 0; idx < swapchain->imageCount; ++idx)
		if (swapchain->imageView[idx])
			return false;
	return true;
}


cdrawVkPresentation* cdrawVkPresentationCtor(cdrawVkPresentation* const presentation_out,
	label_t const name, cdrawVkSwapchain const* const swapchain)
{
	failassertret(presentation_out && cdrawVkPresentationUnused(presentation_out), NULL);
	label_copy_safe(presentation_out->name, name);
	presentation_out->swapchain = *swapchain;
	return presentation_out;
}

void cdrawVkPresentationDtor(cdrawVkPresentation* const presentation_out)
{
	failassertret(presentation_out);
	label_init(presentation_out->name);
	memset(&presentation_out->swapchain, 0, sizeof(presentation_out->swapchain));
}

bool cdrawVkPresentationValid(cdrawVkPresentation const* const presentation)
{
	uint32_t idx;
	cdraw_assert(presentation);
	if (!cdrawVkSwapchainValid(&presentation->swapchain)
		|| !cdrawVkRenderPassValid(&presentation->renderPass_present))
		return false;
	for (idx = 0; idx < presentation->swapchain.imageCount; ++idx)
		if (!cdrawVkQueueValid(&presentation->queue_graphics[idx])
			|| !cdrawVkFramebufferValid(&presentation->framebuffer_present[idx]))
			return false;
	return true;
}

bool cdrawVkPresentationUnused(cdrawVkPresentation const* const presentation)
{
	uint32_t idx;
	cdraw_assert(presentation);
	if (!cdrawVkSwapchainUnused(&presentation->swapchain)
		|| !cdrawVkRenderPassUnused(&presentation->renderPass_present))
		return false;
	for (idx = 0; idx < presentation->swapchain.imageCount; ++idx)
		if (!cdrawVkQueueUnused(&presentation->queue_graphics[idx])
			|| !cdrawVkFramebufferUnused(&presentation->framebuffer_present[idx]))
			return false;
	return true;
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
	cdraw_assert(cdrawVkSurfaceUnused(surface_out));
	return true;
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

bool cdrawVkSwapchainCreate(cdrawVkSwapchain* const swapchain_out,
	label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkSurface const* const surface,
	//cdrawVkQueue const* const queue_graphics, cdrawVkCommandPool const* const commandPool,
	VkAllocationCallbacks const* const alloc_opt)
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
	VkCompositeAlphaFlagBitsKHR compositeAlpha = (VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR);

	uint32_t nSwapchainImage = 0;
	VkImage* pSwapchainImage = NULL;

	// queue families
	bool const logicalDeviceValid = logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice);
	VkPhysicalDevice const physicalDevice = logicalDeviceValid ? logicalDevice->physicalDevice.physicalDevice : NULL;
	uint32_t const queueFamilyIdx_graphics = logicalDeviceValid ? logicalDevice->queueFamilyIdx_graphics : uint32_invalid;
	uint32_t const queueFamilyIndices[] = {
		queueFamilyIdx_graphics,
	};

	VkResult result = VK_SUCCESS;
	cdraw_assert(swapchain_out && cdrawVkSwapchainUnused(swapchain_out) && logicalDeviceValid && physicalDevice
		//&& queue_graphics && cdrawVkQueueValid(queue_graphics) && commandPool && cdrawVkCommandPoolValid(commandPool)
		&& surface && cdrawVkSurfaceValid(surface));
	printf("\n Creating Vulkan swapchain \"%s\"...", name);

	// wait
	vkDeviceWaitIdle(logicalDevice->logicalDevice);

	// query surface capabilities
	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface->surface, &surfaceCapabilities);
	if (result == VK_SUCCESS)
	{
		// if surface has zero dimension, all of the following is pointless
		// still considered successful, just wait for resize later
		if (!surfaceCapabilities.currentExtent.width || !surfaceCapabilities.currentExtent.height)
		{
			printf("\n Vulkan swapchain \"%s\" not created; surface \"%s\" has zero dimension.", name, surface->name);
			return true;
		}

		// desired number of images in swapchain
		imageCount = surfaceCapabilities.minImageCount + 1;
		imageCount = gClamp(imageCount, surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);
		imageCount = min(imageCount, buffer_len(swapchain_out->imageView));
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

	// FINAL CREATE SWAPCHAIN
	{
		// how can the images be used
		VkImageUsageFlags const imageUsage = (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);

		// image sharing mode
		VkSharingMode const imageSharingMode = (VK_SHARING_MODE_EXCLUSIVE);

		// creation info
		VkSwapchainCreateInfoKHR const swapchainCreateInfo = cdrawVkSwapchainCreateInfoCtor(
			surface->surface,
			imageCount,
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
		result = vkCreateSwapchainKHR(logicalDevice->logicalDevice, &swapchainCreateInfo, alloc_opt, &swapchain_out->swapchain);
		if (swapchain_out->swapchain)
			cdraw_assert(result == VK_SUCCESS);
	}

	// next, get images
	if (swapchain_out->swapchain)
	{
		// reusable image view descriptor
		VkImageViewCreateInfo imageViewCreateInfo = cdrawVkImageViewCreateInfoCtor(
			VK_NULL_HANDLE, // updated per image below
			VK_IMAGE_VIEW_TYPE_2D,
			surfaceFormat.format,
			cdrawVkComponentMappingCtorDefault(),
			cdrawVkImageSubresourceRangeCtorDefaultColor()
		);
		bool createdImageViews = false;
		cdrawVkCommandBuffer commandBuffer_color = { 0 };

		// get count and allocate for color images
		result = vkGetSwapchainImagesKHR(logicalDevice->logicalDevice, swapchain_out->swapchain, &nSwapchainImage, NULL);
		if (result == VK_SUCCESS && nSwapchainImage)
		{
			pSwapchainImage = (VkImage*)malloc(sizeof(VkImage) * nSwapchainImage);
			if (pSwapchainImage)
			{
				result = vkGetSwapchainImagesKHR(logicalDevice->logicalDevice, swapchain_out->swapchain, &nSwapchainImage, pSwapchainImage);
				cdraw_assert(result == VK_SUCCESS);

				// create image views
				cdraw_assert(nSwapchainImage <= buffer_len(swapchain_out->imageView));
				//if (cdrawVkCommandBufferAlloc(&commandBuffer_color, "commandBuffer_swapchain", 1, commandPool, logicalDevice, VK_COMMAND_BUFFER_LEVEL_PRIMARY))
				{
					//VkImageLayout const imageLayout_color_orig = (VK_IMAGE_LAYOUT_UNDEFINED);
					//VkImageLayout const imageLayout_color = (VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
					//VkPipelineStageFlags const stage_orig = (VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
					//VkPipelineStageFlags const stage = (VK_PIPELINE_STAGE_TRANSFER_BIT);
					//VkImageSubresourceRange const imageSubResourceRange = cdrawVkImageSubresourceRangeCtorDefaultColor();
					//VkCommandBufferBeginInfo const cmdBeginInfo = cdrawVkCommandBufferBeginInfoCtor(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, NULL);
					//VkCommandBuffer cmdBuf_color = commandBuffer_color.commandBuffer[0];
					//VkSubmitInfo submitInfo;
					//result = vkBeginCommandBuffer(cmdBuf_color, &cmdBeginInfo);
					//for (idx = 0; idx < nSwapchainImage; ++idx)
					//	cdrawVkCmdImageSetLayout(pSwapchainImage[idx], cmdBuf_color, logicalDevice->queueFamilyIdx_graphics,
					//		0, 0, stage_orig, stage, imageLayout_color_orig, imageLayout_color, imageSubResourceRange);
					//submitInfo = cdrawVkSubmitInfoCtor(0, NULL, NULL, 1, &cmdBuf_color, 0, NULL);
					//result = vkEndCommandBuffer(cmdBuf_color);
					//cdraw_assert(result == VK_SUCCESS);
					//result = vkQueueSubmit(queue_graphics->queue, 1, &submitInfo, VK_NULL_HANDLE);
					//cdraw_assert(result == VK_SUCCESS);
					//result = vkQueueWaitIdle(queue_graphics->queue);
					//cdraw_assert(result == VK_SUCCESS);
					//if (cdrawVkCommandBufferFree(&commandBuffer_color, commandPool, logicalDevice))
					{
						for (idx = 0; idx < nSwapchainImage; ++idx)
						{
							cdraw_assert(pSwapchainImage[idx]);
							imageViewCreateInfo.image = pSwapchainImage[idx];
							result = vkCreateImageView(logicalDevice->logicalDevice, &imageViewCreateInfo, alloc_opt, &swapchain_out->imageView[idx]);
							if (swapchain_out->imageView[idx])
								cdraw_assert(result == VK_SUCCESS);
							else
								break;
						}
						cdraw_assert(idx == nSwapchainImage);
						swapchain_out->imageCount = nSwapchainImage;
						swapchain_out->imageDimensions = surfaceCapabilities.currentExtent;
					}
					cdraw_assert(swapchain_out->imageCount == nSwapchainImage);
				}

				free(pSwapchainImage);
				pSwapchainImage = NULL;
			}
		}
	}

	// describe attachment
	if (cdrawVkSwapchainValid(swapchain_out))
	{
		bool const clearImage = true;
		VkSampleCountFlagBits const sampleCount = VK_SAMPLE_COUNT_1_BIT;
		VkAttachmentLoadOp const loadOp = clearImage ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
		VkAttachmentStoreOp const storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		VkAttachmentLoadOp const loadOpStencil = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		VkAttachmentStoreOp const storeOpStencil = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		VkImageLayout const initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		VkImageLayout const finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		swapchain_out->imageAttach = cdrawVkAttachmentDescriptionCtor(false, surfaceFormat.format,
			sampleCount, loadOp, storeOp, loadOpStencil, storeOpStencil, initialLayout, finalLayout);
	}

	// set final outputs or clean up
	if (!cdrawVkSwapchainValid(swapchain_out) || (result != VK_SUCCESS))
	{
		cdrawVkSwapchainDestroy(swapchain_out, logicalDevice, alloc_opt);
		printf("\n Vulkan swapchain \"%s\" creation failed.", name);
		return false;
	}
	printf("\n Vulkan swapchain \"%s\" created.", name);
	label_copy_safe(swapchain_out->name, name);
	cdraw_assert(cdrawVkSwapchainValid(swapchain_out));
	return true;
}

bool cdrawVkSwapchainDestroy(cdrawVkSwapchain* const swapchain_out,
	cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt)
{
	uint32_t idx;
	failassertret(swapchain_out, false);
	if (cdrawVkSwapchainUnused(swapchain_out))
		return true;

	cdraw_assert(logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice));
	printf("\n Destroying Vulkan swapchain \"%s\"...", swapchain_out->name);

	vkDeviceWaitIdle(logicalDevice->logicalDevice);
	for (idx = 0; idx < swapchain_out->imageCount; ++idx)
	{
		if (swapchain_out->imageView[idx])
		{
			vkDestroyImageView(logicalDevice->logicalDevice, swapchain_out->imageView[idx], alloc_opt);
			swapchain_out->imageView[idx] = VK_NULL_HANDLE;
		}
	}
	if (swapchain_out->swapchain)
	{
		vkDestroySwapchainKHR(logicalDevice->logicalDevice, swapchain_out->swapchain, alloc_opt);
		swapchain_out->swapchain = VK_NULL_HANDLE;
	}
	swapchain_out->imageCount = 0;
	swapchain_out->imageDimensions.width = swapchain_out->imageDimensions.height = 0;
	memset(&swapchain_out->imageAttach, 0, sizeof(swapchain_out->imageAttach));
	printf("\n Vulkan swapchain \"%s\" destroyed.", swapchain_out->name);
	label_init(swapchain_out->name);
	cdraw_assert(cdrawVkSwapchainUnused(swapchain_out));
	return true;
}


#if CDRAW_DEBUG
static VkQueryPoolCreateInfo cdrawVkQueryPoolCreateInfoCtor(
	VkQueryType const queryType,
	uint32_t const queryCount)
{
	VkQueryPoolCreateInfo queryPoolCreateInfo = { 0 };
	queryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
	queryPoolCreateInfo.queryType = queryType;
	queryPoolCreateInfo.queryCount = queryCount;
	return queryPoolCreateInfo;
}
#endif // #if CDRAW_DEBUG

bool cdrawVkPresentationCreate(cdrawVkPresentation* const presentation_out,
	label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkSurface const* const surface, cdrawVkCommandPool const* const commandPool, VkAllocationCallbacks const* const alloc_opt)
{
	uint32_t idx;

	// queue families
	bool const logicalDeviceValid = logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice);
	VkPhysicalDevice const physicalDevice = logicalDeviceValid ? logicalDevice->physicalDevice.physicalDevice : NULL;
	uint32_t const queueFamilyIdx_graphics = logicalDeviceValid ? logicalDevice->queueFamilyIdx_graphics : uint32_invalid;
	VkBool32 supportsSurface = VK_FALSE;

	VkResult result = VK_SUCCESS;
	cdraw_assert(presentation_out && cdrawVkPresentationUnused(presentation_out) && logicalDeviceValid && physicalDevice && surface && cdrawVkSurfaceValid(surface) && commandPool && cdrawVkCommandPoolValid(commandPool));
	printf("\n Creating Vulkan swapchain \"%s\"...", name);

	// wait
	vkDeviceWaitIdle(logicalDevice->logicalDevice);

	// search for queue that supports surface
	result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIdx_graphics, surface->surface, &supportsSurface);
	if (supportsSurface)
	{
		uint32_t const nQueues = buffer_len(presentation_out->queue_graphics);
		VkQueue queue_graphics = VK_NULL_HANDLE;

		cdraw_assert(result == VK_SUCCESS);
		cdraw_assert(nQueues <= cdrawVkSwapchainImage_max);

		// get device queues for graphics
		for (idx = 0; idx < nQueues; ++idx)
		{
			vkGetDeviceQueue(logicalDevice->logicalDevice, queueFamilyIdx_graphics, idx, &queue_graphics);
			cdraw_assert(queue_graphics);
			cdrawVkQueueCtor(&presentation_out->queue_graphics[idx], "cdrawVkPres.queue_graphics", queue_graphics);
		}
		// presentation
		{
			cdraw_assert(idx == nQueues);
			vkGetDeviceQueue(logicalDevice->logicalDevice, queueFamilyIdx_graphics, idx, &queue_graphics);
			cdraw_assert(queue_graphics);
			cdrawVkQueueCtor(&presentation_out->queue_present, "cdrawVkPres.queue_present", queue_graphics);
		}
	}

	// allocate swapchain and command buffers
	if (cdrawVkQueueValid(&presentation_out->queue_present))
	{
		cdrawVkSwapchainCreate(&presentation_out->swapchain, "cdrawVkPres.swapchain",
			logicalDevice, surface, alloc_opt);
	}

	// allocate command buffers
	if (cdrawVkSwapchainValid(&presentation_out->swapchain))
	{
		cdrawVkCommandBufferAlloc(&presentation_out->commandBuffer_present, "cdrawVkPres.cmdBuf",
			cdrawFramesInFlight_max,
			//presentation_out->swapchain.imageCount,
			commandPool, logicalDevice, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	}

	// create fences and semaphores for presentation
	if (cdrawVkCommandBufferValid(&presentation_out->commandBuffer_present))
	{
		for (idx = 0; idx < cdrawFramesInFlight_max; ++idx)
		{
			cdrawRendererCreateFence_vk(&presentation_out->fence_acquire[idx], logicalDevice->logicalDevice, true, alloc_opt);
			cdrawRendererCreateFence_vk(&presentation_out->fence_submit[idx], logicalDevice->logicalDevice, true, alloc_opt);
			cdrawRendererCreateSemaphore_vk(&presentation_out->semaphore_acquire[idx], logicalDevice->logicalDevice, alloc_opt);
			cdrawRendererCreateSemaphore_vk(&presentation_out->semaphore_submit[idx], logicalDevice->logicalDevice, alloc_opt);
		}
		cdraw_assert(result == VK_SUCCESS);
	}

	// next, get images
	if (cdrawVkSwapchainValid(&presentation_out->swapchain))
	{
		// default depth image (if requested) and attachment info, render pass and framebuffer
		{
			bool const useDepth = true, useDepthFloat = false, useStencil = true;
			uint32_t attachmentCount = 0, colorAttachmentCount = 0;
			VkAttachmentDescription attachmentDesc[2] = { 0 };
			VkImageView attachmentView[cdrawVkSwapchainImage_max][buffer_len(attachmentDesc)] = { VK_NULL_HANDLE };
			VkAttachmentReference colorAttachmentRef[cdrawVkFramebufferColorAttach_max] = { 0 };
			VkAttachmentReference resolveAttachmentRef[buffer_len(colorAttachmentRef)] = { 0 };
			VkAttachmentReference depthAttachmentRef[1] = { 0 };
			VkSubpassDescription subpassDesc[1] = { 0 };
			uint32_t const subpassCount = buffer_len(subpassDesc);

			if (presentation_out->swapchain.imageCount)
			{
				// image layouts: 
				//	-> initial: at beginning of render pass (undefined if don't care)
				//	-> final: at end of render pass (present if used for presentation)
				//	-> subpass: during subpass (color attachment)
				VkImageLayout const subpassLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				//presentation_out->colorImage_attachment = cdrawVkAttachmentDescriptionCtorDefaultColor(
				//	false, true, false, 0);
				//if (cdrawVkImageCreateColorAttachments(presentation_out->colorImage_present, presentation_out->swapchain.imageCount,
				//	"cdraw Present Color[]", logicalDevice, commandPool, presentation_out->queue_graphics,
				//	presentation_out->swapchain.imageDimensions.width, presentation_out->swapchain.imageDimensions.height,
				//	&presentation_out->colorImage_attachment, alloc_opt))
				{
					for (idx = 0; idx < presentation_out->swapchain.imageCount; ++idx)
						attachmentView[idx][attachmentCount] = presentation_out->swapchain.imageView[idx];// presentation_out->colorImage_present[idx].imageView;
					attachmentDesc[attachmentCount] = presentation_out->swapchain.imageAttach;
					colorAttachmentRef[attachmentCount] = cdrawVkAttachmentReferenceCtor(attachmentCount, subpassLayout);
					if (presentation_out->swapchain.imageAttach.samples <= VK_SAMPLE_COUNT_1_BIT)
						resolveAttachmentRef[attachmentCount] = cdrawVkAttachmentReferenceCtorDefault();
					else
						resolveAttachmentRef[attachmentCount] = colorAttachmentRef[attachmentCount];
					++attachmentCount;
				}
				colorAttachmentCount = attachmentCount;
			}

			if (useDepth)
			{
				presentation_out->depthStencilImage_attachment = cdrawVkAttachmentDescriptionCtorDefaultDepthStencil(
					true, false, false);
				if (cdrawVkImageCreateDepthStencilAttachment(&presentation_out->depthStencilImage_present,
					"cdraw Present Depth/Stencil", logicalDevice, commandPool, presentation_out->queue_graphics,
					presentation_out->swapchain.imageDimensions.width, presentation_out->swapchain.imageDimensions.height,
					&presentation_out->depthStencilImage_attachment, alloc_opt))
				{
					// share depth image
					for (idx = 0; idx < presentation_out->swapchain.imageCount; ++idx)
						attachmentView[idx][attachmentCount] = presentation_out->depthStencilImage_present.imageView;
					attachmentDesc[attachmentCount] = presentation_out->depthStencilImage_attachment;
					*depthAttachmentRef = cdrawVkAttachmentReferenceCtor(attachmentCount, presentation_out->depthStencilImage_attachment.finalLayout);
					++attachmentCount;
				}
				else
				{
					*depthAttachmentRef = cdrawVkAttachmentReferenceCtorDefault();
				}
			}

			if (attachmentCount)
			{
				VkSubpassDependency const dependency = cdrawVkSubpassDependencyCtor(VK_SUBPASS_EXTERNAL, 0,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, 0);
				*subpassDesc = cdrawVkSubpassDescriptionCtorDefaultColorDepth(colorAttachmentCount, colorAttachmentRef, resolveAttachmentRef, depthAttachmentRef);
				if (cdrawVkRenderPassCreate(&presentation_out->renderPass_present, "cdrawVkPres.renderPass",
					logicalDevice, attachmentCount, attachmentDesc, subpassCount, subpassDesc, 1, &dependency, alloc_opt))
				{
					for (idx = 0; idx < presentation_out->swapchain.imageCount; ++idx)
					{
						cdrawVkFramebufferCreate(&presentation_out->framebuffer_present[idx], "cdrawVkPres.framebuffer",
							logicalDevice, &presentation_out->renderPass_present, attachmentCount, attachmentView[idx],
							presentation_out->swapchain.imageDimensions.width, presentation_out->swapchain.imageDimensions.height, 1, alloc_opt);
					}
				}
			}
		}
	}

#if CDRAW_DEBUG
	// query pool for performance
	{
		VkQueryType const queryType = (VK_QUERY_TYPE_TIMESTAMP);
		uint32_t const queryCount = buffer_len(presentation_out->timestamp);//(cdrawVkQuery_max * cdrawFramesInFlight_max);
		VkQueryPoolCreateInfo const queryPoolCreateInfo = cdrawVkQueryPoolCreateInfoCtor(queryType, queryCount);

		result = vkCreateQueryPool(logicalDevice->logicalDevice, &queryPoolCreateInfo, alloc_opt, &presentation_out->queryPool_present);
		if (presentation_out->queryPool_present)
		{
			cdraw_assert(result == VK_SUCCESS);
			vkResetQueryPool(logicalDevice->logicalDevice, presentation_out->queryPool_present, 0, queryCount);
			memset(presentation_out->timestamp, 0, sizeof(presentation_out->timestamp));
		}

		presentation_out->frameCount = 0;
		presentation_out->presentCount = 0;
		presentation_out->dt_present_total = 0;
		presentation_out->dt_renderPass_total = 0;
	}
#endif // #if CDRAW_DEBUG

	// set final outputs or clean up
	presentation_out->frame = 0;
	presentation_out->image = 0;
	if (!cdrawVkPresentationValid(presentation_out) || (result != VK_SUCCESS))
	{
		cdrawVkPresentationDestroy(presentation_out, logicalDevice, commandPool, alloc_opt);
		printf("\n Vulkan presentation \"%s\" creation failed.", name);
		return false;
	}
	printf("\n Vulkan presentation \"%s\" created.", name);
	label_copy_safe(presentation_out->name, name);
	cdraw_assert(cdrawVkPresentationValid(presentation_out));
	return true;
}

bool cdrawVkPresentationDestroy(cdrawVkPresentation* const presentation_out,
	cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandPool const* const commandPool, VkAllocationCallbacks const* const alloc_opt)
{
	uint32_t idx;
	failassertret(presentation_out, false);
	if (cdrawVkPresentationUnused(presentation_out))
		return true;

	cdraw_assert(logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice));
	printf("\n Destroying Vulkan presentation \"%s\"...", presentation_out->name);

	vkDeviceWaitIdle(logicalDevice->logicalDevice);
	if (presentation_out->queryPool_present)
	{
		vkDestroyQueryPool(logicalDevice->logicalDevice, presentation_out->queryPool_present, alloc_opt);
		presentation_out->queryPool_present = VK_NULL_HANDLE;
	}
	for (idx = 0; idx < cdrawFramesInFlight_max; ++idx)
	{
		cdrawRendererDestroySemaphore_vk(&presentation_out->semaphore_submit[idx], logicalDevice->logicalDevice, alloc_opt);
		cdrawRendererDestroySemaphore_vk(&presentation_out->semaphore_acquire[idx], logicalDevice->logicalDevice, alloc_opt);
		cdrawRendererDestroyFence_vk(&presentation_out->fence_submit[idx], logicalDevice->logicalDevice, alloc_opt);
		cdrawRendererDestroyFence_vk(&presentation_out->fence_acquire[idx], logicalDevice->logicalDevice, alloc_opt);
	}
	cdrawVkCommandBufferFree(&presentation_out->commandBuffer_present, commandPool, logicalDevice);
	cdrawVkQueueDtor(&presentation_out->queue_present);
	for (idx = 0; idx < cdrawVkSwapchainImage_max; ++idx)
	{
		cdrawVkQueueDtor(&presentation_out->queue_graphics[idx]);
		cdrawVkFramebufferDestroy(&presentation_out->framebuffer_present[idx], logicalDevice, alloc_opt);
	}
	cdrawVkRenderPassDestroy(&presentation_out->renderPass_present, logicalDevice, alloc_opt);
	cdrawVkImageDestroy(&presentation_out->depthStencilImage_present, logicalDevice, alloc_opt);
	cdrawVkSwapchainDestroy(&presentation_out->swapchain, logicalDevice, alloc_opt);
	printf("\n Vulkan presentation \"%s\" destroyed.", presentation_out->name);
	label_init(presentation_out->name);
	cdraw_assert(cdrawVkPresentationUnused(presentation_out));
	return true;
}
