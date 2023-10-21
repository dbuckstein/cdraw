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
* cdrawInput.h
* Input management interface.
*/

#ifndef _CDRAW_INPUT_H_
#define _CDRAW_INPUT_H_

#include "cdraw/cdraw/cdrawConfig.h"

/******************************************************************************
* Public interfaces.
******************************************************************************/

/// <summary>
/// Enumeration of mouse buttons.
/// </summary>
typedef enum cdrawMouseButton
{
	mouse_left,		// Mouse left button.
	mouse_middle,	// Mouse middle button.
	mouse_right,	// Mouse right button.
	mouse_extra1,	// Mouse extra button (if available).
	mouse_extra2,	// Mouse extra button (if available).
} cdrawMouseButton;

/// <summary>
/// Enumeration of virtual keys.
/// These do not represent ASCII key codes, they are the codes for the actual keys.
/// </summary>
typedef enum cdrawVirtKey
{
	virtkey_null,
	virtkey_backspace = 8,
	virtkey_tab,
	virtkey_clear = 12,
	virtkey_return,
	virtkey_enter = virtkey_return,
	virtkey_shift = 16,
	virtkey_control,
	virtkey_alt,							// not processed
	virtkey_pause,
	virtkey_capslock,
	virtkey_escape = 27,
	virtkey_esc = virtkey_escape,
	virtkey_space = 32,
	virtkey_pageup,
	virtkey_pagedown,
	virtkey_end,
	virtkey_home,
	virtkey_left,
	virtkey_up,
	virtkey_right,
	virtkey_down,
	virtkey_select,
	virtkey_print,
	virtkey_execute,
	virtkey_printscreen,					// processed only on release
	virtkey_insert,
	virtkey_delete,
	virtkey_help,
	virtkey_0,
	virtkey_1,
	virtkey_2,
	virtkey_3,
	virtkey_4,
	virtkey_5,
	virtkey_6,
	virtkey_7,
	virtkey_8,
	virtkey_9,
	virtkey_rightparen = virtkey_0,
	virtkey_exclamation,
	virtkey_at,
	virtkey_number,
	virtkey_dollar,
	virtkey_percent,
	virtkey_hat,
	virtkey_ampersand,
	virtkey_asterisk,
	virtkey_leftparen,
	virtkey_A = 65,
	virtkey_B,
	virtkey_C,
	virtkey_D,
	virtkey_E,
	virtkey_F,
	virtkey_G,
	virtkey_H,
	virtkey_I,
	virtkey_J,
	virtkey_K,
	virtkey_L,
	virtkey_M,
	virtkey_N,
	virtkey_O,
	virtkey_P,
	virtkey_Q,
	virtkey_R,
	virtkey_S,
	virtkey_T,
	virtkey_U,
	virtkey_V,
	virtkey_W,
	virtkey_X,
	virtkey_Y,
	virtkey_Z,
	virtkey_command_left,
	virtkey_command_right,
	virtkey_window_left = virtkey_command_left,
	virtkey_window_right,
	virtkey_menu,
	virtkey_sleep = 95,
	virtkey_numpad_0,
	virtkey_numpad_1,
	virtkey_numpad_2,
	virtkey_numpad_3,
	virtkey_numpad_4,
	virtkey_numpad_5,
	virtkey_numpad_6,
	virtkey_numpad_7,
	virtkey_numpad_8,
	virtkey_numpad_9,
	virtkey_numpad_times,
	virtkey_numpad_plus,
	virtkey_numpad_separator,
	virtkey_numpad_minus,
	virtkey_numpad_point,
	virtkey_numpad_divide,
	virtkey_F1,
	virtkey_F2,
	virtkey_F3,
	virtkey_F4,
	virtkey_F5,
	virtkey_F6,
	virtkey_F7,
	virtkey_F8,
	virtkey_F9,
	virtkey_F10,							// not processed
	virtkey_F11,
	virtkey_F12,
	virtkey_F13,
	virtkey_F14,
	virtkey_F15,
	virtkey_F16,
	virtkey_F17,
	virtkey_F18,
	virtkey_F19,
	virtkey_F20,
	virtkey_F21,
	virtkey_F22,
	virtkey_F23,
	virtkey_F24,
	virtkey_numlock = 144,
	virtkey_scrolllock,
	virtkey_numpad_equal,
	virtkey_shift_left = 160,				// not processed with callbacks
	virtkey_shift_right,					// not processed with callbacks
	virtkey_control_left,					// not processed with callbacks
	virtkey_control_right,					// not processed with callbacks
	virtkey_menu_left,						// not processed with callbacks
	virtkey_menu_right,						// not processed with callbacks
	virtkey_browser_back,
	virtkey_browser_forward,
	virtkey_browser_refresh,
	virtkey_browser_stop,
	virtkey_browser_search,
	virtkey_browser_favorites,
	virtkey_browser_home,
	virtkey_media_mute,
	virtkey_media_volumedown,
	virtkey_media_volumeup,
	virtkey_media_next,
	virtkey_media_prev,
	virtkey_media_stop,
	virtkey_media_playpause,
	virtkey_launch_email,
	virtkey_launch_media,
	virtkey_launch_app1,
	virtkey_launch_app2,
	virtkey_semicolon = 186,
	virtkey_equal,
	virtkey_comma,
	virtkey_dash,
	virtkey_period,
	virtkey_slash,
	virtkey_colon = virtkey_semicolon,
	virtkey_minus = virtkey_dash,
	virtkey_plus,
	virtkey_lessthan,
	virtkey_underscore,
	virtkey_greaterthan,
	virtkey_question,
	virtkey_tilde,
	virtkey_leftbracket = 219,
	virtkey_backslash,
	virtkey_rightbracket,
	virtkey_singlequote,
	virtkey_leftbrace = virtkey_leftbracket,
	virtkey_pipe,
	virtkey_rightbrace,
	virtkey_doublequote,
	virtkey_quotes = virtkey_singlequote,
	virtkey_apostrophe = virtkey_singlequote,
	virtkey_function = 255
} cdrawVirtKey;

