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
* cdrawPlugin.c
* Common implementation of plugin management.
*/

#include <stdio.h>
#include <stdlib.h>

#include "cdraw/cdrawPlatform/cdrawPlugin.h"


/******************************************************************************
* Private/internal functions.
******************************************************************************/

cstr_t cdrawPluginInternalDylibGetExt(cstr_t const ext_out, size_t const arrlen_ext);
ptr_t cdrawPluginInternalDylibLoad(cstrk_t const path);
bool cdrawPluginInternalDylibUnload(ptr_t const handle);
ptr_t cdrawPluginInternalDylibGetSymbol(ptrk_t const handle, cstrk_t const name);



/******************************************************************************
* Public functions.
******************************************************************************/

