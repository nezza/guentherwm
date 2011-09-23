#include "gwm_events.h"
#include "gwm_context.h"

void buttonpress2(gwm_context *gwm, XEvent *e) {
	puts("BTN!!!!");
}

gwm_event_handler handler[LASTEvent] = {
	[ButtonPress] = buttonpress,
};

