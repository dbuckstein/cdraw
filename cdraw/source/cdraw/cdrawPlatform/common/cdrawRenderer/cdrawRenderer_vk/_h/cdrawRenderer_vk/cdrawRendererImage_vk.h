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

	/// <summary>
	/// Details for use as framebuffer attachment.
	/// </summary>
	VkAttachmentDescription imageAttach;
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
	/// <returns>Attachment description structure.</returns>
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
	/// Create color attachment image and dependencies.
	/// </summary>
	/// <param name="image_out">Target image descriptor (non-null and unused).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="commandPool">Command pool descriptor (non-null and valid).</param>
	/// <param name="queue">Queue descriptor (non-null and valid).</param>
	/// <param name="width">Image width.</param>
	/// <param name="height">Image height.</param>
	/// <param name="useHighPrecision">True to use higher precision storage; False to use lowest precision storage.</param>
	/// <param name="attachLoadOp">Load operation if used as attachment.</param>
	/// <param name="attachStoreOp">Store operation if used as attachment.</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkImageCreateColorAttachment(cdrawVkImage* const image_out,
		label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandPool const* const commandPool, cdrawVkQueue const* const queue,
		uint32_t const width, uint32_t const height, bool const useHighPrecision,
		VkAttachmentLoadOp const attachLoadOp, VkAttachmentStoreOp const attachStoreOp,
		VkAllocationCallbacks const* const alloc_opt);

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
	/// <param name="useDepthFloat">True to use floating point storage for depth component; False to use medium-precision integer storage.</param>
	/// <param name="useStencil">True to use stencil component; False to disable stencil.</param>
	/// <param name="attachLoadOp">Load operation if used as attachment.</param>
	/// <param name="attachStoreOp">Store operation if used as attachment.</param>
	/// <param name="attachStencilLoadOp">Load operation for stencil component if used as attachment.</param>
	/// <param name="attachStencilStoreOp">Store operation for stencil component if used as attachment.</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkImageCreateDepthStencilAttachment(cdrawVkImage* const image_out,
		label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandPool const* const commandPool, cdrawVkQueue const* const queue,
		uint32_t const width, uint32_t const height, bool const useDepthFloat, bool const useStencil,
		VkAttachmentLoadOp const attachLoadOp, VkAttachmentStoreOp const attachStoreOp,
		VkAttachmentLoadOp const attachStencilLoadOp, VkAttachmentStoreOp const attachStencilStoreOp,
		VkAllocationCallbacks const* const alloc_opt);

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