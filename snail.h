/*
 * name: snail.h
 * last-update:
 * 		20 sep 2022 -bjr: created
 *
 */

#ifndef _SNAIL_H
#define _SNAIL_H 1

#define BOARD_SIZE 16

#define USAGE_MSG "usage: %s [-v]\n"
#define PROG_NAME "snail"

#define SLEEP_TIME 2
#define LINE_MAX 1024
#define QUIT_STRING "quit"
#define INSTRUCTIONS "***\n*** Guess the number to open the gate for the snail! (\"%s\" to quit)\n***\n" 

#define SNAIL_EMOJI "@/"
#define GATE_EMOJI_FORMAT "[%c]"
#define CLOSED_GATE_CHAR '?'
#define OPENED_GATE_CHAR ' '
#define GATE_IS_CLOSED 0
#define GATE_IS_OPEN 1
#define GATE_LOC_1 8

struct Board {
	int snail_loc ;
	char guess ;
	int gate_loc ;
	int gate_key ;
	int gate_state ;
	} ;

struct T_arg {
 	int req_exit ;
 	pthread_mutex_t * mutex ;
 	pthread_cond_t * cond  ;
 	} ;
 		
extern int is_verbose_g ;

void init_board(struct Board * board) ;
char * board_draw(char * buf, struct Board * board) ;
int snail_collide(struct Board * board) ;
int board_gameover(struct Board * board)  ;

#endif /* _SNAIL_H */

