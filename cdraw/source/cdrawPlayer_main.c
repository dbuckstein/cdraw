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
* cdrawPlayer_main.c
* Common entry point for player application.
*/

#include "cdraw/cdrawPlatform/cdrawConsole.h"
#include "cdraw/cdrawPlatform/cdrawWindow.h"

result_t cdrawPlayer_main(cstrk_t const cmd, ptr_t const data_opt)
{
	result_init();
	ptr_t appHandle = NULL;
	int32_t appCount = -1;
	cdrawWindow window = { 0 };
	cdrawWindowControl const control = (cdrawWindowControl_all & ~(cdrawWindowControl_cursor_hide | cdrawWindowControl_cursor_lock | cdrawWindowControl_active_unfocused));
	cdrawApplicationStartSingleInstance(&appHandle, "cdraw Player Application", &appCount);
	cdrawConsoleCreate();
	cdrawWindowCreate(&window, "cdraw Player", 0, 0, 1280, 720, false, control, data_opt);
	cdrawWindowLoop(&window, data_opt);
	cdrawWindowRelease(&window);
	cdrawConsoleRelease();
	cdrawApplicationStopSingleInstance(&appHandle, &appCount);
	result_return();
}
