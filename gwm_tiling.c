#include "gwm_tiling.h"
#include "gwm_workspace.h"
#include "gwm_window.h"

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
	return ret;
}

void gwm_tiling_destroy_column(gwm_tiling_column *column) {
	if(column->data->columns == column) {
		column->data->columns = column->next;
	}
	if(column->prev) {
		if(column->next) {
			column->prev->next = column->next;
			column->next->prev = column->prev;
		} else {
			column->prev->next = NULL;
		}
	}
	if(column->next) {
		if(column->prev) {
			column->next->prev = column->prev;
			column->prev->next = column->next;
		} else {
			column->next->prev = NULL;
		}
	}
	gwm_tiling_reorganize_all(column->data->spc);
	free(column);
}

void gwm_tiling_add_window(gwm_window *win) {
	gwm_tiling_data *data = win->spc->organizer->data;
	win->organizer_data = malloc(sizeof(gwm_tiling_window_data));
	if(!win->organizer_data) {
		printf("Malloc failed!\n");
		exit(EXIT_FAILURE);
	}
	gwm_tiling_window_data *wdata = win->organizer_data;
	wdata->column = data->active_column;
	wdata->data = data;

	gwm_tiling_move_resize(win);
}

void gwm_tiling_remove_window(gwm_window *win) {
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
		gwm_tiling_window_data *wdata = win->organizer_data;
		if(wdata->column == column) ret++;
	}
	return ret;
}

void gwm_tiling_move_resize(gwm_window *win) {
	gwm_tiling_window_data *wdata = win->organizer_data;
	gwm_tiling_data *data = wdata->data;

	// Column width
	unsigned int cw = win->spc->gwm->sw / data->column_count;
	gwm_window_move(win, cw * wdata->column->column_nr, 0);
	gwm_window_resize(win, cw, win->spc->gwm->sh);
}

void gwm_tiling_move_window_left(gwm_window *win) {
	gwm_tiling_window_data *wdata = win->organizer_data;
	gwm_tiling_column *orig_column = wdata->column;
	if(wdata->column->prev) {
		wdata->column = wdata->column->prev;
		gwm_tiling_move_resize(win);
	} else {
		wdata->column = gwm_tiling_create_column(
				wdata->data,
				NULL,
				wdata->column);
		wdata->data->columns = wdata->column;
		gwm_tiling_reorganize_all(win->spc);
	}
	if(!gwm_tiling_column_count_windows(win->spc, orig_column)) {
		gwm_tiling_destroy_column(orig_column);
	}
}

void gwm_tiling_move_window_right(gwm_window *win) {
	gwm_tiling_window_data *wdata = win->organizer_data;
	gwm_tiling_column *orig_column = wdata->column;
	if(wdata->column->next) {
		wdata->column = wdata->column->next;
		gwm_tiling_move_resize(win);
	} else {
		wdata->column = gwm_tiling_create_column(
				wdata->data,
				wdata->column,
				NULL);
		gwm_tiling_reorganize_all(win->spc);
	}
	if(!gwm_tiling_column_count_windows(win->spc, orig_column)) {
		gwm_tiling_destroy_column(orig_column);
	}
}

