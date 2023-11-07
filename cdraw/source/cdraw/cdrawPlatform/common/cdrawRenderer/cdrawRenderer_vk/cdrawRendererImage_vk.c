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
* cdrawRendererImage_vk.c
* Internal source for Vulkan image management.
*/

#include "_h/cdrawRenderer_vk.h"
#include "_h/cdrawRenderer_vk/cdrawRendererImage_vk.h"

#include <stdio.h>
#include <string.h>

#include "cdraw/cdrawCore/cdrawUtility.h"

/******************************************************************************
* Internal implementations.
******************************************************************************/

/// <summary>
/// Constructor for Vulkan image descriptor.
/// </summary>
/// <param name="image_out">Target image descriptor (non-null).</param>
/// <param name="name">Descriptor name.</param>
/// <param name="image">Vulkan image handle.</param>
/// <param name="imageMem">Vulkan image memory handle.</param>
/// <param name="imageView">Vulkan image view handle.</param>
/// <returns>Success: <paramref name="image_out"/>; Failure: <c>NULL</c>.</returns>
static cdrawVkImage* cdrawVkImageCtor(cdrawVkImage* const image_out,
	label_t const name, VkImage const image, VkDeviceMemory const imageMem, VkImageView const imageView);

/// <summary>
/// Destructor interface for Vulkan image descriptor.
/// </summary>
/// <param name="image_out">Target image descriptor (non-null).</param>
static void cdrawVkImageDtor(cdrawVkImage* const image_out);


/******************************************************************************
* Private implementations.
******************************************************************************/

static VkFormat cdrawVkFormatColor(bool const useHighPrecision)
{
	// of note: may consider blitting color target into swapchain image
	// (instead of drawing swapchain images directly as framebuffer targets)
	//	-> https://www.reddit.com/r/vulkan/comments/p3iy0o/why_use_bgra_instead_of_rgba/

	VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
	if (useHighPrecision)
		format = VK_FORMAT_R16G16B16A16_UNORM;
	return format;
}

static VkFormat cdrawVkFormatDepthStencil(bool const useDepthFloat, bool const useStencil)
{
	// depth/stencil buffer: can change bit size of depth and add _S8_UINT for stencil
	VkFormat format = VK_FORMAT_D16_UNORM;
	if (useStencil)
		format = useDepthFloat ? VK_FORMAT_D32_SFLOAT_S8_UINT : VK_FORMAT_D24_UNORM_S8_UINT;
	else if (useDepthFloat)
		format = VK_FORMAT_D32_SFLOAT;
	return format;
}


/******************************************************************************
* Public implementations.
******************************************************************************/

cdrawVkImage* cdrawVkImageCtor(cdrawVkImage* const image_out,
	label_t const name, VkImage const image, VkDeviceMemory const imageMem, VkImageView const imageView)
{
	failassertret(image_out, NULL);
	failassertret(image && imageMem && imageView, NULL);
	label_copy_safe(image_out->name, name);
	image_out->image = image;
	image_out->imageMem = imageMem;
	image_out->imageView = imageView;
	return image_out;
}

void cdrawVkImageDtor(cdrawVkImage* const image_out)
{
	failassertret(image_out);
	image_out->imageView = VK_NULL_HANDLE;
	image_out->imageMem = VK_NULL_HANDLE;
	image_out->image = VK_NULL_HANDLE;
	label_init(image_out->name);
}

bool cdrawVkImageValid(cdrawVkImage const* const image)
{
	cdraw_assert(image);
	return (image->image && image->imageMem && image->imageView);
}

bool cdrawVkImageUnused(cdrawVkImage const* const image)
{
	cdraw_assert(image);
	return (!image->image && !image->imageMem && !image->imageView);
}


/******************************************************************************
* SECTION: Image management.
******************************************************************************/

VkAttachmentDescription cdrawVkAttachmentDescriptionCtor(
	bool const mayAlias,
	VkFormat const format,
	VkSampleCountFlagBits const samples,
	VkAttachmentLoadOp const loadOp, // color or depth/stencil attachments
	VkAttachmentStoreOp const storeOp, // color or depth/stencil attachments
	VkAttachmentLoadOp const stencilLoadOp, // depth/stencil attachments
	VkAttachmentStoreOp const stencilStoreOp, // depth/stencil attachments
	VkImageLayout const initialLayout,
	VkImageLayout const finalLayout)
{
	VkAttachmentDescription attachmentDescription = { 0 };
	if (mayAlias)
		attachmentDescription.flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
	attachmentDescription.format = format;
	attachmentDescription.samples = samples;
	attachmentDescription.loadOp = loadOp;
	attachmentDescription.storeOp = storeOp;
	attachmentDescription.stencilLoadOp = stencilLoadOp;
	attachmentDescription.stencilStoreOp = stencilStoreOp;
	attachmentDescription.initialLayout = initialLayout;
	attachmentDescription.finalLayout = finalLayout;
	return attachmentDescription;
}

