#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<sys/time.h>
#include<assert.h>
#include<time.h>

#include "snail.h"

/*
 * author: burt rosenberg
 * last update:
 *    20 september 2022
 *
 */

void init_board(struct Board * board) {
	unsigned int r ;
	board->snail_loc = 0 ;
	board->gate_loc = GATE_LOC_1 ;
	//srandomdev();
	srandom(time(NULL)) ;
	r = random()%9+1 ;
	board->gate_key = (char)(r+'0') ;
	return ;
}

char * board_gate_draw(char * buf, struct Board * board) {
	int i , j ;
	char gate_char = CLOSED_GATE_CHAR ;
	char * t = (char *) malloc(1+strlen(GATE_EMOJI_FORMAT)) ;
	assert(t) ;
	if (board->gate_state==GATE_IS_OPEN) 
		gate_char = OPENED_GATE_CHAR ;
	j = sprintf(t,GATE_EMOJI_FORMAT,gate_char) ;
	for (i=0;i<j;i++) 
		buf[i+board->gate_loc] = t[i] ;
	free(t) ;
	return buf ;
}

char * board_snail_draw(char * buf, struct Board * board) {
	int i ; 
	for (i=0;i<strlen(SNAIL_EMOJI);i++)
		buf[board->snail_loc+i] = SNAIL_EMOJI[i] ;
	return buf ;
}

char * board_draw(char * buf, struct Board * board) {
	int i ;
	for (i=0;i<BOARD_SIZE;i++) {
		buf[i] = '_' ;
	}
	buf[BOARD_SIZE] = '\0' ;
	board_gate_draw(buf,board) ;
	board_snail_draw(buf,board) ;
	return buf ;
}

int snail_collide(struct Board * board) {
	return board->snail_loc==board->gate_loc ;
}

int board_gameover(struct Board * board) {
	return (board->snail_loc-BOARD_SIZE)>=-strlen(SNAIL_EMOJI) ;
}

/* end of file */
