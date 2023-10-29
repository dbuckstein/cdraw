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
* cdrawRendererInstance_vk.h
* Facade interface for Vulkan instance management.
*/

#if ((!defined _CDRAW_RENDERER_INSTANCE_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))
#define _CDRAW_RENDERER_INSTANCE_VK_H_


/// <summary>
/// Get Vulkan instance-dependent function address.
/// </summary>
#define cdrawVkGetInstanceProcAddr(name,f,inst) if ((f->name = (PFN_##name)vkGetInstanceProcAddr(inst, #name)) == NULL) cdraw_assert(f->name)


/// <summary>
/// Instance-dependent extension function pointer storage.
/// </summary>
typedef struct cdrawVkInstanceFuncTable
{
#if CDRAW_DEBUG
	cdrawVkInstanceDebugFuncTable f_debug;
#endif // #if CDRAW_DEBUG
} cdrawVkInstanceFuncTable;


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

	/// <summary>
	/// Instance function pointers.
	/// </summary>
	cdrawVkInstanceFuncTable f;
} cdrawVkInstance;


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

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

	/// <summary>
	/// Create instance.
	/// </summary>
	/// <param name="instance_out">Target instance descriptor (non-null and unused).</param>
	/// <param name="name">Descriptor name.</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if created.</returns>
	bool cdrawVkInstanceCreate(cdrawVkInstance* const instance_out,
		label_t const name, VkAllocationCallbacks const* const alloc_opt);

	/// <summary>
	/// Destroy instance.
	/// </summary>
	/// <param name="instance_out">Target instance descriptor (non-null and valid).</param>
	/// <param name="alloc_opt">Optional allocation callbacks.</param>
	/// <returns>True if destroyed.</returns>
	bool cdrawVkInstanceDestroy(cdrawVkInstance* const instance_out,
		VkAllocationCallbacks const* const alloc_opt);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if ((!defined _CDRAW_RENDERER_INSTANCE_VK_H_) && (defined _CDRAW_RENDERER_VK_H_))