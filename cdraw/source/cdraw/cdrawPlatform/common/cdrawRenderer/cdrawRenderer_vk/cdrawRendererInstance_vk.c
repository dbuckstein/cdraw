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
* cdrawRendererInstance_vk.c
* Internal source for Vulkan instance management.
*/

#include "_h/cdrawRenderer_vk.h"
#include "_h/cdrawRenderer_vk/cdrawRendererInstance_vk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cdraw/cdrawCore/cdrawUtility.h"


/******************************************************************************
* Private implementations.
******************************************************************************/

/// <summary>
/// Constructor for Vulkan instance descriptor.
/// </summary>
/// <param name="instance_out">Target instance descriptor (non-null).</param>
/// <param name="name">Descriptor name.</param>
/// <param name="instance">Vulkan instance handle.</param>
/// <returns>Success: <paramref name="instance_out"/>; Failure: <c>NULL</c>.</returns>
static cdrawVkInstance* cdrawVkInstanceCtor(cdrawVkInstance* const instance_out,
	label_t const name, VkInstance const instance);

/// <summary>
/// Destructor interface for Vulkan instance descriptor.
/// </summary>
/// <param name="instance_out">Target instance descriptor (non-null).</param>
static void cdrawVkInstanceDtor(cdrawVkInstance* const instance_out);

#if CDRAW_DEBUG
void cdrawVkInstanceDebugRefresh(VkInstance const instance,
	cdrawVkInstanceDebugFuncTable* const f);
bool cdrawVkInstanceDebugFuncValid(
	cdrawVkInstanceDebugFuncTable const* const f);
#endif // #if CDRAW_DEBUG

void cdrawVkInstanceRefresh(VkInstance const instance,
	cdrawVkInstanceFuncTable* const f)
{
	cdraw_assert(instance && f);

#if CDRAW_DEBUG
	cdrawVkInstanceDebugRefresh(instance, &f->f_debug);
#endif // #if CDRAW_DEBUG
}

static bool cdrawVkInstanceFuncValid(
	cdrawVkInstanceFuncTable const* const f)
{
	cdraw_assert(f);
	return (
#if CDRAW_DEBUG
		cdrawVkInstanceDebugFuncValid(&f->f_debug)
#endif // #if CDRAW_DEBUG
		);
}

int32_t cdrawVkLayerPropertiesPrint(VkLayerProperties const* const layerProp, uint32_t const index, cstrk_t const prefix)
{
	return printf("\n %s layerProp[%u] = { \"%s\" (%u.%u.%u; %u.%u.%u): \"%s\" }", prefix, index,
		layerProp->layerName,
		VK_VERSION_MAJOR(layerProp->specVersion), VK_VERSION_MINOR(layerProp->specVersion), VK_VERSION_PATCH(layerProp->specVersion),
		VK_VERSION_MAJOR(layerProp->implementationVersion), VK_VERSION_MINOR(layerProp->implementationVersion), VK_VERSION_PATCH(layerProp->implementationVersion),
		layerProp->description);
}

int32_t cdrawVkExtensionPropertiesPrint(VkExtensionProperties const* const extProp, uint32_t const index, cstrk_t const prefix)
{
	return printf("\n %s extensionProp[%u] = { \"%s\" (%u.%u.%u) }", prefix, index,
		extProp->extensionName,
		VK_VERSION_MAJOR(extProp->specVersion), VK_VERSION_MINOR(extProp->specVersion), VK_VERSION_PATCH(extProp->specVersion));
}

#if CDRAW_DEBUG
VkDebugReportCallbackCreateInfoEXT cdrawVkDebugReportCallbackCreateInfoCtorDefault();
VkDebugUtilsMessengerCreateInfoEXT cdrawVkDebugUtilsMessengerCreateInfoCtorDefault();
#endif // #if CDRAW_DEBUG


/******************************************************************************
* Public implementations.
******************************************************************************/

cdrawVkInstance* cdrawVkInstanceCtor(cdrawVkInstance* const instance_out,
	label_t const name, VkInstance const instance)
{
	failassertret(instance_out, NULL);
	failassertret(instance, NULL);
	label_copy_safe(instance_out->name, name);
	instance_out->instance = instance;
	return instance_out;
}

