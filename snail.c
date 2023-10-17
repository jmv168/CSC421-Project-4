#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<sys/time.h>
#include<assert.h>

#include "snail.h"

/*
 * author: burt rosenberg
 * last update:
 *    20 september 2022
 *
 */

int is_verbose_g = 0 ; 
struct Board board_g ;

void * draw_thread(void * the_args) {
	//printf("Start of draw_thread");
	struct T_arg * t_arg = (struct T_arg *) the_args ;
	struct timeval tv;
    	struct timespec ts;
    
    char * board_string ;
    
	board_string = (char *) malloc(BOARD_SIZE+1) ;
	assert(board_string) ;


	while (1) {
	   //printf("Draw_thread loop entering");
	    pthread_mutex_lock(t_arg->mutex) ;
	    {
	        if (t_arg->req_exit) {
	            pthread_mutex_unlock(t_arg->mutex);
	            break;
	        }
	        
	        if (snail_collide(&board_g)) {                     // If snail crashes with the gate, print it, and game over 
	            if (board_g.gate_state == GATE_IS_CLOSED) {
	                printf("Game Over! Snail has tragically crashed!\n");
			exit(0);
	            }
	        }
	        
	        if (board_gameover(&board_g)) {                // If the game is over and snail passed, print it, break out of loop and game is over
	            printf("Success! Snail has made it through!\n");
		    exit(0);
	        }
	        
	        if (board_g.guess == board_g.gate_key) {   // If the user's guess matches the key, open the gate
	            board_g.gate_state = GATE_IS_OPEN;
	        }
	        
	        board_draw(board_string, &board_g);
	        printf("%s\n", board_string);

	        board_g.snail_loc++;
	        
	        sleep(SLEEP_TIME);
	    }   
	    pthread_mutex_unlock(t_arg->mutex) ;
	}
	

	pthread_exit(NULL) ;
	assert(0==1) ; /* never gets here */
}


int main(int argc, char * argv[]) {
	//printf("Start of main function");
	int ch ;
	pthread_t thread_id ;
	char buf[LINE_MAX] ;
	struct T_arg t_arg ;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	
	
	while ((ch = getopt(argc, argv, "v")) != -1) {
		switch(ch) {
			case 'v':
				is_verbose_g += 1 ;
				break ;
			default:
				printf(USAGE_MSG, PROG_NAME) ;
				return 0 ;
		}
	}
	argc -= optind;
	argv += optind;
	
	init_board( &board_g ) ;
	printf(INSTRUCTIONS, QUIT_STRING) ;
	
	if (is_verbose_g) {
		printf("(%s,%d): key %c\n",__FILE__,__LINE__,board_g.gate_key) ;
	}
//////
	t_arg.req_exit = 0;
	t_arg.mutex = &mutex;
	t_arg.cond = &cond;
	
	pthread_create(&thread_id, NULL, draw_thread, (void *) &t_arg);

	while (fgets(buf,LINE_MAX,stdin) != NULL) {
	    //printf("Entering main while loop");
	    buf[strcspn(buf, "\n")] = 0; // removing newline character
	    
	    if (strcmp(buf, QUIT_STRING) == 0) {
	        break;
	    }
	    
	    pthread_mutex_lock(&mutex);
	    board_g.guess = buf[0]; // Assuming first character of input is the guess
	    pthread_mutex_unlock(&mutex);
            
	}
	//printf("Outside of main while loop");

	
	t_arg.req_exit = 1;
//////
	
	if (is_verbose_g) {
		printf("(%s,%d): requesting thread to exit ...\n", __FILE__,__LINE__) ;
	}

// ***
// stuff to do 
	pthread_join(thread_id, NULL);
// ***	
	
	
	pthread_exit(NULL) ; /* wait for the draw thread to exit */
	assert(0==1) ; /* never gets here */
}
