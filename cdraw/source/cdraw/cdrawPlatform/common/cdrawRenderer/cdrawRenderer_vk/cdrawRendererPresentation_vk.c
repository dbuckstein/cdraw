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
/// Constructor for Vulkan presentation descriptor.
/// </summary>
/// <param name="presentation_out">Target presentation descriptor (non-null).</param>
/// <param name="name">Descriptor name.</param>
/// <param name="swapchain">Vulkan swapchain handle.</param>
/// <returns>Success: <paramref name="presentation_out"/>; Failure: <c>NULL</c>.</returns>
static cdrawVkPresentation* cdrawVkPresentationCtor(cdrawVkPresentation* const presentation_out,
	label_t const name, VkSwapchainKHR const swapchain);

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
	label_t const name, VkSwapchainKHR const swapchain)
{
	failassertret(presentation_out && cdrawVkPresentationUnused(presentation_out), NULL);
	label_copy_safe(presentation_out->name, name);
	presentation_out->swapchain = swapchain;
	return presentation_out;
}

void cdrawVkPresentationDtor(cdrawVkPresentation* const presentation_out)
{
	failassertret(presentation_out);
	label_init(presentation_out->name);
	presentation_out->swapchain = VK_NULL_HANDLE;
}

bool cdrawVkPresentationValid(cdrawVkPresentation const* const presentation)
{
	uint32_t idx;
	cdraw_assert(presentation);
	if (!presentation->swapchain || !presentation->colorImageCount
		|| !cdrawVkRenderPassValid(&presentation->renderPass_present))
		return false;
	for (idx = 0; idx < presentation->colorImageCount; ++idx)
		if (!cdrawVkQueueValid(&presentation->queue_graphics[idx])
			|| !cdrawVkFramebufferValid(&presentation->framebuffer_present[idx]))
			return false;
	return true;
}

