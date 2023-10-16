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
* cdrawConsole_win.c
* Windows implementation of console window.
*/

#include "cdraw/cdrawPlatform/cdrawConsole.h"
#if CDRAW_TARGET_WINDOWS
#include <io.h>
#include <stdio.h>
#include <Windows.h>


/******************************************************************************
* Private/internal data structures and functions.
******************************************************************************/

typedef struct cdrawConsole
{
	ptr_t handle[4];
	int32_t io[3];
} cdrawConsole;
static cdrawConsole gConsoleMain;


static void cdrawConsoleInternalRedirectToggle(cdrawConsole* console, bool const redirectInput, bool const redirectOutput, bool const redirectError)
{
	FILE* str = 0;
	int32_t i = -1, j = -1;

	// redirect input
	i = 0;
	if (redirectInput)
	{
		if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE && !console->handle[i])
		{
			// flush buffer, duplicate handle and reopen stream to console
			//j = fprintf(stdin, "\n STDIN =/= DEFAULT \n");
			j = fflush(stdin);
			j = _dup(i);
			str = freopen("CONIN$", "r+", stdin);
			if (str)
			{
				// store values and configure
				console->handle[i] = str;
				console->io[i] = j;
				j = setvbuf(stdin, NULL, _IONBF, 0);
				//j = fprintf(stdin, "\n STDIN == CONSOLE \n");
			}
		}
	}
	else
	{
		if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE && console->handle[i])
		{
			// flush and reopen
			//j = fprintf(stdin, "\n STDIN =/= CONSOLE \n");
			j = fflush(stdin);
			str = freopen("NUL:", "r+", stdin);
			if (str)
			{
				// duplicate handle and reconfigure stream, reset variables
				j = _dup2(console->io[i], i);
				j = setvbuf(stdin, NULL, _IONBF, 0);
				//j = fprintf(stdin, "\n STDIN == DEFAULT \n");
				console->handle[i] = 0;
				console->io[i] = -1;
			}
		}
	}

	// redirect output
	i = 1;
	if (redirectOutput)
	{
		if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE && !console->handle[i])
		{
			// flush buffer, duplicate handle and reopen stream to console
			//j = fprintf(stdout, "\n STDOUT =/= DEFAULT \n");
			j = fflush(stdout);
			j = _dup(i);
			str = freopen("CONOUT$", "a+", stdout);
			if (str)
			{
				// store values and configure
				console->handle[i] = str;
				console->io[i] = j;
				j = setvbuf(stdout, NULL, _IONBF, 0);
				//j = fprintf(stdout, "\n STDOUT == CONSOLE \n");
			}
		}
	}
	else
	{
		if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE && console->handle[i])
		{
			// flush and reopen
			//j = fprintf(stdout, "\n STDOUT =/= CONSOLE \n");
			j = fflush(stdout);
			str = freopen("NUL:", "a+", stdout);
			if (str)
			{
				// duplicate handle and reconfigure stream, reset variables
				j = _dup2(console->io[i], i);
				j = setvbuf(stdout, NULL, _IONBF, 0);
				//j = fprintf(stdout, "\n STDOUT == DEFAULT \n");
				console->handle[i] = 0;
				console->io[i] = -1;
			}
		}
	}

	// redirect error
	i = 2;
	if (redirectError)
	{
		if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE && !console->handle[i])
		{
			// flush buffer, duplicate handle and reopen stream to console
			//j = fprintf(stderr, "\n STDERR =/= DEFAULT \n");
			j = fflush(stderr);
			j = _dup(i);
			str = freopen("CONOUT$", "a+", stderr);
			if (str)
			{
				// store values and configure
				console->handle[i] = str;
				console->io[i] = j;
				j = setvbuf(stderr, NULL, _IONBF, 0);
				//j = fprintf(stderr, "\n STDERR == CONSOLE \n");
			}
		}
	}
	else
	{
		if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE && console->handle[i])
		{
			// flush and reopen
			//j = fprintf(stderr, "\n STDERR =/= CONSOLE \n");
			j = fflush(stderr);
			str = freopen("NUL:", "a+", stderr);
			if (str)
			{
				// duplicate handle and reconfigure stream, reset variables
				j = _dup2(console->io[i], i);
				j = setvbuf(stderr, NULL, _IONBF, 0);
				//j = fprintf(stderr, "\n STDERR == DEFAULT \n");
				console->handle[i] = 0;
				console->io[i] = -1;
			}
		}
	}
}


/******************************************************************************
* Public functions.
******************************************************************************/

