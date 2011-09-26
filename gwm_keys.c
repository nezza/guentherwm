#include "gwm_keys.h"

gwm_keys keys[] = {
	{Mod1Mask, XK_u, spawn},
	{Mod1Mask, XK_Return, terminal},
	{Mod1Mask, XK_Tab, focus_next},
	{Mod1Mask|ShiftMask, XK_Tab, focus_prev},
	{Mod1Mask, XK_i, hide_workspace},
	{Mod1Mask|ShiftMask, XK_i, show_workspace},
	{Mod1Mask, XK_Left, move_window_left},
	{Mod1Mask, XK_Right, move_window_right},
	{Mod1Mask, XK_Up, move_window_up},
	{Mod1Mask, XK_Down, move_window_down},
	{Mod1Mask, XK_n, new_workspace},
	{Mod1Mask, XK_o, prev_workspace},
	{Mod1Mask, XK_p, next_workspace},
	{Mod1Mask|ShiftMask, XK_o, move_to_prev_workspace},
	{Mod1Mask|ShiftMask, XK_p, move_to_next_workspace},
	{Mod1Mask|ShiftMask, XK_Left, resize_window_left},
	{Mod1Mask|ShiftMask, XK_Right, resize_window_right},
	{Mod1Mask|ShiftMask, XK_Up, resize_window_up},
	{Mod1Mask|ShiftMask, XK_Down, resize_window_down},
	{Mod1Mask|ShiftMask, XK_f, fullscreen},
	{.func = NULL}
};

