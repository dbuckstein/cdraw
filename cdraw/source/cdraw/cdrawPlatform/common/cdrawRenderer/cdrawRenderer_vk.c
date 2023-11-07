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
* cdrawRenderer_vk.c
* Common implementation of modern Vulkan renderer management.
*/

#include "cdraw/cdrawPlatform/cdrawRenderer.h"
#include "cdrawRenderer_vk/_h/cdrawRenderer_vk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cdraw/cdrawCore/cdrawUtility.h"


/******************************************************************************
* Private implementations.
******************************************************************************/

void cdrawVkInstanceRefresh(VkInstance const instance,
	cdrawVkInstanceFuncTable* const f);

void cdrawVkDeviceRefresh(VkDevice const device,
	cdrawVkDeviceFuncTable* const f);


/******************************************************************************
* Private implementations.
******************************************************************************/

static result_t cdrawRendererPrint_vk(cdrawRenderer_vk const* const r)
{
	VkResult result = VK_SUCCESS;
	uint32_t apiVersion = 0;
	cdraw_assert(r);

	result = vkEnumerateInstanceVersion(&apiVersion);
	printf("\n Vulkan API: %u.%u.%u", VK_API_VERSION_MAJOR(apiVersion), VK_API_VERSION_MINOR(apiVersion), VK_API_VERSION_PATCH(apiVersion));
	return result;
}