result_t cdrawConsoleCreate()
{
	result_init();

	// if console not already open
	ptr_t handle = GetConsoleWindow();
	bool create = !handle && !gConsoleMain.handle[3];
	if (!create)
		result_return();

	// allocate and show console
	create = AllocConsole();
	asserterr(create, errcode_console_create);

	// reset flags
	gConsoleMain.handle[0] = gConsoleMain.handle[1] = gConsoleMain.handle[2] = NULL;
	gConsoleMain.io[0] = gConsoleMain.io[1] = gConsoleMain.io[2] = -1;

	// init flag
	gConsoleMain.handle[3] = handle = GetConsoleWindow();

	// disable closing console manually because doing this kills 
	//	the whole application; could also start a new process, 
	//	but then there's also that to manage
	DeleteMenu(GetSystemMenu(handle, FALSE), SC_CLOSE, MF_BYCOMMAND);

	// redirect to new console (in/out, not err)
	cdrawConsoleInternalRedirectToggle(&gConsoleMain, 1, 1, 0);
	result_return();
}

result_t cdrawConsoleRelease()
{
	result_init();

	ptrk_t const handle = GetConsoleWindow();
	bool release = (gConsoleMain.handle[3] == handle) && handle;
	if (!release)
		result_return();

	// reset to original standard i/o
	cdrawConsoleInternalRedirectToggle(&gConsoleMain, 0, 0, 0);

	// delete console instance
	// console will hide when all standard handles are closed
	release = FreeConsole();
	asserterr(release, errcode_console_release);

	// reset
	gConsoleMain.handle[3] = 0;
	result_return();
}

result_t cdrawConsoleRedirect(bool const redirectInput, bool const redirectOutput, bool const redirectError)
{
	result_init();

	ptrk_t const handle = GetConsoleWindow();
	bool const redirect = (gConsoleMain.handle[3] == handle) && handle;
	asserterr(redirect, errcode_console_manip);

	// redirect toggle
	cdrawConsoleInternalRedirectToggle(&gConsoleMain, redirectInput, redirectOutput, redirectError);
	result_return();
}

result_t cdrawConsoleToggleCursor(bool const visible)
{
	result_init();
	CONSOLE_CURSOR_INFO cursorInfo[1];
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	bool completed = stdHandle && console &&
		GetConsoleCursorInfo(stdHandle, cursorInfo);
	asserterr(completed, errcode_console_manip);

	cursorInfo->bVisible = visible;
	completed = SetConsoleCursorInfo(stdHandle, cursorInfo);
	asserterr(completed, errcode_console_manip);

	result_return();
}

result_t cdrawConsoleGetCursor(int16_t* const x_out, int16_t* const y_out)
{
	result_init();
	asserterr_ptr(x_out, errcode_console_manip);
	asserterr_ptr(y_out, errcode_console_manip);

	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo[1];
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	bool const completed = stdHandle && console &&
		GetConsoleScreenBufferInfo(stdHandle, screenBufferInfo);
	asserterr(completed, errcode_console_manip);

	*x_out = screenBufferInfo->dwCursorPosition.X;
	*y_out = screenBufferInfo->dwCursorPosition.Y;
	result_return();
}

result_t cdrawConsoleSetCursor(int16_t const x, int16_t const y)
{
	result_init();
	COORD const pos = { x, y };
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	bool const completed = stdHandle && console &&
		SetConsoleCursorPosition(stdHandle, pos);
	asserterr(completed, errcode_console_manip);
	result_return();
}

result_t cdrawConsoleGetColor(cdrawConsoleColor* const fg_out, cdrawConsoleColor* const bg_out)
{
	result_init();
	asserterr_ptr(fg_out, errcode_console_manip);
	asserterr_ptr(bg_out, errcode_console_manip);

	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo[1];
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	bool const completed = stdHandle && console &&
		GetConsoleScreenBufferInfo(stdHandle, screenBufferInfo);
	asserterr(completed, errcode_console_manip);

	*fg_out = (cdrawConsoleColor)(screenBufferInfo->wAttributes & 0xf);
	*bg_out = (cdrawConsoleColor)(screenBufferInfo->wAttributes >> 4 & 0xf);
	result_return();
}

result_t cdrawConsoleSetColor(cdrawConsoleColor const fg, cdrawConsoleColor const bg)
{
	result_init();
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	bool const completed = stdHandle && console &&
		SetConsoleTextAttribute(stdHandle, (int16_t)(fg | bg << 4));
	asserterr(completed, errcode_console_manip);
	result_return();
}

result_t cdrawConsoleResetColor()
{
	return cdrawConsoleSetColor(cdrawConsole_white, cdrawConsole_black);
}

