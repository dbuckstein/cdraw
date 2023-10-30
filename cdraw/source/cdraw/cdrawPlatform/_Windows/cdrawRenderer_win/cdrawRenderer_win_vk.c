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
* cdrawRenderer_win_vk.c
* Windows implementation of modern Vulkan renderer management.
*/

/*
*	Download and install Vulkan SDK for current platform: 
*		-> https://vulkan.lunarg.com/sdk/home#windows 
*		-> Environment variables: VK_SDK_PATH & VULKAN_SDK
* 
*	Primary references (chapters cited):
*		Graham Sellers, "Vulkan Programming Guide"
*		Parminder Singh, "Learning Vulkan"
*		Pawel Lapinski, "Vulkan Cookbook"
*/

#define VK_USE_PLATFORM_WIN32_KHR
#include "cdraw/cdrawPlatform/cdrawRenderer.h"
#include "../../common/cdrawRenderer/cdrawRenderer_vk/_h/cdrawRenderer_vk.h"
#if CDRAW_TARGET_WINDOWS
#include <Windows.h>


#endif // #if !CDRAW_TARGET_WINDOWS