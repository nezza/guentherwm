#include "gwm_focus_raise.h"

#include "gwm_context.h"

void focus(gwm_window *win) {
	if(gwm.focused)
		unfocus(gwm.focused);
	XSetWindowBorder(gwm.dpy, win->win, gwm.cs_act.border);
	XSetInputFocus(gwm.dpy, win->win, RevertToPointerRoot, CurrentTime);
	XUngrabButton(gwm.dpy, AnyButton, AnyModifier, win->win);
	XGrabButton(gwm.dpy, AnyButton, AnyModifier, win->win, False, ButtonPressMask, GrabModeSync, GrabModeSync, None, None);
	win->spc->organizer->focus(win);
	gwm.focused = win;
}

void unfocus(gwm_window *win) {
	XSetWindowBorder(gwm.dpy, win->win, gwm.cs_norm.border);
	XGrabButton(gwm.dpy, AnyButton, AnyModifier, win->win, False, ButtonPressMask|ButtonReleaseMask, GrabModeSync, GrabModeSync, None, None);
}

void raise(gwm_window *win) {
	XRaiseWindow(gwm.dpy, win->win);
}

