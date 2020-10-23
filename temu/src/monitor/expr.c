#include "temu.h"
#include "reg.h"
#include "memory.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
	NOTYPE = 256,

	/* TODO: Add more token types */
    EQ, NEQ, AND, OR, NEG, NOT, DEREF,
    NUM, HEX_NUM, REG
};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
    {"\\*", '*'},
    {"-", '-'},
    {"/", '/'},
    {"\\(", '('},
    {"\\)", ')'},
    {"==", EQ},			    		// equal
    {"!=", NEQ},
    {"&&", AND},
    {"\\|\\|", OR},
    {"!", NOT},
    {"^(0x)([0-9a-fA-F]+)", HEX_NUM},
    {"(0|[1-9][0-9]*)", NUM},
    {"^\\$(zero|at|v0|v1|a[0-3]|t[0-9]|s[0-7]|k0|k1|gp|sp|fp|ra)", REG},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

//				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				if (rules[i].token_type != NOTYPE) {
                    if (nr_token == 32) {
                        printf("too many tokens\n");
                        return false;
                    }
                    tokens[nr_token].type = rules[i].token_type;

                    if (rules[i].token_type >= NUM) {
                        if (substr_len > 32) {
                            printf("token is too long at position %d\n%s\n%*.s^\n", position, e, position, "");
                            return false;
                        }
                        strcpy(tokens[nr_token].str, substr_start);
                    }

                    nr_token++;
                }

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

bool check_parentheses(int p, int q) {
    int n = 0;
    if (tokens[p].type != '(' || tokens[q].type != ')') {
        return false;
    }
    while (p <= q) {
        if (tokens[p].type == '(') {
            n++;
        } else if (tokens[p].type == ')') {
            if(--n == 0 && p != q) {
                return false;
            }
        }
        p++;
    }
    if (n == 0) {
        return true;
    }
    return false;
}

uint get_priority(int type) {
    int priority = -1;
    switch (type) {
        case NEG:
        case NOT:
        case DEREF: priority++;
        case '*':
        case '/': priority++;
        case '+':
        case '-': priority++;
        case EQ:
        case NEQ: priority++;
        case AND: priority++;
        case OR: priority++;
    }
    return (uint) priority;
}

int dominant_operator(int p, int q) {
    uint min_priority = (uint) -1;
    int op = -1;
    int in_parentheses = 0;
    q++;
    while (q > p) {
        q--;
        if (tokens[q].type == ')') {
            in_parentheses++;
            continue;
        } else if (tokens[q].type == '(') {
            if (--in_parentheses < 0) {
                return -1;
            }
            continue;
        }
        if (!in_parentheses) {
            uint priority = get_priority(tokens[q].type);
            if (priority < min_priority) {
                min_priority = priority;
                op = q;
            }
        }
    }
    return op;
}

uint32_t eval(int p, int q, bool *success) {
    if (p > q) {
        *success = false;
        return 0;
    } else if (p == q) {
        *success = true;
        switch (tokens[p].type) {
            case REG: return reg_val_w(tokens[p].str + 1);
            case NUM: return atoi(tokens[p].str);
            case HEX_NUM: return strtol(tokens[p].str, NULL, 16);
            default: {
                *success = false;
                return 0;
            }
        }
    } else if (check_parentheses(p, q) == true) {
        return eval(p+1, q-1, success);
    } else {
        int op = dominant_operator(p, q);
        if (op < 0) {
            *success = false;
            return 0;
        }
        if (tokens[op].type >= NEG) {
            uint32_t val = eval(op + 1, q, success);
            if (!(*success)) return 0;
            *success = true;
            switch (tokens[op].type) {
                case NEG: return -val;
                case NOT: return !val;
                /* TODO: Imply * by memory */
                case DEREF: return mem_read(val, 4);
                default: {
                    *success = false;
                    return 0;
                }
            }
        }
        uint32_t val1 = eval(p, op - 1, success);
        if (!(*success)) return 0;
        uint32_t val2 = eval(op + 1, q, success);
        if (!(*success)) return 0;
        *success = true;
        switch (tokens[op].type) {
            case '+': return val1 + val2;
            case '-': return val1 - val2;
            case '*': return val1 * val2;
            case '/': return val1 / val2;
            case EQ: return val1 == val2;
            case NEQ: return val1 != val2;;
            case AND: return val1 && val2;;
            case OR: return val1 || val2;;
            default: {
                *success = false;
                return 0;
            }
        }
    }
}

uint32_t expr(char *e, bool *success) {
	if(e == NULL || !make_token(e)) {
		*success = false;
		return 0;
	}

    for (int i = 0; i < nr_token; ++i) {
        if (tokens[i].type == '-' && (i == 0 || (tokens[i-1].type < NUM && tokens[i-1].type != ')'))) {
            tokens[i].type = NEG;
        }
        if (tokens[i].type == '*' && (i == 0 || (tokens[i-1].type < NUM && tokens[i-1].type != ')'))) {
            tokens[i].type = DEREF;
        }
    }

	/* TODO: Insert codes to evaluate the expression. */
	return eval(0, nr_token - 1, success);
}

