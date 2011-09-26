#include "gwm_keys.h"

gwm_keys keys[] = {
	{Mod1Mask, XK_p, spawn},
	{Mod1Mask, XK_Tab, focus_next},
	{Mod1Mask|ShiftMask, XK_Tab, focus_prev},
	{Mod1Mask, XK_Left, move_window_left},
	{Mod1Mask, XK_Right, move_window_right},
	{Mod1Mask, XK_Up, move_window_up},
	{Mod1Mask, XK_Down, move_window_down},
	{.func = NULL}
};

