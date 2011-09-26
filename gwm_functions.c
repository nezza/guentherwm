#include "gwm_context.h"
#include "gwm_window.h"
#include "gwm_focus_raise.h"
#include "config.h"

#include <unistd.h>
#include <stdio.h>

void spawn() {
	if(fork() == 0) {
		if(gwm.dpy) {
			close(ConnectionNumber(gwm.dpy));
		}
		setsid();
		char *cmd[] = { "dmenu_run", NULL };
		execvp(cmd[0], cmd);
	}
}

void focus_next() {
	if(gwm.focused) {
		gwm_window *win;
		if((win = gwm_get_window(gwm.focused->win))) {
			if(win->next) {
				focus(win->next);
			} else {
				focus(gwm.wins);
			}
		}
	} else {
		puts("Unknown window.");
	}
}

void focus_prev() {
	if(gwm.focused) {
		gwm_window *win;
		if((win = gwm_get_window(gwm.focused->win))) {
			if(win->prev) {
				focus(win->prev);
			} else {
				focus(gwm_window_get_last(gwm.focused));
			}
		}
	} else {
		puts("Unknown window.");
	}
}

void _move_window(unsigned x, unsigned y) {
	if(gwm.focused) {
		gwm_window_move_relative(gwm.focused, x, y);
	}
}

void move_window_up() {_move_window(0, -CONFIG_MOVE_BY);}
void move_window_down() {_move_window(0, CONFIG_MOVE_BY);}
void move_window_left() {_move_window(-CONFIG_MOVE_BY, 0);}
void move_window_right() {_move_window(CONFIG_MOVE_BY, 0);}

void _resize_window(unsigned w, unsigned h) {
	if(gwm.focused) {
		puts("RESIZING :)");
		gwm_window_resize_relative(gwm.focused, w, h);
	}
}

void resize_window_up() {_resize_window(0, -CONFIG_RESIZE_BY);}
void resize_window_down() {_resize_window(0, CONFIG_RESIZE_BY);}
void resize_window_left() {_resize_window(-CONFIG_RESIZE_BY, 0);}
void resize_window_right() {_resize_window(CONFIG_RESIZE_BY, 0);}
