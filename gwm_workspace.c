#include "gwm_workspace.h"

#include <stdlib.h>

gwm_workspace *gwm_workspace_create(gwm_context *gwm) {
	gwm_workspace *ret = malloc(sizeof(gwm_workspace));
	if(!ret) return NULL;
	ret->gwm = gwm;
	ret->wins = NULL;
	if(gwm->spcs) {
		gwm_workspace *last = gwm_workspace_get_last(gwm->spcs);
		ret->prev = last;
		last->next = ret;
	}
	ret->next = NULL;
	return ret;
}

void gwm_workspace_add_window(gwm_workspace *spc, gwm_window *win) {
	// If there are no windows yet, we need to store the first window
	// in the workspace.
	if(!spc->wins) {
		spc->wins = win;
	} else { // Else we put it at the end of the window chain of the space.
		gwm_window *last = gwm_window_get_last(spc->wins);
		win->prev = last;
		last->next = win;
	}
	win->next = NULL;
}

// Find a window on the workspace. Can also be used to just determine
// if the window is on the workspace.
gwm_window *gwm_workspace_get_window(gwm_workspace *spc, Window w) {
	return gwm_window_get_window(spc->wins, w);
}

void gwm_workspace_show(gwm_workspace *spc) {
	gwm_window *cur;

	// Iterate over all windows on the workspace.
	for(cur = spc->wins; cur; cur = cur->next) {
		// Map ('show') each window.
		XMapWindow(spc->gwm->dpy, cur->win);
	}
}

void gwm_workspace_hide(gwm_workspace *spc) {
	gwm_window *cur;

	// Iterate over all windows on the workspace.
	for(cur = spc->wins; cur; cur = cur->next) {
		// Unmap ('hide') each window.
		XUnmapWindow(spc->gwm->dpy, cur->win);
	}
}

gwm_workspace *gwm_workspace_get_last(gwm_workspace *spc) {
	for(; spc->next; spc = spc->next);
	return spc;
}

