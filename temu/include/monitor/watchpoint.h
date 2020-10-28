#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
	char exp[500];
	uint32_t old_value;

} WP;

WP* new_wp();
void free_wp(WP* wp);
WP *wp_gethead();

#endif