#if CDRAW_DEBUG
static bool cdrawRendererDisplayTest_vk(cdrawRenderer_vk const* const r, uint32_t const index)
{
	/*
	* ****TO-DO:
	*	-> DRAWING:
	*		0) first time - use/make command buffer
	*		1) begin recording
	*		2) call draw commands to build buffer: 
	*			begin render pass
	*			bind pipeline
	*			bind descriptors
	*			bind buffers
	*			set viewport
	*			set scissor
	*			draw
	*			end render pass
	*		3) end recording
	*		4) submit to queue
	*		5) queue wait idle
	*		6) reset to reuse
	*	-> assumptions/predictions: 
	*		create render pass
	*		create set of "present" framebuffers that will manage how final presentation works
	*		attach color (and possibly depth/stencil) images from swapchain
	*/

	static VkClearColorValue const clearColor[] = {
		{ 1.00f, 0.00f, 0.00f, 1.00f, },
		{ 0.75f, 0.25f, 0.00f, 1.00f, },
		{ 0.50f, 0.50f, 0.00f, 1.00f, },
		{ 0.25f, 0.75f, 0.00f, 1.00f, },
		{ 0.00f, 1.00f, 0.00f, 1.00f, },
		{ 0.00f, 0.75f, 0.25f, 1.00f, },
		{ 0.00f, 0.50f, 0.50f, 1.00f, },
		{ 0.00f, 0.25f, 0.75f, 1.00f, },
		{ 0.00f, 0.00f, 1.00f, 1.00f, },
		{ 0.25f, 0.00f, 0.75f, 1.00f, },
		{ 0.50f, 0.00f, 0.50f, 1.00f, },
		{ 0.75f, 0.00f, 0.25f, 1.00f, },
	};
	static VkClearDepthStencilValue const clearDepthStencil = { 1.0f, 0 };
	static uint32_t clearColorIdx;
	VkClearValue clearValue[2];
	VkResult result = VK_SUCCESS;
	
	
	// BEGIN
	VkQueue const queue = r->presentation->queue_graphics[index].queue;
		
	// PREPARATION
	{
		VkCommandBuffer const cmdBuf = r->presentation->commandBuffer_present.commandBuffer[0];
		VkCommandBufferBeginInfo const cmdBufBegin = cdrawVkCommandBufferBeginInfoCtor(0, NULL);
		cdrawVkRenderPass const* const renderPass = &r->presentation->renderPass_present;
		cdrawVkFramebuffer const* const framebuffer = &r->presentation->framebuffer_present[index];
		VkRenderPassBeginInfo const renderPassBegin = cdrawVkRenderPassBeginInfoCtor(
			renderPass->renderPass, framebuffer->framebuffer,
			framebuffer->region, buffer_len(clearValue), clearValue);
	
		clearValue[0].color = clearColor[((clearColorIdx++) / 5) % buffer_len(clearColor)];
		clearValue[1].depthStencil = clearDepthStencil;
		
		result = vkBeginCommandBuffer(cmdBuf, &cmdBufBegin);
		cdraw_assert(result == VK_SUCCESS);
		
		vkCmdBeginRenderPass(cmdBuf, &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE);
		// ...
		vkCmdEndRenderPass(cmdBuf);
		
		//// NOTE: https://github.com/KhronosGroup/Vulkan-ValidationLayers/issues/4422#issuecomment-1256257653 
		////	-> getting validation error about image format being "undefined" - need actual commands from render pass to set it
		//cdrawVkCmdImageSetLayout(r->presentation->colorImage_present[index], cmdBuf, r->logicalDevice.queueFamilyIdx_graphics, 0, 0,
		//	VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
		//	VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		//	cdrawVkImageSubresourceRangeCtorDefaultColor());
	
		result = vkEndCommandBuffer(cmdBuf);
		cdraw_assert(result == VK_SUCCESS);


		// SUBMISSION
		{
			VkFence const fence = VK_NULL_HANDLE;// r->presentation->fence[index];
			VkSubmitInfo const submitInfo = cdrawVkSubmitInfoCtor(0, NULL, NULL, 1, &cmdBuf, 0, NULL);
			result = vkQueueSubmit(queue, 1, &submitInfo, fence);
			cdraw_assert(result == VK_SUCCESS);
			//result = vkWaitForFences(r->logicalDevice.logicalDevice, 1, &r->presentation->fence[index], VK_TRUE, UINT64_MAX);
			//cdraw_assert(result == VK_SUCCESS);
			//result = vkResetFences(r->logicalDevice.logicalDevice, 1, &r->presentation->fence[index]);
			//cdraw_assert(result == VK_SUCCESS);
		}
	}

	//VkCommandBuffer const cmdBuf = r->cmdBuf[0];
	//VkCommandBufferBeginInfo const cmdBufBeginInfo = cdrawVkCommandBufferBeginInfoCtor(0, NULL);
	//VkRenderPassBeginInfo const renderPassBeginInfo = cdrawVkRenderPassBeginInfoCtor();
	////VkSemaphore const semaphores_signal[] = {
	////	r->semaphore,
	////};
	//VkSubmitInfo const submitInfo[] = {
	//	cdrawVkSubmitInfoCtor(0, NULL, NULL, 1, &cmdBuf, 0, NULL),
	//};
	//result = vkBeginCommandBuffer(cmdBuf, &cmdBufBeginInfo);
	//vkCmdBeginRenderPass(cmdBuf, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	//// ...
	//vkCmdEndRenderPass(cmdBuf);
	//result = vkEndCommandBuffer(cmdBuf);
	//result = vkQueueSubmit(r->queue, buffer_len(submitInfo), submitInfo, VK_NULL_HANDLE);
	//result = vkQueueWaitIdle(r->queue);
	////vkResetCommandBuffer(cmdBuf, 0);

	return (result == VK_SUCCESS);
}
#endif // #if CDRAW_DEBUG

static VkPresentInfoKHR cdrawVkPresentInfoCtor(
	uint32_t const waitSemaphoreCount,
	VkSemaphore const waitSemaphores[/*waitSemaphoreCount*/],
	uint32_t const swapchainCount,
	VkSwapchainKHR const swapchains[/*swapchainCount*/],
	uint32_t const imageIndices[/*swapchainCount*/],
	VkResult results_out_opt[/*swapchainCount*/])
{
	VkPresentInfoKHR presentInfo = { 0 };
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = waitSemaphoreCount;
	presentInfo.pWaitSemaphores = waitSemaphores;
	presentInfo.swapchainCount = swapchainCount;
	presentInfo.pSwapchains = swapchains;
	presentInfo.pImageIndices = imageIndices;
	presentInfo.pResults = results_out_opt;
	return presentInfo;
}

