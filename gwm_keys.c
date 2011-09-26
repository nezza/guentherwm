#include "gwm_keys.h"

gwm_keys keys[] = {
	{Mod1Mask, XK_p, spawn},
	{Mod1Mask, XK_Tab, focus_next},
	{Mod1Mask|ShiftMask, XK_Tab, focus_prev},
	{.func = NULL}
};

