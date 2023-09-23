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
* cdrawPlayer_WinMain.c
* Windows entry point for player application.
*/

#include "cdraw/cdraw/cdrawConfig.h"
#if CDRAW_USING_WINDOWS
#include <Windows.h>

int WINAPI WinMain(
	_In_		HINSTANCE 	hInstance,
	_In_opt_	HINSTANCE 	hPrevInstance,
	_In_		LPSTR 		lpCmdLine,
	_In_		int 		nCmdShow)
{
	int result = 0;

	return result;
}

#endif // #if CDRAW_USING_WINDOWS