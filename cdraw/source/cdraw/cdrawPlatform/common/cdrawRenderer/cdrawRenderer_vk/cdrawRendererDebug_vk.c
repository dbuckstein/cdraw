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
* cdrawRendererDebug_vk.c
* Internal source for Vulkan debugging and validation layers management.
*/

#include "_h/cdrawRenderer_vk.h"
#include "_h/cdrawRenderer_vk/cdrawRendererDebug_vk.h"
#if CDRAW_DEBUG

#include <stdio.h>
#include <string.h>


/******************************************************************************
* Internal implementations.
******************************************************************************/

static VkBool32 cdrawVkDebugInternalDebugReportCallback(
	VkDebugReportFlagsEXT const flags,
	VkDebugReportObjectTypeEXT const objectType,
	uint64_t const object,
	size_t const location,
	int32_t const messageCode,
	cstrk_t const pLayerPrefix,
	cstrk_t const pMessage,
	ptr_t const pUserData)
{
	// same format as "PFN_vkDebugReportCallbackEXT"

	cstrk_t const flagStr[] = { "[INFORMATION]", "[WARNING]", "[PERFORMANCE WARNING]", "[ERROR]", "[DEBUG]" };
	printf("\n\t cdrawVkDebugReport %s%s%s%s%s (T:%d, MsgCode:%d, LayerPrefix:%s):\n\t\t \"%s\"",
		(flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) ? flagStr[0] : "",
		(flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) ? flagStr[1] : "",
		(flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) ? flagStr[2] : "",
		(flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) ? flagStr[3] : "",
		(flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) ? flagStr[4] : "",
		(int32_t)objectType, messageCode, pLayerPrefix, pMessage);

	// handled, but should still return false according to spec
	return VK_SUCCESS;
}

static VkBool32 cdrawVkDebugInternalDebugUtilsMessengerCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT const messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT const messageTypes,
	VkDebugUtilsMessengerCallbackDataEXT const* const pCallbackData,
	ptr_t const pUserData)
{
	// same format as "PFN_vkDebugUtilsMessengerCallbackEXT"

	uint32_t i;
	cstrk_t const severityStr[] = { "[VERBOSE]", "[INFO]", "[WARNING]", "[ERROR]" };
	cstrk_t const typeStr[] = { "[GENERAL]", "[VALIDATION]", "[PERFORMANCE]" };
	printf("\n\t cdrawVkDebugUtilsMessenger %s%s%s%s%s%s%s (ID \'%s\' (%d)):\n\t\t \"%s\"",
		(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) ? severityStr[0] : "",
		(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) ? severityStr[1] : "",
		(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) ? severityStr[2] : "",
		(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) ? severityStr[3] : "",
		(messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) ? typeStr[0] : "",
		(messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) ? typeStr[1] : "",
		(messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) ? typeStr[2] : "",
		pCallbackData->pMessageIdName, pCallbackData->messageIdNumber, pCallbackData->pMessage);
	if (pCallbackData->queueLabelCount)
	{
		printf("\n\t\t\t Queue Labels: ");
		for (i = 0; i < pCallbackData->queueLabelCount; ++i)
			printf("[\'%s\']", pCallbackData->pQueueLabels[i].pLabelName);
	}
	if (pCallbackData->cmdBufLabelCount)
	{
		printf("\n\t\t\t CmdBuf Labels: ");
		for (i = 0; i < pCallbackData->queueLabelCount; ++i)
			printf("[\'%s\']", pCallbackData->pCmdBufLabels[i].pLabelName);
	}
	if (pCallbackData->objectCount)
	{
		printf("\n\t\t\t Objects: ");
		for (i = 0; i < pCallbackData->objectCount; ++i)
			printf("[T:%d, \'%s\']", (int32_t)pCallbackData->pObjects[i].objectType, pCallbackData->pObjects[i].pObjectName);
	}

	return VK_SUCCESS;
}