static result_t cdrawRendererDisplay_vk(cdrawRenderer_vk const* const r, uint32_t const windowIndex)
{
	uint32_t idx;
	VkResult result = VK_SUCCESS;
	cdrawVkPresentation const* presentation;
	cdraw_assert(r);
	presentation = &r->presentation[windowIndex];
	if (!cdrawVkPresentationValid(presentation))
		return result;

	VkSemaphore const semaphores[] = {
		r->semaphore,
	};
	VkSwapchainKHR const swapchains[] = {
		presentation->swapchain.swapchain, // first is main
	};
	VkResult result_swapchain[buffer_len(swapchains)] = { VK_INCOMPLETE };
	uint32_t const nSwapchains = buffer_len(swapchains);
	uint32_t imageIndices[buffer_len(swapchains)];
	VkPresentInfoKHR const presentInfo = cdrawVkPresentInfoCtor(buffer_len(semaphores), semaphores, nSwapchains, swapchains, imageIndices, result_swapchain);
	VkQueue queue = VK_NULL_HANDLE;

	for (idx = 0; idx < nSwapchains; ++idx)
	{
		// grab next image in chain
		result = vkAcquireNextImageKHR(r->logicalDevice.logicalDevice, swapchains[idx], UINT64_MAX, r->semaphore, VK_NULL_HANDLE, &imageIndices[idx]);
		cdraw_assert(result == VK_SUCCESS);
	}

	queue = presentation->queue_graphics[imageIndices[0]].queue;
	//result = vkResetFences(r->logicalDevice.logicalDevice, 1, &r->presentation->fence[imageIndices[0]]);
	//cdraw_assert(result == VK_SUCCESS);

	// submission happens here
#if CDRAW_DEBUG
	cdrawRendererDisplayTest_vk(r, imageIndices[0]);
#endif // #if CDRAW_DEBUG

	// blit image - alternative to mandatory draw target support
	//vkCmdBlitImage();

	// draw
	if (result == VK_SUCCESS)
	{
		result = vkQueueWaitIdle(queue);
		cdraw_assert(result == VK_SUCCESS);
		result = vkQueuePresentKHR(queue, &presentInfo);
		if (result != VK_SUCCESS)
		{
			// handle display error
		}
	}
	return result;
}


static result_t cdrawRendererResize_vk(cdrawRenderer_vk* const r, uint32_t const windowIndex, uint32_t const w_old, uint32_t const h_old, uint32_t const w_new, uint32_t const h_new)
{
	VkResult result = VK_SUCCESS;
	cdrawVkPresentation* presentation;
	VkAllocationCallbacks const* alloc_opt;
	cdraw_assert(r);
	if ((w_old != w_new) || (h_old != h_new))
	{
		alloc_opt = cdrawVkAllocatorUse(&r->allocator);
		presentation = &r->presentation[windowIndex];

		// release old swapchain if the window size was valid
		if (w_old && h_old)
			result = cdrawVkPresentationDestroy(presentation, &r->logicalDevice, &r->commandPool, alloc_opt);

		// create new swapchain if the new window size is valid
		if (w_new && h_new)
			result = cdrawVkPresentationCreate(presentation, "cdrawVkPresentation", &r->logicalDevice, &r->surface[windowIndex], &r->commandPool, alloc_opt);
	}
	return result;
}


static result_t cdrawRendererAttachWindow_vk(cdrawRenderer_vk* const r, uint32_t const windowIndex, ptrk_t const p_data)
{
	result_init();
	bool result = false;
	VkAllocationCallbacks const* alloc_opt;
	cdraw_assert(r);
	if (!r->instance.instance || !r->logicalDevice.logicalDevice)
		result_return();

	cdraw_assert(windowIndex < cdrawVkSurfacePresent_max);
	cdraw_assert(p_data);
	alloc_opt = cdrawVkAllocatorUse(&r->allocator);

	{
		cdrawVkSurface* const surface = &r->surface[windowIndex];
		cdrawVkPresentation* const presentation = &r->presentation[windowIndex];

		// CREATE PRESENTATION SURFACE
		result = cdrawVkSurfaceCreate(surface, "cdrawVkSurface", & r->instance, p_data, alloc_opt);
		failassertret(result, result_seterror(errcode_renderer_init));

		// CREATE PRESENTATION (may not be created due to surface size)
		result = cdrawVkPresentationCreate(presentation, "cdrawVkPresentation", &r->logicalDevice, surface, &r->commandPool, alloc_opt);
		failassertret(result, result_seterror(errcode_renderer_init));

		// CREATE SEMAPHORES
		result = cdrawRendererCreateSemaphore_vk(&r->semaphore, r->logicalDevice.logicalDevice, alloc_opt);
		failassertret(result, result_seterror(errcode_renderer_init));
	}
	result_return();
}


