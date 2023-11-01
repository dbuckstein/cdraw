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
	cdrawVkImagePresent_max = 4,	// convenience: max number of presentation images
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
	VkSwapchainKHR swapchain;

	/// <summary>
	/// Vulkan graphics/presentation queue.
	/// Should have one for each swapchain image to avoid locking.
	/// </summary>
	cdrawVkQueue queue_graphics[cdrawVkImagePresent_max];

	/// <summary>
	/// Color image view resources associated with swapchain images.
	/// Images themselves are not needed as they are owned by the swapchain; can query later.
	/// </summary>
	VkImageView colorImage_present[cdrawVkImagePresent_max];

	/// <summary>
	/// Description of presentation color images for use as framebuffer attachments.
	/// </summary>
	VkAttachmentDescription colorImage_attachment;

	/// <summary>
	/// Number of color images generated for swapchain.
	/// </summary>
	uint32_t colorImageCount;

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
	cdrawVkFramebuffer framebuffer_present[cdrawVkImagePresent_max];

	/// <summary>
	/// Vulkan command buffers for presentation.
	/// Should have one for each image to be processed.
	/// </summary>
	cdrawVkCommandBuffer commandBuffer_presentation[cdrawVkImagePresent_max];
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
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if destroyed.</returns>
	bool cdrawVkPresentationDestroy(cdrawVkPresentation* const presentation_out,
		cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if ((!defined _CDRAW_RENDERER_PRESENTATION_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))