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

#include "cdraw/cdrawMath.h"


result_t cdrawMathTest()
{
	result_init();

	result_return();
}


/******************************************************************************
* Precompile all inlines.
******************************************************************************/

#if CDRAW_USING_PRECOMPILE_LIBS
#undef CDRAW_INL
#define CDRAW_INL
#include "cdraw/cdrawMath/_inl/cdrawScalar.inl"
#endif // #if CDRAW_USING_PRECOMPILE_LIBS