/// <summary>
/// Enumeration of ASCII character keys. These are for special keys that are 
/// accompanied/represented by ASCII codes and respond to character key 
/// callbacks. If a key can be represented by a character, the enum can be 
/// replaced with the literal character (e.g. 'a'); otherwise, use the enum 
/// codes to check the state of an ASCII key.
/// </summary>
typedef enum cdrawKey
{
	key_null,								// '\0'
	key_starthead,
	key_starttext,
	key_endtext,
	key_endtrans,
	key_enquiry,
	key_acknowledge,
	key_bell,								// '\a'
	key_backspace,							// '\b'
	key_tab_horiz,							// '\t'
	key_linefeed,							// '\n' or ctrl + enter
	key_tab_vert,							// '\v'
	key_formfeed,							// '\f'
	key_return,								// '\r'
	key_shift_out,
	key_shift_in,
	key_datalinkesc,
	key_devicectrl1,
	key_devicectrl2,
	key_devicectrl3,
	key_devicectrl4,
	key_acknowledge_negative,
	key_syncidle,
	key_endtrans_block,
	key_cancel,
	key_endmedium,
	key_substitute,
	key_escape,
	key_filesep,
	key_groupsep,
	key_recordsep,
	key_unitsep,
	key_space,
	key_exclamation,
	key_quotes,								// '\"'
	key_number,
	key_dollar,
	key_percent,
	key_ampersand,
	key_apostrophe,							// '\''
	key_leftparen,
	key_rightparen,
	key_asterisk,
	key_plus,
	key_comma,
	key_dash,
	key_period,
	key_slash,
	key_0,
	key_1,
	key_2,
	key_3,
	key_4,
	key_5,
	key_6,
	key_7,
	key_8,
	key_9,
	key_colon,
	key_semicolon,
	key_less,
	key_equal,
	key_greater,
	key_question,							// '\?'
	key_at,
	key_A,
	key_B,
	key_C,
	key_D,
	key_E,
	key_F,
	key_G,
	key_H,
	key_I,
	key_J,
	key_K,
	key_L,
	key_M,
	key_N,
	key_O,
	key_P,
	key_Q,
	key_R,
	key_S,
	key_T,
	key_U,
	key_V,
	key_W,
	key_X,
	key_Y,
	key_Z,
	key_leftbracket,
	key_backslash,							// '\\'
	key_rightbracket,
	key_hat,
	key_underscore,
	key_backquote,
	key_a,
	key_b,
	key_c,
	key_d,
	key_e,
	key_f,
	key_g,
	key_h,
	key_i,
	key_j,
	key_k,
	key_l,
	key_m,
	key_n,
	key_o,
	key_p,
	key_q,
	key_r,
	key_s,
	key_t,
	key_u,
	key_v,
	key_w,
	key_x,
	key_y,
	key_z,
	key_leftbrace,
	key_pipe,
	key_rightbrace,
	key_tilde,
	key_delete,
	key_ctrl_a = 1,							// ctrl + 'a'
	key_ctrl_b,								// ctrl + 'b'
	key_ctrl_c,								// ctrl + 'c' etc.
	key_ctrl_d,
	key_ctrl_e,
	key_ctrl_f,
	key_ctrl_g,
	key_ctrl_h,
	key_ctrl_i,
	key_ctrl_j,
	key_ctrl_k,
	key_ctrl_l,
	key_ctrl_m,
	key_ctrl_n,
	key_ctrl_o,
	key_ctrl_p,
	key_ctrl_q,
	key_ctrl_r,
	key_ctrl_s,
	key_ctrl_t,
	key_ctrl_u,
	key_ctrl_v,
	key_ctrl_w,
	key_ctrl_x,
	key_ctrl_y,
	key_ctrl_z,
	key_ctrl_leftbracket,					// ctrl + '['
	key_ctrl_backslash,						// ctrl + '\'
	key_ctrl_rightbracket,					// ctrl + ']'
	key_ctrl_pause = 3,						// ctrl + pause
	key_ctrl_scrolllock = 3,				// ctrl + scrolllock
	key_alert = key_bell,					// '\a' etc.
	key_bs,
	key_tab,
	key_newline,
	key_vtab,
	key_newpage,
	key_enter,
	key_ctrl_enter = key_newline,
	key_ctrl_backspace = key_delete
} cdrawKey;


#endif // #ifndef _CDRAW_INPUT_H_