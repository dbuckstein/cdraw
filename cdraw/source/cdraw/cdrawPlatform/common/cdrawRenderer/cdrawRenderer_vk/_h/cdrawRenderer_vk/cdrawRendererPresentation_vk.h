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
* cdrawRendererPresentation_vk.h
* Facade interface for Vulkan presentation management.
*/

#if ((!defined _CDRAW_RENDERER_PRESENTATION_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))
#define _CDRAW_RENDERER_PRESENTATION_VK_H_

#include "cdrawRendererFramebuffer_vk.h"


enum
{
	cdrawVkSwapchainImage_max = 4,	// convenience: max number of swapchain images
};


/// <summary>
/// Vulkan surface descriptor.
/// </summary>
typedef struct cdrawVkSurface
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Vulkan surface handle.
	/// </summary>
	VkSurfaceKHR surface;
} cdrawVkSurface;


/// <summary>
/// Vulkan swapchain descriptor.
/// </summary>
typedef struct cdrawVkSwapchain
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Vulkan swapchain.
	/// </summary>
	VkSwapchainKHR swapchain;

	/// <summary>
	/// Color image view resources associated with images owned by the swapchain.
	/// Image handles themselves are not needed as they are owned by the swapchain; can query later if needed.
	/// </summary>
	VkImageView imageView[cdrawVkSwapchainImage_max];

	/// <summary>
	/// Number of color images owned by swapchain.
	/// </summary>
	uint32_t imageCount;

	/// <summary>
	/// Image dimensions.
	/// </summary>
	VkExtent2D imageDimensions;

	/// <summary>
	/// Description of images as needed for render pass and framebuffer attachments.
	/// </summary>
	VkAttachmentDescription imageAttach;
} cdrawVkSwapchain;


