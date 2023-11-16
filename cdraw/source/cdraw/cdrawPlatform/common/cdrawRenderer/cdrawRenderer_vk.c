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
#include "cdraw/cdrawPlatform/cdrawTimer.h"


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
// Adapted from https://gist.github.com/cdwfs/4222ca09cb259f8dd50f7f2cf7d09179
static bool cdrawRendererSyncClocks_vk(
	VkDevice const logicalDevice,
	VkCommandBuffer const commandBuffer,
	VkQueue const queue,
	VkFence const fence,
	VkEvent const event_begin,
	VkEvent const event_end,
	VkQueryPool const queryPool,
	uint64_t* const t_cpu_out,
	uint64_t* const t_gpu_out,
	uint64_t* const dt_error_cpu_out,
	uint64_t* const dt_error_gpu_out)
{
	VkResult result = VK_SUCCESS;
	uint64_t t_cpu[2], t_gpu[2];
	VkCommandBufferBeginInfo const commandBufferBeginInfo = cdrawVkCommandBufferBeginInfoCtor(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, NULL);
	VkSubmitInfo const submitInfo = cdrawVkSubmitInfoCtor(0, NULL, NULL, 1, &commandBuffer, 0, NULL);

	uint32_t const queryBase = cdrawVkQuery_presentBegin;
	uint32_t const queryCount = (1 + cdrawVkQuery_presentEnd - cdrawVkQuery_presentBegin);

	// reset sync objects and begin recording
	result = vkResetFences(logicalDevice, 1, &fence);
	cdraw_assert(result == VK_SUCCESS);
	result = vkResetEvent(logicalDevice, event_begin);
	cdraw_assert(result == VK_SUCCESS);
	result = vkResetEvent(logicalDevice, event_end);
	cdraw_assert(result == VK_SUCCESS);
	result = vkResetCommandBuffer(commandBuffer, 0);
	cdraw_assert(result == VK_SUCCESS);
	result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
	cdraw_assert(result == VK_SUCCESS);

	// reset queries
	vkCmdResetQueryPool(commandBuffer, queryPool, queryBase, queryCount);

	// wait for first event
	vkCmdWaitEvents(commandBuffer, 1, &event_begin, VK_PIPELINE_STAGE_HOST_BIT, (VK_PIPELINE_STAGE_HOST_BIT | VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT), 0, NULL, 0, NULL, 0, NULL);

	// write first timestamp
	vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, queryPool, cdrawVkQuery_presentBegin);

	// wait for second event
	vkCmdWaitEvents(commandBuffer, 1, &event_end, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, NULL, 0, NULL, 0, NULL);

	// write second timestamp
	vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, queryPool, cdrawVkQuery_presentEnd);

	// end recording and submit
	result = vkEndCommandBuffer(commandBuffer);
	cdraw_assert(result == VK_SUCCESS);
	result = vkQueueSubmit(queue, 1, &submitInfo, fence);
	cdraw_assert(result == VK_SUCCESS);

	// get begin time and set end event
	result = vkSetEvent(logicalDevice, event_begin);
	cdraw_assert(result == VK_SUCCESS);
	cdrawTimerGetSystemTime(t_cpu + 0);

	// get end time and set end event
	result = vkSetEvent(logicalDevice, event_end);
	cdraw_assert(result == VK_SUCCESS);
	cdrawTimerGetSystemTime(t_cpu + 1);

	// wait for GPU to finish
	result = vkWaitForFences(logicalDevice, 1, &fence, VK_TRUE, UINT64_MAX);
	cdraw_assert(result == VK_SUCCESS);

	// get GPU results
	result = vkGetQueryPoolResults(logicalDevice, queryPool, queryBase, queryCount,
		sizeof(t_gpu), t_gpu, sizeof(*t_gpu), (VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT));

	// output all
	*t_cpu_out = (t_cpu[0]);
	*dt_error_cpu_out = (t_cpu[1] - t_cpu[0]);
	*t_gpu_out = (t_gpu[0]);
	*dt_error_gpu_out = (t_gpu[1] - t_gpu[0]);

	return (result == VK_SUCCESS);
}