static VkDebugUtilsObjectNameInfoEXT cdrawVkDebugUtilsObjectNameInfoCtor(
	ptrk_t const object, VkObjectType const objectType, label_t const objectName)
{
	VkDebugUtilsObjectNameInfoEXT debugUtilsObjectNameInfo = { 0 };
	debugUtilsObjectNameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	debugUtilsObjectNameInfo.objectHandle = (uint64_t)object;
	debugUtilsObjectNameInfo.objectType = objectType;
	debugUtilsObjectNameInfo.pObjectName = objectName;
	return debugUtilsObjectNameInfo;
}

static VkDebugUtilsObjectTagInfoEXT cdrawVkDebugUtilsObjectTagInfoCtor(
	ptrk_t const object, VkObjectType const objectType, uint64_t const tagName, size_t const tagSize, ptrk_t const tag)
{
	VkDebugUtilsObjectTagInfoEXT debugUtilsObjectTagInfo = { 0 };
	debugUtilsObjectTagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
	debugUtilsObjectTagInfo.objectHandle = (uint64_t)object;
	debugUtilsObjectTagInfo.objectType = objectType;
	debugUtilsObjectTagInfo.tagName = tagName;
	debugUtilsObjectTagInfo.tagSize = tagSize;
	debugUtilsObjectTagInfo.pTag = tag;
	return debugUtilsObjectTagInfo;
}

static VkDebugUtilsLabelEXT cdrawVkDebugUtilsLabelCtor(
	label_t const labelName, fp32_t const color_opt[4])
{
	VkDebugUtilsLabelEXT debugUtilsLabel = { 0 };
	fp32_t* const color = debugUtilsLabel.color;
	debugUtilsLabel.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
	debugUtilsLabel.pLabelName = labelName;
	if (color_opt)
		memcpy(color, color_opt, sizeof(debugUtilsLabel.color));
	else
		color[0] = color[1] = color[2] = color[3] = 1.0f;
	return debugUtilsLabel;
}

static VkDebugMarkerObjectNameInfoEXT cdrawVkDebugMarkerObjectNameInfoCtor(
	ptrk_t const object, VkDebugReportObjectTypeEXT const objectType, label_t const objectName)
{
	VkDebugMarkerObjectNameInfoEXT debugMarkerObjectNameInfo = { 0 };
	debugMarkerObjectNameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
	debugMarkerObjectNameInfo.object = (uint64_t)object;
	debugMarkerObjectNameInfo.objectType = objectType;
	debugMarkerObjectNameInfo.pObjectName = objectName;
	return debugMarkerObjectNameInfo;
}

static VkDebugMarkerObjectTagInfoEXT cdrawVkDebugMarkerObjectTagInfoCtor(
	ptrk_t const object, VkDebugReportObjectTypeEXT const objectType, uint64_t const tagName, size_t const tagSize, ptrk_t const tag)
{
	VkDebugMarkerObjectTagInfoEXT debugMarkerObjectTagInfo = { 0 };
	debugMarkerObjectTagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
	debugMarkerObjectTagInfo.object = (uint64_t)object;
	debugMarkerObjectTagInfo.objectType = objectType;
	debugMarkerObjectTagInfo.tagName = tagName;
	debugMarkerObjectTagInfo.tagSize = tagSize;
	debugMarkerObjectTagInfo.pTag = tag;
	return debugMarkerObjectTagInfo;
}

static VkDebugMarkerMarkerInfoEXT cdrawVkDebugMarkerMarkerInfoCtor(
	label_t const markerName, fp32_t const color_opt[4])
{
	VkDebugMarkerMarkerInfoEXT debugMarkerMarkerInfo = { 0 };
	fp32_t* const color = debugMarkerMarkerInfo.color;
	debugMarkerMarkerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
	debugMarkerMarkerInfo.pMarkerName = markerName;
	if (color_opt)
		memcpy(color, color_opt, sizeof(debugMarkerMarkerInfo.color));
	else
		color[0] = color[1] = color[2] = color[3] = 1.0f;
	return debugMarkerMarkerInfo;
}


/******************************************************************************
* Private implementations.
******************************************************************************/

