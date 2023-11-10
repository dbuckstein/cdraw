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
* cdrawRendererFramebuffer_vk.h
* Facade interface for Vulkan framebuffer management.
*/

#if ((!defined _CDRAW_RENDERER_FRAMEBUFFER_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))
#define _CDRAW_RENDERER_FRAMEBUFFER_VK_H_

#include "cdrawRendererImage_vk.h"


enum
{
	cdrawVkFramebufferAttach_max = 8,	// convenience: desired maximum attachment count
	cdrawVkFramebufferInputAttach_max = cdrawVkFramebufferAttach_max,
	cdrawVkFramebufferColorAttach_max = cdrawVkFramebufferAttach_max,
};


/// <summary>
/// Vulkan render pass and dependencies descriptor.
/// </summary>
typedef struct cdrawVkRenderPass
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Vulkan render pass handle.
	/// </summary>
	VkRenderPass renderPass;
} cdrawVkRenderPass;


/// <summary>
/// Vulkan framebuffer and dependencies descriptor.
/// Decoupled from renderpass to allow multi-buffering (e.g. one per swapchain image).
/// </summary>
typedef struct cdrawVkFramebuffer
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Vulkan framebuffer handle.
	/// </summary>
	VkFramebuffer framebuffer;

	/// <summary>
	/// Default framebuffer affected region.
	/// </summary>
	VkRect2D region;
} cdrawVkFramebuffer;


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Indicate whether descriptor is valid (set up correctly) and should not be modified.
	/// </summary>
	/// <param name="renderPass">Render pass descriptor (non-null).</param>
	/// <returns>True if valid.</returns>
	bool cdrawVkRenderPassValid(cdrawVkRenderPass const* const renderPass);

	/// <summary>
	/// Indicate whether descriptor is unused (not set up) and can be modified.
	/// </summary>
	/// <param name="renderPass">Render pass descriptor (non-null).</param>
	/// <returns>True if unused.</returns>
	bool cdrawVkRenderPassUnused(cdrawVkRenderPass const* const renderPass);

	/// <summary>
	/// Create render pass and dependencies.
	/// </summary>
	/// <param name="renderPass_out">Target render pass descriptor (non-null and unused).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="attachmentCount">Number of attachments to describe.</param>
	/// <param name="attachment">Image references representing attachments (description component).</param>
	/// <param name="subpassCount">Number of subpasses to describe.</param>
	/// <param name="subpass">Subpass descriptions.</param>
	/// <param name="dependencyCount">Number of subpass dependencies to describe.</param>
	/// <param name="depencency">Subpass depencency descriptions.</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkRenderPassCreate(cdrawVkRenderPass* const renderPass_out,
		label_t const name, cdrawVkLogicalDevice const* const logicalDevice,
		uint32_t const attachmentCount, VkAttachmentDescription const attachment[/*attachmentCount*/],
		uint32_t const subpassCount, VkSubpassDescription const subpass[/*subpassCount*/],
		uint32_t const dependencyCount, VkSubpassDependency const dependency[/*dependencyCount*/],
		VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Destroy renderPass and dependencies.
	/// </summary>
	/// <param name="renderPass_out">Target render pass descriptor (non-null and valid).</param>
	/// <param name="device">Logical device descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if destroyed.</returns>
	bool cdrawVkRenderPassDestroy(cdrawVkRenderPass* const renderPass_out,
		cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt);


	/// <summary>
	/// Indicate whether descriptor is valid (set up correctly) and should not be modified.
	/// </summary>
	/// <param name="framebuffer">Framebuffer descriptor (non-null).</param>
	/// <returns>True if valid.</returns>
	bool cdrawVkFramebufferValid(cdrawVkFramebuffer const* const framebuffer);

	/// <summary>
	/// Indicate whether descriptor is unused (not set up) and can be modified.
	/// </summary>
	/// <param name="framebuffer">Framebuffer descriptor (non-null).</param>
	/// <returns>True if unused.</returns>
	bool cdrawVkFramebufferUnused(cdrawVkFramebuffer const* const framebuffer);

	/// <summary>
	/// Create framebuffer and dependencies.
	/// </summary>
	/// <param name="framebuffer_out">Target framebuffer descriptor (non-null and unused).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="renderPass">Render pass descriptor (non-null and valid).</param>
	/// <param name="attachmentCount">Number of attachments to describe.</param>
	/// <param name="attachment">Image references representing attachments (image view component).</param>
	/// <param name="width">Framebuffer area width (must be larger or same as targets).</param>
	/// <param name="height">Framebuffer area height (must be larger or same as targets).</param>
	/// <param name="layers">Framebuffer area layers (must be larger or same as targets).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkFramebufferCreate(cdrawVkFramebuffer* const framebuffer_out,
		label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkRenderPass const* const renderPass,
		uint32_t const attachmentCount, VkImageView const attachment[/*attachmentCount*/],
		uint32_t const width, uint32_t const height, uint32_t const layers,
		VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Destroy framebuffer and dependencies.
	/// </summary>
	/// <param name="framebuffer_out">Target framebuffer descriptor (non-null and valid).</param>
	/// <param name="logicalDevice">Logical device descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if destroyed.</returns>
	bool cdrawVkFramebufferDestroy(cdrawVkFramebuffer* const framebuffer_out,
		cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt);


	/// <summary>
	/// Construct begin render pass info structure.
	/// </summary>
	/// <param name="renderPass">Render pass to use.</param>
	/// <param name="framebuffer">Framebuffer to use.</param>
	/// <param name="renderArea">Affected draw region.</param>
	/// <param name="clearValueCount">Number of clear values.</param>
	/// <param name="clearValues">List of clear values.</param>
	/// <returns>Vulkan begin render pass info.</returns>
	VkRenderPassBeginInfo cdrawVkRenderPassBeginInfoCtor(
		VkRenderPass const renderPass,
		VkFramebuffer const framebuffer,
		VkRect2D const renderArea,
		uint32_t const clearValueCount,
		VkClearValue const clearValues[/*clearValueCount*/]);

	/// <summary>
	/// Construct attachment reference.
	/// </summary>
	/// <param name="attachment">Index in render pass attachment list.</param>
	/// <param name="layout">Layout of attachment during subpass.</param>
	/// <returns>Vulkan attachment reference structure.</returns>
	VkAttachmentReference cdrawVkAttachmentReferenceCtor(
		uint32_t const attachment,
		VkImageLayout const layout);

	/// <summary>
	/// Construct default attachment reference (for unused slot).
	/// </summary>
	/// <returns>Vulkan attachment reference structure.</returns>
	VkAttachmentReference cdrawVkAttachmentReferenceCtorDefault();

	/// <summary>
	/// Construct default subpass description with color and depth/stencil.
	/// </summary>
	/// <param name="colorAttachmentCount">Number of color targets.</param>
	/// <param name="colorAttachments">Color targets.</param>
	/// <param name="resolveAttachments">Resolve targets (corresponding to color targets, must be UNUSED if not multisampled).</param>
	/// <param name="depthStencilAttachment">Depth/stencil target.</param>
	/// <returns>Vulkan subpapss description structure.</returns>
	VkSubpassDescription cdrawVkSubpassDescriptionCtorDefaultColorDepth(
		uint32_t const colorAttachmentCount,
		VkAttachmentReference const colorAttachments[cdrawVkFramebufferColorAttach_max /*colorAttachmentCount*/], // color drawing targets
		VkAttachmentReference const resolveAttachments[cdrawVkFramebufferColorAttach_max /*colorAttachmentCount*/], // auto downsampled
		VkAttachmentReference const depthStencilAttachment[1 /*1*/]);

	/// <summary>
	/// Construct subpass dependency description.
	/// </summary>
	/// <param name="srcSubpass">Index of source (initial) subpass (or EXTERNAL) described by dependency.</param>
	/// <param name="dstSubpass">Index of destination (terminal) subpass (or EXTERNAL) described by dependency.</param>
	/// <param name="srcStageMask">Source pipeline stage.</param>
	/// <param name="dstStageMask">Destination pipeline stage.</param>
	/// <param name="srcAccessMask">Source access mask.</param>
	/// <param name="dstAccessMask">Destination access mask.</param>
	/// <param name="dependencyFlags">Flags.</param>
	/// <returns>Vulkan subpass dependency structure.</returns>
	VkSubpassDependency cdrawVkSubpassDependencyCtor(
		uint32_t const srcSubpass,
		uint32_t const dstSubpass,
		VkPipelineStageFlags const srcStageMask,
		VkPipelineStageFlags const dstStageMask,
		VkAccessFlags const srcAccessMask,
		VkAccessFlags const dstAccessMask,
		VkDependencyFlags const dependencyFlags);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if ((!defined _CDRAW_RENDERER_FRAMEBUFFER_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))