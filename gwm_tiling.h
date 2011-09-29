#ifndef GWM_TILING_H_
#define GWM_TILING_H_ 1

#include "gwm_types.h"

typedef struct gwm_tiling_column gwm_tiling_column;
typedef struct gwm_tiling_data gwm_tiling_data;

struct gwm_tiling_column {
	gwm_tiling_data *data;
	unsigned int column_nr;
	gwm_tiling_column *prev;
	gwm_tiling_column *next;
};

//TODO: Not only column based.
struct gwm_tiling_data {
	gwm_tiling_column *columns;
	gwm_tiling_column *active_column;
	unsigned int column_count;
	gwm_workspace *spc;
};

typedef struct {
	gwm_tiling_data *data;
	gwm_tiling_column *column;
} gwm_tiling_window_data;

void gwm_tiling_init(gwm_workspace *spc);
void gwm_tiling_free(gwm_workspace *spc);
gwm_tiling_column *gwm_tiling_create_column(
		gwm_tiling_data *data,
		gwm_tiling_column *prev,
		gwm_tiling_column *next);
void gwm_tiling_add_window(gwm_window *win);
void gwm_tiling_remove_window(gwm_window *win);
void gwm_tiling_reorganize_all(gwm_workspace *spc);
void gwm_tiling_move_resize(gwm_window *win);
void gwm_tiling_move_window_left(gwm_window *win);
void gwm_tiling_move_window_right(gwm_window *win);

#endif

