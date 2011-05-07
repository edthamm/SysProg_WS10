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

#define STR_MAX 1024
#define SHM_KEY 8795

static char* prog;

/**
@brief This function cleans up the mess on errors.
@details Is called by merror() and cleanupsig() to free all recources uses the globals fd, *res.
*/
static void cleanup(void){

}

/**
@brief Eases the handling of errors throughout the programm.
@param s The String that shall be printed to stderr.
*/
static void merror(char *s){
	(void)fprintf(stderr,"%s: A fatal error has occured: %s.\nCan not continue...\nExiting...\n",prog,s);
	(void)cleanup();
	exit(EXIT_FAILURE);
}

/**
@brief This function cleans up the mess after an external signal has been cought, if there is a chance to do so.
@param nSignal The SigNum.
*/
static void cleanupsig(int nSignal){
	(void)merror("Recived external termination request");
	(void)cleanup();
	exit(EXIT_FAILURE);
}

/**
@brief Trys to attatch to a shared memory and semaphore.
@return Returns 0 on a successful attatch 1 otherwise.
*/
static int attach(void){
}

/**
@brief Creates a shared memory segment and a semaphore for use by caesar.
*/
static void create(void){
}

/**
@brief Checks if caesar has been started before if so connects to resources else creates resources.
@return Returns 0 if no instance of caesar has been started 1 otherwise.
*/
static int checkfirst(void){
	if(attatch() == 0){
		return 1;
	}
	else{
		create();
		return 0;
	}
}


/**
@brief The main function. Handling arguments, sorting through the chaos.
@param argc Number of arguments passed.
@param argv[] The argument string.
*/
int main(int argc,char* argv[]){
	
	prog = argv[0];

	if(argc != 2){
		(void)fprintf(stderr,"Usage: %s <filename>",argv[0]);
		exit(EXIT_FAILURE); /*no cleanup nothing done up until now*/
	}



}
