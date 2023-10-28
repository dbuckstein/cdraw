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
* cdrawInstance_vk.h
* Facade interface for Vulkan instance management.
*/

#ifndef _CDRAW_INSTANCE_VK_H_
#define _CDRAW_INSTANCE_VK_H_


#include "cdraw/cdraw/cdrawConfig.h"
#include "vulkan/vulkan.h"


/// <summary>
/// Vulkan instance descriptor.
/// </summary>
typedef struct cdrawVkInstance
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Vulkan instance handle.
	/// </summary>
	VkInstance instance;
} cdrawVkInstance;


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Constructor for Vulkan instance descriptor.
	/// </summary>
	/// <param name="instance_out">Target instance descriptor (non-null).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="instance">Vulkan instance handle.</param>
	/// <returns>Success: <paramref name="instance_out"/>; Failure: <c>NULL</c>.</returns>
	cdrawVkInstance* cdrawVkInstanceCtor(cdrawVkInstance* const instance_out,
		label_t const name, VkInstance const instance);

	/// <summary>
	/// Destructor interface for Vulkan instance descriptor.
	/// </summary>
	/// <param name="instance_out">Target instance descriptor (non-null).</param>
	void cdrawVkInstanceDtor(cdrawVkInstance* const instance_out);

	/// <summary>
	/// Indicate whether descriptor is valid (set up correctly) and should not be modified.
	/// </summary>
	/// <param name="instance">Instance descriptor (non-null).</param>
	/// <returns>True if valid.</returns>
	bool cdrawVkInstanceValid(cdrawVkInstance const* const instance);

	/// <summary>
	/// Indicate whether descriptor is unused (not set up) and can be modified.
	/// </summary>
	/// <param name="instance">Instance descriptor (non-null).</param>
	/// <returns>True if unused.</returns>
	bool cdrawVkInstanceUnused(cdrawVkInstance const* const instance);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef _CDRAW_INSTANCE_VK_H_