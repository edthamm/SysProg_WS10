/**
@file caesar.c
@author Eduard Thamm 0525087
@brief Encrypts/Decrypts Caesar Ciphers. Valid input value 1-25.
@details This programm takes the output of readin from shared memory and right-shifts every symbol by the distance given in the argument. Overflow will be handled modulo wise. Output will be generated to stdout.
@date 19.5.11
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include "common.h"
#include <sem182.h>


char* prog;
share* mshare = NULL;
/**
@brief This function cleans up the mess on errors.
@details Is called by merror() and cleanupsig() to free all resources.
*/
void cleanup(void){
	(void)free_share(mshare);
}



/**
@brief This function cleans up the mess after an external signal has been cought, if there is a chance to do so.
@param nSignal The SigNum.
*/
static void cleanupsig(int nSignal){
	(void)merror(prog,"Recived external termination request");
	(void)cleanup();
	exit(EXIT_FAILURE);
}

/**
@brief This function shifts code in shm by i and prints to stdout.
@detail Contains critical.
*/
static void mencrypt(long l){
	char buffer[STR_MAX];
	int i;

	if(P(mshare->semidc) < 0){
		merror(prog,"Failed to wait on sem");
	}
		if(mshare->shm->state == 0){
			for(i = 0;i < (strlen(mshare->shm->data)+2); i++){
				buffer[i] = (((mshare->shm->data[i]+l)%25)+65); /*i am not sure about this line*/
			}
		}
		else{
			(void)fprintf(stdout,"%s",buffer);
			return;
		}
	if(V(mshare->semidr) < 0){
		merror(prog,"Failed to signal Semaphore to readin");
	}
	(void)fprintf(stdout,"%s",buffer);
}

/**
@brief The main function. Handling arguments, sorting through the chaos.
@param argc Number of arguments passed.
@param argv[] The argument string.
*/
int main(int argc,char* argv[]){

	long l;

	(void)signal(SIGINT,cleanupsig);
	(void)signal(SIGQUIT,cleanupsig);
	(void)signal(SIGTERM,cleanupsig);
	(void)signal(SIGABRT,cleanupsig);
	
	prog = argv[0];

	if(argc != 2){
		(void)fprintf(stderr,"Usage: %s <integer>\n",argv[0]);
		exit(EXIT_FAILURE); /*no cleanup nothing done up until now*/
	}

	if((l = strtol(argv[1],NULL,10)) < 2 || l > 24){
		merror(prog, "Enter an integer between 1 and 25");/*did they really mean between???*/
	}

	(void)mencrypt(l);
	(void)cleanup();
	
	exit(EXIT_SUCCESS);
}