bool cdrawVkPresentationUnused(cdrawVkPresentation const* const presentation)
{
	uint32_t idx;
	cdraw_assert(presentation);
	if (presentation->swapchain || presentation->colorImageCount
		|| !cdrawVkRenderPassUnused(&presentation->renderPass_present))
		return false;
	for (idx = 0; idx < presentation->colorImageCount; ++idx)
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
	VkSampleCountFlagBits const colorSampleCount = VK_SAMPLE_COUNT_1_BIT;

	uint32_t nSwapchainImage = 0;
	VkImage* pSwapchainImage = NULL;

	// queue families
	bool const logicalDeviceValid = logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice);
	uint32_t const queueFamilyIdx_graphics = logicalDeviceValid ? logicalDevice->queueFamilyIdx_graphics : uint32_invalid;
	uint32_t const queueFamilyIndices[] = {
		queueFamilyIdx_graphics,
	};
	VkPhysicalDevice const physicalDevice = logicalDeviceValid ? logicalDevice->physicalDevice.physicalDevice : NULL;

	VkResult result = VK_SUCCESS;
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
		VkQueue queue_graphics = VK_NULL_HANDLE;

		cdraw_assert(result == VK_SUCCESS);

		// get device queue for presentation
		for (idx = 0; idx < cdrawVkImagePresent_max; ++idx)
		{
			vkGetDeviceQueue(logicalDevice->logicalDevice, queueFamilyIdx_graphics, idx, &queue_graphics);
			cdraw_assert(queue_graphics);
			cdrawVkQueueCtor(&presentation_out->queue_graphics[idx], "cdrawVkPres.queue_graphics", queue_graphics);
		}
	}

	// FINAL CREATE SWAPCHAIN
	if (cdrawVkQueueValid(&presentation_out->queue_graphics[0]))
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
		result = vkCreateSwapchainKHR(logicalDevice->logicalDevice, &swapchainCreateInfo, alloc_opt, &presentation_out->swapchain);
		if (presentation_out->swapchain)
			cdraw_assert(result == VK_SUCCESS);
	}

	// next, get images
	if (presentation_out->swapchain)
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

		// get count and allocate for color images
		result = vkGetSwapchainImagesKHR(logicalDevice->logicalDevice, presentation_out->swapchain, &nSwapchainImage, NULL);
		if (result == VK_SUCCESS && nSwapchainImage)
		{
			pSwapchainImage = (VkImage*)malloc(sizeof(VkImage) * nSwapchainImage);
			if (pSwapchainImage)
			{
				result = vkGetSwapchainImagesKHR(logicalDevice->logicalDevice, presentation_out->swapchain, &nSwapchainImage, pSwapchainImage);
				cdraw_assert(result == VK_SUCCESS);

				// create image views
				cdraw_assert(nSwapchainImage <= buffer_len(presentation_out->colorImage_present));
				for (idx = 0; idx < nSwapchainImage; ++idx)
				{
					cdraw_assert(pSwapchainImage[idx]);
					imageViewCreateInfo.image = pSwapchainImage[idx];
					result = vkCreateImageView(logicalDevice->logicalDevice, &imageViewCreateInfo, alloc_opt, &presentation_out->colorImage_present[idx]);
					if (presentation_out->colorImage_present[idx])
						cdraw_assert(result == VK_SUCCESS);
					else
						break;
				}
				createdImageViews = (idx == nSwapchainImage);
				cdraw_assert(createdImageViews);

				free(pSwapchainImage);
				pSwapchainImage = NULL;
			}
		}

		// default depth image (if requested) and attachment info, render pass and framebuffer
		{
			bool const useDepth = true, useDepthFloat = false, useStencil = true;
			uint32_t attachmentCount = 0, colorAttachmentCount = 0;
			VkAttachmentDescription attachmentDesc[2] = { 0 };
			VkImageView attachmentView[cdrawVkImagePresent_max][buffer_len(attachmentDesc)] = { VK_NULL_HANDLE };

			VkAttachmentReference colorAttachmentRef[cdrawVkFramebufferColorAttach_max] = { 0 };
			VkAttachmentReference resolveAttachmentRef[buffer_len(colorAttachmentRef)] = { 0 };
			VkAttachmentReference depthAttachmentRef[1] = { 0 };
			VkSubpassDescription subpassDesc[1] = { 0 };
			uint32_t const subpassCount = buffer_len(subpassDesc);

			if (createdImageViews)
			{
				presentation_out->colorImageCount = nSwapchainImage;
				presentation_out->colorImage_attachment = cdrawVkAttachmentDescriptionCtor(
					false, imageViewCreateInfo.format, colorSampleCount,
					VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
				for (idx = 0; idx < nSwapchainImage; ++idx)
					attachmentView[idx][attachmentCount] = presentation_out->colorImage_present[idx];
				attachmentDesc[attachmentCount] = presentation_out->colorImage_attachment;
				colorAttachmentRef[attachmentCount] = cdrawVkAttachmentReferenceCtor(attachmentCount, presentation_out->colorImage_attachment.initialLayout);
				if (presentation_out->colorImage_attachment.samples <= VK_SAMPLE_COUNT_1_BIT)
					resolveAttachmentRef[attachmentCount] = cdrawVkAttachmentReferenceCtorDefault();
				else
					resolveAttachmentRef[attachmentCount] = colorAttachmentRef[attachmentCount];
				++attachmentCount;
				colorAttachmentCount = attachmentCount;
			}

			if (useDepth)
			{
				if (cdrawVkImageCreateDepthStencilAttachment(&presentation_out->depthStencilImage_present,
					"cdraw Present Depth/Stencil", logicalDevice, commandPool, &presentation_out->queue_graphics[0],
					surfaceCapabilities.currentExtent.width, surfaceCapabilities.currentExtent.height, useDepthFloat, useStencil,
					VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, alloc_opt))
				{
					// share depth image
					attachmentView[idx][attachmentCount] = presentation_out->depthStencilImage_present.imageView;
					attachmentDesc[attachmentCount] = presentation_out->depthStencilImage_present.imageAttach;
					*depthAttachmentRef = cdrawVkAttachmentReferenceCtor(attachmentCount, presentation_out->depthStencilImage_present.imageAttach.initialLayout);
					++attachmentCount;
				}
				else
				{
					*depthAttachmentRef = cdrawVkAttachmentReferenceCtorDefault();
				}
			}

			if (attachmentCount)
			{
				*subpassDesc = cdrawVkSubpassDescriptionCtorDefaultColorDepth(colorAttachmentCount, colorAttachmentRef, resolveAttachmentRef, depthAttachmentRef);
				if (cdrawVkRenderPassCreate(&presentation_out->renderPass_present, "cdrawVkPres.renderPass",
					logicalDevice, attachmentCount, attachmentDesc, subpassCount, subpassDesc, alloc_opt))
				{
					for (idx = 0; idx < nSwapchainImage; ++idx)
					{
						cdrawVkFramebufferCreate(&presentation_out->framebuffer_present[idx], "cdrawVkPres.framebuffer",
							logicalDevice, &presentation_out->renderPass_present, attachmentCount, attachmentView[idx], alloc_opt);
					}
				}
			}
		}
	}

	// set final outputs or clean up
	if (!cdrawVkPresentationValid(presentation_out) || (result != VK_SUCCESS))
	{
		cdrawVkPresentationDestroy(presentation_out, logicalDevice, alloc_opt);
		printf("\n Vulkan presentation \"%s\" creation failed.", name);
		return false;
	}
	printf("\n Vulkan presentation \"%s\" created.", name);
	label_copy_safe(presentation_out->name, name);
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

	for (idx = 0; idx < cdrawVkImagePresent_max; ++idx)
	{
		cdrawVkFramebufferDestroy(&presentation_out->framebuffer_present[idx], logicalDevice, alloc_opt);
		if (presentation_out->colorImage_present[idx])
		{
			vkDestroyImageView(logicalDevice->logicalDevice, presentation_out->colorImage_present[idx], alloc_opt);
			presentation_out->colorImage_present[idx] = VK_NULL_HANDLE;
		}
		cdrawVkQueueDtor(&presentation_out->queue_graphics[idx]);
	}
	cdrawVkRenderPassDestroy(&presentation_out->renderPass_present, logicalDevice, alloc_opt);
	cdrawVkImageDestroy(&presentation_out->depthStencilImage_present, logicalDevice, alloc_opt);
	if (presentation_out->swapchain)
	{
		vkDestroySwapchainKHR(logicalDevice->logicalDevice, presentation_out->swapchain, alloc_opt);
		presentation_out->swapchain = VK_NULL_HANDLE;
	}
	presentation_out->colorImageCount = 0;
	printf("\n Vulkan presentation \"%s\" destroyed.", presentation_out->name);
	label_init(presentation_out->name);
	return true;
}
