#include "gwm_keys.h"
#include "config.h"

gwm_keys keys[] = {
	{CONFIG_MODKEY, XK_u, spawn},
	{CONFIG_MODKEY, XK_Return, terminal},
	{CONFIG_MODKEY, XK_Tab, focus_next},
	{CONFIG_MODKEY|ShiftMask, XK_Tab, focus_prev},
	{CONFIG_MODKEY, XK_i, hide_workspace},
	{CONFIG_MODKEY|ShiftMask, XK_i, show_workspace},
	{CONFIG_MODKEY, XK_Left, move_window_left},
	{CONFIG_MODKEY, XK_Right, move_window_right},
	{CONFIG_MODKEY, XK_Up, move_window_up},
	{CONFIG_MODKEY, XK_Down, move_window_down},
	{CONFIG_MODKEY, XK_n, new_workspace},
	{CONFIG_MODKEY, XK_o, prev_workspace},
	{CONFIG_MODKEY, XK_p, next_workspace},
	{CONFIG_MODKEY|ShiftMask, XK_o, move_to_prev_workspace},
	{CONFIG_MODKEY|ShiftMask, XK_p, move_to_next_workspace},
	{CONFIG_MODKEY|ShiftMask, XK_Left, resize_window_left},
	{CONFIG_MODKEY|ShiftMask, XK_Right, resize_window_right},
	{CONFIG_MODKEY|ShiftMask, XK_Up, resize_window_up},
	{CONFIG_MODKEY|ShiftMask, XK_Down, resize_window_down},
	{CONFIG_MODKEY|ShiftMask, XK_f, fullscreen},
	{.func = NULL}
};

