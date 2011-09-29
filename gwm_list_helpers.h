#ifndef GWM_LIST_HELPERS
#define GWM_LIST_HELPERS 1

#define GWM_LIST_REMOVE(list_obj) \
		if(list_obj->prev) { \
			list_obj->prev->next = list_obj->next; \
			if(list_obj->next) { \
				list_obj->next->prev = list_obj->prev; \
			} \
		} \
		if(list_obj->next) { \
			list_obj->next->prev = list_obj->prev; \
			if(list_obj->prev) { \
				list_obj->prev->next = list_obj->next; \
			} \
		}

#endif