static result_t cdrawRendererDetachWindow_vk(cdrawRenderer_vk* const r, uint32_t const windowIndex)
{
	result_init();
	bool result = false;
	VkAllocationCallbacks const* alloc_opt;
	cdraw_assert(r);
	if (!r->instance.instance || !r->logicalDevice.logicalDevice)
		result_return();

	cdraw_assert(windowIndex < cdrawVkSurfacePresent_max);
	alloc_opt = cdrawVkAllocatorUse(&r->allocator);

	{
		cdrawVkSurface* const surface = &r->surface[windowIndex];
		cdrawVkPresentation* const presentation = &r->presentation[windowIndex];

		// semaphores
		result = cdrawRendererDestroySemaphore_vk(&r->semaphore, r->logicalDevice.logicalDevice, alloc_opt);
		failassertret(result, result_seterror(errcode_renderer_init));

		// presentation (requires device and surface; may not have been created due to surface size)
		result = cdrawVkPresentationDestroy(presentation, &r->logicalDevice, &r->commandPool, alloc_opt);
		failassertret(result, result_seterror(errcode_renderer_init));

		// presentation surface (requires instance)
		result = cdrawVkSurfaceDestroy(surface, &r->instance, alloc_opt);
		failassertret(result, result_seterror(errcode_renderer_init));
	}
	result_return();
}


static result_t cdrawRendererWindowCountMax_vk(cdrawRenderer_vk const* const r, uint32_t* const count_out)
{
	VkResult result = VK_SUCCESS;
	cdraw_assert(r);
	cdraw_assert(count_out);
	*count_out = (r->instance.instance && r->logicalDevice.logicalDevice) ? cdrawVkSurfacePresent_max : 0;
	return result;
}


/// <summary>
/// Vulkan convenience function table.
/// </summary>
static cdrawRendererFuncTable gRendererFuncTable_vk;


bool cdrawVkDebugReportCreate(VkDebugReportCallbackEXT* const debugReport_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt,
	cdrawVkInstanceDebugFuncTable const* const f);

bool cdrawVkDebugReportDestroy(VkDebugReportCallbackEXT* const debugReport_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt,
	cdrawVkInstanceDebugFuncTable const* const f);

bool cdrawVkDebugUtilsMessengerCreate(VkDebugUtilsMessengerEXT* const debugUtilsMsg_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt,
	cdrawVkInstanceDebugFuncTable const* const f);

bool cdrawVkDebugUtilsMessengerDestroy(VkDebugUtilsMessengerEXT* const debugUtilsMsg_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt,
	cdrawVkInstanceDebugFuncTable const* const f);


/******************************************************************************
* Public implementations.
******************************************************************************/

