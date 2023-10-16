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

#define CDRAW_ASSERT_TEST cdraw_assert_test_flag

#include "cdraw/cdrawPlatform.h"


result_t cdrawConsoleTest()
{
	result_init();
	cdrawConsoleCreate();
	cdrawConsoleDebugPatch();
	cdrawConsoleRelease();
	result_return();
}


result_t cdrawPlatformTest()
{
	result_init();
	cdrawConsoleTest();
	result_return();
}
