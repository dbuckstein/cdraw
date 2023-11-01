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
* cdrawRendererFramebuffer_vk.c
* Internal source for Vulkan framebuffer management.
*/

#include "_h/cdrawRenderer_vk.h"
#include "_h/cdrawRenderer_vk/cdrawRendererFramebuffer_vk.h"

#include <stdio.h>


/******************************************************************************
* Internal implementations.
******************************************************************************/

/// <summary>
/// Constructor for Vulkan render pass descriptor.
/// </summary>
/// <param name="renderPass_out">Target render pass descriptor (non-null).</param>
/// <param name="name">Descriptor name.</param>
/// <param name="renderPass">Vulkan render pass handle.</param>
/// <returns>Success: <paramref name="renderPass_out"/>; Failure: <c>NULL</c>.</returns>
static cdrawVkRenderPass* cdrawVkRenderPassCtor(cdrawVkRenderPass* const renderPass_out,
	label_t const name, VkRenderPass const renderPass);

/// <summary>
/// Destructor interface for Vulkan render pass descriptor.
/// </summary>
/// <param name="renderPass_out">Target render pass descriptor (non-null).</param>
static void cdrawVkRenderPassDtor(cdrawVkRenderPass* const renderPass_out);

/// <summary>
/// Constructor for Vulkan framebuffer descriptor.
/// </summary>
/// <param name="framebuffer_out">Target framebuffer descriptor (non-null).</param>
/// <param name="name">Descriptor name.</param>
/// <param name="framebuffer">Vulkan framebuffer handle.</param>
/// <returns>Success: <paramref name="framebuffer_out"/>; Failure: <c>NULL</c>.</returns>
static cdrawVkFramebuffer* cdrawVkFramebufferCtor(cdrawVkFramebuffer* const framebuffer_out,
	label_t const name, VkFramebuffer const framebuffer);

/// <summary>
/// Destructor interface for Vulkan framebuffer descriptor.
/// </summary>
/// <param name="framebuffer_out">Target framebuffer descriptor (non-null).</param>
static void cdrawVkFramebufferDtor(cdrawVkFramebuffer* const framebuffer_out);


/******************************************************************************
* Public implementations.
******************************************************************************/

cdrawVkRenderPass* cdrawVkRenderPassCtor(cdrawVkRenderPass* const renderPass_out,
	label_t const name, VkRenderPass const renderPass)
{
	failassertret(renderPass_out && cdrawVkRenderPassUnused(renderPass_out), NULL);
	failassertret(renderPass, NULL);
	label_copy_safe(renderPass_out->name, name);
	renderPass_out->renderPass = renderPass;
	return renderPass_out;
}

void cdrawVkRenderPassDtor(cdrawVkRenderPass* const renderPass_out)
{
	failassertret(renderPass_out);
	renderPass_out->renderPass = VK_NULL_HANDLE;
	label_init(renderPass_out->name);
}

bool cdrawVkRenderPassValid(cdrawVkRenderPass const* const renderPass)
{
	cdraw_assert(renderPass);
	return (renderPass->renderPass);
}

bool cdrawVkRenderPassUnused(cdrawVkRenderPass const* const renderPass)
{
	cdraw_assert(renderPass);
	return (!renderPass->renderPass);
}


cdrawVkFramebuffer* cdrawVkFramebufferCtor(cdrawVkFramebuffer* const framebuffer_out,
	label_t const name, VkFramebuffer const framebuffer)
{
	failassertret(framebuffer_out && cdrawVkFramebufferUnused(framebuffer_out), NULL);
	failassertret(framebuffer, NULL);
	label_copy_safe(framebuffer_out->name, name);
	framebuffer_out->framebuffer = framebuffer;
	return framebuffer_out;
}

void cdrawVkFramebufferDtor(cdrawVkFramebuffer* const framebuffer_out)
{
	failassertret(framebuffer_out);
	framebuffer_out->framebuffer = VK_NULL_HANDLE;
	label_init(framebuffer_out->name);
}

bool cdrawVkFramebufferValid(cdrawVkFramebuffer const* const framebuffer)
{
	cdraw_assert(framebuffer);
	return (framebuffer->framebuffer);
}

bool cdrawVkFramebufferUnused(cdrawVkFramebuffer const* const framebuffer)
{
	cdraw_assert(framebuffer);
	return (!framebuffer->framebuffer);
}


/******************************************************************************
* SECTION: Render pass and framebuffer management.
* Reference: Singh, c.7.
* Substantial improvements: translated to C and organized.
******************************************************************************/

VkAttachmentReference cdrawVkAttachmentReferenceCtor(
	uint32_t const attachment, // index in "VkRenderPassCreateInfo::pAttachments"
	VkImageLayout const layout)
{
	VkAttachmentReference attachmentReference = { 0 };
	attachmentReference.attachment = attachment;
	attachmentReference.layout = layout;
	return attachmentReference;
}

VkAttachmentReference cdrawVkAttachmentReferenceCtorDefault()
{
	return cdrawVkAttachmentReferenceCtor(VK_ATTACHMENT_UNUSED, VK_IMAGE_LAYOUT_UNDEFINED);
}

