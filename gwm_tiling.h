#ifndef GWM_TILING_H_
#define GWM_TILING_H_ 1

#include "gwm_types.h"

//TODO: Not only column based.
typedef struct {
	unsigned int columns;
	unsigned int active_column;
} gwm_tiling_data;

typedef struct {
	gwm_tiling_data *data;
	unsigned int column;
} gwm_tiling_window_data;

void gwm_tiling_init(gwm_workspace *spc);
void gwm_tiling_free(gwm_workspace *spc);
void gwm_tiling_add_window(gwm_window *win);
void gwm_tiling_remove_window(gwm_window *win);
void gwm_tiling_move_resize(gwm_window *win);
void gwm_tiling_move_window_left(gwm_window *win);
void gwm_tiling_move_window_right(gwm_window *win);

#endif

