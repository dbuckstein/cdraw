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
* cdrawRenderer_vk.c
* Common implementation of modern Vulkan renderer management.
*/

#include "cdraw/cdrawPlatform/cdrawRenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vulkan/vulkan.h"


int32_t cdrawRendererPrintLayer_vk(VkLayerProperties const* const layerProp, uint32_t const index, cstrk_t const prefix)
{
	return printf("\n %s layerProp[%u] = { \"%s\" (%u.%u.%u; %u.%u.%u): \"%s\" }", prefix, index,
		layerProp->layerName,
		VK_VERSION_MAJOR(layerProp->specVersion), VK_VERSION_MINOR(layerProp->specVersion), VK_VERSION_PATCH(layerProp->specVersion),
		VK_VERSION_MAJOR(layerProp->implementationVersion), VK_VERSION_MINOR(layerProp->implementationVersion), VK_VERSION_PATCH(layerProp->implementationVersion),
		layerProp->description);
}

int32_t cdrawRendererPrintExt_vk(VkExtensionProperties const* const extProp, uint32_t const index, cstrk_t const prefix)
{
	return printf("\n %s extensionProp[%u] = { \"%s\" (%u.%u.%u) }", prefix, index,
		extProp->extensionName,
		VK_VERSION_MAJOR(extProp->specVersion), VK_VERSION_MINOR(extProp->specVersion), VK_VERSION_PATCH(extProp->specVersion));
}

int32_t cdrawRendererEnumDeviceID_vk(uint32_t const id)
{
	switch (id)
	{
		// 0 - other
		// https://www.reddit.com/r/vulkan/comments/4ta9nj/is_there_a_comprehensive_list_of_the_names_and/ 
	case 0x1002: return 1; // AMD
	case 0x1010: return 2; // ImgTec
	case 0x10DE: return 3; // NVIDIA
	case 0x13B5: return 4; // ARM
	case 0x5143: return 5; // QualComm
	case 0x8086: return 6; // INTEL
		// 7 - null
	case 0x10000: return 8; // KHR
	case VK_VENDOR_ID_VIV: return 9;
	case VK_VENDOR_ID_VSI: return 10;
	case VK_VENDOR_ID_KAZAN: return 11;
	case VK_VENDOR_ID_CODEPLAY: return 12;
	case VK_VENDOR_ID_MESA: return 13;
	case VK_VENDOR_ID_POCL: return 14;
		// 15 - null
	}
	return -1;
}


bool cdrawRendererPrint_vk(cdrawRenderer const* const renderer)
{
	uint32_t apiVersion;
	VkResult const result = vkEnumerateInstanceVersion(&apiVersion);
	printf("\n Vulkan API: %u.%u.%u", VK_API_VERSION_MAJOR(apiVersion), VK_API_VERSION_MINOR(apiVersion), VK_API_VERSION_PATCH(apiVersion));
	return (result == VK_SUCCESS);
}