VkAttachmentDescription cdrawVkAttachmentDescriptionCtorDefaultColor(
	bool const mayAlias, bool const clear, bool const useHighPrecision, uint8_t const sampleBit_0to6)
{
	VkFormat const format = cdrawVkFormatColor(useHighPrecision);
	VkSampleCountFlagBits const sampleBits[] = {
		VK_SAMPLE_COUNT_1_BIT,
		VK_SAMPLE_COUNT_2_BIT,
		VK_SAMPLE_COUNT_4_BIT,
		VK_SAMPLE_COUNT_8_BIT,
		VK_SAMPLE_COUNT_16_BIT,
		VK_SAMPLE_COUNT_32_BIT,
		VK_SAMPLE_COUNT_64_BIT,
	}, samples = sampleBits[sampleBit_0to6 % buffer_len(sampleBits)];
	VkAttachmentLoadOp const loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
	VkAttachmentStoreOp const storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	VkAttachmentLoadOp const loadOpStencil = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	VkAttachmentStoreOp const storeOpStencil = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	VkImageLayout const layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	return cdrawVkAttachmentDescriptionCtor(mayAlias, format, samples, loadOp, storeOp, loadOpStencil, storeOpStencil, layout, layout);
}

VkAttachmentDescription cdrawVkAttachmentDescriptionCtorDefaultDepthStencil(
	bool const clear, bool const useStencil, bool const useDepthFloat)
{
	VkFormat const format = cdrawVkFormatDepthStencil(useDepthFloat, useStencil);
	VkSampleCountFlagBits const samples = VK_SAMPLE_COUNT_1_BIT;
	VkAttachmentLoadOp const loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
	VkAttachmentStoreOp const storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	VkAttachmentLoadOp const loadOpStencil = useStencil ? loadOp : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	VkAttachmentStoreOp const storeOpStencil = useStencil ? storeOp : VK_ATTACHMENT_STORE_OP_DONT_CARE;
	VkImageLayout const layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	return cdrawVkAttachmentDescriptionCtor(false, format, samples, loadOp, storeOp, loadOpStencil, storeOpStencil, layout, layout);
}

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

VkComponentMapping cdrawVkComponentMappingCtorDefault()
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

VkImageSubresourceRange cdrawVkImageSubresourceRangeCtorDefaultColor()
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

VkImageViewCreateInfo cdrawVkImageViewCreateInfoCtor(
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

void cdrawVkCmdImageSetLayout(VkImage const image,
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

bool cdrawVkImageCreate(cdrawVkImage* const image_out,
	label_t const name, cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt)
{
	VkResult result = VK_SUCCESS;

	failassertret(image_out && cdrawVkImageUnused(image_out) && logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice), false);
	printf("\n Creating Vulkan image \"%s\"...", name);

	// ****TO-DO

	if (!cdrawVkImageValid(image_out) || (result != VK_SUCCESS))
	{
		cdrawVkImageDestroy(image_out, logicalDevice, alloc_opt);
		printf("\n Vulkan image \"%s\" creation failed.", name);
		return false;
	}
	printf("\n Vulkan image \"%s\" created.", name);
	label_copy_safe(image_out->name, name);
	cdraw_assert(cdrawVkImageValid(image_out));
	return true;
}

bool cdrawVkImageDestroy(cdrawVkImage* const image_out,
	cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt)
{
	failassertret(image_out, false);
	if (cdrawVkImageUnused(image_out))
		return true;

	cdraw_assert(logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice));
	printf("\n Destroying Vulkan image \"%s\"...", image_out->name);
	if (image_out->imageView)
	{
		vkDestroyImageView(logicalDevice->logicalDevice, image_out->imageView, alloc_opt);
		image_out->imageView = VK_NULL_HANDLE;
	}
	if (image_out->imageMem)
	{
		vkFreeMemory(logicalDevice->logicalDevice, image_out->imageMem, alloc_opt);
		image_out->imageMem = VK_NULL_HANDLE;
	}
	if (image_out->image)
	{
		vkDestroyImage(logicalDevice->logicalDevice, image_out->image, alloc_opt);
		image_out->image = VK_NULL_HANDLE;
	}
	printf("\n Vulkan image \"%s\" destroyed.", image_out->name);
	label_init(image_out->name);
	cdraw_assert(cdrawVkImageUnused(image_out));
	return true;
}


