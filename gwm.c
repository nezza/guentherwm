#include "gwm.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <X11/keysym.h>
#include <X11/Xlib.h>

#include "gwm_keys.h"

typedef void(*gwm_event_handler)(gwm_context *gwm, XEvent *e);

gwm_context gwm;

// Default Xlib errorhandler is saved into this.
int (*xerrorxlib)(Display *, XErrorEvent *);

unsigned long getcolor(const char *colstr) {
	Colormap cmap = DefaultColormap(gwm.dpy, gwm.screen);
	XColor color;
	if(!XAllocNamedColor(gwm.dpy, cmap, colstr, &color, &color)) {
		printf("Failed to allocate color %s\n", colstr);
		exit(EXIT_FAILURE);
	}
	return color.pixel;
}

int xerrorstart(Display *dpy, XErrorEvent *ee) {
	if(ee->error_code == BadWindow) return 0;
	printf("Another window manager is already running.\n");
	exit(EXIT_FAILURE);
}

void manage(Window w, XWindowAttributes *wa) {
	gwm_window *win;
	if(!(win = gwm_get_window(w))) {
		gwm_context_add_window(w);
	} else {
		puts("We know that window, but we do nothing....");
	}
}

void configurerequest(gwm_context *gc, XEvent *e) {
	XConfigureRequestEvent *ev = &e->xconfigurerequest;
	XWindowChanges wc;
	switch(gc->launch_mode) {
		case launch_fullscreen:
			wc.x = 1;
			wc.y = 1;
			wc.width = gc->sw-2;
			wc.height = gc->sh-2;
			break;
		case launch_upper_left:
			wc.x = 0;
			wc.y = 0;
			wc.width = ev->width;
			wc.height = ev->height;
			break;
		case launch_fit_in:
			if(ev->x > gc->sw-1) wc.x = gc->sw-1;
			else wc.x = ev->x;
			if(ev->y > gc->sh-1) wc.y = gc->sh-1;
			else wc.y = ev->y;
			if((wc.x + ev->width) > gc->sw-2)
				wc.width = (gc->sw - wc.x) - 2;
			else
				wc.width = ev->width;
			if((wc.y + ev->height) > gc->sh-2)
				wc.height = (gc->sh - wc.y) - 2;
			else
				wc.height = ev->height;
			break;
		default:
			puts("Unknown launch mode!");
			return;
	}
	wc.border_width = ev->border_width;
	wc.sibling = ev->above;
	wc.stack_mode = ev->detail;
	printf("configure request: %d %d %d %d\n", 
			wc.x, wc.y, wc.width, wc.height);
	XConfigureWindow(gwm.dpy, ev->window, ev->value_mask, &wc);
	XSync(gwm.dpy, False);
}

void maprequest(gwm_context *gc, XEvent *e) {
	XWindowAttributes wa;
	XMapRequestEvent *ev = &e->xmaprequest;

	if(!XGetWindowAttributes(gwm.dpy, ev->window, &wa)) {
		puts("Failed to get attributes.");
		return;
	}
	// This is a notification to the window manager to tell him that we 
	// shouldn't fiddle with the map requests of this window.
	// http://tronche.com/gui/x/xlib/window/attributes/override-redirect.html
	if(wa.override_redirect) {
		puts("Override redirect is set.");
		return;
	}
	printf("Window attributes: %d %d %d %d\n",
			wa.x, wa.y, wa.width, wa.height);
	manage(ev->window, &wa);
	XWindowChanges wc;
	wc.border_width = 1;
	XConfigureWindow(gwm.dpy, ev->window, CWBorderWidth, &wc);
	XSetWindowBorder(gwm.dpy, ev->window, gwm.cs_norm.border);
	XSelectInput(gwm.dpy, ev->window,
			EnterWindowMask|
			FocusChangeMask|
			PropertyChangeMask|
			StructureNotifyMask);
	XGrabButton(
			gwm.dpy,
			AnyButton,
			AnyModifier,
			ev->window,
			False,
			ButtonPressMask|ButtonReleaseMask,
			GrabModeSync,
			GrabModeSync,
			None,
			None);
	XRaiseWindow(gwm.dpy, ev->window);
	XMapWindow(gwm.dpy, ev->window);
}
void enternotify(gwm_context *gc, XEvent *e){
	XCrossingEvent *ev = &e->xcrossing;
	if(gwm.raise_mode == raise_on_mouse) {
		XRaiseWindow(gwm.dpy, ev->window);
	}
	if(gwm.focus_mode == focus_on_mouse) {
		gwm_window *win = gwm_get_window(ev->window);
		if(win) {
			focus(win);
		}
	}
}
void buttonpress(gwm_context *gc, XEvent *e) {
	XButtonPressedEvent *ev = &e->xbutton;
	XAllowEvents(gwm.dpy, ReplayPointer, ev->time);
	if(ev->window != gwm.root) {
		if(gwm.raise_mode == raise_on_click) {
			XRaiseWindow(gwm.dpy, ev->window);
		}
		if(gwm.focus_mode == focus_on_click) {
			gwm_window *win = gwm_get_window(ev->window);
			if(win) {
				focus(win);
			}
		}
	}
}

void keypress(gwm_context *gc, XEvent *e) {
	XKeyEvent *ev = &e->xkey;
	int i;
	for(i=0; keys[i].func; i++) {
		if((ev->state == keys[i].modifier) &&
				(ev->keycode == XKeysymToKeycode(gwm.dpy, keys[i].key))) {
			keys[i].func();
		}
	}
}

gwm_event_handler handler[LASTEvent] = {
	[ButtonPress] = buttonpress,
	[MapRequest] = maprequest,
	[EnterNotify] = enternotify,
	[ConfigureRequest] = configurerequest,
	[KeyPress] = keypress
};
int main() {
	gwm_context_init(&gwm, NULL);
	// main loop:
	XEvent ev;
	while(!XNextEvent(gwm.dpy, &ev)) {
		if(handler[ev.type]) {
			handler[ev.type](&gwm, &ev);
		} else {
			printf("Unhandled event: %d\n", ev.type);
		}
	}
}