static bool cdrawRendererDisplayTest_vk(
	VkCommandBuffer const commandBuffer,
	VkQueryPool const queryPool,
	VkRenderPass const renderPass,
	VkFramebuffer const framebuffer,
	VkRect2D const region)
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
	static fp32_t const s_clear = 0.5f;
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

	VkRenderPassBeginInfo const renderPassBegin = cdrawVkRenderPassBeginInfoCtor(renderPass, framebuffer, region, buffer_len(clearValue), clearValue);

	VkResult result = VK_SUCCESS;
	uint32_t const queryBase = cdrawVkQuery_renderPassBegin;
	uint32_t const queryCount = (1 + cdrawVkQuery_renderPassEnd - cdrawVkQuery_renderPassBegin);

	clearValue[0].color = clearColor[((clearColorIdx++) / rateToCount) % buffer_len(clearColor)];
	{
		clearValue[0].color.float32[0] *= s_clear;
		clearValue[0].color.float32[1] *= s_clear;
		clearValue[0].color.float32[2] *= s_clear;
		clearValue[0].color.float32[3] *= s_clear;
	}
	clearValue[1].depthStencil = clearDepthStencil;

#if CDRAW_DEBUG
	vkCmdResetQueryPool(commandBuffer, queryPool, queryBase, queryCount);
	vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, queryPool, cdrawVkQuery_renderPassBegin);
#endif // #if CDRAW_DEBUG

	vkCmdBeginRenderPass(commandBuffer, &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE);

	// ...

	vkCmdEndRenderPass(commandBuffer);
	
#if CDRAW_DEBUG
	vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, queryPool, cdrawVkQuery_renderPassEnd);
#endif // #if CDRAW_DEBUG

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

static bool cdrawRendererPresent_vk(VkQueue const queue,
	uint32_t const waitSemaphoreCount, VkSemaphore const waitSemaphores[/*waitSemaphoreCount*/],
	uint32_t const swapchainCount, VkSwapchainKHR const swapchains[/*swapchainCount*/], uint32_t const imageIndices[/*swapchainCount*/])
{
	uint32_t idx;
	VkResult result_swapchain[cdrawVkSurfacePresent_max] = { VK_SUCCESS };
	VkPresentInfoKHR const presentInfo = cdrawVkPresentInfoCtor(
		waitSemaphoreCount, waitSemaphores,
		swapchainCount, swapchains, imageIndices, result_swapchain);
	VkResult result = vkQueuePresentKHR(queue, &presentInfo);
	if (result != VK_SUCCESS)
	{
		// handle presentation error
		for (idx = 0; idx < swapchainCount; ++idx)
		{
			result = result_swapchain[idx];
			cdraw_assert(result == VK_SUCCESS);
		}
		return false;
	}
	return true;
}