static VkSubpassDescription cdrawVkSubpassDescriptionCtor(
	VkPipelineBindPoint const pipelineBindPoint,
	uint32_t const inputAttachmentCount,
	VkAttachmentReference const inputAttachments[cdrawVkFramebufferInputAttach_max /*inputAttachmentCount*/], // one-tap (current texel only) access for shaders
	uint32_t const colorAttachmentCount,
	VkAttachmentReference const colorAttachments[cdrawVkFramebufferColorAttach_max /*colorAttachmentCount*/], // color drawing targets
	VkAttachmentReference const resolveAttachments[cdrawVkFramebufferColorAttach_max /*colorAttachmentCount*/], // auto downsampled - must be UNUSED if corresponding color target has sample count of 1
	VkAttachmentReference const depthStencilAttachment[1 /*1*/], // depth/stencil drawing target
	uint32_t const preserveAttachmentCount, // unique to Vulkan: contents are kept but not touched during subpass
	uint32_t const preserveAttachments[/*preserveAttachmentCount*/])
{
	VkSubpassDescription subpassDescription = { 0 };
	subpassDescription.pipelineBindPoint = pipelineBindPoint;
	subpassDescription.inputAttachmentCount = inputAttachmentCount;
	subpassDescription.pInputAttachments = inputAttachments;
	subpassDescription.colorAttachmentCount = colorAttachmentCount;
	subpassDescription.pColorAttachments = colorAttachments;
	subpassDescription.pResolveAttachments = resolveAttachments;
	subpassDescription.pDepthStencilAttachment = depthStencilAttachment;
	subpassDescription.preserveAttachmentCount = preserveAttachmentCount;
	subpassDescription.pPreserveAttachments = preserveAttachments;
	return subpassDescription;
}

VkSubpassDescription cdrawVkSubpassDescriptionCtorDefaultColorDepth(
	uint32_t const colorAttachmentCount,
	VkAttachmentReference const colorAttachments[cdrawVkFramebufferColorAttach_max /*colorAttachmentCount*/], // color drawing targets
	VkAttachmentReference const resolveAttachments[cdrawVkFramebufferColorAttach_max /*colorAttachmentCount*/], // auto downsampled
	VkAttachmentReference const depthStencilAttachment[1 /*1*/]) // depth/stencil drawing target
{
	return cdrawVkSubpassDescriptionCtor(VK_PIPELINE_BIND_POINT_GRAPHICS,
		0, NULL,
		colorAttachmentCount, colorAttachments, resolveAttachments, depthStencilAttachment,
		0, NULL);
}

static VkSubpassDependency cdrawVkSubpassDependencyCtor(
	uint32_t const srcSubpass,
	uint32_t const dstSubpass,
	VkPipelineStageFlags const srcStageMask,
	VkPipelineStageFlags const dstStageMask,
	VkAccessFlags const srcAccessMask,
	VkAccessFlags const dstAccessMask,
	VkDependencyFlags const dependencyFlags)
{
	VkSubpassDependency subpassDependency = { 0 };
	subpassDependency.srcSubpass = srcSubpass;
	subpassDependency.dstAccessMask = dstSubpass;
	subpassDependency.srcStageMask = srcStageMask;
	subpassDependency.dstStageMask = dstStageMask;
	subpassDependency.srcAccessMask = srcAccessMask;
	subpassDependency.dstAccessMask = dstAccessMask;
	subpassDependency.dependencyFlags = dependencyFlags;
	return subpassDependency;
}

static VkRenderPassCreateInfo cdrawVkRenderPassCreateInfoCtor(
	uint32_t const attachmentCount,
	VkAttachmentDescription const attachments[/*attachmentCount*/],
	uint32_t const subpassCount,
	VkSubpassDescription const subpasses[/*subpassCount*/],
	uint32_t const dependencyCount,
	VkSubpassDependency const dependencies[/*dependencyCount*/])
{
	VkRenderPassCreateInfo renderPassCreateInfo = { 0 };
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = attachmentCount;
	renderPassCreateInfo.pAttachments = attachments;
	renderPassCreateInfo.subpassCount = subpassCount;
	renderPassCreateInfo.pSubpasses = subpasses;
	renderPassCreateInfo.dependencyCount = dependencyCount;
	renderPassCreateInfo.pDependencies = dependencies;
	return renderPassCreateInfo;
}