VkDebugReportCallbackCreateInfoEXT cdrawVkDebugReportCallbackCreateInfoCtor(
	VkDebugReportFlagsEXT const flags,
	PFN_vkDebugReportCallbackEXT const callback,
	ptr_t const data)
{
	VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo = { 0 };
	debugReportCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	debugReportCallbackCreateInfo.flags = flags;
	debugReportCallbackCreateInfo.pfnCallback = callback;
	debugReportCallbackCreateInfo.pUserData = data;
	return debugReportCallbackCreateInfo;
}

VkDebugReportCallbackCreateInfoEXT cdrawVkDebugReportCallbackCreateInfoCtorDefault()
{
	// all flags
	VkDebugReportFlagsEXT const flags =
		(VK_DEBUG_REPORT_INFORMATION_BIT_EXT
			| VK_DEBUG_REPORT_WARNING_BIT_EXT
			| VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
			| VK_DEBUG_REPORT_ERROR_BIT_EXT
			| VK_DEBUG_REPORT_DEBUG_BIT_EXT);
	return cdrawVkDebugReportCallbackCreateInfoCtor(
		flags,
		cdrawVkDebugInternalDebugReportCallback,
		NULL);
}

VkDebugUtilsMessengerCreateInfoEXT cdrawVkDebugUtilsMessengerCreateInfoCtor(
	VkDebugUtilsMessageSeverityFlagsEXT const messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT const messageType,
	PFN_vkDebugUtilsMessengerCallbackEXT const callback,
	ptr_t const data)
{
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = { 0 };
	debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsMessengerCreateInfo.messageSeverity = messageSeverity;
	debugUtilsMessengerCreateInfo.messageType = messageType;
	debugUtilsMessengerCreateInfo.pfnUserCallback = callback;
	debugUtilsMessengerCreateInfo.pUserData = data;
	return debugUtilsMessengerCreateInfo;
}

