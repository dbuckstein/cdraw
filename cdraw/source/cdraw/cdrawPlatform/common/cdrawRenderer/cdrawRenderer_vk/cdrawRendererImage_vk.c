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
* cdrawRendererImage_vk.c
* Internal source for Vulkan image management.
*/

#include "_h/cdrawRenderer_vk.h"
#include "_h/cdrawRenderer_vk/cdrawRendererImage_vk.h"


/******************************************************************************
* Public implementations.
******************************************************************************/

cdrawVkImage* cdrawVkImageCtor(cdrawVkImage* const image_out,
	label_t const name, VkImage const image, VkDeviceMemory const imageMem, VkImageView const imageView)
{
	failassertret(image_out, NULL);
	failassertret(image && imageMem && imageView, NULL);
	label_copy_safe(image_out->name, name);
	image_out->image = image;
	image_out->imageMem = imageMem;
	image_out->imageView = imageView;
	return image_out;
}

void cdrawVkImageDtor(cdrawVkImage* const image_out)
{
	failassertret(image_out);
	image_out->imageView = VK_NULL_HANDLE;
	image_out->imageMem = VK_NULL_HANDLE;
	image_out->image = VK_NULL_HANDLE;
	label_init(image_out->name);
}

bool cdrawVkImageValid(cdrawVkImage const* const image)
{
	cdraw_assert(image);
	return (image->image && image->imageMem && image->imageView);
}

bool cdrawVkImageUnused(cdrawVkImage const* const image)
{
	cdraw_assert(image);
	return (!image->image && !image->imageMem && !image->imageView);
}

bool cdrawVkImageCreate(cdrawVkImage* const image_out,
	label_t const name, VkDevice const device, VkAllocationCallbacks const* const alloc_opt)
{
	failassertret(image_out && cdrawVkImageUnused(image_out), false);
	failassertret(device, false);

	// ****TO-DO

	cdraw_assert(cdrawVkImageValid(image_out));
	return true;
}

bool cdrawVkImageDestroy(cdrawVkImage* const image_out,
	VkDevice const device, VkAllocationCallbacks const* const alloc_opt)
{
	failassertret(image_out, false);
	failassertret(device, false);
	if (cdrawVkImageUnused(image_out))
		return false;

	if (image_out->imageView)
	{
		vkDestroyImageView(device, image_out->imageView, alloc_opt);
		image_out->imageView = VK_NULL_HANDLE;
	}
	if (image_out->imageMem)
	{
		vkFreeMemory(device, image_out->imageMem, alloc_opt);
		image_out->imageMem = VK_NULL_HANDLE;
	}
	if (image_out->image)
	{
		vkDestroyImage(device, image_out->image, alloc_opt);
		image_out->image = VK_NULL_HANDLE;
	}
	label_init(image_out->name);
	return true;
}
