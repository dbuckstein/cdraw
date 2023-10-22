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
* cdrawRenderer_win_gl.c
* Windows implementation of modern OpenGL renderer management.
*/

/*
*	Download latest version of GLEW (OpenGL Extension Wrangler):
*		-> http://glew.sourceforge.net/
*	Unzip versioned folder (e.g. "glew-2.1.0") into container directory at
*	system level: "C:/GLEW" (e.g. above, full path "C:/GLEW/glew-2.1.0").
*	After unzipping, run install utility:
*		-> "<framework root>/_dev/Windows/_glewlocate.bat"
*		-> Environment variables: GLEW_PATH
*	Other info about GL loaders and extension managers:
*		-> https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library
*/

#include "cdraw/cdrawPlatform/cdrawRenderer.h"
#if CDRAW_TARGET_WINDOWS
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GL/glew.h"
#include "GL/wglew.h"



#endif // #if !CDRAW_TARGET_WINDOWS