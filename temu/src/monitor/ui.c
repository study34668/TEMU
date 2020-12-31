#include "monitor.h"
#include "temu.h"
#include "expr.h"
#include "watchpoint.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

void display_reg();

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(temu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_p(char *args) {
	bool success;
	int val = expr(args, &success);
	if (success) {
		printf("0x%08x %d\n", val, val);
	} else {
		printf("Invalid expression\n");
	}
	return 0;
}

static int cmd_w(char *args) {
	WP *result = new_wp();
	if(result == NULL){
		printf("Watchpoints have been too many So this is invalid\n");
	}else {
		strcpy(result->exp,args);
		//printf("exp in cmd is %s\n",result->exp);
		bool success;
		uint32_t eval = expr(args,&success);
		if(success){
			result->old_value = eval;
		}
		else
			printf("Invalid expression\n");
	}
	//printf("cmd is ok\n");
	return 0;
}

static int cmd_si(char *args) {
	bool success;
	int i=expr(args, &success);
	if(success){
		for(;i>0;--i){
			cpu_exec(1);
		}
	}
	else cpu_exec(1);
	return 0;
}

static int cmd_info(char *args){
	if(args == NULL){
		printf("No args.\n");
		return 0;
	}
	if(args[0] == 'r'){
		      display_reg();
	}else if(args[0] == 'w'){
		      WP *p;
		      if((p = wp_gethead()->next) == NULL){
		              printf("There is no watchpoint.\n");
		      }else{
		              while(p){
		                      printf("WatchPoint %d\n",p->NO);
		                      p = p->next;
		              }
		      }
	}else{
		      printf("Wrong args!\n");
	}
	return 0;
}

static int cmd_x(char *args){
	char* arg1 = strtok(args," ");
	if(!arg1)
	{
		printf("Two arguments are needed!\n");
		return 0;
	}

	char* arg2 = arg1 + strlen(arg1) + 1;
	arg2 = strtok(arg2," ");
	if(!arg2)
	{
		printf("Two arguments are needed!\n");
		return 0;
	}

	bool success;
	int val = expr(arg2, &success);
	if (!success) {
		printf("Invalid expression\n");
		return 0;
	}

	int N = atoi(arg1);
	printf("Ptint %d 4-bytes starting at %d...\n", N, val);
	for(int i = 0;i < N;i++)
	{
		printf("%d:",val);
		for(int j = 0;j < 4;j++)
		{
			printf(" %02x",mem_read(val, 1));
			val += 1;
		}
		printf("\n");
	}
	return 0;
}

static int cmd_d(char *args){
	WP *pre = get_head();
	WP *uf;

	bool success;
	int i = expr(args,&success);
	while(pre != NULL){
	if(pre->NO == i ){
	 uf->next = pre->next; //remove i from head
	 free_wp(pre) ;   //inserve into free_
	 printf("Completely delete the watchpoint(%d).\n",i);
	 return 0;
         }
	else{uf = pre; pre = pre->next;}
      }
 	return 0;
}

static int cmd_help(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit TEMU", cmd_q },
	{ "si", "Single Step", cmd_si },
	/* TODO: Add more commands */
	{ "p", "Print value of the expression", cmd_p },
	{ "w", "Set a watchpoint with an expression", cmd_w },
	{ "info", "Print states of program", cmd_info},
	{ "x", "Print N consecutive 4-bytes in hexadecimal format starting at EXPR.", cmd_x},
	{ "d", "Delete a watchpoint with an expression", cmd_d}
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
