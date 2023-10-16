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
* cdrawConsole.h
* Interface for versatile console window.
*/

#ifndef _CDRAW_CONSOLE_H_
#define _CDRAW_CONSOLE_H_


#include "cdraw/cdraw/cdrawConfig.h"


/******************************************************************************
* Typedefs.
******************************************************************************/

enum
{
	errcode_console_create = errcode_CUSTOM_BEGIN,	// Console creation failed.
	errcode_console_release,						// Console release failed.
	errcode_console_manip,							// Console manipulation failed.
};

/// <summary>
/// Output formatted string to standard error.
/// </summary>
#define eprintf(format,...)	fprintf(stderr, format, __VA_ARGS__)

	/// <summary>
	/// Output formatted string to debugging interface.
	/// </summary>
#define dprintf(format,...)	cdrawConsoleDebugPrint(format, __VA_ARGS__)

	/// <summary>
	/// Named console color channel and combo flags in 4-bit ARGB format.
	/// </summary>
typedef enum cdrawConsoleColor
{
	cdrawConsole_0,							// Color is disabled.
	cdrawConsole_b,							// Color contains blue channel.
	cdrawConsole_g,							// Color contains green channel.
	cdrawConsole_gb,						// Color contains green and blue channels.
	cdrawConsole_r,							// Color contains red channel.
	cdrawConsole_br,						// Color contains red and blue channels.
	cdrawConsole_rg,						// Color contains red and green channels
	cdrawConsole_rgb,						// Color contains red and green and blue channels
	cdrawConsole_a,							// Color contains opacity channel.
	cdrawConsole_black = cdrawConsole_0,	// Color dark black:	0,0,0,0.
	cdrawConsole_blueDark,					// Color dark blue:		0,0,0,1.
	cdrawConsole_greenDark,					// Color dark green:	0,0,1,0.
	cdrawConsole_cyanDark,					// Color dark cyan:		0,0,1,1.
	cdrawConsole_redDark,					// Color dark red:		0,1,0,0.
	cdrawConsole_magentaDark,				// Color dark magenta:	0,1,0,1.
	cdrawConsole_yellowDark,				// Color dark yellow:	0,1,1,0.
	cdrawConsole_gray,						// Color gray:			0,1,1,1.
	cdrawConsole_grayDark,					// Color dark gray:		1,0,0,0.
	cdrawConsole_blue,						// Color blue:			1,0,0,1.
	cdrawConsole_green,						// Color green:			1,0,1,0.
	cdrawConsole_cyan,						// Color cyan:			1,0,1,1.
	cdrawConsole_red,						// Color red:			1,1,0,0.
	cdrawConsole_magenta,					// Color magenta:		1,1,0,1.
	cdrawConsole_yellow,					// Color yellow:		1,1,1,0.
	cdrawConsole_white,						// Color white:			1,1,1,1.
} cdrawConsoleColor;


/******************************************************************************
* Functions.
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus
	
	/// <summary>
	/// Create main console window.
	/// </summary>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleCreate();

	/// <summary>
	/// Release main console window.
	/// </summary>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleRelease();

	/// <summary>
	/// Redirect standard IO to main console window.
	/// </summary>
	/// <param name="redirectInput">True to redirect standard input (stdin) to main console window.</param>
	/// <param name="redirectOutput">True to redirect standard output (stdout) to main console window.</param>
	/// <param name="redirectError">True to redirect standard error (stderr) to main console window.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleRedirect(bool const redirectInput, bool const redirectOutput, bool const redirectError);

	/// <summary>
	/// Toggle main console window cursor visibility.
	/// </summary>
	/// <param name="visible">True to enable console cursor.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleToggleCursor(bool const visible);

	/// <summary>
	/// Get display position of cursor in main console window (relative to top-left).
	/// </summary>
	/// <param name="x_out">Pointer to storage for horizontal position of cursor from left.</param>
	/// <param name="y_out">Pointer to storage for vertical position of cursor from top.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleGetCursor(int16_t* const x_out, int16_t* const y_out);

	/// <summary>
	/// Set next display position of cursor in main console window (relative to top-left).
	/// </summary>
	/// <param name="x">Horizontal position of cursor from left.</param>
	/// <param name="y">Vertical position of cursor from top.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleSetCursor(int16_t const x, int16_t const y);

	/// <summary>
	/// Get display color of output in main console window.
	/// </summary>
	/// <param name="fg_out">Pointer to storage for foreground color (text).</param>
	/// <param name="bg_out">Pointer to storage for background color (window).</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleGetColor(cdrawConsoleColor* const fg_out, cdrawConsoleColor* const bg_out);

	/// <summary>
	/// Set next display color of output in main console window.
	/// </summary>
	/// <param name="fg">Foreground color (text).</param>
	/// <param name="bg">Background color (window).</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleSetColor(cdrawConsoleColor const fg, cdrawConsoleColor const bg);

	/// <summary>
	/// Reset display color to default.
	/// </summary>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleResetColor();

	/// <summary>
	/// Get display position of cursor (relative to top-left) and display color of output in main console window.
	/// </summary>
	/// <param name="x_out">Pointer to storage for horizontal position of cursor from left.</param>
	/// <param name="y_out">Pointer to storage for vertical position of cursor from top.</param>
	/// <param name="fg_out">Pointer to storage for foreground color (text).</param>
	/// <param name="bg_out">Pointer to storage for background color (window).</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleGetCursorColor(int16_t* const x_out, int16_t* const y_out, cdrawConsoleColor* const fg_out, cdrawConsoleColor* const bg_out);

	/// <summary>
	/// Set next display position of cursor (relative to top-left) and display color of output in main console window.
	/// </summary>
	/// <param name="x">Horizontal position of cursor from left.</param>
	/// <param name="y">Vertical position of cursor from top.</param>
	/// <param name="fg">Foreground color (text).</param>
	/// <param name="bg">Background color (window).</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleSetCursorColor(int16_t const x, int16_t const y, cdrawConsoleColor const fg, cdrawConsoleColor const bg);

	/// <summary>
	/// Get size of main console window (in characters).
	/// </summary>
	/// <param name="w_out">Pointer to storage for width of window.</param>
	/// <param name="h_out">Pointer to storage for height of window.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleGetSize(int16_t* const w_out, int16_t* const h_out);

	/// <summary>
	/// Set size of main console window (in characters).
	/// </summary>
	/// <param name="w">Width of window.</param>
	/// <param name="h">Height of window.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleSetSize(int16_t const w, int16_t const h);

	/// <summary>
	/// Clear main console window.
	/// </summary>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleClear();

	/// <summary>
	/// Display debug patch in main console window.
	/// </summary>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleDebugPatch();

	/// <summary>
	/// Print formatted string to debug interface.
	/// </summary>
	/// <param name="format">Format string using standard escape characters.</param>
	/// <param name="...">Variadic arguments following format.</param>
	/// <returns>Zero if success, error code otherwise.</returns>
	result_t cdrawConsoleDebugPrint(cstrk_t const format, ...);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef _CDRAW_CONSOLE_H_