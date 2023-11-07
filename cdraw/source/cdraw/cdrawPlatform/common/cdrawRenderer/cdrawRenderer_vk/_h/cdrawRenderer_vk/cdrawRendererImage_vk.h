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
* cdrawRendererImage_vk.h
* Facade interface for Vulkan image management.
*/

#if ((!defined _CDRAW_RENDERER_IMAGE_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))
#define _CDRAW_RENDERER_IMAGE_VK_H_

#include "cdrawRendererDevice_vk.h"


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


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Indicate whether descriptor is valid (set up correctly) and should not be modified.
	/// </summary>
	/// <param name="image">Image descriptor (non-null).</param>
	/// <returns>True if valid.</returns>
	bool cdrawVkImageValid(cdrawVkImage const* const image);

	/// <summary>
	/// Indicate whether descriptor is unused (not set up) and can be modified.
	/// </summary>
	/// <param name="image">Image descriptor (non-null).</param>
	/// <returns>True if unused.</returns>
	bool cdrawVkImageUnused(cdrawVkImage const* const image);

	/// <summary>
	/// Create image and dependencies.
	/// </summary>
	/// <param name="image_out">Target image descriptor (non-null and unused).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkImageCreate(cdrawVkImage* const image_out,
		label_t const name, cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Destroy image and dependencies.
	/// </summary>
	/// <param name="image_out">Target image descriptor (non-null and valid).</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if destroyed.</returns>
	bool cdrawVkImageDestroy(cdrawVkImage* const image_out,
		cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Create descriptor for using image as framebuffer attachment.
	/// </summary>
	/// <param name="mayAlias">Data may be aliased in memory.</param>
	/// <param name="format">Image format.</param>
	/// <param name="samples">Sample count (can resolve if multiple).</param>
	/// <param name="loadOp">Load operation for color or depth/stencil attachment.</param>
	/// <param name="storeOp">Store operation for color or depth/stencil attachment.</param>
	/// <param name="stencilLoadOp">Load operation for stencil component of depth/stencil attachment.</param>
	/// <param name="stencilStoreOp">Store operation for stencil component of depth/stencil attachment.</param>
	/// <param name="initialLayout">Layout before render pass.</param>
	/// <param name="finalLayout">Layout after render pass.</param>
	/// <returns>Vulkan attachment description structure.</returns>
	VkAttachmentDescription cdrawVkAttachmentDescriptionCtor(
		bool const mayAlias,
		VkFormat const format,
		VkSampleCountFlagBits const samples,
		VkAttachmentLoadOp const loadOp,
		VkAttachmentStoreOp const storeOp,
		VkAttachmentLoadOp const stencilLoadOp,
		VkAttachmentStoreOp const stencilStoreOp,
		VkImageLayout const initialLayout,
		VkImageLayout const finalLayout);

	/// <summary>
	/// Create descriptor for using image as default color framebuffer attachment.
	/// </summary>
	/// <param name="mayAlias">Data may be aliased in memory.</param>
	/// <param name="clear">True if attachment should clear at the beginning of a render pass.</param>
	/// <param name="useHighPrecision">True if using higher precision storage.</param>
	/// <param name="sampleBit_0to6">Multi-sample bit (0 represents 1 sample, 6 represents 64 samples).</param>
	/// <returns>Vulkan attachment description structure.</returns>
	VkAttachmentDescription cdrawVkAttachmentDescriptionCtorDefaultColor(
		bool const mayAlias, bool const clear, bool const useHighPrecision, uint8_t const sampleBit_0to6);

	/// <summary>
	/// Create descriptor for using image as default depth/stencil framebuffer attachment.
	/// </summary>
	/// <param name="clear">True if attachment should clear at the beginning of a render pass.</param>
	/// <param name="useStencil">True if using stencil component.</param>
	/// <param name="useDepthFloat">True if using float storage for depth component.</param>
	/// <returns>Vulkan attachment description structure.</returns>
	VkAttachmentDescription cdrawVkAttachmentDescriptionCtorDefaultDepthStencil(
		bool const clear, bool const useStencil, bool const useDepthFloat);

	/// <summary>
	/// Create set of color attachment images and dependencies.
	/// </summary>
	/// <param name="images_out">Target image descriptor array (each non-null and unused).</param>
	/// <param name="imageCount">Number of images to be prepared.</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="commandPool">Command pool descriptor (non-null and valid).</param>
	/// <param name="queue">Queue descriptor (non-null and valid).</param>
	/// <param name="width">Image width.</param>
	/// <param name="height">Image height.</param>
	/// <param name="attachmentDescription">Pointer to attachment description for render passes.</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkImageCreateColorAttachments(cdrawVkImage images_out[/*imageCount*/], uint32_t const imageCount,
		label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandPool const* const commandPool, cdrawVkQueue const* const queue,
		uint32_t const width, uint32_t const height,
		VkAttachmentDescription const* const attachmentDescription, VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Create depth/stencil attachment image and dependencies.
	/// </summary>
	/// <param name="image_out">Target image descriptor (non-null and unused).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="commandPool">Command pool descriptor (non-null and valid).</param>
	/// <param name="queue">Queue descriptor (non-null and valid).</param>
	/// <param name="width">Image width.</param>
	/// <param name="height">Image height.</param>
	/// <param name="attachmentDescription">Pointer to attachment description for render passes.</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkImageCreateDepthStencilAttachment(cdrawVkImage* const image_out,
		label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandPool const* const commandPool, cdrawVkQueue const* const queue,
		uint32_t const width, uint32_t const height,
		VkAttachmentDescription const* const attachmentDescription, VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Set image layout via memory barrier.
	/// </summary>
	/// <param name="image">Vulkan image handle.</param>
	/// <param name="cmdBuf">Vulkan command buffer handle.</param>
	/// <param name="queueFamilyIndex">Queue family index.</param>
	/// <param name="srcAccessMask">Source access mask.</param>
	/// <param name="dstAccessMask">Destination access mask.</param>
	/// <param name="srcStageMask">Source stage mask.</param>
	/// <param name="dstStageMask">Destination stage mask.</param>
	/// <param name="oldLayout">Old layout.</param>
	/// <param name="newLayout">New layout.</param>
	/// <param name="imageSubResourceRange">Subresource range.</param>
	void cdrawVkCmdImageSetLayout(VkImage const image,
		VkCommandBuffer const cmdBuf,
		uint32_t const queueFamilyIndex,
		VkAccessFlags const srcAccessMask, VkAccessFlags const dstAccessMask,
		VkPipelineStageFlags const srcStageMask, VkPipelineStageFlags const dstStageMask,
		VkImageLayout const oldLayout, VkImageLayout const newLayout,
		VkImageSubresourceRange const imageSubResourceRange);

	/// <summary>
	/// Construct image view create info.
	/// </summary>
	/// <param name="image">Vulkan image handle.</param>
	/// <param name="viewType">View type flag.</param>
	/// <param name="format">Image format.</param>
	/// <param name="components">Component mapping.</param>
	/// <param name="subresourceRange">Underlying resource details.</param>
	/// <returns>Image view create info structure.</returns>
	VkImageViewCreateInfo cdrawVkImageViewCreateInfoCtor(
		VkImage const image,
		VkImageViewType const viewType,
		VkFormat const format,
		VkComponentMapping const components,
		VkImageSubresourceRange const subresourceRange);

	/// <summary>
	/// Get default component mapping structure (RGBA).
	/// </summary>
	/// <returns>Component mapping structure.</returns>
	VkComponentMapping cdrawVkComponentMappingCtorDefault();

	/// <summary>
	/// Get default subresource range structure for color image.
	/// </summary>
	/// <returns>Subresource range structure.</returns>
	VkImageSubresourceRange cdrawVkImageSubresourceRangeCtorDefaultColor();


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if ((!defined _CDRAW_RENDERER_IMAGE_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))