static bool cdrawRendererPrepareNextPresent_vk(cdrawRenderer_vk* const r, uint32_t const windowIndex)
{
	VkResult result = VK_SUCCESS;
	cdrawVkPresentation* presentation = NULL;
	uint32_t frameUsingImage = UINT32_MAX;
	uint32_t imageUsedByFrame = UINT32_MAX;
	uint32_t imageExpect = UINT32_MAX;

	VkDevice logicalDevice = VK_NULL_HANDLE;
	VkFence fence_submit = VK_NULL_HANDLE;
	VkFence fence_acquire = VK_NULL_HANDLE;
	VkSemaphore semaphore_acquire = VK_NULL_HANDLE;

#if CDRAW_DEBUG
	uint64_t t_renderPassBegin_prev = 0;
	uint64_t t_renderPassEnd_prev = 0;
	uint64_t t_renderPassBegin_prevCycle = 0;
	uint64_t t_renderPassEnd_prevCycle = 0;
	int64_t dt_renderPassBegin_prev = 0;
	int64_t dt_renderPassEnd_prev = 0;
	uint64_t* timestamp_prev = NULL;
	uint64_t* timestamp_prevCycle = NULL;
	uint32_t queryBase = 0;
	uint32_t queryCount = 0;
	VkQueryPool queryPool = VK_NULL_HANDLE;
	fp32_t timestamp_nsPerCount = 0.0f;
	fp32_t timestamp_msPerCount = 0.0f;
#endif // #if CDRAW_DEBUG

	cdraw_assert(r);
	presentation = &r->presentation[windowIndex];
	if (!cdrawVkPresentationValid(presentation))
		return false;

	// device
	logicalDevice = r->logicalDevice.logicalDevice;

#if CDRAW_DEBUG
	// previous timestamps
	timestamp_prev = presentation->timestamp[presentation->frame];
	t_renderPassBegin_prev = timestamp_prev[cdrawVkQuery_renderPassBegin];
	t_renderPassEnd_prev = timestamp_prev[cdrawVkQuery_renderPassEnd];
#endif // #if CDRAW_DEBUG

	// next
	presentation->frame = (presentation->frame + 1) % cdrawFramesInFlight_max;

#if CDRAW_DEBUG
	// current frame
	printf("\n frame %u", presentation->frame);
#endif // #if CDRAW_DEBUG
	
	// (private - at end of public display and end of creation)
	//	-> reset image acquire fence
	//	-> acquire image
	//	-> proceed when fence signals
	//		 - create list of swapchains above and wait semaphores

	// wait for submission of current frame to complete
	// putting this here avoids "pending" state clash for submission
	imageUsedByFrame = presentation->imageIdx_frame[presentation->frame];
	if (uint32_valid(imageUsedByFrame))
	{
		fence_submit = presentation->fence_submit[presentation->frame];
		result = vkWaitForFences(logicalDevice, 1, &fence_submit, VK_TRUE, UINT64_MAX);
		cdraw_assert(result == VK_SUCCESS);

#if CDRAW_DEBUG
		// if N is frames in flight, timestamps 2N frames ago
		timestamp_prevCycle = presentation->timestamp[presentation->frame];
		t_renderPassBegin_prevCycle = timestamp_prevCycle[cdrawVkQuery_renderPassBegin];
		t_renderPassEnd_prevCycle = timestamp_prevCycle[cdrawVkQuery_renderPassEnd];

		// timestamps previously in this slot, or N frames ago
		queryPool = presentation->queryPool_frame[presentation->frame];
		queryBase = cdrawVkQuery_renderPassBegin;
		queryCount = (1 + cdrawVkQuery_renderPassEnd - cdrawVkQuery_renderPassBegin);
		result = vkGetQueryPoolResults(logicalDevice, queryPool, queryBase, queryCount,
			(queryCount * sizeof(*timestamp_prevCycle)), &timestamp_prevCycle[queryBase], sizeof(*timestamp_prevCycle),
			(VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT));
		cdraw_assert(result == VK_SUCCESS);

		timestamp_prevCycle = presentation->timestamp[(presentation->frame + cdrawFramesInFlight_max - 2) % cdrawFramesInFlight_max];
		t_renderPassBegin_prevCycle = timestamp_prevCycle[cdrawVkQuery_renderPassBegin];
		t_renderPassEnd_prevCycle = timestamp_prevCycle[cdrawVkQuery_renderPassEnd];
		dt_renderPassBegin_prev = (t_renderPassBegin_prev - t_renderPassBegin_prevCycle);
		dt_renderPassEnd_prev = (t_renderPassEnd_prev - t_renderPassEnd_prevCycle);
		timestamp_nsPerCount = r->logicalDevice.physicalDevice.physicalDeviceProp.limits.timestampPeriod;
		timestamp_msPerCount = timestamp_nsPerCount * 1.0e-6f;

		if (dt_renderPassBegin_prev >= 0)
		{
			++presentation->presentCount;
			presentation->dt_present_total += dt_renderPassBegin_prev;
			printf(" | dt_renderPassBegin_prev=%.6f",
				timestamp_msPerCount * (fp32_t)dt_renderPassBegin_prev);
		}
#endif // #if CDRAW_DEBUG
	}

	fence_acquire = presentation->fence_acquire[presentation->frame];
	semaphore_acquire = presentation->semaphore_acquire[presentation->frame];
	imageExpect = (presentation->image + 1) % presentation->swapchain.imageCount;
	result = vkAcquireNextImageKHR(logicalDevice, presentation->swapchain.swapchain, UINT64_MAX, semaphore_acquire, fence_acquire, &presentation->image);
	cdraw_assert(result == VK_SUCCESS);
	cdraw_assert(presentation->image == imageExpect);

	frameUsingImage = presentation->frameIdx_image[presentation->image];
	if (uint32_valid(frameUsingImage))
	{
		fence_submit = presentation->fence_submit[frameUsingImage];
		result = vkWaitForFences(logicalDevice, 1, &fence_submit, VK_TRUE, UINT64_MAX);
		cdraw_assert(result == VK_SUCCESS);
	}

	// ensure image has been acquired
	result = vkWaitForFences(logicalDevice, 1, &fence_acquire, VK_TRUE, UINT64_MAX);
	cdraw_assert(result == VK_SUCCESS);
	result = vkResetFences(logicalDevice, 1, &fence_acquire);
	cdraw_assert(result == VK_SUCCESS);

	// set frame-image pairs
	presentation->frameIdx_image[presentation->image] = presentation->frame;
	presentation->imageIdx_frame[presentation->frame] = presentation->image;

	// reset
	r->swapchainCount_present = 0;
	r->waitSemaphoreCount_present = 0;

	return true;
}

