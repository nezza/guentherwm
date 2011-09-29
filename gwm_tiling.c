#include "gwm_tiling.h"
#include "gwm_workspace.h"
#include "gwm_window.h"
#include "gwm_focus_raise.h"
#include "gwm_list_helpers.h"

#include <stdlib.h>
#include <stdio.h>

void gwm_tiling_init(gwm_workspace *spc) {
	spc->organizer = malloc(sizeof(gwm_organizer));
	if(!spc->organizer) {
		printf("Malloc failed!\n");
		exit(EXIT_FAILURE);
	}

	spc->organizer->add_window = gwm_tiling_add_window;
	spc->organizer->remove_window = gwm_tiling_remove_window;
	spc->organizer->free_organizer = gwm_tiling_free;
	spc->organizer->move_left = gwm_tiling_move_window_left;
	spc->organizer->move_right = gwm_tiling_move_window_right;
	spc->organizer->focus = gwm_tiling_received_focus;
	spc->organizer->select_left = gwm_tiling_select_left;
	spc->organizer->select_right = gwm_tiling_select_right;
	spc->organizer->data = malloc(sizeof(gwm_tiling_data));
	if(!spc->organizer->data) {
		free(spc->organizer);
		printf("Malloc failed!\n");
		exit(EXIT_FAILURE);
	}
	gwm_tiling_data *data = spc->organizer->data;

	data->columns = gwm_tiling_create_column(data, NULL, NULL);
	data->column_count = 1;
	data->active_column = data->columns;
	data->spc = spc;
}

void gwm_tiling_free(gwm_workspace *spc) {
}

gwm_tiling_column *gwm_tiling_create_column(
		gwm_tiling_data *data,
		gwm_tiling_column *prev,
		gwm_tiling_column *next) {
	gwm_tiling_column *ret = malloc(sizeof(gwm_tiling_column));
	if(!ret) {
		printf("Malloc failed!\n");
		exit(EXIT_FAILURE);
	}
	ret->data = data;
	ret->column_nr = 0;
	ret->prev = prev;
	if(prev) prev->next = ret;
	ret->next = next;
	if(next) next->prev = ret;
	ret->wins = NULL;
	ret->focus = NULL;
	return ret;
}

void gwm_tiling_received_focus(gwm_window *win) {
	gwm_tiling_window *wdata = win->organizer_data;
	wdata->data->active_column = wdata->column;
	wdata->column->focus = wdata;
}

void gwm_tiling_destroy_column(gwm_tiling_column *column) {
	GWM_NEXT_IF_FIRST(column->data->columns, column);
	GWM_LIST_REMOVE(column);
	gwm_tiling_reorganize_all(column->data->spc);
	free(column);
}

gwm_tiling_column *gwm_tiling_column_get_last(gwm_tiling_column *column) {
	for(;column->next;column = column->next);
	return column;
}

gwm_tiling_window *gwm_tiling_window_get_last(gwm_tiling_window *win) {
	for(;win->next;win = win->next);
	return win;
}

void gwm_tiling_add_window(gwm_window *win) {
	gwm_tiling_data *data = win->spc->organizer->data;
	win->organizer_data = malloc(sizeof(gwm_tiling_window));
	if(!win->organizer_data) {
		printf("Malloc failed!\n");
		exit(EXIT_FAILURE);
	}
	gwm_tiling_window *wdata = win->organizer_data;
	wdata->column = data->active_column;
	wdata->data = data;
	wdata->win = win;
	wdata->next = NULL;
	if(!data->active_column->wins) {
		data->active_column->wins = win->organizer_data;
	} else {
		wdata->prev = gwm_tiling_window_get_last(data->active_column->wins);
	}
	wdata->column->focus = wdata;
	gwm_tiling_move_resize(win);
	focus(win);
}

// Private function for removing a tiling window.
static void _gwm_tiling_remove_window(gwm_tiling_window *win) {
	GWM_NEXT_IF_FIRST(win->column->wins, win);
	GWM_LIST_REMOVE(win);
}