void cdrawVkInstanceDtor(cdrawVkInstance* const instance_out)
{
	failassertret(instance_out);
	label_init(instance_out->name);
	instance_out->instance = VK_NULL_HANDLE;
}

bool cdrawVkInstanceValid(cdrawVkInstance const* const instance)
{
	cdraw_assert(instance);
	return (instance->instance
		&& cdrawVkInstanceFuncValid(&instance->f));
}

bool cdrawVkInstanceUnused(cdrawVkInstance const* const instance)
{
	cdraw_assert(instance);
	return (!instance->instance);
}


/******************************************************************************
* SECTION: Instance management.
* Reference: Singh, c.3.
* Substantial improvements: translated to C and organized.
******************************************************************************/

static VkApplicationInfo cdrawVkApplicationInfoCtor(
	cstrk_t const appName,
	uint32_t const appVer,
	cstrk_t const engineName,
	uint32_t const engineVer)
{
	VkApplicationInfo applicationInfo = { 0 };
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = appName;
	applicationInfo.applicationVersion = appVer;
	applicationInfo.pEngineName = engineName;
	applicationInfo.engineVersion = engineVer;
	vkEnumerateInstanceVersion(&applicationInfo.apiVersion);
	return applicationInfo;
}

static VkInstanceCreateInfo cdrawVkInstanceCreateInfoCtor(
#if CDRAW_DEBUG
	VkDebugReportCallbackCreateInfoEXT* const debugReportCreateInfo,
	VkDebugUtilsMessengerCreateInfoEXT* const debugUtilsMsgCreateInfo,
#endif // #if CDRAW_DEBUG
	VkApplicationInfo const* const appInfo,
	uint32_t const layerCount,
	cstrk_t const layerNames[/*layerCount*/],
	uint32_t const extCount,
	cstrk_t const extNames[/*extCount*/])
{
	VkInstanceCreateInfo instanceCreateInfo = { 0 };
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
#if CDRAW_DEBUG
	if (debugReportCreateInfo)
	{
		instanceCreateInfo.pNext = debugReportCreateInfo;
		if (debugUtilsMsgCreateInfo)
		{
			debugReportCreateInfo->pNext = debugUtilsMsgCreateInfo;
		}
	}
	else if (debugUtilsMsgCreateInfo)
	{
		instanceCreateInfo.pNext = debugUtilsMsgCreateInfo;
	}
#endif // #if CDRAW_DEBUG
	instanceCreateInfo.pApplicationInfo = appInfo;
	instanceCreateInfo.enabledLayerCount = layerCount;
	instanceCreateInfo.ppEnabledLayerNames = layerNames;
	instanceCreateInfo.enabledExtensionCount = extCount;
	instanceCreateInfo.ppEnabledExtensionNames = extNames;
	return instanceCreateInfo;
}

cstrk_t cdrawRendererInternalPlatformSurfaceExtName_vk();

