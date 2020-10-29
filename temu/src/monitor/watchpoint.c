#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;
WP rep_head;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
		// *************************************
		//wp_pool[i].exp = NULL;
		wp_pool[i].old_value = 0;
		// *************************************
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	// ******************************
	rep_head.NO = -1;
	rep_head.next = NULL;
	//rep_head.exp= NULL;
	rep_head.old_value = 0;
	head = &rep_head;
	// ******************************
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
	WP *alloc = NULL;
	if(free_ != NULL){
		alloc = free_;
		free_ = free_->next; // free list lost one
		alloc->next = head->next;
		head->next = alloc;  // head insert
	}// allocate
	return alloc;
}

void free_wp(WP *wp){
	wp->next = free_;
	free_ = wp;
}

WP *get_head(){
	return head;
}

WP *wp_gethead(){
	return head;
}


