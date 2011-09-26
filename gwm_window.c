#include "gwm_window.h"

#include <stdio.h>
#include <stdlib.h>

gwm_window *gwm_get_window(Window w) {
	if(gwm.wins) {
		gwm_window *cur;
		for(cur = gwm.wins; cur; cur = cur->next) {
			if(cur->win == w) return cur;
		}
	}
	return NULL;
}

gwm_window *gwm_create_window(Window w) {
	gwm_window *ret = malloc(sizeof(gwm_window));
	if(!ret) {
		printf("Malloc failed!");
		exit(EXIT_FAILURE);
	}
	ret->win = w;

	// TODO: Focus mode based on config!
	ret->focus_mode = focus_on_click;
	ret->next = NULL;
	ret->prev = NULL;

	return ret;
}

gwm_window *gwm_window_get_last(gwm_window *win) {
	if(!win) return NULL;
	gwm_window *cur;
	for(cur = win; cur; win = cur, cur = cur->next);
	return win;
}

void gwm_window_move_relative(gwm_window *win, unsigned x, unsigned y) {
	XWindowAttributes wa;
	XGetWindowAttributes(gwm.dpy, win->win, &wa);
	XMoveWindow(gwm.dpy, win->win, wa.x + x, wa.y + y);
}

