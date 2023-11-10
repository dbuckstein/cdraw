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
static bool cdrawRendererDisplayTest_vk(
	VkCommandBuffer const commandBuffer,
	VkQueryPool const queryPool,
	VkRenderPass const renderPass,
	VkFramebuffer const framebuffer,
	VkRect2D const region,
	uint32_t const queryBase)
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
	static uint32_t const rateToCount = 60 / buffer_len(clearColor);
	static VkClearDepthStencilValue const clearDepthStencil = { 1.0f, 0 };
	static uint32_t clearColorIdx;
	VkClearValue clearValue[2];

	VkCommandBufferBeginInfo const commandBufferBegin = cdrawVkCommandBufferBeginInfoCtor(0, NULL);
	VkRenderPassBeginInfo const renderPassBegin = cdrawVkRenderPassBeginInfoCtor(renderPass, framebuffer, region, buffer_len(clearValue), clearValue);

	uint32_t const query_renderPassBegin = (queryBase + cdrawVkQuery_renderPassBegin);
	uint32_t const query_renderPassEnd = (queryBase + cdrawVkQuery_renderPassEnd);

	VkResult result = VK_SUCCESS;

	clearValue[0].color = clearColor[((clearColorIdx++) / rateToCount) % buffer_len(clearColor)];
	clearValue[1].depthStencil = clearDepthStencil;

	result = vkResetCommandBuffer(commandBuffer, 0);
	cdraw_assert(result == VK_SUCCESS);

	result = vkBeginCommandBuffer(commandBuffer, &commandBufferBegin);
	cdraw_assert(result == VK_SUCCESS);

#if CDRAW_DEBUG
	vkCmdResetQueryPool(commandBuffer, queryPool, queryBase, cdrawVkQuery_max);
	vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, queryPool, query_renderPassBegin);
#endif // #if CDRAW_DEBUG

	vkCmdBeginRenderPass(commandBuffer, &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE);
	// ...
	vkCmdEndRenderPass(commandBuffer);
	
#if CDRAW_DEBUG
	vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, queryPool, query_renderPassEnd);
#endif // #if CDRAW_DEBUG

	result = vkEndCommandBuffer(commandBuffer);
	cdraw_assert(result == VK_SUCCESS);

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