static result_t cdrawRendererBeginDraw_vk(cdrawRenderer_vk* const r, uint32_t const windowIndex)
{
	VkResult result = VK_SUCCESS;
	cdrawVkPresentation* presentation = NULL;

	uint32_t frame = UINT32_MAX;
	VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
	VkCommandBufferBeginInfo const commandBufferBegin = cdrawVkCommandBufferBeginInfoCtor(0, NULL);

	cdraw_assert(r);
	presentation = &r->presentation[windowIndex];
	if (!cdrawVkPresentationValid(presentation))
		return result;

	// prepare for next presentation with this window
	result = cdrawRendererPrepareNextPresent_vk(r, windowIndex);

	// reset and begin command recording
	frame = presentation->frame;
	commandBuffer = presentation->commandBuffer_present.commandBuffer[frame];
	result = vkResetCommandBuffer(commandBuffer, 0);
	cdraw_assert(result == VK_SUCCESS);
	result = vkBeginCommandBuffer(commandBuffer, &commandBufferBegin);
	cdraw_assert(result == VK_SUCCESS);

#if CDRAW_DEBUG
	// TEST DRAW PRIOR TO PRESENTATION
	{
		uint32_t const image = presentation->image;
		VkQueryPool const queryPool = presentation->queryPool_frame[frame];
		VkQueue const queue = presentation->queue_graphics[frame].queue;
		VkRenderPass const renderPass = presentation->renderPass_present.renderPass;
		VkFramebuffer const framebuffer = presentation->framebuffer_present[image].framebuffer; // match image because the targets are attached to it
		VkRect2D const region = presentation->framebuffer_present[image].region;
		cdrawRendererDisplayTest_vk(commandBuffer, queryPool, renderPass, framebuffer, region);
	}
#endif // #if CDRAW_DEBUG

	return result;
}