VkDebugUtilsMessengerCreateInfoEXT cdrawVkDebugUtilsMessengerCreateInfoCtorDefault()
{
	VkDebugUtilsMessageSeverityFlagsEXT const messageSeverity =
		(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
	VkDebugUtilsMessageTypeFlagsEXT const messageType =
		(VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);
	return cdrawVkDebugUtilsMessengerCreateInfoCtor(
		messageSeverity,
		messageType,
		cdrawVkDebugInternalDebugUtilsMessengerCallback,
		NULL);
}


/******************************************************************************
* SECTION: Debugging and validation layers.
* Reference: Singh, c.4.
* Substantial improvements: translated to C and organized.
******************************************************************************/

bool cdrawVkDebugReportDestroy(VkDebugReportCallbackEXT* const debugReport_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt,
	cdrawVkInstanceDebugFuncTable const* const f)
{
	cdraw_assert(debugReport_out);
	if (!*debugReport_out)
		return false;
	printf("\n Destroying Vulkan debug report...");

	cdraw_assert(inst);
	cdraw_assert(f && f->vkDestroyDebugReportCallbackEXT);
	f->vkDestroyDebugReportCallbackEXT(inst, *debugReport_out, alloc_opt);

	printf("\n Vulkan debug report destroyed.");
	*debugReport_out = VK_NULL_HANDLE;
	return true;
}

bool cdrawVkDebugReportCreate(VkDebugReportCallbackEXT* const debugReport_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt,
	cdrawVkInstanceDebugFuncTable const* const f)
{
	VkResult result = VK_SUCCESS;
	VkDebugReportCallbackEXT debugReport = VK_NULL_HANDLE;
	cdraw_assert(debugReport_out && !*debugReport_out && inst);
	printf("\n Creating Vulkan debug report...");

	// FINAL CREATE DEBUGGING
	{
		// first need to get address of debug report callback function pointer
		VkDebugReportCallbackCreateInfoEXT const debugReportCreateInfo = cdrawVkDebugReportCallbackCreateInfoCtorDefault();
		cdraw_assert(f && f->vkCreateDebugReportCallbackEXT);
		result = f->vkCreateDebugReportCallbackEXT(inst, &debugReportCreateInfo, alloc_opt, &debugReport);
		if (debugReport)
			cdraw_assert(result == VK_SUCCESS);
	}

	// set final outputs or clean up
	if (!debugReport || (result != VK_SUCCESS))
	{
		cdrawVkDebugReportDestroy(&debugReport, inst, alloc_opt, f);
		printf("\n Vulkan debug report creation failed.");
		return false;
	}
	*debugReport_out = debugReport;
	cdraw_assert(*debugReport_out);
	printf("\n Vulkan debug report created.");
	return true;
}

bool cdrawVkDebugUtilsMessengerDestroy(VkDebugUtilsMessengerEXT* const debugUtilsMsg_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt,
	cdrawVkInstanceDebugFuncTable const* const f)
{
	cdraw_assert(debugUtilsMsg_out);
	if (!*debugUtilsMsg_out)
		return false;
	printf("\n Destroying Vulkan debug utility messenger...");

	cdraw_assert(inst);
	{
		cdraw_assert(f && f->vkDestroyDebugUtilsMessengerEXT);
		f->vkDestroyDebugUtilsMessengerEXT(inst, *debugUtilsMsg_out, alloc_opt);
	}

	printf("\n Vulkan debug utility messenger destroyed.");
	*debugUtilsMsg_out = VK_NULL_HANDLE;
	return true;
}

bool cdrawVkDebugUtilsMessengerCreate(VkDebugUtilsMessengerEXT* const debugUtilsMsg_out,
	VkInstance const inst, VkAllocationCallbacks const* const alloc_opt,
	cdrawVkInstanceDebugFuncTable const* const f)
{
	VkResult result = VK_SUCCESS;
	VkDebugUtilsMessengerEXT debugUtilsMsg = VK_NULL_HANDLE;
	cdraw_assert(debugUtilsMsg_out && !*debugUtilsMsg_out && inst);
	printf("\n Creating Vulkan debug utility messenger...");

	// FINAL CREATE DEBUGGING
	{
		// first need to get address of debug report callback function pointer
		VkDebugUtilsMessengerCreateInfoEXT const debugUtilsMsgCreateInfo = cdrawVkDebugUtilsMessengerCreateInfoCtorDefault();
		cdraw_assert(f && f->vkCreateDebugUtilsMessengerEXT);
		result = f->vkCreateDebugUtilsMessengerEXT(inst, &debugUtilsMsgCreateInfo, alloc_opt, &debugUtilsMsg);
		if (debugUtilsMsg)
			cdraw_assert(result == VK_SUCCESS);
	}

	// set final outputs or clean up
	if (!debugUtilsMsg || (result != VK_SUCCESS))
	{
		cdrawVkDebugUtilsMessengerDestroy(&debugUtilsMsg, inst, alloc_opt, f);
		printf("\n Vulkan debug utility messenger creation failed.");
		return false;
	}
	*debugUtilsMsg_out = debugUtilsMsg;
	cdraw_assert(*debugUtilsMsg_out);
	printf("\n Vulkan debug utility messenger created.");
	return true;
}


void cdrawVkInstanceDebugRefresh(VkInstance const instance,
	cdrawVkInstanceDebugFuncTable* const f)
{
	cdraw_assert(instance && f);

	// DEBUG REPORT EXTENSION
	{
		cdrawVkGetInstanceProcAddr(vkCreateDebugReportCallbackEXT, f, instance);
		cdrawVkGetInstanceProcAddr(vkDestroyDebugReportCallbackEXT, f, instance);
		cdrawVkGetInstanceProcAddr(vkDebugReportMessageEXT, f, instance);
	}

	// DEBUG UTIL MESSENGER EXTENSION
	{
		cdrawVkGetInstanceProcAddr(vkCreateDebugUtilsMessengerEXT, f, instance);
		cdrawVkGetInstanceProcAddr(vkDestroyDebugUtilsMessengerEXT, f, instance);
		cdrawVkGetInstanceProcAddr(vkSubmitDebugUtilsMessageEXT, f, instance);
		cdrawVkGetInstanceProcAddr(vkSetDebugUtilsObjectNameEXT, f, instance);
		cdrawVkGetInstanceProcAddr(vkSetDebugUtilsObjectTagEXT, f, instance);
		cdrawVkGetInstanceProcAddr(vkQueueBeginDebugUtilsLabelEXT, f, instance);
		cdrawVkGetInstanceProcAddr(vkQueueEndDebugUtilsLabelEXT, f, instance);
		cdrawVkGetInstanceProcAddr(vkQueueInsertDebugUtilsLabelEXT, f, instance);
		cdrawVkGetInstanceProcAddr(vkCmdBeginDebugUtilsLabelEXT, f, instance);
		cdrawVkGetInstanceProcAddr(vkCmdEndDebugUtilsLabelEXT, f, instance);
		cdrawVkGetInstanceProcAddr(vkCmdInsertDebugUtilsLabelEXT, f, instance);
	}
}

bool cdrawVkInstanceDebugFuncValid(cdrawVkInstanceDebugFuncTable const* const f)
{
	cdraw_assert(f);
	return (f->vkCreateDebugReportCallbackEXT
		&& f->vkDestroyDebugReportCallbackEXT
		&& f->vkDebugReportMessageEXT
		&& f->vkCreateDebugUtilsMessengerEXT
		&& f->vkDestroyDebugUtilsMessengerEXT
		&& f->vkSubmitDebugUtilsMessageEXT
		&& f->vkSetDebugUtilsObjectNameEXT
		&& f->vkSetDebugUtilsObjectTagEXT
		&& f->vkQueueBeginDebugUtilsLabelEXT
		&& f->vkQueueEndDebugUtilsLabelEXT
		&& f->vkQueueInsertDebugUtilsLabelEXT
		&& f->vkCmdBeginDebugUtilsLabelEXT
		&& f->vkCmdEndDebugUtilsLabelEXT
		&& f->vkCmdInsertDebugUtilsLabelEXT);
}

void cdrawVkDeviceDebugRefresh(VkDevice const device,
	cdrawVkDeviceDebugFuncTable* const f)
{
	cdraw_assert(device && f);

	// DEBUG MARKER EXTENSION
	{
		cdrawVkGetDeviceProcAddr(vkDebugMarkerSetObjectNameEXT, f, device);
		cdrawVkGetDeviceProcAddr(vkDebugMarkerSetObjectTagEXT, f, device);
		cdrawVkGetDeviceProcAddr(vkCmdDebugMarkerBeginEXT, f, device);
		cdrawVkGetDeviceProcAddr(vkCmdDebugMarkerEndEXT, f, device);
		cdrawVkGetDeviceProcAddr(vkCmdDebugMarkerInsertEXT, f, device);
	}
}

bool cdrawVkDeviceDebugFuncValid(cdrawVkDeviceDebugFuncTable const* const f)
{
	cdraw_assert(f);
	return (f->vkDebugMarkerSetObjectNameEXT
		&& f->vkDebugMarkerSetObjectTagEXT
		&& f->vkCmdDebugMarkerBeginEXT
		&& f->vkCmdDebugMarkerEndEXT
		&& f->vkCmdDebugMarkerInsertEXT);
}


/******************************************************************************
* Public implementations.
******************************************************************************/

bool cdrawVkDebugUtilsObjectName(cdrawVkInstance const* const instance, cdrawVkLogicalDevice const* const logicalDevice,
	ptrk_t const object, VkObjectType const objectType, label_t const objectName)
{
	cdraw_assert(cdrawVkInstanceValid(instance) && cdrawVkLogicalDeviceValid(logicalDevice));
	VkDebugUtilsObjectNameInfoEXT const debugUtilsObjectNameInfo = cdrawVkDebugUtilsObjectNameInfoCtor(object, objectType, objectName);
	return (instance->f.f_debug.vkSetDebugUtilsObjectNameEXT(logicalDevice->logicalDevice, &debugUtilsObjectNameInfo) == VK_SUCCESS);
}

bool cdrawVkDebugUtilsObjectTag(cdrawVkInstance const* const instance, cdrawVkLogicalDevice const* const logicalDevice,
	ptrk_t const object, VkObjectType const objectType, uint64_t const tagName, size_t const tagSize, ptrk_t const tag)
{
	cdraw_assert(cdrawVkInstanceValid(instance) && cdrawVkLogicalDeviceValid(logicalDevice));
	VkDebugUtilsObjectTagInfoEXT const debugUtilsObjectTagInfo = cdrawVkDebugUtilsObjectTagInfoCtor(object, objectType, tagName, tagSize, tag);
	return (instance->f.f_debug.vkSetDebugUtilsObjectTagEXT(logicalDevice->logicalDevice, &debugUtilsObjectTagInfo) == VK_SUCCESS);
}

bool cdrawVkDebugUtilsLabelCommandBegin(cdrawVkInstance const* const instance, cdrawVkCommandBuffer const* const commandBuffer, uint32_t const commandBufferIndex,
	label_t const labelName, fp32_t const color_opt[4])
{
	cdraw_assert(cdrawVkInstanceValid(instance) && cdrawVkCommandBufferValid(commandBuffer) && (commandBufferIndex < commandBuffer->commandBufferCount));
	VkDebugUtilsLabelEXT const debugUtilsLabel = cdrawVkDebugUtilsLabelCtor(labelName, color_opt);
	instance->f.f_debug.vkCmdBeginDebugUtilsLabelEXT(commandBuffer->commandBuffer[commandBufferIndex], &debugUtilsLabel);
	return true;
}

bool cdrawVkDebugUtilsLabelCommandEnd(cdrawVkInstance const* const instance, cdrawVkCommandBuffer const* const commandBuffer, uint32_t const commandBufferIndex)
{
	cdraw_assert(cdrawVkInstanceValid(instance) && cdrawVkCommandBufferValid(commandBuffer) && (commandBufferIndex < commandBuffer->commandBufferCount));
	instance->f.f_debug.vkCmdEndDebugUtilsLabelEXT(commandBuffer->commandBuffer[commandBufferIndex]);
	return true;
}

bool cdrawVkDebugUtilsLabelCommandInsert(cdrawVkInstance const* const instance, cdrawVkCommandBuffer const* const commandBuffer, uint32_t const commandBufferIndex,
	label_t const labelName, fp32_t const color_opt[4])
{
	cdraw_assert(cdrawVkInstanceValid(instance) && cdrawVkCommandBufferValid(commandBuffer) && (commandBufferIndex < commandBuffer->commandBufferCount));
	VkDebugUtilsLabelEXT const debugUtilsLabel = cdrawVkDebugUtilsLabelCtor(labelName, color_opt);
	instance->f.f_debug.vkCmdInsertDebugUtilsLabelEXT(commandBuffer->commandBuffer[commandBufferIndex], &debugUtilsLabel);
	return true;
}

bool cdrawVkDebugUtilsLabelQueueBegin(cdrawVkInstance const* const instance, cdrawVkQueue const* const queue,
	label_t const labelName, fp32_t const color_opt[4])
{
	cdraw_assert(cdrawVkInstanceValid(instance) && cdrawVkQueueValid(queue));
	VkDebugUtilsLabelEXT const debugUtilsLabel = cdrawVkDebugUtilsLabelCtor(labelName, color_opt);
	instance->f.f_debug.vkQueueBeginDebugUtilsLabelEXT(queue->queue, &debugUtilsLabel);
	return true;
}

bool cdrawVkDebugUtilsLabelQueueEnd(cdrawVkInstance const* const instance, cdrawVkQueue const* const queue)
{
	cdraw_assert(cdrawVkInstanceValid(instance) && cdrawVkQueueValid(queue));
	instance->f.f_debug.vkQueueEndDebugUtilsLabelEXT(queue->queue);
	return true;
}

bool cdrawVkDebugUtilsLabelQueueInsert(cdrawVkInstance const* const instance, cdrawVkQueue const* const queue,
	label_t const labelName, fp32_t const color_opt[4])
{
	cdraw_assert(cdrawVkInstanceValid(instance) && cdrawVkQueueValid(queue));
	VkDebugUtilsLabelEXT const debugUtilsLabel = cdrawVkDebugUtilsLabelCtor(labelName, color_opt);
	instance->f.f_debug.vkQueueInsertDebugUtilsLabelEXT(queue->queue, &debugUtilsLabel);
	return true;
}

bool cdrawVkDebugMarkerObjectName(cdrawVkLogicalDevice const* const logicalDevice,
	ptrk_t const object, VkDebugReportObjectTypeEXT const objectType, label_t const objectName)
{
	cdraw_assert(cdrawVkLogicalDeviceValid(logicalDevice));
	VkDebugMarkerObjectNameInfoEXT const debugMarkerObjectNameInfo = cdrawVkDebugMarkerObjectNameInfoCtor(object, objectType, objectName);
	return (logicalDevice->f.f_debug.vkDebugMarkerSetObjectNameEXT(logicalDevice->logicalDevice, &debugMarkerObjectNameInfo) == VK_SUCCESS);
}

bool cdrawVkDebugMarkerObjectTag(cdrawVkLogicalDevice const* const logicalDevice,
	ptrk_t const object, VkDebugReportObjectTypeEXT const objectType, uint64_t const tagName, size_t const tagSize, ptrk_t const tag)
{
	cdraw_assert(cdrawVkLogicalDeviceValid(logicalDevice));
	VkDebugMarkerObjectTagInfoEXT const debugMarkerObjectTagInfo = cdrawVkDebugMarkerObjectTagInfoCtor(object, objectType, tagName, tagSize, tag);
	return (logicalDevice->f.f_debug.vkDebugMarkerSetObjectTagEXT(logicalDevice->logicalDevice, &debugMarkerObjectTagInfo) == VK_SUCCESS);
}

bool cdrawVkDebugMarkerCommandBegin(cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandBuffer const* const commandBuffer, uint32_t const commandBufferIndex,
	label_t const markerName, fp32_t const color_opt[4])
{
	cdraw_assert(cdrawVkLogicalDeviceValid(logicalDevice) && cdrawVkCommandBufferValid(commandBuffer) && (commandBufferIndex < commandBuffer->commandBufferCount));
	VkDebugMarkerMarkerInfoEXT const debugMarkerMarkerInfo = cdrawVkDebugMarkerMarkerInfoCtor(markerName, color_opt);
	logicalDevice->f.f_debug.vkCmdDebugMarkerBeginEXT(commandBuffer->commandBuffer[commandBufferIndex], &debugMarkerMarkerInfo);
	return true;
}

bool cdrawVkDebugMarkerCommandEnd(cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandBuffer const* const commandBuffer, uint32_t const commandBufferIndex)
{
	cdraw_assert(cdrawVkLogicalDeviceValid(logicalDevice) && cdrawVkCommandBufferValid(commandBuffer) && (commandBufferIndex < commandBuffer->commandBufferCount));
	logicalDevice->f.f_debug.vkCmdDebugMarkerEndEXT(commandBuffer->commandBuffer[commandBufferIndex]);
	return true;
}

bool cdrawVkDebugMarkerCommandInsert(cdrawVkLogicalDevice const* const logicalDevice, cdrawVkCommandBuffer const* const commandBuffer, uint32_t const commandBufferIndex,
	label_t const markerName, fp32_t const color_opt[4])
{
	cdraw_assert(cdrawVkLogicalDeviceValid(logicalDevice) && cdrawVkCommandBufferValid(commandBuffer) && (commandBufferIndex < commandBuffer->commandBufferCount));
	VkDebugMarkerMarkerInfoEXT const debugMarkerMarkerInfo = cdrawVkDebugMarkerMarkerInfoCtor(markerName, color_opt);
	logicalDevice->f.f_debug.vkCmdDebugMarkerInsertEXT(commandBuffer->commandBuffer[commandBufferIndex], &debugMarkerMarkerInfo);
	return true;
}


#endif // #if CDRAW_DEBUG