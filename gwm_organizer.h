#ifndef GWM_ORGANIZER_H_
#define GWM_ORGANIZER_H_ 1

#include "gwm_types.h"

struct gwm_organizer {
	void (*add_window)(gwm_window *win);
	void (*remove_window)(gwm_window *win);
	void (*free_organizer)(gwm_workspace *spc);
	void (*move_left)(gwm_window *win);
	void (*move_right)(gwm_window *win);
	void *data;
};

#endif