bool cdrawVkImageCreateColorAttachments(cdrawVkImage images_out[/*imageCount*/], uint32_t const imageCount,
	label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandPool const* const commandPool, cdrawVkQueue const* const queue,
	uint32_t const width, uint32_t const height,
	VkAttachmentDescription const* const attachmentDescription, VkAllocationCallbacks const* const alloc_opt)
{
	uint32_t idx, count;
	VkExtent3D const imageExtent = { width, height, 1 };
	VkImageUsageFlags const imageUsage = (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
	VkImageTiling imageTiling = (VK_IMAGE_TILING_OPTIMAL);

	VkResult result = VK_SUCCESS;
	failassertret(images_out && imageCount && logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice) && commandPool && cdrawVkCommandPoolValid(commandPool) && queue && cdrawVkQueueValid(queue), false);
	printf("\n Creating Vulkan images \"%s\" (%u color attachments)...", name, imageCount);

	// CREATE IMAGE
	{
		VkImageCreateInfo imageCreateInfo = cdrawVkImageCreateInfoCtor(
			VK_IMAGE_TYPE_2D,
			attachmentDescription->format,
			imageExtent,
			1,
			1,
			attachmentDescription->samples,
			imageTiling, // validate below
			imageUsage,
			VK_SHARING_MODE_EXCLUSIVE,
			1,
			&logicalDevice->queueFamilyIdx_graphics,
			VK_IMAGE_LAYOUT_UNDEFINED);

		// confirm support
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(logicalDevice->physicalDevice.physicalDevice, attachmentDescription->format, &formatProperties);
		cdraw_assert(VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT & (formatProperties.optimalTilingFeatures | formatProperties.linearTilingFeatures));
		if (!(VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT & formatProperties.optimalTilingFeatures))
			imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;

		// create image
		count = 0;
		for (idx = 0; idx < imageCount; ++idx)
		{
			result = vkCreateImage(logicalDevice->logicalDevice, &imageCreateInfo, alloc_opt, &images_out[idx].image);
			if (images_out[idx].image)
			{
				cdraw_assert(result == VK_SUCCESS);
				++count;
			}
		}
		cdraw_assert(count == imageCount);
	}

	// image handle created, allocate memory and create view for depth image
	if (count == imageCount)
	{
		int8_t memTypeIndex;
		VkMemoryRequirements memReq;
		VkMemoryAllocateInfo memAllocInfo;
		vkGetImageMemoryRequirements(logicalDevice->logicalDevice, images_out->image, &memReq);
		count = 0;
		memTypeIndex = cdrawUtilityLowestBit32(memReq.memoryTypeBits);
		if (memTypeIndex >= 0)
		{
			memAllocInfo = cdrawVkMemoryAllocateInfoCtor(memReq.size, memTypeIndex);
			for (idx = 0; idx < imageCount; ++idx)
			{
				result = vkAllocateMemory(logicalDevice->logicalDevice, &memAllocInfo, alloc_opt, &images_out[idx].imageMem);
				if (images_out[idx].imageMem)
				{
					cdraw_assert(result == VK_SUCCESS);
					result = vkBindImageMemory(logicalDevice->logicalDevice, images_out[idx].image, images_out[idx].imageMem, 0);
					cdraw_assert(result == VK_SUCCESS);
					++count;
				}
			}
		}
		cdraw_assert(count == imageCount);
	}

	// memory allocated, set layout and create view
	if (count == imageCount)
	{
		cdrawVkCommandBuffer commandBuffer = { 0 };
		if (cdrawVkCommandBufferAlloc(&commandBuffer, "commandBuffer_color", 1, commandPool, logicalDevice, VK_COMMAND_BUFFER_LEVEL_PRIMARY))
		{
			VkPipelineStageFlags const stage = (VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
			VkImageSubresourceRange const imageSubResourceRange = cdrawVkImageSubresourceRangeCtorDefaultColor();
			VkCommandBufferBeginInfo cmdBeginInfo = cdrawVkCommandBufferBeginInfoCtor(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, NULL);;
			VkSubmitInfo submitInfo;
			VkCommandBuffer cmdBuf = commandBuffer.commandBuffer[0];

			result = vkBeginCommandBuffer(cmdBuf, &cmdBeginInfo);
			cdraw_assert(result == VK_SUCCESS);
			for (idx = 0; idx < imageCount; ++idx)
			{
				cdrawVkCmdImageSetLayout(images_out[idx].image, cmdBuf, logicalDevice->queueFamilyIdx_graphics, 0, 0, stage, stage,
					VK_IMAGE_LAYOUT_UNDEFINED, attachmentDescription->finalLayout, imageSubResourceRange);
			}
			result = vkEndCommandBuffer(cmdBuf);
			cdraw_assert(result == VK_SUCCESS);

			submitInfo = cdrawVkSubmitInfoCtor(0, NULL, NULL, 1, &cmdBuf, 0, NULL);
			result = vkQueueSubmit(queue->queue, 1, &submitInfo, VK_NULL_HANDLE);
			cdraw_assert(result == VK_SUCCESS);
			result = vkQueueWaitIdle(queue->queue);
			cdraw_assert(result == VK_SUCCESS);
			if (cdrawVkCommandBufferFree(&commandBuffer, commandPool, logicalDevice))
			{
				VkComponentMapping const imageComponent = { VK_COMPONENT_SWIZZLE_IDENTITY };
				VkImageViewCreateInfo imageViewCreateInfo = cdrawVkImageViewCreateInfoCtor(
					VK_NULL_HANDLE, // update per-image below
					VK_IMAGE_VIEW_TYPE_2D,
					attachmentDescription->format,
					imageComponent,
					imageSubResourceRange);
				count = 0;
				for (idx = 0; idx < imageCount; ++idx)
				{
					imageViewCreateInfo.image = images_out[idx].image;
					result = vkCreateImageView(logicalDevice->logicalDevice, &imageViewCreateInfo, alloc_opt, &images_out[idx].imageView);
					if (images_out[idx].imageView)
					{
						cdraw_assert(result == VK_SUCCESS);
						++count;
					}
				}
				cdraw_assert(count == imageCount);
			}
		}
	}

	if ((count != imageCount) || (result != VK_SUCCESS))
	{
		for (idx = 0; idx < imageCount; ++idx)
			cdrawVkImageDestroy(&images_out[idx], logicalDevice, alloc_opt);
		printf("\n Vulkan images \"%s\" (%u color attachments) creation failed.", name, imageCount);
		return false;
	}
	printf("\n Vulkan image \"%s\" (%u color attachments) created.", name, imageCount);
	for (idx = 0; idx < imageCount; ++idx)
	{
		label_copy_safe(images_out[idx].name, name);
		cdraw_assert(cdrawVkImageValid(&images_out[idx]));
	}
	return true;
}

bool cdrawVkImageCreateDepthStencilAttachment(cdrawVkImage* const image_out,
	label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandPool const* const commandPool, cdrawVkQueue const* const queue,
	uint32_t const width, uint32_t const height,
	VkAttachmentDescription const* const attachmentDescription, VkAllocationCallbacks const* const alloc_opt)
{
	VkExtent3D const imageExtent = { width, height, 1 }; // depth extent for 2D image must be 1
	VkImageUsageFlags const imageUsage = (VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
	VkImageTiling imageTiling = (VK_IMAGE_TILING_OPTIMAL);
	bool useStencil = false;

	VkResult result = VK_SUCCESS;
	failassertret(image_out && cdrawVkImageUnused(image_out) && logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice) && commandPool && cdrawVkCommandPoolValid(commandPool) && queue && cdrawVkQueueValid(queue), false);
	failassertret(width && height && attachmentDescription, false);
	printf("\n Creating Vulkan image \"%s\" (depth/stencil attachment)...", name);

	// settings
	if (attachmentDescription->stencilLoadOp != VK_ATTACHMENT_LOAD_OP_DONT_CARE)
	{
		useStencil = true;
		cdraw_assert(attachmentDescription->stencilStoreOp != VK_ATTACHMENT_STORE_OP_DONT_CARE);
	}
	cdraw_assert(attachmentDescription->samples == VK_SAMPLE_COUNT_1_BIT);
	cdraw_assert(attachmentDescription->finalLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	
	// CREATE IMAGE
	{
		VkImageCreateInfo imageCreateInfo = cdrawVkImageCreateInfoCtor(
			VK_IMAGE_TYPE_2D,
			attachmentDescription->format,
			imageExtent,
			1,
			1,
			attachmentDescription->samples,
			imageTiling, // validate below
			imageUsage,
			VK_SHARING_MODE_EXCLUSIVE,
			1,
			&logicalDevice->queueFamilyIdx_graphics,
			VK_IMAGE_LAYOUT_UNDEFINED);

		// confirm support
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(logicalDevice->physicalDevice.physicalDevice, attachmentDescription->format, &formatProperties);
		cdraw_assert(VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
			& (formatProperties.optimalTilingFeatures | formatProperties.linearTilingFeatures));
		if (!(VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT & formatProperties.optimalTilingFeatures))
			imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;

		// create image
		result = vkCreateImage(logicalDevice->logicalDevice, &imageCreateInfo, alloc_opt, &image_out->image);
		if (image_out->image)
			cdraw_assert(result == VK_SUCCESS);
	}

	// image handle created, allocate memory and create view for depth image
	if (image_out->image)
	{
		int8_t memTypeIndex;
		VkMemoryRequirements memReq;
		VkMemoryAllocateInfo memAllocInfo;
		vkGetImageMemoryRequirements(logicalDevice->logicalDevice, image_out->image, &memReq);
		memTypeIndex = cdrawUtilityLowestBit32(memReq.memoryTypeBits);
		if (memTypeIndex >= 0)
		{
			memAllocInfo = cdrawVkMemoryAllocateInfoCtor(memReq.size, memTypeIndex);
			result = vkAllocateMemory(logicalDevice->logicalDevice, &memAllocInfo, alloc_opt, &image_out->imageMem);
			if (image_out->imageMem)
				cdraw_assert(result == VK_SUCCESS);
		}
	}

	// memory allocated, set layout and create view
	if (image_out->imageMem)
	{
		cdrawVkCommandBuffer commandBuffer = { 0 };

		result = vkBindImageMemory(logicalDevice->logicalDevice, image_out->image, image_out->imageMem, 0);
		cdraw_assert(result == VK_SUCCESS);
		if (cdrawVkCommandBufferAlloc(&commandBuffer, "commandBuffer_depth_stencil", 1, commandPool, logicalDevice, VK_COMMAND_BUFFER_LEVEL_PRIMARY))
		{
			VkPipelineStageFlags const stage = (VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT);
			VkImageSubresourceRange const imageSubResourceRange = useStencil
				? cdrawVkImageSubresourceRangeCtorDefaultDepthStencil()
				: cdrawVkImageSubresourceRangeCtorDefaultDepth();
			VkCommandBufferBeginInfo cmdBeginInfo = cdrawVkCommandBufferBeginInfoCtor(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, NULL);;
			VkSubmitInfo submitInfo;
			VkCommandBuffer cmdBuf = commandBuffer.commandBuffer[0];

			result = vkBeginCommandBuffer(cmdBuf, &cmdBeginInfo);
			cdraw_assert(result == VK_SUCCESS);

			cdrawVkCmdImageSetLayout(image_out->image, cmdBuf, logicalDevice->queueFamilyIdx_graphics, 0, 0, stage, stage,
				VK_IMAGE_LAYOUT_UNDEFINED, attachmentDescription->finalLayout, imageSubResourceRange);
	
			result = vkEndCommandBuffer(cmdBuf);
			cdraw_assert(result == VK_SUCCESS);

			submitInfo = cdrawVkSubmitInfoCtor(0, NULL, NULL, 1, &cmdBuf, 0, NULL);
			result = vkQueueSubmit(queue->queue, 1, &submitInfo, VK_NULL_HANDLE);
			cdraw_assert(result == VK_SUCCESS);
			result = vkQueueWaitIdle(queue->queue);
			cdraw_assert(result == VK_SUCCESS);
			if (cdrawVkCommandBufferFree(&commandBuffer, commandPool, logicalDevice))
			{
				VkComponentMapping const imageComponent = { VK_COMPONENT_SWIZZLE_IDENTITY };
				VkImageViewCreateInfo const imageViewCreateInfo = cdrawVkImageViewCreateInfoCtor(
					image_out->image,
					VK_IMAGE_VIEW_TYPE_2D,
					attachmentDescription->format,
					imageComponent,
					imageSubResourceRange);
				result = vkCreateImageView(logicalDevice->logicalDevice, &imageViewCreateInfo, alloc_opt, &image_out->imageView);
				if (image_out->imageView)
					cdraw_assert(result == VK_SUCCESS);
			}
		}
	}

	// set final outputs or clean up
	if (!cdrawVkImageValid(image_out) || (result != VK_SUCCESS))
	{
		cdrawVkImageDestroy(image_out, logicalDevice, alloc_opt);
		printf("\n Vulkan image \"%s\" creation failed.", name);
		return false;
	}
	printf("\n Vulkan image \"%s\" created.", name);
	label_copy_safe(image_out->name, name);
	cdraw_assert(cdrawVkImageValid(image_out));
	return true;
}