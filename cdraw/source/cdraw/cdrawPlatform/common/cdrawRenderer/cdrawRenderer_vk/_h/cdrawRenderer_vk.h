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
* cdrawRenderer_vk.h
* Root header for Vulkan renderer implementation.
*/

#ifndef _CDRAW_RENDERER_VK_H_
#define _CDRAW_RENDERER_VK_H_

#include "vulkan/vulkan.h"

#include "cdraw/cdraw/cdrawConfig.h"

#if CDRAW_DEBUG
#include "cdrawRenderer_vk/cdrawRendererDebug_vk.h"
#endif // #if CDRAW_DEBUG
#include "cdrawRenderer_vk/cdrawRendererMemory_vk.h"
#include "cdrawRenderer_vk/cdrawRendererInstance_vk.h"
#include "cdrawRenderer_vk/cdrawRendererDevice_vk.h"
#include "cdrawRenderer_vk/cdrawRendererImage_vk.h"
#include "cdrawRenderer_vk/cdrawRendererFramebuffer_vk.h"
#include "cdrawRenderer_vk/cdrawRendererPresentation_vk.h"


/// <summary>
/// Vulkan renderer data (platform-agnostic).
/// </summary>
typedef struct cdrawRenderer_vk
{
	/// <summary>
	/// Vulkan handles and data related to instance, logical device and global usage.
	/// </summary>
	struct {
		/// <summary>
		/// Vulkan instance.
		/// </summary>
		cdrawVkInstance instance;

		/// <summary>
		/// Vulkan logical device.
		/// </summary>
		cdrawVkLogicalDevice logicalDevice;

		/// <summary>
		/// Vulkan command pool.
		/// </summary>
		cdrawVkCommandPool commandPool;

		/// <summary>
		/// Allocator data for callbacks.
		/// </summary>
		cdrawVkAllocator allocator;

#if CDRAW_DEBUG
		/// <summary>
		/// Debugging extensions and callbacks.
		/// </summary>
		cdrawVkDebug debug;
#endif // #if CDRAW_DEBUG
	};

	/// <summary>
	/// Vulkan handles and data related to presentation.
	/// </summary>
	struct {
		/// <summary>
		/// Vulkan presentation surface.
		/// </summary>
		cdrawVkSurface surface[cdrawVkSurfacePresent_max];

		/// <summary>
		/// Presentation data.
		/// </summary>
		cdrawVkPresentation presentation[cdrawVkSurfacePresent_max];

		/// <summary>
		/// Presentation queue.
		/// </summary>
		cdrawVkQueue queue_present;

		/// <summary>
		/// Number of swapchains for presentation.
		/// </summary>
		uint32_t swapchainCount_present;

		/// <summary>
		/// Number of semaphores on which to wait for presentation.
		/// </summary>
		uint32_t waitSemaphoreCount_present;

		/// <summary>
		/// Semaphores on which to wait for presentation.
		/// </summary>
		VkSemaphore waitSemaphore_present[cdrawVkSurfacePresent_max];

		/// <summary>
		/// Swapchains for presentation.
		/// </summary>
		VkSwapchainKHR swapchain_present[cdrawVkSurfacePresent_max];

		/// <summary>
		/// Swapchain image indices for presentation.
		/// </summary>
		uint32_t imageIndex_present[cdrawVkSurfacePresent_max];

#if CDRAW_DEBUG
		/// <summary>
		/// Timer submission queue.
		/// </summary>
		cdrawVkQueue queue_timer;

		/// <summary>
		/// Counts-per-second of CPU.
		/// </summary>
		uint64_t cps_cpu;

		/// <summary>
		/// Counts-per-second of GPU.
		/// </summary>
		uint64_t cps_gpu;

		/// <summary>
		/// Ratio of GPU to CPU rates.
		/// </summary>
		uint64_t cps_gpu2cpu;

		/// <summary>
		/// Remainder of ratio.
		/// </summary>
		uint64_t cps_gpu2cpu_rem;
#endif // #if CDRAW_DEBUG
	};
} cdrawRenderer_vk;


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus



#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef _CDRAW_RENDERER_VK_H_