bool cdrawVkRenderPassCreate(cdrawVkRenderPass* const renderPass_out,
	label_t const name, cdrawVkLogicalDevice const* const logicalDevice,
	uint32_t const attachmentCount, VkAttachmentDescription const attachment[/*attachmentCount*/],
	uint32_t const subpassCount, VkSubpassDescription const subpass[/*subpassCount*/],
	VkAllocationCallbacks const* const alloc_opt)
{
	VkResult result = VK_SUCCESS;
	failassertret(renderPass_out && cdrawVkRenderPassUnused(renderPass_out) && logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice), false);
	printf("\n Creating Vulkan render pass \"%s\"...", name);

	// CREATE RENDER PASS
	{
		VkRenderPassCreateInfo const renderPassCreateInfo = cdrawVkRenderPassCreateInfoCtor(
			attachmentCount, attachment, subpassCount, subpass, 0, NULL);
		result = vkCreateRenderPass(logicalDevice->logicalDevice, &renderPassCreateInfo, alloc_opt, &renderPass_out->renderPass);
		if (renderPass_out->renderPass)
			cdraw_assert(result == VK_SUCCESS);
	}

	if (!cdrawVkRenderPassValid(renderPass_out) || (result != VK_SUCCESS))
	{
		cdrawVkRenderPassDestroy(renderPass_out, logicalDevice, alloc_opt);
		printf("\n Vulkan render pass \"%s\" creation failed.", name);
		return false;
	}
	printf("\n Vulkan render pass \"%s\" created.", name);
	label_copy_safe(renderPass_out->name, name);
	cdraw_assert(cdrawVkRenderPassValid(renderPass_out));
	return true;
}

bool cdrawVkRenderPassDestroy(cdrawVkRenderPass* const renderPass_out,
	cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt)
{
	failassertret(renderPass_out, false);
	if (cdrawVkRenderPassUnused(renderPass_out))
		return true;

	cdraw_assert(logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice));
	printf("\n Destroying Vulkan render pass \"%s\"...", renderPass_out->name);
	//if (renderPass_out->renderPass)
	{
		vkDestroyRenderPass(logicalDevice->logicalDevice, renderPass_out->renderPass, alloc_opt);
		renderPass_out->renderPass = VK_NULL_HANDLE;
	}
	printf("\n Vulkan render pass \"%s\" destroyed.", renderPass_out->name);
	label_init(renderPass_out->name);
	return true;
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


static VkFramebufferCreateInfo cdrawVkFramebufferCreateInfoCtor(
	VkRenderPass const renderPass,
	uint32_t const attachmentCount,
	VkImageView const attachments[/*attachmentCount*/],
	uint32_t const width,
	uint32_t const height,
	uint32_t const layers)
{
	VkFramebufferCreateInfo framebufferCreateInfo = { 0 };
	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	//framebufferCreateInfo.flags = 0; // VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT - can provide only details - ext "VK_KHR_imageless_framebuffer"
	framebufferCreateInfo.renderPass = renderPass;
	framebufferCreateInfo.attachmentCount = attachmentCount;
	framebufferCreateInfo.pAttachments = attachments;
	framebufferCreateInfo.width = width;
	framebufferCreateInfo.height = height;
	framebufferCreateInfo.layers = layers;
	return framebufferCreateInfo;
}

bool cdrawVkFramebufferCreate(cdrawVkFramebuffer* const framebuffer_out,
	label_t const name, cdrawVkLogicalDevice const* const logicalDevice, cdrawVkRenderPass const* const renderPass,
	uint32_t const attachmentCount, VkImageView const attachment[/*attachmentCount*/],
	VkAllocationCallbacks const* const alloc_opt)
{
	VkResult result = VK_SUCCESS;
	failassertret(framebuffer_out && cdrawVkFramebufferUnused(framebuffer_out) && logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice), false);
	printf("\n Creating Vulkan framebuffer \"%s\"...", name);

	// ****TO-DO
	{
		VkFramebufferCreateInfo const framebufferCreateInfo = { 0 };// = cdrawVkFramebufferCreateInfoCtor();
		result = vkCreateFramebuffer(logicalDevice->logicalDevice, &framebufferCreateInfo, alloc_opt, &framebuffer_out->framebuffer);
		if (framebuffer_out->framebuffer)
			cdraw_assert(result == VK_SUCCESS);
	}

	if (!cdrawVkFramebufferValid(framebuffer_out) || (result != VK_SUCCESS))
	{
		cdrawVkFramebufferDestroy(framebuffer_out, logicalDevice, alloc_opt);
		printf("\n Vulkan framebuffer \"%s\" creation failed.", name);
		return false;
	}
	printf("\n Vulkan framebuffer \"%s\" created.", name);
	label_copy_safe(framebuffer_out->name, name);
	cdraw_assert(cdrawVkFramebufferValid(framebuffer_out));
	return true;
}

bool cdrawVkFramebufferDestroy(cdrawVkFramebuffer* const framebuffer_out,
	cdrawVkLogicalDevice const* const logicalDevice, VkAllocationCallbacks const* const alloc_opt)
{
	failassertret(framebuffer_out, false);
	if (cdrawVkFramebufferUnused(framebuffer_out))
		return true;

	cdraw_assert(logicalDevice && cdrawVkLogicalDeviceValid(logicalDevice));
	printf("\n Destroying Vulkan framebuffer \"%s\"...", framebuffer_out->name);
	//if (framebuffer_out->framebuffer)
	{
		vkDestroyFramebuffer(logicalDevice->logicalDevice, framebuffer_out->framebuffer, alloc_opt);
		framebuffer_out->framebuffer = VK_NULL_HANDLE;
	}
	printf("\n Vulkan framebuffer \"%s\" destroyed.", framebuffer_out->name);
	label_init(framebuffer_out->name);
	return true;
}