result_t cdrawConsoleGetCursorColor(int16_t* const x_out, int16_t* const y_out, cdrawConsoleColor* const fg_out, cdrawConsoleColor* const bg_out)
{
	result_init();
	asserterr_ptr(x_out, errcode_console_manip);
	asserterr_ptr(y_out, errcode_console_manip);
	asserterr_ptr(fg_out, errcode_console_manip);
	asserterr_ptr(bg_out, errcode_console_manip);

	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo[1];
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	bool const completed = stdHandle && console &&
		GetConsoleScreenBufferInfo(stdHandle, screenBufferInfo);
	asserterr(completed, errcode_console_manip);

	*x_out = screenBufferInfo->dwCursorPosition.X;
	*y_out = screenBufferInfo->dwCursorPosition.Y;
	*fg_out = (cdrawConsoleColor)(screenBufferInfo->wAttributes & 0xf);
	*bg_out = (cdrawConsoleColor)(screenBufferInfo->wAttributes >> 4 & 0xf);
	result_return();
}

result_t cdrawConsoleSetCursorColor(int16_t const x, int16_t const y, cdrawConsoleColor const fg, cdrawConsoleColor const bg)
{
	result_init();
	COORD const pos = { x, y };
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	bool const completed = stdHandle && console &&
		SetConsoleCursorPosition(stdHandle, pos) &&
		SetConsoleTextAttribute(stdHandle, (int16_t)(fg | bg << 4));
	asserterr(completed, errcode_console_manip);
	result_return();
}

result_t cdrawConsoleGetSize(int16_t* const w_out, int16_t* const h_out)
{
	result_init();
	asserterr_ptr(w_out, errcode_console_manip);
	asserterr_ptr(h_out, errcode_console_manip);

	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo[1];
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	bool const completed = stdHandle && console &&
		GetConsoleScreenBufferInfo(stdHandle, screenBufferInfo);
	asserterr(completed, errcode_console_manip);

	*w_out = screenBufferInfo->dwSize.X;
	*h_out = screenBufferInfo->dwSize.Y;
	result_return();
}

result_t cdrawConsoleSetSize(int16_t const w, int16_t const h)
{
	result_init();
	COORD const sz = { w, h };
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	bool const completed = stdHandle && console &&
		SetConsoleScreenBufferSize(stdHandle, sz);
	asserterr(completed, errcode_console_manip);
	result_return();
}

result_t cdrawConsoleClear()
{
	result_init();

	// simple clear
	//system("cls");
	// help to avoid using system("cls"): https://docs.microsoft.com/en-us/windows/console/clearing-the-screen 
	CONSOLE_SCREEN_BUFFER_INFO buffer[1];
	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	bool completed = stdHandle && console &&
		GetConsoleScreenBufferInfo(stdHandle, buffer);
	asserterr(completed, errcode_console_manip);

	COORD const coord = { 0, 0 };
	dword_t const sz = (buffer->dwSize.X * buffer->dwSize.Y);
	dword_t write[1] = { 0 };
	completed = FillConsoleOutputCharacterA(stdHandle, ' ', sz, coord, write) &&
		GetConsoleScreenBufferInfo(stdHandle, buffer) &&
		FillConsoleOutputAttribute(stdHandle, buffer->wAttributes, sz, coord, write) &&
		SetConsoleCursorPosition(stdHandle, coord);
	asserterr(completed, errcode_console_manip);

	result_return();
}

result_t cdrawConsoleDebugPatch()
{
	result_init();

	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	bool const exists = stdHandle && console;
	asserterr(exists, errcode_console_manip);

	// test all colors and shifts
	int16_t x, y;
	cdrawConsoleColor fg, bg;
	for (y = 0; y < 16; ++y)
	{
		for (x = 0; x < 16; ++x)
		{
			fg = (cdrawConsoleColor)y;
			bg = (cdrawConsoleColor)x;
			cdrawConsoleSetColor(fg, bg);
			cdrawConsoleSetCursor(x * 2, y);
			printf("%x", (int32_t)x);
			cdrawConsoleSetCursorColor(x * 2 + 1, y, fg, bg);
			printf("%x", (int32_t)y);
		}
	}
	cdrawConsoleGetCursor(&x, &y);
	cdrawConsoleGetColor(&fg, &bg);
	cdrawConsoleGetCursorColor(&x, &y, &fg, &bg);
	cdrawConsoleResetColor();
	printf("[]=(%d, %d) \n", (int32_t)x, (int32_t)y);

	// done
	result_return();
}

result_t cdrawConsoleDebugPrint(cstrk_t const format, ...)
{
	result_init();
	asserterr_cstr(format, errcode_console_manip);

	byte_t str[256] = { 0 };
	va_list args = 0;

	// fill buffer with formatted arguments
	va_start(args, format);
	{
		int const result = _vsnprintf(str, sizeof(str), format, args);
		result_inc(result);
	}
	va_end(args);

	// internal print
	OutputDebugStringA(str);

	// return length
	result_return();
}


#endif // #if CDRAW_TARGET_WINDOWS