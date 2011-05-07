/**
@file readin.c
@author Eduard Thamm 0525087
@brief Reads in Ascii-files handed as parameter
@details Reads in the file handed as parameter. File may contain only ASCII characters and lines must be shorter than 1023 symbols.
[A-Z] will be unchanged [a-z] will be converter tu upper case. All other symbols will be ignored. Content of file will be writen to a shared memory for processing by caesar.
@date 19.05.11
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

/**
@brief This function cleans up the mess on errors.
@details Is called by merror() and cleanupsig() to free all resources.
*/
void cleanup(void){

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
@brief The main function. Handling arguments, sorting through the chaos.
@param argc Number of arguments passed.
@param argv[] The argument string.
*/
int main(int argc,char* argv[]){

	(void)signal(SIGINT,cleanupsig);
	(void)signal(SIGQUIT,cleanupsig);
	(void)signal(SIGTERM,cleanupsig);
	(void)signal(SIGABRT,cleanupsig);
	
	prog = argv[0];

	if(argc != 2){
		(void)fprintf(stderr,"Usage: %s <filename>",argv[0]);
		exit(EXIT_FAILURE); /*no cleanup nothing done up until now*/
	}

	exit(EXIT_SUCCESS);

}