result_t cdrawRendererCreate_vk(cdrawRenderer* const renderer, uint32_t const surfaceCountMax, ptrk_t const p_data_opt)
{
	result_init();
	bool result = false;
	size_t const dataSz = sizeof(cdrawRenderer_vk);
	cdrawRenderer_vk* r;
	asserterr(renderer && !renderer->r && !renderer->renderAPI, errcode_invalidarg);
	r = (cdrawRenderer_vk*)malloc(dataSz);
	asserterr_ptr(r, errcode_renderer_init);
	memset(r, 0, dataSz);

	// setup allocation callbacks
	VkAllocationCallbacks const* const alloc_opt = cdrawVkAllocatorInit(&r->allocator, "cdrawVkAllocator", false);

	// CREATE INSTANCE
	result = cdrawVkInstanceCreate(&r->instance, "cdrawVkInstance", alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

#if CDRAW_DEBUG
	//// CREATE DEBUG REPORT
	//result = cdrawVkDebugReportCreate(&r->debug.debugReport, r->instance.instance, alloc_opt, &r->instance.f.f_debug);
	//failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE DEBUG MESSENGER
	result = cdrawVkDebugUtilsMessengerCreate(&r->debug.debugMessenger, r->instance.instance, alloc_opt, &r->instance.f.f_debug);
	failassertret(result, result_seterror(errcode_renderer_init));
#endif // #if CDRAW_DEBUG

	// CREATE LOGICAL DEVICE
	result = cdrawVkLogicalDeviceCreate(&r->logicalDevice, "cdrawVkLogicalDevice", &r->instance, surfaceCountMax, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// CREATE COMMAND POOL
	result = cdrawVkCommandPoolCreate(&r->commandPool, "cdrawVkCommandPool", &r->logicalDevice, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));
	
	if (p_data_opt)
	{
		result_t result_attach = cdrawRendererAttachWindow_vk(r, 0, p_data_opt);
		failassertret(result_isclean(result_attach), result_seterror(errcode_renderer_init));
	}

	// all done
	renderer->r = r;
	renderer->f = &gRendererFuncTable_vk;
	result_return();
}

result_t cdrawRendererDestroy_vk(cdrawRenderer* const renderer)
{
	result_init();
	bool result = false;
	uint32_t i;
	cdrawRenderer_vk* r;
	VkAllocationCallbacks const* alloc_opt;
	asserterr(renderer && renderer->r && renderer->renderAPI == cdrawRenderAPI_Vulkan, errcode_invalidarg);
	r = ((cdrawRenderer_vk*)renderer->r);
	alloc_opt = cdrawVkAllocatorUse(&r->allocator);

	for (i = 0; i < cdrawVkSurfacePresent_max; ++i)
	{
		result_t result_detach = cdrawRendererDetachWindow_vk(r, i);
		failassertret(result_isclean(result_detach), result_seterror(errcode_renderer_init));
	}

	// command pool (requires device)
	result = cdrawVkCommandPoolDestroy(&r->commandPool, &r->logicalDevice, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// logical device (wait for it to finish work)
	result = cdrawVkLogicalDeviceDestroy(&r->logicalDevice, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

#if CDRAW_DEBUG
	// destroy debug messenger
	result = cdrawVkDebugUtilsMessengerDestroy(&r->debug.debugMessenger, r->instance.instance, alloc_opt, &r->instance.f.f_debug);
	failassertret(result, result_seterror(errcode_renderer_init));

	//// destroy debug report
	//result = cdrawVkDebugReportDestroy(&r->debug.debugReport, r->instance.instance, alloc_opt, &r->instance.f.f_debug);
	//failassertret(result, result_seterror(errcode_renderer_init));
#endif // #if CDRAW_DEBUG

	// finally, destroy instance
	result = cdrawVkInstanceDestroy(&r->instance, alloc_opt);
	failassertret(result, result_seterror(errcode_renderer_init));

	// clean allocation callbacks
	cdrawVkAllocatorReset(&r->allocator);

	// all done
	free(renderer->r);
	renderer->r = NULL;
	renderer->f = NULL;
	result_return();
}

result_t cdrawRendererRefresh_vk(cdrawRenderer const* const renderer)
{
	result_init();
	cdrawRenderer_vk* r;
	asserterr(renderer && renderer->r && renderer->renderAPI == cdrawRenderAPI_Vulkan, errcode_invalidarg);
	r = ((cdrawRenderer_vk*)renderer->r);

	cdrawVkInstanceRefresh(r->instance.instance, &r->instance.f);
	cdrawVkDeviceRefresh(r->logicalDevice.logicalDevice, &r->logicalDevice.f);

	result_return();
}

result_t cdrawRendererRefreshAPI_vk()
{
	result_init();

	gRendererFuncTable_vk.cdrawRendererPrint = cdrawRendererPrint_vk;
	gRendererFuncTable_vk.cdrawRendererDisplay = cdrawRendererDisplay_vk;
	gRendererFuncTable_vk.cdrawRendererResize = cdrawRendererResize_vk;
	gRendererFuncTable_vk.cdrawRendererAttachWindow = cdrawRendererAttachWindow_vk;
	gRendererFuncTable_vk.cdrawRendererDetachWindow = cdrawRendererDetachWindow_vk;
	gRendererFuncTable_vk.cdrawRendererWindowsSupported = cdrawRendererWindowCountMax_vk;

	result_return();
}
