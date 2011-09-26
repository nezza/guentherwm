#ifndef GWM_CONTEXT_H_
#define GWM_CONTEXT_H_ 1

#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#include "gwm_types.h"
#include "gwm_window.h"

typedef struct {
	unsigned long fg;
	unsigned long bg;
	unsigned long border;
} window_colorset;

struct gwm_context {
	Display *dpy;
	int screen;
	int sw, sh; // Screen width & height

	Window root;

	window_colorset cs_norm;
	window_colorset cs_act;

	enum raise_mode raise_mode;
	enum focus_mode focus_mode;
	enum launch_mode launch_mode;

	Cursor c_normal;
	Cursor c_move;
	Cursor c_resize;

	gwm_window *focused;

	gwm_window *wins; // Windows known to gwm.
};

extern gwm_context gwm;

void gwm_context_add_window(Window w);
void gwm_context_init(gwm_context *gwm, char *display_name);

#endif

