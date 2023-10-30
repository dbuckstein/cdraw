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


typedef struct cdrawVkInstance		cdrawVkInstance;
typedef struct cdrawVkLogicalDevice	cdrawVkLogicalDevice;
typedef struct cdrawVkCommandBuffer	cdrawVkCommandBuffer;
typedef struct cdrawVkQueue			cdrawVkQueue;


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

	/// <summary>
	/// Set object name using debug utility messenger extension.
	/// </summary>
	/// <param name="instance">Target instance (non-null and valid).</param>
	/// <param name="logicalDevice">Target logical device (non-null and valid).</param>
	/// <param name="object">Target object.</param>
	/// <param name="objectType">Vulkan object type.</param>
	/// <param name="objectName">Name of object.</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugUtilsObjectName(cdrawVkInstance const* const instance, cdrawVkLogicalDevice const* const logicalDevice,
		ptrk_t const object, VkObjectType const objectType, label_t const objectName);

	/// <summary>
	/// Create object tag using debug utility messenger extension.
	/// </summary>
	/// <param name="instance">Target instance (non-null and valid).</param>
	/// <param name="logicalDevice">Target logical device (non-null and valid).</param>
	/// <param name="object">Target object.</param>
	/// <param name="objectType">Vulkan object type.</param>
	/// <param name="tagName">Unique identifier for tag.</param>
	/// <param name="tagSize">Size of tag data.</param>
	/// <param name="tag">Pointer to tag data.</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugUtilsObjectTag(cdrawVkInstance const* const instance, cdrawVkLogicalDevice const* const logicalDevice,
		ptrk_t const object, VkObjectType const objectType, uint64_t const tagName, size_t const tagSize, ptrk_t const tag);

	/// <summary>
	/// Create begin marker command using debug utility messenger extension.
	/// </summary>
	/// <param name="instance">Target instance (non-null and valid).</param>
	/// <param name="commandBuffer">Target command buffer (non-null and valid).</param>
	/// <param name="labelName">Name of label.</param>
	/// <param name="color_opt">Optional label color in RGBA.</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugUtilsLabelCommandBegin(cdrawVkInstance const* const instance, cdrawVkCommandBuffer const* const commandBuffer,
		label_t const labelName, fp32_t const color_opt[4]);

	/// <summary>
	/// Create begin marker command using debug utility messenger extension.
	/// </summary>
	/// <param name="instance">Target instance (non-null and valid).</param>
	/// <param name="commandBuffer">Target command buffer (non-null and valid).</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugUtilsLabelCommandEnd(cdrawVkInstance const* const instance, cdrawVkCommandBuffer const* const commandBuffer);

	/// <summary>
	/// Create insert marker command using debug utility messenger extension.
	/// </summary>
	/// <param name="instance">Target instance (non-null and valid).</param>
	/// <param name="commandBuffer">Target command buffer (non-null and valid).</param>
	/// <param name="labelName">Name of label.</param>
	/// <param name="color_opt">Optional label color in RGBA.</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugUtilsLabelCommandInsert(cdrawVkInstance const* const instance, cdrawVkCommandBuffer const* const commandBuffer,
		label_t const labelName, fp32_t const color_opt[4]);

	/// <summary>
	/// Create queue begin label using debug utility messenger extension.
	/// </summary>
	/// <param name="instance">Target instance (non-null and valid).</param>
	/// <param name="queue">Target queue (non-null and valid).</param>
	/// <param name="labelName">Name of label.</param>
	/// <param name="color_opt">Optional label color in RGBA.</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugUtilsLabelQueueBegin(cdrawVkInstance const* const instance, cdrawVkQueue const* const queue,
		label_t const labelName, fp32_t const color_opt[4]);

	/// <summary>
	/// Create queue end label using debug utility messenger extension.
	/// </summary>
	/// <param name="instance">Target instance (non-null and valid).</param>
	/// <param name="queue">Target queue (non-null and valid).</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugUtilsLabelQueueEnd(cdrawVkInstance const* const instance, cdrawVkQueue const* const queue);

	/// <summary>
	/// Insert queue label using debug utility messenger extension.
	/// </summary>
	/// <param name="instance">Target instance (non-null and valid).</param>
	/// <param name="queue">Target queue (non-null and valid).</param>
	/// <param name="labelName">Name of label.</param>
	/// <param name="color_opt">Optional label color in RGBA.</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugUtilsLabelQueueInsert(cdrawVkInstance const* const instance, cdrawVkQueue const* const queue,
		label_t const labelName, fp32_t const color_opt[4]);

	/// <summary>
	/// Set object name using debug marker extension.
	/// </summary>
	/// <param name="logicalDevice">Target logical device (non-null and valid).</param>
	/// <param name="object">Target object.</param>
	/// <param name="objectType">Vulkan object type.</param>
	/// <param name="objectName">Name of object.</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugMarkerObjectName(cdrawVkLogicalDevice const* const logicalDevice,
		ptrk_t const object, VkDebugReportObjectTypeEXT const objectType, label_t const objectName);

	/// <summary>
	/// Create object tag using debug marker extension.
	/// </summary>
	/// <param name="logicalDevice">Target logical device (non-null and valid).</param>
	/// <param name="object">Target object.</param>
	/// <param name="objectType">Vulkan object type.</param>
	/// <param name="tagName">Unique identifier for tag.</param>
	/// <param name="tagSize">Size of tag data.</param>
	/// <param name="tag">Pointer to tag data.</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugMarkerObjectTag(cdrawVkLogicalDevice const* const logicalDevice,
		ptrk_t const object, VkDebugReportObjectTypeEXT const objectType, uint64_t const tagName, size_t const tagSize, ptrk_t const tag);

	/// <summary>
	/// Marker begin command using debug marker extension.
	/// </summary>
	/// <param name="logicalDevice">Target logical device (non-null and valid).</param>
	/// <param name="commandBuffer">Target command buffer (non-null and valid).</param>
	/// <param name="markerName">Name of marker.</param>
	/// <param name="color_opt">Optional marker color in RGBA.</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugMarkerCommandBegin(cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandBuffer const* const commandBuffer,
		label_t const markerName, fp32_t const color_opt[4]);

	/// <summary>
	/// Marker end command using debug marker extension.
	/// </summary>
	/// <param name="logicalDevice">Target logical device (non-null and valid).</param>
	/// <param name="commandBuffer">Target command buffer (non-null and valid).</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugMarkerCommandEnd(cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandBuffer const* const commandBuffer);

	/// <summary>
	/// Marker insert command using debug marker extension.
	/// </summary>
	/// <param name="logicalDevice">Target logical device (non-null and valid).</param>
	/// <param name="commandBuffer">Target command buffer (non-null and valid).</param>
	/// <param name="markerName">Name of marker.</param>
	/// <param name="color_opt">Optional marker color in RGBA.</param>
	/// <returns>True if succcess.</returns>
	bool cdrawVkDebugMarkerCommandInsert(cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandBuffer const* const commandBuffer,
		label_t const markerName, fp32_t const color_opt[4]);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #if ((!defined _CDRAW_RENDERER_DEBUG_VK_H_) && (defined _CDRAW_RENDERER_VK_H_)) && CDRAW_DEBUG