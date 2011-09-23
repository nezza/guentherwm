#include "gwm_context.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>

extern gwm_context gwm;
extern int (*xerrorxlib)(Display *, XErrorEvent *);
extern int xerrorstart(Display *dpy, XErrorEvent *ee);

void gwm_context_add_window(Window w) {
	gwm_window *new = gwm_create_window(w);
	if(!gwm.wins) {
		gwm.wins = new;
	} else {
		gwm_window *last = gwm_window_get_last(gwm.wins);
		new->prev = last;
		last->next = new;
	}
}

void gwm_context_init(gwm_context *gwm, char *display_name) {
	// Start of by open the default display.
	gwm->dpy = XOpenDisplay(display_name);
	if(!gwm->dpy) {
		printf("Failed to open default display.");
		exit(EXIT_FAILURE);
	}

	// Temporary error handler to get the error for the next request.
	xerrorxlib = XSetErrorHandler(xerrorstart);
	// We try to get SubstructureRedirectMask. This is only needed by
	// WMs, and can only be selected by one X client at a time.
	// If this fails, it means that there's already another WM running.
	XSelectInput(
			gwm->dpy,
			DefaultRootWindow(gwm->dpy),
			SubstructureRedirectMask|ButtonPressMask);
	XSync(gwm->dpy, False);

	// Get the default screen for the display we're running on.
	// (Remember that display means Xserver, and one display can as such
	// spread over multiple screens.)
	gwm->screen = DefaultScreen(gwm->dpy);
	gwm->sw = DisplayWidth(gwm->dpy, gwm->screen);
	gwm->sh = DisplayHeight(gwm->dpy, gwm->screen);
	gwm->root = RootWindow(gwm->dpy, gwm->screen);
	printf("Screen geom: %d %d\n", gwm->sw, gwm->sh);
	gwm->c_normal = XCreateFontCursor(gwm->dpy, XC_left_ptr);
	gwm->c_move   = XCreateFontCursor(gwm->dpy, XC_fleur);
	gwm->c_resize = XCreateFontCursor(gwm->dpy, XC_sizing);
	gwm->cs_norm.border = getcolor(CONFIG_BORDERCOLOR_NORMAL);
	gwm->cs_act.border  = getcolor(CONFIG_BORDERCOLOR_SELECTED);
	gwm->focus_mode = CONFIG_FOCUS_MODE;
	gwm->raise_mode = CONFIG_RAISE_MODE;
	gwm->launch_mode = CONFIG_LAUNCH_MODE;

	gwm->wins = NULL;

	XGrabKey(gwm->dpy,
			XKeysymToKeycode(gwm->dpy, XK_Tab),
			Mod1Mask,
			gwm->root,
			True,
			GrabModeAsync,
			GrabModeAsync);


	XSetWindowAttributes wa;
	wa.cursor = gwm->c_normal;
	wa.event_mask = SubstructureRedirectMask|
		SubstructureNotifyMask|
		ButtonPressMask|
		EnterWindowMask|
		LeaveWindowMask|
		StructureNotifyMask|
		PropertyChangeMask;
	XChangeWindowAttributes(
			gwm->dpy,
			gwm->root,
			CWEventMask|CWCursor,
			&wa);
	XSelectInput(gwm->dpy, gwm->root, wa.event_mask);
}

