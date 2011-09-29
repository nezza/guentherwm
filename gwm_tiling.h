#ifndef GWM_TILING_H_
#define GWM_TILING_H_ 1

#include "gwm_types.h"

typedef struct gwm_tiling_column gwm_tiling_column;
typedef struct gwm_tiling_data gwm_tiling_data;
typedef struct gwm_tiling_window gwm_tiling_window;

struct gwm_tiling_column {
	gwm_tiling_data *data;
	unsigned int column_nr;
	gwm_tiling_window *wins;
	gwm_tiling_window *focus;
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

struct gwm_tiling_window {
	gwm_tiling_data *data;
	gwm_tiling_column *column;
	gwm_window *win;
	gwm_tiling_window *prev;
	gwm_tiling_window *next;
};

void gwm_tiling_init(gwm_workspace *spc);

// The functions that are needed for each organizer:
void gwm_tiling_org_add_window(gwm_window *win);
void gwm_tiling_org_remove_window(gwm_window *win);
void gwm_tiling_org_free(gwm_workspace *spc);
void gwm_tiling_org_move_window_left(gwm_window *win);
void gwm_tiling_org_move_window_right(gwm_window *win);
void gwm_tiling_org_focus(gwm_workspace *spc);
void gwm_tiling_org_focus_window(gwm_window *win);
void gwm_tiling_org_select_left(gwm_window *win);
void gwm_tiling_org_select_right(gwm_window *win);

void gwm_tiling_reorganize_all(gwm_workspace *spc);

// Column functions
gwm_tiling_column *gwm_tiling_create_column(
		gwm_tiling_data *data,
		gwm_tiling_column *prev,
		gwm_tiling_column *next);
void gwm_tiling_destroy_column(gwm_tiling_column *column);
gwm_tiling_column *gwm_tiling_column_get_last(gwm_tiling_column *column);
unsigned gwm_tiling_column_count_windows(gwm_tiling_column *column);
void gwm_tiling_column_focus(gwm_tiling_column *col);

// Window functions
gwm_tiling_window *gwm_tiling_window_get_last(gwm_tiling_window *win);
void gwm_tiling_move_resize(gwm_window *win);

#endif

