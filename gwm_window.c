#include "gwm_window.h"

#include <stdio.h>
#include <stdlib.h>

gwm_window *gwm_window_get_window(gwm_window *wins, Window w) {
	gwm_window *cur;
	for(cur = wins; cur; cur = cur->next) {
		if(cur->win == w) return cur;
	}
	return NULL;
}

gwm_window *gwm_create_window(gwm_workspace *spc, Window w) {
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

	ret->spc = spc;

	ret->organizer_data = NULL;

	return ret;
}

gwm_window *gwm_window_get_first(gwm_window *win) {
	for(; win->prev; win = win->prev);
	return win;
}

gwm_window *gwm_window_get_last(gwm_window *win) {
	for(; win->next; win = win->next);
	return win;
}

void gwm_window_move(gwm_window *win, unsigned x, unsigned y) {
	XMoveWindow(gwm.dpy, win->win, x, y);
}

void gwm_window_move_relative(gwm_window *win, unsigned x, unsigned y) {
	XWindowAttributes wa;
	XGetWindowAttributes(gwm.dpy, win->win, &wa);
	gwm_window_move(win, wa.x + x, wa.y + y);
}

void gwm_window_resize(gwm_window *win, unsigned w, unsigned h) {
	XResizeWindow(gwm.dpy, win->win, w, h);
}

void gwm_window_resize_relative(gwm_window *win, unsigned w, unsigned h) {
	XWindowAttributes wa;
	XGetWindowAttributes(gwm.dpy, win->win, &wa);
	gwm_window_resize(win, wa.width + w, wa.height + h);
}

void gwm_window_fullscreen(gwm_window *win) {
	XMoveResizeWindow(gwm.dpy, win->win, -1, -1, gwm.sw+1, gwm.sh+1);
}

void gwm_window_show(gwm_window *win) {
	XMapWindow(win->spc->gwm->dpy, win->win);
}

void gwm_window_hide(gwm_window *win) {
	XUnmapWindow(win->spc->gwm->dpy, win->win);
}

