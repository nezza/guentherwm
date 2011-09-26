#ifndef GWM_WORKSPACE_H_
#define GWM_WORKSPACE_H_ 1

#include "gwm_types.h"
#include "gwm_context.h"
#include "gwm_organizer.h"
#include "gwm_window.h"

struct gwm_workspace {
	gwm_context *gwm;
	gwm_window *wins;

	gwm_organizer *organizer;

	gwm_workspace *prev;
	gwm_workspace *next;
};

gwm_workspace *gwm_workspace_create(gwm_context *gwm);
void gwm_workspace_add_window(gwm_workspace *spc, gwm_window *win);
void gwm_workspace_remove_window(gwm_window *win);
gwm_window *gwm_workspace_get_window(gwm_workspace *spc, Window w);
void gwm_workspace_show(gwm_workspace *spc);
void gwm_workspace_hide(gwm_workspace *spc);
gwm_workspace *gwm_workspace_get_first(gwm_workspace *spc);
gwm_workspace *gwm_workspace_get_last(gwm_workspace *spc);

#endif