bool cdrawVkInstanceCreate(cdrawVkInstance* const instance_out,
	label_t const name, VkAllocationCallbacks const* const alloc_opt)
{
	// print prefixes
	cstrk_t const pref1 = "\t    ", pref1A = "\t -> ", pref2 = "\t\t    ", pref2A = "\t\t -> ", pref3 = "\t\t\t    ", pref3A = "\t\t\t -> ";

	// instance layers
	cstrk_t const instLayerName_request[] = {
#if CDRAW_DEBUG
		"VK_LAYER_KHRONOS_validation",
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_LUNARG_api_dump",
		//"VK_LAYER_LUNARG_monitor",
		//"VK_LAYER_LUNARG_screenshot",

		/*// included in standard validation (7):
		"VK_LAYER_GOOGLE_threading",
		"VK_LAYER_LUNARG_parameter_validation",
		"VK_LAYER_LUNARG_object_tracker",
		"VK_LAYER_LUNARG_image",
		"VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_LUNARG_swapchain",
		"VK_LAYER_GOOGLE_unique_objects",*/
#endif // #if CDRAW_DEBUG
		NULL
	};
	cstrk_t const instLayerName_require[] = {
		NULL
	};
	cstrk_t instLayerName[buffer_len(instLayerName_request) + buffer_len(instLayerName_require)] = { NULL };
	uint32_t const instLayerName_request_count = cdrawUtilityPtrCount(instLayerName_request, buffer_len(instLayerName_request));
	uint32_t const instLayerName_require_count = cdrawUtilityPtrCount(instLayerName_require, buffer_len(instLayerName_require));
	uint32_t const instLayerName_baseLen = buffer_len(instLayerName);

	// instance extensions
	cstrk_t const instExtName_request[] = {
#if CDRAW_DEBUG
		// included with KHR validation layer (3): 
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME,
#endif // #if CDRAW_DEBUG
		NULL
	};
	cstrk_t const instExtName_require[] = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		cdrawRendererInternalPlatformSurfaceExtName_vk(),
		NULL
	};
	cstrk_t instExtName[buffer_len(instExtName_request) + buffer_len(instExtName_require)] = { NULL };
	uint32_t const instExtName_request_count = cdrawUtilityPtrCount(instExtName_request, buffer_len(instExtName_request));
	uint32_t const instExtName_require_count = cdrawUtilityPtrCount(instExtName_require, buffer_len(instExtName_require));
	uint32_t const instExtName_baseLen = buffer_len(instExtName);

	// iterators and counts
	uint32_t nInstLayer = 0, nInstLayerEnabled = 0;
	uint32_t nInstExt = 0, nInstExtEnabled = 0;
	VkLayerProperties* pInstLayerProp = NULL;
	VkExtensionProperties* pInstExtProp = NULL;
	uint32_t layerItr, extItr;
	int32_t layerIdx, extIdx;
	cstrk_t nameCmp;

	VkResult result = VK_SUCCESS;
	VkInstance instance = VK_NULL_HANDLE;

	failassertret(instance_out && cdrawVkInstanceUnused(instance_out), false);
	printf("\n Creating Vulkan instance \"%s\"...", name);

	// get available instance layers; may be zero
	result = vkEnumerateInstanceLayerProperties(&nInstLayer, NULL);
	if (result == VK_SUCCESS && nInstLayer)
	{
		pInstLayerProp = (VkLayerProperties*)malloc(sizeof(VkLayerProperties) * nInstLayer);
		if (pInstLayerProp)
		{
			result = vkEnumerateInstanceLayerProperties(&nInstLayer, pInstLayerProp);
			cdraw_assert(result == VK_SUCCESS);
			printf("\n\t pInstLayerProp[%u]: { \"layerName\" (specVer; implVer) \"description\" }", nInstLayer);
			for (layerItr = 0; layerItr < nInstLayer; ++layerItr)
			{
				// copy requested layers to final list if they are found
				layerIdx = -1;
				nameCmp = pInstLayerProp[layerItr].layerName;
				if (cdrawUtilityStrFind(nameCmp, instLayerName, nInstLayerEnabled) < 0)
					if ((layerIdx = cdrawUtilityStrFind(nameCmp, instLayerName_request, instLayerName_request_count)) >= 0)
						instLayerName[nInstLayerEnabled++] = instLayerName_request[layerIdx];
				cdrawVkLayerPropertiesPrint(&pInstLayerProp[layerItr], layerItr, (layerIdx >= 0 ? pref1A : pref1));

				// get available instance extensions related to this layer; may be zero
				result = vkEnumerateInstanceExtensionProperties(nameCmp, &nInstExt, NULL);
				if (result == VK_SUCCESS && nInstExt)
				{
					pInstExtProp = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * nInstExt);
					if (pInstExtProp)
					{
						result = vkEnumerateInstanceExtensionProperties(nameCmp, &nInstExt, pInstExtProp);
						cdraw_assert(result == VK_SUCCESS);
						printf("\n\t\t pInstExtProp[%u]: { \"extensionName\" (specVer) }", nInstExt);
						for (extItr = 0; extItr < nInstExt; ++extItr)
						{
							// copy requested extensions to final list if they are found
							extIdx = -1;
							nameCmp = pInstExtProp[extItr].extensionName;
							if (cdrawUtilityStrFind(nameCmp, instExtName, nInstExtEnabled) < 0)
								if ((extIdx = cdrawUtilityStrFind(nameCmp, instExtName_request, instExtName_request_count)) >= 0)
									instExtName[nInstExtEnabled++] = instExtName_request[extItr];
							cdrawVkExtensionPropertiesPrint(&pInstExtProp[extItr], extItr, (extIdx >= 0 ? pref2A : pref2));
						}
						free(pInstExtProp);
						pInstExtProp = NULL;
					}
				}
			}
			free(pInstLayerProp);
			pInstLayerProp = NULL;
		}
	}

	// copy required layers to final list, confirm count
	for (layerItr = 0; layerItr < instLayerName_require_count; ++layerItr)
	{
		nameCmp = instLayerName_require[layerItr];
		if (cdrawUtilityStrFind(nameCmp, instLayerName, nInstLayerEnabled) < 0)
		{
			instLayerName[nInstLayerEnabled++] = nameCmp;
			printf("\n\t Additional layer: \"%s\"", nameCmp);
		}
	}
	cdraw_assert(nInstLayerEnabled == cdrawUtilityPtrCount(instLayerName, instLayerName_baseLen));

	// copy required extensions to final list, confirm count
	for (extItr = 0; extItr < instExtName_require_count; ++extItr)
	{
		nameCmp = instExtName_require[extItr];
		if (cdrawUtilityStrFind(nameCmp, instExtName, nInstExtEnabled) < 0)
		{
			instExtName[nInstExtEnabled++] = nameCmp;
			printf("\n\t\t Additional extension: \"%s\"", nameCmp);
		}
	}
	cdraw_assert(nInstExtEnabled == cdrawUtilityPtrCount(instExtName, instExtName_baseLen));

	// FINAL CREATE INSTANCE
	if (result == VK_SUCCESS)
	{
#if CDRAW_DEBUG
		// add debug create info to show messages on instance create
		//VkDebugReportCallbackCreateInfoEXT debugReportCreateInfo = cdrawVkDebugReportCallbackCreateInfoCtorDefault();
		VkDebugUtilsMessengerCreateInfoEXT debugUtilsMsgCreateInfo = cdrawVkDebugUtilsMessengerCreateInfoCtorDefault();
#endif // #if CDRAW_DEBUG

		// app descriptor
		VkApplicationInfo const appInfo = cdrawVkApplicationInfoCtor(
			"cdraw Plugin Application",
			VK_MAKE_VERSION(0, 0, 1),
			"cdraw",
			VK_MAKE_VERSION(0, 0, 1));

		// instance info
		VkInstanceCreateInfo const instCreateInfo = cdrawVkInstanceCreateInfoCtor(
#if CDRAW_DEBUG
			NULL, //& debugReportCreateInfo,
			&debugUtilsMsgCreateInfo,
#endif // #if CDRAW_DEBUG
			& appInfo,
			nInstLayerEnabled,
			instLayerName,
			nInstExtEnabled,
			instExtName);

		// create instance
		result = vkCreateInstance(&instCreateInfo, alloc_opt, &instance);
		if (instance)
			cdraw_assert(result == VK_SUCCESS);
	}

	// set final outputs or clean up
	if (!instance || (result != VK_SUCCESS))
	{
		cdrawVkInstanceDestroy(instance_out, alloc_opt);
		printf("\n Vulkan instance \"%s\" creation failed.", name);
		return false;
	}
	cdrawVkInstanceCtor(instance_out, name, instance);
	cdrawVkInstanceRefresh(instance, &instance_out->f);
	cdraw_assert(cdrawVkInstanceValid(instance_out));
	printf("\n Vulkan instance \"%s\" created.", name);
	return true;
}

bool cdrawVkInstanceDestroy(cdrawVkInstance* const instance_out,
	VkAllocationCallbacks const* const alloc_opt)
{
	failassertret(instance_out, false);
	if (cdrawVkInstanceUnused(instance_out))
		return false;

	printf("\n Destroying Vulkan instance \"%s\"...", instance_out->name);
	//if (instance_out->instance)
	{
		vkDestroyInstance(instance_out->instance, alloc_opt);
		instance_out->instance = VK_NULL_HANDLE;
	}
	printf("\n Vulkan instance \"%s\" destroyed.", instance_out->name);
	label_init(instance_out->name);
	return true;
}
