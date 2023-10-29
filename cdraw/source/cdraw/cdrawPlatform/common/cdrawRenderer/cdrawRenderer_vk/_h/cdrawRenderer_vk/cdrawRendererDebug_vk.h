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
* cdrawRendererDebug_vk.h
* Facade interface for Vulkan debugging and validation layer management.
*/

#if ((!defined _CDRAW_RENDERER_DEBUG_VK_H_) && (defined _CDRAW_RENDERER_VK_H_)) && CDRAW_DEBUG
#define _CDRAW_RENDERER_DEBUG_VK_H_


/// <summary>
/// Instance-dependent debug extension function pointer storage.
/// </summary>
typedef struct cdrawVkInstanceDebugFuncTable
{
	/// <summary>
	/// Debug report extension.
	/// </summary>
	struct {
		PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
		PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
		PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;
	};

	/// <summary>
	/// Debug utility messenger extension.
	/// </summary>
	struct {
		PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
		PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
		PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
		PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
		PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT;
		PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT;
		PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT;
		PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT;
		PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
		PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
		PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
	};
} cdrawVkInstanceDebugFuncTable;


/// <summary>
/// Device-dependent debug extension function pointer storage.
/// </summary>
typedef struct cdrawVkDeviceDebugFuncTable
{
	/// <summary>
	/// Debug marker extension.
	/// </summary>
	struct {
		PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
		PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
		PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
		PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
		PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
	};
} cdrawVkDeviceDebugFuncTable;


/// <summary>
/// Vulkan debugging descriptor.
/// </summary>
typedef struct cdrawVkDebug
{
	/// <summary>
	/// Descriptor name.
	/// </summary>
	label_t name;

	/// <summary>
	/// Debug report extension.
	/// </summary>
	VkDebugReportCallbackEXT debugReport;

	/// <summary>
	/// Debug messenger extension.
	/// </summary>
	VkDebugUtilsMessengerEXT debugMessenger;
} cdrawVkDebug;


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus



#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if ((!defined _CDRAW_RENDERER_DEBUG_VK_H_) && (defined _CDRAW_RENDERER_VK_H_)) && CDRAW_DEBUG