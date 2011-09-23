#ifndef GWM_EVENT_H_
#define GWM_EVENT_H_ 1

#include "gwm_context.h"

typedef void(*gwm_event_handler)(gwm_context *gwm, XEvent *e);

//extern gwm_event_handler handler[LASTEvent];

#endif

