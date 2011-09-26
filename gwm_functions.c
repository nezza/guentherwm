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

void move_window_up() {
	if(gwm.focused) {
		gwm_window_move_relative(gwm.focused, 0, -CONFIG_MOVE_BY);
	}
}

void move_window_down() {
	if(gwm.focused) {
		gwm_window_move_relative(gwm.focused, 0, CONFIG_MOVE_BY);
	}
}

void move_window_left() {
	if(gwm.focused) {
		gwm_window_move_relative(gwm.focused, -CONFIG_MOVE_BY, 0);
	}
}

void move_window_right() {
	if(gwm.focused) {
		gwm_window_move_relative(gwm.focused, CONFIG_MOVE_BY, 0);
	}
}

void make_fullscreen() {
	if(gwm.focused) {

	}
}

