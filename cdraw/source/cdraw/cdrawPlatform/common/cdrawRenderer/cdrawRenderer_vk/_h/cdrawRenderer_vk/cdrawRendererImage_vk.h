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
	/// Constructor for Vulkan image descriptor.
	/// </summary>
	/// <param name="image_out">Target image descriptor (non-null).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="image">Vulkan image handle.</param>
	/// <returns>Success: <paramref name="image_out"/>; Failure: <c>NULL</c>.</returns>
	cdrawVkImage* cdrawVkImageCtor(cdrawVkImage* const image_out,
		label_t const name, VkImage const image, VkDeviceMemory const imageMem, VkImageView const imageView);

	/// <summary>
	/// Destructor interface for Vulkan image descriptor.
	/// </summary>
	/// <param name="image_out">Target image descriptor (non-null).</param>
	void cdrawVkImageDtor(cdrawVkImage* const image_out);

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
	/// <param name="device">Device handle.</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkImageCreate(cdrawVkImage* const image_out,
		label_t const name, VkDevice const device, VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Destroy image and dependencies.
	/// </summary>
	/// <param name="image_out">Target image descriptor (non-null and valid).</param>
	/// <param name="device">Device handle.</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if destroyed.</returns>
	bool cdrawVkImageDestroy(cdrawVkImage* const image_out,
		VkDevice const device, VkAllocationCallbacks const* const alloc_opt);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if ((!defined _CDRAW_RENDERER_IMAGE_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))