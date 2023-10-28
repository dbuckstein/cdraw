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
* cdrawDevice_vk.h
* Facade interface for Vulkan device management.
*/

#ifndef _CDRAW_DEVICE_VK_H_
#define _CDRAW_DEVICE_VK_H_


#include "cdraw/cdraw/cdrawConfig.h"
#include "vulkan/vulkan.h"


/// <summary>
/// Vulkan logical device descriptor.
/// </summary>
typedef struct cdrawVkLogicalDevice
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Vulkan logical device handle.
	/// </summary>
	VkDevice logicalDevice;
} cdrawVkLogicalDevice;


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Constructor for Vulkan logical device descriptor.
	/// </summary>
	/// <param name="logicalDevice_out">Target logical device descriptor (non-null).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="logicalDevice">Vulkan logical device handle.</param>
	/// <returns>Success: <paramref name="logicalDevice_out"/>; Failure: <c>NULL</c>.</returns>
	cdrawVkLogicalDevice* cdrawVkLogicalDeviceCtor(cdrawVkLogicalDevice* const logicalDevice_out,
		label_t const name, VkDevice const logicalDevice);

	/// <summary>
	/// Destructor interface for Vulkan logical device descriptor.
	/// </summary>
	/// <param name="logicalDevice_out">Target logical device descriptor (non-null).</param>
	void cdrawVkLogicalDeviceDtor(cdrawVkLogicalDevice* const logicalDevice_out);

	/// <summary>
	/// Indicate whether descriptor is valid (set up correctly) and should not be modified.
	/// </summary>
	/// <param name="logicalDevice">Descriptor (non-null).</param>
	/// <returns>True if valid.</returns>
	bool cdrawVkLogicalDeviceValid(cdrawVkLogicalDevice const* const logicalDevice);

	/// <summary>
	/// Indicate whether descriptor is unused (not set up) and can be modified.
	/// </summary>
	/// <param name="logicalDevice">Descriptor (non-null).</param>
	/// <returns>True if unused.</returns>
	bool cdrawVkLogicalDeviceUnused(cdrawVkLogicalDevice const* const logicalDevice);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef _CDRAW_DEVICE_VK_H_