void gwm_tiling_remove_window(gwm_window *win) {
	gwm_tiling_window *wdata = win->organizer_data;
	gwm_tiling_column *col = wdata->column;
	_gwm_tiling_remove_window(wdata);
	if(!col->wins) {
		gwm_tiling_destroy_column(col);
	}
}

void gwm_tiling_reorganize_all(gwm_workspace *spc) {
	gwm_tiling_data *data = spc->organizer->data;
	gwm_window *cur;
	unsigned int cc = 0; // column count
	gwm_tiling_column *colcur;
	for(colcur = data->columns; colcur; colcur = colcur->next) {
		colcur->column_nr = cc;
		cc++;
	}
	data->column_count = cc;
	for(cur = spc->wins; cur; cur = cur->next) {
		gwm_tiling_move_resize(cur);
	}
}

unsigned gwm_tiling_column_count_windows(
		gwm_workspace *spc,
		gwm_tiling_column *column) {
	unsigned ret = 0;
	gwm_window *win;
	for(win = spc->wins; win; win = win->next) {
		gwm_tiling_window *wdata = win->organizer_data;
		if(wdata->column == column) ret++;
	}
	return ret;
}

void gwm_tiling_select_left(gwm_window *win) {
	gwm_tiling_window *wdata = win->organizer_data;
	gwm_tiling_column *col = wdata->column;
	if(col->prev) {
		gwm_tiling_column_focus(col->prev);
	}
}

void gwm_tiling_select_right(gwm_window *win) {
	gwm_tiling_window *wdata = win->organizer_data;
	gwm_tiling_column *col = wdata->column;
	if(col->next) {
		gwm_tiling_column_focus(col->next);
	}
}
void gwm_tiling_column_focus(gwm_tiling_column *col) {
	col->data->active_column = col;
	if(!col->focus) return;
	focus(col->focus->win);
}

void gwm_tiling_window_change_column(
		gwm_tiling_window *w,
		gwm_tiling_column *col) {
}

void gwm_tiling_move_resize(gwm_window *win) {
	gwm_tiling_window *wdata = win->organizer_data;
	gwm_tiling_data *data = wdata->data;

	// Column width
	unsigned int cw = win->spc->gwm->sw / (data->column_count ? data->column_count : 0);
	gwm_window_move(win, cw * wdata->column->column_nr, 0);
	gwm_window_resize(win, cw, win->spc->gwm->sh);
}

void gwm_tiling_move_window_left(gwm_window *win) {
	gwm_tiling_window *wdata = win->organizer_data;
	gwm_tiling_column *orig_column = wdata->column;
	gwm_tiling_column *prev = wdata->column->prev;
	_gwm_tiling_remove_window(wdata);
	if(prev) {
		wdata->column = prev;
		gwm_tiling_move_resize(win);
	} else {
		wdata->column = gwm_tiling_create_column(
				wdata->data,
				NULL,
				orig_column);
		wdata->data->columns = wdata->column;
		wdata->column->wins = wdata;
	}
	if(!orig_column->wins) {
		gwm_tiling_destroy_column(orig_column);
	}
	wdata->data->active_column = wdata->column;
	wdata->column->focus = wdata;
	gwm_tiling_reorganize_all(win->spc);
	focus(win);
	raise(win);
}

void gwm_tiling_move_window_right(gwm_window *win) {
	gwm_tiling_window *wdata = win->organizer_data;
	gwm_tiling_column *orig_column = wdata->column;
	gwm_tiling_column *next = wdata->column->next;
	_gwm_tiling_remove_window(wdata);
	if(next) {
		wdata->column = next;
		gwm_tiling_move_resize(win);
	} else {
		wdata->column = gwm_tiling_create_column(
				wdata->data,
				orig_column,
				NULL);
		wdata->column->wins = wdata;
	}
	if(!orig_column->wins) {
		gwm_tiling_destroy_column(orig_column);
	}
	wdata->data->active_column = wdata->column;
	wdata->column->focus = wdata;
	gwm_tiling_reorganize_all(win->spc);
	focus(win);
	raise(win);
}

