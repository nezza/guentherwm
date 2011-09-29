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

void terminal() {
	if(fork() == 0) {
		if(gwm.dpy) {
			close(ConnectionNumber(gwm.dpy));
		}
		setsid();
		char *cmd[] = {"/usr/bin/urxvt",
			"-bg", "black",
			"-fg", "white",
			"+sb", "-fn",
			"xft:Courier 10 Pitch:pixelsize=14", NULL};
		execvp(cmd[0], cmd);
	}
}

void focus_next() {
	if(gwm.focused) {
		gwm_window *win;
		if((win = gwm_workspace_get_window(gwm.active, gwm.focused->win))) {
			if(win->next) {
				focus(win->next);
			} else {
				focus(gwm.active->wins);
			}
		}
	} else {
		puts("Unknown window.");
	}
}

void focus_prev() {
	if(gwm.focused) {
		gwm_window *win;
		if((win = gwm_workspace_get_window(gwm.active, gwm.focused->win))) {
			if(win->prev) {
				focus(win->prev);
			} else {
				focus(gwm_window_get_last(gwm.active->wins));
			}
		}
	} else {
		puts("Unknown window.");
	}
}

void new_workspace() {
	gwm_workspace *new = gwm_workspace_create(&gwm);
	gwm_workspace_hide(gwm.active);
	gwm.active = new;
}

void next_workspace() {
	if(gwm.active->next) {
		gwm_workspace_hide(gwm.active);
		gwm.active = gwm.active->next;
		gwm_workspace_show(gwm.active);
	}
}

void prev_workspace() {
	if(gwm.active->prev) {
		gwm_workspace_hide(gwm.active);
		gwm.active = gwm.active->prev;
		gwm_workspace_show(gwm.active);
	}
}

void hide_workspace() {
	gwm_workspace_hide(gwm.active);
}

void show_workspace() {
	gwm_workspace_show(gwm.active);
}

void move_to_next_workspace() {
	if(gwm.focused && gwm.active->next) {
		gwm_window_hide(gwm.focused);
		gwm_workspace_remove_window(gwm.focused);
		gwm_workspace_add_window(gwm.active->next, gwm.focused);
		unfocus(gwm.focused);
		gwm.focused = NULL;
	}
}

void move_to_prev_workspace() {
	if(gwm.focused && gwm.active->prev) {
		gwm_window_hide(gwm.focused);
		gwm_workspace_remove_window(gwm.focused);
		gwm_workspace_add_window(gwm.active->prev, gwm.focused);
		unfocus(gwm.focused);
		gwm.focused = NULL;
	}
}

static void _move_window(unsigned x, unsigned y) {
	if(gwm.focused) {
		gwm_window_move_relative(gwm.focused, x, y);
	}
}

void move_window_up() {_move_window(0, -CONFIG_MOVE_BY);}
void move_window_down() {_move_window(0, CONFIG_MOVE_BY);}

void move_window_left() {
	gwm.focused->spc->organizer->move_left(gwm.focused);
}
void move_window_right(){
	gwm.focused->spc->organizer->move_right(gwm.focused);
}

//void move_window_left() {_move_window(-CONFIG_MOVE_BY, 0);}
//void move_window_right() {_move_window(CONFIG_MOVE_BY, 0);}

static void _resize_window(unsigned w, unsigned h) {
	if(gwm.focused) {
		gwm_window_resize_relative(gwm.focused, w, h);
	}
}

void resize_window_up() {_resize_window(0, -CONFIG_RESIZE_BY);}
void resize_window_down() {_resize_window(0, CONFIG_RESIZE_BY);}
void resize_window_left() {_resize_window(-CONFIG_RESIZE_BY, 0);}
void resize_window_right() {_resize_window(CONFIG_RESIZE_BY, 0);}

void fullscreen() {
	if(gwm.focused) {
		gwm_window_fullscreen(gwm.focused);
	}
}