static result_t cdrawRendererDisplay_vk(cdrawRenderer_vk* const r, uint32_t const windowIndex)
{
	// steps: 
	//	-> wait for current frame queue fence to be ready
	//	-> acquire image from swapchain (get image index)
	//	-> wait for next image queue fence to be ready
	//	-> wait on current frame semaphore, passed to acquire image (e.g. color targets must have been output for there to be any point in continuing)
	//	-> signal other semaphores for completed drawing on submit
	//	-> reset current frame queue fence
	//	-> submit queue with current frame queue fence
	//	-> present after waiting from semaphores signalled from above submit
	//	-> increment current frame

	uint32_t idx;
	VkResult result = VK_SUCCESS;
	cdrawVkPresentation* presentation;
	cdraw_assert(r);
	presentation = &r->presentation[windowIndex];
	if (!cdrawVkPresentationValid(presentation))
		return result;

	uint32_t const nTimestamp = buffer_len(presentation->timestamp);
	fp32_t const timestamp_nanosPerCount = r->logicalDevice.physicalDevice.physicalDeviceProp.limits.timestampPeriod;

	VkSwapchainKHR const swapchains[] = {
		presentation->swapchain.swapchain, // first is main
	};
	uint32_t const nSwapchains = buffer_len(swapchains);
	VkSemaphore semaphore_present_wait[buffer_len(swapchains)] = { VK_NULL_HANDLE },
		* semaphore_present_wait_ptr = semaphore_present_wait;
	VkResult result_swapchain[buffer_len(swapchains)] = { VK_INCOMPLETE };
	uint32_t imageIndices[buffer_len(swapchains)] = { UINT64_MAX };

	VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
	VkFramebuffer framebuffer = VK_NULL_HANDLE;
	VkQueue queue = VK_NULL_HANDLE;
	VkRect2D region = { 0 };
	uint32_t queryInFlight[cdrawFramesInFlight_max] = { 0 };
	uint32_t frameInFlight[cdrawFramesInFlight_max] = { 0 };
	uint32_t imageInFlight[cdrawFramesInFlight_max] = { 0 };

	cdraw_assert(nSwapchains <= cdrawFramesInFlight_max);

	// could have one submission loop per swapchain
	// potentially facilitates multiple displays
	//	-> acquire image for each
	//	-> prepare queues for each
	//	-> submit for each
	//	-> present for all at once
	for (idx = 0; idx < nSwapchains; ++idx)
	{
		uint32_t image = UINT32_MAX;
		uint32_t const frame = frameInFlight[idx] = presentation->frame;
		VkSemaphore const semaphore_acquire = presentation->semaphore_acquire[presentation->frame];
		VkSemaphore const semaphores_acquire_signal_submit_wait[] = {
			semaphore_acquire,
		};
		uint32_t const nSemaphore_acquire = buffer_len(semaphores_acquire_signal_submit_wait);
		VkPipelineStageFlags const semaphore_submit_wait_stage[buffer_len(semaphores_acquire_signal_submit_wait)] = {
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		};
		VkSemaphore const semaphores_submit_signal_present_wait[] = {
			presentation->semaphore_submit[presentation->frame],
		};
		uint32_t const nSemaphore_submit = buffer_len(semaphores_submit_signal_present_wait);
		VkFence const fence_acquire = VK_NULL_HANDLE;//presentation->fence_acquire[frame];
		VkFence const fence_submit = presentation->fence_submit[frame];
		VkFence fence_image;

#if CDRAW_DEBUG
		uint32_t const query = queryInFlight[idx] = (frame * cdrawVkQuery_max);
#endif // #if CDRAW_DEBUG

		// copy present wait semaphores
		cdraw_assert(nSemaphore_acquire == buffer_len(semaphore_submit_wait_stage));
		memcpy(semaphore_present_wait_ptr, semaphores_submit_signal_present_wait, sizeof(semaphores_submit_signal_present_wait));
		semaphore_present_wait_ptr += nSemaphore_submit;

		// wait on fence for this frame
		result = vkWaitForFences(r->logicalDevice.logicalDevice, 1, &fence_submit, VK_TRUE, UINT64_MAX);
		cdraw_assert(result == VK_SUCCESS);

		// grab next image in chain
		//result = vkResetFences(r->logicalDevice.logicalDevice, 1, &fence_acquire);
		result = vkAcquireNextImageKHR(r->logicalDevice.logicalDevice, swapchains[idx], UINT64_MAX, semaphore_acquire, fence_acquire, &imageIndices[idx]);
		cdraw_assert(result == VK_SUCCESS);

		// wait on fence corresponding to image in use
		image = imageInFlight[idx] = imageIndices[idx];
		fence_image = presentation->fence_submit[image];
		result = vkWaitForFences(r->logicalDevice.logicalDevice, 1, &fence_image, VK_TRUE, UINT64_MAX);
		cdraw_assert(result == VK_SUCCESS);

#if CDRAW_DEBUG
		// get times previously in current slot
		result = vkGetQueryPoolResults(r->logicalDevice.logicalDevice, presentation->queryPool_present,
			query, cdrawVkQuery_max,
			(cdrawVkQuery_max * sizeof(*presentation->timestamp)),
			&presentation->timestamp[query],
			sizeof(*presentation->timestamp),
			VK_QUERY_RESULT_64_BIT);

		// TEST DRAW PRIOR TO PRESENTATION
		{
			queue = presentation->queue_graphics[frame].queue;
			commandBuffer = presentation->commandBuffer_present.commandBuffer[frame];
			framebuffer = presentation->framebuffer_present[image].framebuffer; // match image because the targets are attached to it
			region = presentation->framebuffer_present[image].region;
			cdrawRendererDisplayTest_vk(commandBuffer, presentation->queryPool_present, presentation->renderPass_present.renderPass, framebuffer, region, query);
		}
#endif // #if CDRAW_DEBUG

		// reset fence for this frame
		result = vkResetFences(r->logicalDevice.logicalDevice, 1, &fence_submit);
		cdraw_assert(result == VK_SUCCESS);

		// SUBMISSION
		{
			VkSubmitInfo const submitInfo = cdrawVkSubmitInfoCtor(nSemaphore_acquire, semaphores_acquire_signal_submit_wait,
				semaphore_submit_wait_stage, 1, &commandBuffer, nSemaphore_submit, semaphores_submit_signal_present_wait);
			
			result = vkQueueSubmit(queue, 1, &submitInfo, fence_submit);
			cdraw_assert(result == VK_SUCCESS);
		}

		// next
		presentation->frame = (frame + 1) % cdrawFramesInFlight_max;
	}

	// PRESENTATION: everything at once
	{
		VkPresentInfoKHR const presentInfo = cdrawVkPresentInfoCtor(buffer_len(semaphore_present_wait),
			semaphore_present_wait, nSwapchains, swapchains, imageIndices, result_swapchain);

		//result = vkWaitForFences(r->logicalDevice.logicalDevice, nSwapchains, presentation->fence_acquire, VK_TRUE, UINT64_MAX);
		//cdraw_assert(result == VK_SUCCESS);
		//result = vkResetFences(r->logicalDevice.logicalDevice, nSwapchains, presentation->fence_acquire);
		//cdraw_assert(result == VK_SUCCESS);

		queue = presentation->queue_present.queue;
		result = vkQueuePresentKHR(queue, &presentInfo);
		if (result != VK_SUCCESS)
		{
			// handle presentation error
			for (idx = 0; idx < nSwapchains; ++idx)
			{
				result = result_swapchain[idx];
				cdraw_assert(result == VK_SUCCESS);
			}
		}
	}

#if CDRAW_DEBUG
	//// get timestamps from last frame in flight
	//result = vkGetQueryPoolResults(r->logicalDevice.logicalDevice, presentation->queryPool_present,
	//	0, nTimestamp, sizeof(presentation->timestamp), presentation->timestamp, sizeof(*presentation->timestamp),
	//	VK_QUERY_RESULT_64_BIT);

	// display rates
	for (idx = 0; idx < nSwapchains; ++idx)
	{
		uint32_t const query = queryInFlight[idx];
		uint32_t const query_renderPassBegin = (query + cdrawVkQuery_renderPassBegin);
		uint32_t const query_renderPassEnd = (query + cdrawVkQuery_renderPassEnd);
		uint32_t const query_renderPassBegin0 = ((query_renderPassBegin + nTimestamp) - (nSwapchains * cdrawVkQuery_max)) % nTimestamp;
		uint64_t const t_renderPassBegin = presentation->timestamp[query_renderPassBegin];
		uint64_t const t_renderPassEnd = presentation->timestamp[query_renderPassEnd];
		uint64_t const t_renderPassBegin0 = presentation->timestamp[query_renderPassBegin0];
		uint64_t const dt_renderPass = (t_renderPassEnd - t_renderPassBegin);
		uint64_t const dt_present = (t_renderPassBegin - t_renderPassBegin0);
		uint64_t const dt_cap = 33333333; // 2x 60Hz interval

		if (gCount(dt_renderPass, dt_cap))
		{
			++presentation->frameCount;
			presentation->dt_renderPass_total += dt_renderPass;
		}
		else if (presentation->frameCount && dt_renderPass)
		{
			printf("\n DROPPED RENDER PASS: %llu", dt_renderPass);
		}

		if (gCount(dt_present, dt_cap))
		{
			++presentation->presentCount;
			presentation->dt_present_total += dt_present;
		}
		else if (presentation->presentCount && dt_present)
		{
			printf("\n DROPPED PRESENTATION: %llu", dt_present);
		}

		cdraw_assert(query == (frameInFlight[idx] * cdrawVkQuery_max));
		printf("\n frame %u, image %u, dt_renderPass=%.6f, dt_present=%.6f",
			frameInFlight[idx], imageInFlight[idx],
			((fp32_t)dt_renderPass * timestamp_nanosPerCount * 1.0e-6f),
			((fp32_t)dt_present * timestamp_nanosPerCount * 1.0e-6f));
	}

	// print averages
	if (presentation->presentCount > (cdrawVkQuery_max + 1))
	{
		printf("\n frameCount %u, presentCount %u, dt_renderPass=%.6f, dt_present=%.6f",
			presentation->frameCount, presentation->presentCount,
			(((fp32_t)(presentation->dt_renderPass_total)* timestamp_nanosPerCount / (fp32_t)(presentation->frameCount)) * 1.0e-6f),
			(((fp32_t)(presentation->dt_present_total)* timestamp_nanosPerCount / (fp32_t)(presentation->presentCount)) * 1.0e-6f));
	}
#endif // #if CDRAW_DEBUG

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