/// <summary>
/// Collection of Vulkan handles and data related to presentation.
/// </summary>
typedef struct cdrawVkPresentation
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Vulkan swapchain.
	/// </summary>
	cdrawVkSwapchain swapchain;

	/// <summary>
	/// Vulkan graphics/presentation queue.
	/// Should have one for each swapchain image to avoid locking.
	/// </summary>
	cdrawVkQueue queue_graphics[cdrawVkSwapchainImage_max];

	/// <summary>
	/// Vulkan command buffers for presentation.
	/// Should have one for each image to be processed.
	/// </summary>
	cdrawVkCommandBuffer commandBuffer_present;

	/// <summary>
	/// Presentation fences.
	/// </summary>
	VkFence fence[cdrawVkSwapchainImage_max];

	/// <summary>
	/// TEMPORARY STUFF - testing swapchain usage for now.
	/// </summary>
	struct
	{
		/// <summary>
		/// Description of presentation color images for use as framebuffer attachments.
		/// </summary>
		VkAttachmentDescription colorImage_attachment;

		/// <summary>
		/// Description of presentation depth/stencil image for use as framebuffer attachment.
		/// </summary>
		VkAttachmentDescription depthStencilImage_attachment;

		/// <summary>
		/// Color images for presentation.
		/// </summary>
		cdrawVkImage colorImage_present[cdrawVkSwapchainImage_max];

		/// <summary>
		/// Depth/stencil image for presentation.
		/// </summary>
		cdrawVkImage depthStencilImage_present;

		/// <summary>
		/// Vulkan render pass for presentation.
		/// </summary>
		cdrawVkRenderPass renderPass_present;

		/// <summary>
		/// Vulkan framebuffer for presentation.
		/// </summary>
		cdrawVkFramebuffer framebuffer_present[cdrawVkSwapchainImage_max];
	};
} cdrawVkPresentation;


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Indicate whether descriptor is valid (set up correctly) and should not be modified.
	/// </summary>
	/// <param name="surface">Descriptor (non-null).</param>
	/// <returns>True if valid.</returns>
	bool cdrawVkSurfaceValid(cdrawVkSurface const* const surface);

	/// <summary>
	/// Indicate whether descriptor is unused (not set up) and can be modified.
	/// </summary>
	/// <param name="surface">Descriptor (non-null).</param>
	/// <returns>True if unused.</returns>
	bool cdrawVkSurfaceUnused(cdrawVkSurface const* const surface);

	/// <summary>
	/// Create Vulkan surface descriptor.
	/// </summary>
	/// <param name="surface_out">Target descriptor (non-null and unused).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="instance">Instance descriptor (non-null and valid).</param>
	/// <param name="p_data">Pointer to platform data (non-null).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkSurfaceCreate(cdrawVkSurface* const surface_out,
		label_t const name, cdrawVkInstance const* const instance, ptrk_t const p_data, VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Destroy Vulkan surface descriptor.
	/// </summary>
	/// <param name="surface_out">Target descriptor (non-null and valid).</param>
	/// <param name="instance">Instance descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if destroyed.</returns>
	bool cdrawVkSurfaceDestroy(cdrawVkSurface* const surface_out,
		cdrawVkInstance const* const instance, VkAllocationCallbacks const* const alloc_opt);


	/// <summary>
	/// Indicate whether descriptor is valid (set up correctly) and should not be modified.
	/// </summary>
	/// <param name="swapchain">Descriptor (non-null).</param>
	/// <returns>True if valid.</returns>
	bool cdrawVkSwapchainValid(cdrawVkSwapchain const* const swapchain);

	/// <summary>
	/// Indicate whether descriptor is unused (not set up) and can be modified.
	/// </summary>
	/// <param name="swapchain">Descriptor (non-null).</param>
	/// <returns>True if unused.</returns>
	bool cdrawVkSwapchainUnused(cdrawVkSwapchain const* const swapchain);

	/// <summary>
	/// Create Vulkan swapchain descriptor.
	/// </summary>
	/// <param name="swapchain_out">Target descriptor (non-null and unused).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="surface">Surface descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkSwapchainCreate(cdrawVkSwapchain* const swapchain_out,
		label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkSurface const* const surface,
		VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Destroy Vulkan swapchain descriptor.
	/// </summary>
	/// <param name="swapchain_out">Target descriptor (non-null and valid).</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if destroyed.</returns>
	bool cdrawVkSwapchainDestroy(cdrawVkSwapchain* const swapchain_out,
		cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt);


	/// <summary>
	/// Indicate whether descriptor is valid (set up correctly) and should not be modified.
	/// </summary>
	/// <param name="presentation">Descriptor (non-null).</param>
	/// <returns>True if valid.</returns>
	bool cdrawVkPresentationValid(cdrawVkPresentation const* const presentation);

	/// <summary>
	/// Indicate whether descriptor is unused (not set up) and can be modified.
	/// </summary>
	/// <param name="presentation">Descriptor (non-null).</param>
	/// <returns>True if unused.</returns>
	bool cdrawVkPresentationUnused(cdrawVkPresentation const* const presentation);

	/// <summary>
	/// Create Vulkan presentation descriptor.
	/// </summary>
	/// <param name="presentation_out">Target descriptor (non-null and unused).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="surface">Surface descriptor (non-null and valid).</param>
	/// <param name="commandPool">Command pool descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkPresentationCreate(cdrawVkPresentation* const presentation_out,
		label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkSurface const* const surface, cdrawVkCommandPool const* const commandPool, VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Destroy Vulkan presentation descriptor.
	/// </summary>
	/// <param name="presentation_out">Target descriptor (non-null and valid).</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="commandPool">Command pool descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if destroyed.</returns>
	bool cdrawVkPresentationDestroy(cdrawVkPresentation* const presentation_out,
		cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandPool const* const commandPool, VkAllocationCallbacks const* const alloc_opt);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if ((!defined _CDRAW_RENDERER_PRESENTATION_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))