#ifndef GWM_WINDOW_H_
#define GWM_WINDOW_H_ 1

#include <X11/Xlib.h>

#include "gwm_enums.h"
#include "gwm_types.h"
#include "gwm_context.h"

struct gwm_window {
	Window win;

	enum focus_mode focus_mode;

	gwm_window *next;
	gwm_window *prev;

	gwm_workspace *spc;

	void *organizer_data;
};

gwm_window *gwm_window_get_window(gwm_window *wins, Window w);

gwm_window *gwm_create_window(gwm_workspace *spc, Window w);
gwm_window *gwm_window_get_first(gwm_window *win);
gwm_window *gwm_window_get_last(gwm_window *win);

void gwm_window_move(gwm_window *win, unsigned x, unsigned y);
void gwm_window_move_relative(gwm_window *win, unsigned x, unsigned y);
void gwm_window_resize(gwm_window *win, unsigned w, unsigned h);
void gwm_window_resize_relative(gwm_window *win, unsigned w, unsigned h);
void gwm_window_fullscreen(gwm_window *win);
void gwm_window_show(gwm_window *win);
void gwm_window_hide(gwm_window *win);

#endif

