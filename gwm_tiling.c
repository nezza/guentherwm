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
	data->columns = 2;
	data->active_column = 0;
}

void gwm_tiling_free(gwm_workspace *spc) {
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
	gwm_window *cur;
	for(cur = spc->wins; cur; cur = cur->next) {
		gwm_tiling_move_resize(cur);
	}
}

void gwm_tiling_move_resize(gwm_window *win) {
	gwm_tiling_window_data *wdata = win->organizer_data;
	gwm_tiling_data *data = wdata->data;

	// Column width
	unsigned int cw = win->spc->gwm->sw / data->columns;
	gwm_window_move(win, cw * wdata->column, 0);
	gwm_window_resize(win, cw, win->spc->gwm->sh);
}

void gwm_tiling_move_window_left(gwm_window *win) {
	gwm_tiling_window_data *wdata = win->organizer_data;
	if(wdata->column > 0) {
		wdata->column--;
		gwm_tiling_move_resize(win);
	} else {
		//TODO
	}
}

void gwm_tiling_move_window_right(gwm_window *win) {
	gwm_tiling_window_data *wdata = win->organizer_data;
	if(wdata->column < (wdata->data->columns - 1)) {
		wdata->column++;
		gwm_tiling_move_resize(win);
	} else {
		wdata->data->columns++;
		gwm_tiling_reorganize_all(win->spc);
	}
}