static result_t cdrawRendererEndDraw_vk(cdrawRenderer_vk* const r, uint32_t const windowIndex)
{
	VkResult result = VK_SUCCESS;
	cdrawVkPresentation* presentation = NULL;

	uint32_t idx;
	uint32_t frame = UINT32_MAX;
	VkDevice logicalDevice = VK_NULL_HANDLE;
	VkQueue queue_submit = VK_NULL_HANDLE;
	VkFence fence_submit = VK_NULL_HANDLE;
	VkPipelineStageFlags semaphoreStage_wait[cdrawSubmitWaitSemaphore_max] = { VK_PIPELINE_STAGE_NONE_KHR };
	VkSemaphore semaphore_wait[cdrawSubmitWaitSemaphore_max] = { VK_NULL_HANDLE };
	VkSemaphore semaphore_signal[cdrawSubmitSignalSemaphore_max] = { VK_NULL_HANDLE };
	VkCommandBuffer commandBuffer[cdrawSubmitCommandBuffer_max] = { VK_NULL_HANDLE };
	uint32_t semaphoreCount_wait = 0;
	uint32_t semaphoreCount_signal = 0;
	uint32_t commandBufferCount = 0;
	VkSubmitInfo submitInfo;

	cdraw_assert(r);
	presentation = &r->presentation[windowIndex];
	if (!cdrawVkPresentationValid(presentation))
		return result;

	logicalDevice = r->logicalDevice.logicalDevice;
	frame = presentation->frame;

	// end command buffer recording
	commandBuffer[commandBufferCount] = presentation->commandBuffer_present.commandBuffer[frame];
	result = vkEndCommandBuffer(commandBuffer[commandBufferCount++]);
	cdraw_assert(result == VK_SUCCESS);

	// reset fence for this frame
	fence_submit = presentation->fence_submit[frame];
	result = vkResetFences(logicalDevice, 1, &fence_submit);
	cdraw_assert(result == VK_SUCCESS);
	
	// SUBMISSION
	queue_submit = presentation->queue_graphics[frame].queue;
	semaphoreStage_wait[semaphoreCount_wait] = (VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
	semaphore_wait[semaphoreCount_wait++] = presentation->semaphore_acquire[frame];
	semaphore_signal[semaphoreCount_signal++] = presentation->semaphore_submit[frame];
	submitInfo = cdrawVkSubmitInfoCtor(semaphoreCount_wait, semaphore_wait, semaphoreStage_wait,
		commandBufferCount, commandBuffer, semaphoreCount_signal, semaphore_signal);
	result = vkQueueSubmit(queue_submit, 1, &submitInfo, fence_submit);
	cdraw_assert(result == VK_SUCCESS);

	// copy data for display
	r->imageIndex_present[r->swapchainCount_present] = presentation->image;
	r->swapchain_present[r->swapchainCount_present++] = presentation->swapchain.swapchain;
	for (idx = 0; idx < semaphoreCount_signal; ++idx)
		r->waitSemaphore_present[r->waitSemaphoreCount_present++] = semaphore_signal[idx];

	return result;
}

#if CDRAW_DEBUG
static bool cdrawRendererPresentDebug_vk(cdrawRenderer_vk* const r, uint32_t const idx)
{
	VkResult result = VK_SUCCESS;
	uint32_t frame;
	cdrawVkPresentation* presentation;
	uint32_t const queryBase = cdrawVkQuery_renderPassBegin;
	uint32_t const queryCount = (1 + cdrawVkQuery_renderPassEnd - cdrawVkQuery_renderPassBegin);

	VkDevice logicalDevice = VK_NULL_HANDLE;
	VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
	fp32_t timestamp_nsPerCount = 0.0f;
	fp32_t timestamp_msPerCount = 0.0f;
	VkQueryPool queryPool;
	uint64_t* timestamp;
	//size_t timestampSize;

	uint64_t t_cpu = 0, t_gpu = 0, dt_error_cpu = 0, dt_error_gpu = 0;

	cdraw_assert(r);
	presentation = &r->presentation[idx];
	if (!cdrawVkPresentationValid(presentation))
		return false;

	logicalDevice = r->logicalDevice.logicalDevice;
	commandBuffer = presentation->commandBuffer_timer.commandBuffer[0];
	timestamp_nsPerCount = r->logicalDevice.physicalDevice.physicalDeviceProp.limits.timestampPeriod;
	timestamp_msPerCount = timestamp_nsPerCount * 1.0e-6f;

	// get times in current slot
	presentation = &r->presentation[idx];
	frame = presentation->frame;
	queryPool = presentation->queryPool_frame[frame];
	timestamp = presentation->timestamp[frame];
	//timestampSize = sizeof(*timestamp);
	//result = vkGetQueryPoolResults(logicalDevice, queryPool, queryBase, queryCount,
	//	(timestampSize * queryCount), timestamp, timestampSize, (VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT));
	//cdraw_assert(result == VK_SUCCESS);

	// sync
	{
		cdrawRendererSyncClocks_vk(logicalDevice, commandBuffer, presentation->queue_graphics[frame].queue,
			presentation->fence_timer, presentation->event_timer_start, presentation->event_timer_stop,
			queryPool, &t_cpu, &t_gpu, &dt_error_cpu, &dt_error_gpu);
		t_cpu *= r->cps_gpu2cpu;
		dt_error_cpu *= r->cps_gpu2cpu;
		t_gpu *= (uint64_t)timestamp_nsPerCount;
		dt_error_gpu *= (uint64_t)timestamp_nsPerCount;
		timestamp[queryCount + 0] = t_cpu;
		timestamp[queryCount + 1] = t_gpu;
	}

	// display queries
	{
		uint32_t const frame_prev = (frame + cdrawFramesInFlight_max - 1) % cdrawFramesInFlight_max;
		uint64_t const t_renderPassBegin = presentation->timestamp[frame][cdrawVkQuery_renderPassBegin];
		uint64_t const t_renderPassEnd = presentation->timestamp[frame][cdrawVkQuery_renderPassEnd];
		uint64_t const t_renderPassBegin_prev = presentation->timestamp[frame_prev][cdrawVkQuery_renderPassBegin];
		uint64_t const t_renderPassEnd_prev = presentation->timestamp[frame_prev][cdrawVkQuery_renderPassEnd];
		uint64_t const t_cpu_prev = presentation->timestamp[frame_prev][queryCount + 0];
		uint64_t const t_gpu_prev = presentation->timestamp[frame_prev][queryCount + 1];
		int64_t const dt_cpu = (t_cpu - t_cpu_prev);
		int64_t const dt_gpu = (t_gpu - t_gpu_prev);
		int64_t const dt_renderPass = (t_renderPassEnd - t_renderPassBegin);
		int64_t const dt_renderPassBegin = (t_renderPassBegin - t_renderPassBegin_prev);
		int64_t const dt_renderPassEnd = (t_renderPassEnd - t_renderPassEnd_prev);

		//// check if sync is correct
		//// looks like the key is waiting for the fence before querying times
		////	...but doing it here will prevent multiple frames in flight
		//// this should be used conservatively for debugging only
		//cdraw_assert(dt_renderPass >= 0 && dt_renderPassBegin >= 0 && dt_renderPassEnd >= 0);

		//// print stats for current present
		//{
		//	printf("\n image %u", presentation->image);
		//	++presentation->frameCount;
		//	presentation->dt_renderPass_total += dt_renderPass;
		//	printf(" | dt_renderPass=%.6f",
		//		timestamp_msPerCount * (fp32_t)dt_renderPass);
		//	printf("\n\t dt_cpu=%.6f | dt_error_cpu=%.6f",
		//		timestamp_msPerCount * (fp32_t)dt_cpu,
		//		timestamp_msPerCount * (fp32_t)dt_error_cpu);
		//	printf("\n\t dt_gpu=%.6f | dt_error_gpu=%.6f",
		//		timestamp_msPerCount * (fp32_t)dt_gpu,
		//		timestamp_msPerCount * (fp32_t)dt_error_gpu);
		//}

		// print averages
		printf("\n frameCount %u, presentCount %u", presentation->frameCount, presentation->presentCount);
		if (presentation->frameCount)
			printf(" | dt_renderPass_avg=%.6f", timestamp_msPerCount * ((fp32_t)(presentation->dt_renderPass_total) / (fp32_t)(presentation->frameCount)));
		if (presentation->presentCount)
			printf(" | dt_present_avg=%.6f", timestamp_msPerCount * ((fp32_t)(presentation->dt_present_total) / (fp32_t)(presentation->presentCount)));
	}

	return (result == VK_SUCCESS);
}
#endif // #if CDRAW_DEBUG

static result_t cdrawRendererDisplay_vk(cdrawRenderer_vk* const r)
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
	// could have one submission loop per swapchain
	// potentially facilitates multiple displays
	//	-> acquire image for each
	//	-> prepare queues for each
	//	-> submit for each

	VkResult result = VK_SUCCESS;
	uint32_t idx;

	// all at once:
	//	-> execute current display: queue present (begin display)
	cdrawRendererPresent_vk(r->queue_present.queue, r->waitSemaphoreCount_present, r->waitSemaphore_present, r->swapchainCount_present, r->swapchain_present, r->imageIndex_present);

	// per swapchain/display: either after present and after init, or in begin draw
	//	-> prepare next display
	for (idx = 0; idx < cdrawVkSurfacePresent_max; ++idx)
	{
#if CDRAW_DEBUG
		cdrawRendererPresentDebug_vk(r, idx);
#endif // #if CDRAW_DEBUG
		//cdrawRendererPrepareNextPresent_vk(r, idx);
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
			result = cdrawVkPresentationCreate(presentation, "cdrawVkPresentation", &r->logicalDevice, &r->commandPool, &r->surface[windowIndex], windowIndex, alloc_opt);
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
		result = cdrawVkPresentationCreate(presentation, "cdrawVkPresentation", &r->logicalDevice, &r->commandPool, surface, windowIndex, alloc_opt);
		failassertret(result, result_seterror(errcode_renderer_init));
	}

	//// prepare for initial presentation
	//result = cdrawRendererPrepareNextPresent_vk(r, windowIndex);
	//failassertret(result, result_seterror(errcode_renderer_init));

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

result_t cdrawRendererCreate_vk(cdrawRenderer* const renderer, uint32_t const surfaceCountMax)
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

	// additional device objects
	if (cdrawVkLogicalDeviceValid(&r->logicalDevice))
	{
		// grab present queue
		uint32_t queueIdx = 0;
		VkQueue queue = VK_NULL_HANDLE;
		vkGetDeviceQueue(r->logicalDevice.logicalDevice, r->logicalDevice.queueFamilyIdx_graphics, queueIdx++, &queue);
		cdraw_assert(queue);
		cdrawVkQueueCtor(&r->queue_present, "cdrawVkQueue_present", queue);

#if CDRAW_DEBUG
		// grab timer queue
		queue = VK_NULL_HANDLE;
		vkGetDeviceQueue(r->logicalDevice.logicalDevice, r->logicalDevice.queueFamilyIdx_graphics, queueIdx++, &queue);
		cdraw_assert(queue);
		cdrawVkQueueCtor(&r->queue_timer, "cdrawVkQueue_timer", queue);

		// set other values
		cdrawTimerGetSystemFrequency(&r->cps_cpu);
		r->cps_gpu = 1000000000; // multiply times by their device's period to get to nanoseconds
		r->cps_gpu2cpu = r->cps_gpu / r->cps_cpu;
		r->cps_gpu2cpu_rem = r->cps_gpu % r->cps_cpu;
		cdraw_assert(r->cps_gpu2cpu_rem == 0);
#else // #if CDRAW_DEBUG
		++queueIdx;
#endif // #else // #if CDRAW_DEBUG
		cdraw_assert(queueIdx == cdrawGraphicsQueuesReserve);
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

	// surfaces and presentation setups
	for (i = 0; i < cdrawVkSurfacePresent_max; ++i)
	{
		result_t result_detach = cdrawRendererDetachWindow_vk(r, i);
		failassertret(result_isclean(result_detach), result_seterror(errcode_renderer_init));
	}

#if CDRAW_DEBUG
	// timer queue
	cdrawVkQueueDtor(&r->queue_timer);
#endif // #if CDRAW_DEBUG

	// presentation queue
	cdrawVkQueueDtor(&r->queue_present);

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
	gRendererFuncTable_vk.cdrawRendererBeginDraw = cdrawRendererBeginDraw_vk;
	gRendererFuncTable_vk.cdrawRendererEndDraw = cdrawRendererEndDraw_vk;
	gRendererFuncTable_vk.cdrawRendererDisplay = cdrawRendererDisplay_vk;
	gRendererFuncTable_vk.cdrawRendererResize = cdrawRendererResize_vk;
	gRendererFuncTable_vk.cdrawRendererAttachWindow = cdrawRendererAttachWindow_vk;
	gRendererFuncTable_vk.cdrawRendererDetachWindow = cdrawRendererDetachWindow_vk;
	gRendererFuncTable_vk.cdrawRendererWindowsSupported = cdrawRendererWindowCountMax_vk;

	result_return();
}
