#ifndef GWM_KEYS_H_
#define GWM_KEYS_H_ 1

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "gwm_functions.h"

typedef struct {
	unsigned int modifier;
	KeySym key;
	void (*func)();
} gwm_keys;

extern gwm_keys keys[];

#endif

