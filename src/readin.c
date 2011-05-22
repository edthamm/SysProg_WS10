/**
@file readin.c
@author Eduard Thamm 0525087
@brief Reads in Ascii-files handed as parameter
@details Reads in the file handed as parameter. File may contain only ASCII characters and lines must be shorter than 1023 symbols.
[A-Z] will be unchanged [a-z] will be converter tu upper case. All other symbols will be ignored. Content of file will be writen to a shared memory for processing by caesar.
@date 19.05.11
*/


/*gegenseitiges terminieren im fehlerfall sollte durch die abfragen auf P und V sichergestellt sein*/
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
#include <ctype.h>



char* prog;
share* mshare = NULL;
FILE* in = NULL;

/**
@brief This function cleans up the mess on errors.
@details Is called by merror() and cleanupsig() to free all resources.
*/
void cleanup(void){
	free_share(mshare);
	if(in != NULL){
		(void)fclose(in);
	}	
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
@brief Reads form file writes to shared memory
@detail This function opens a file reads it line by line than writes it to shm as soon as shm is available. Contains critical section! Lines <1024 ASCII signs.
@param infile Location of the input file 
*/
static void read_write(char* infile){

	char buffer[STR_MAX];
	int i,j;

	if((in = fopen((const char *) infile,"r")) == (FILE*) NULL){
		(void)merror(prog, "Failed to open file");
	}
	
	while(fgets(buffer,STR_MAX,in) != NULL){
		if(P(mshare->semidr) < 0){
			(void)merror(prog,"Semaphore wait failed");
		}
			j = 0;
			for(i = 0;i < (strlen(buffer)+2); i++){/*strlen counts w/o /0 -> +1; < ->+2*/
				if((buffer[i] >= 65 && buffer[i] <= 90) || (buffer[i] >= 97 && buffer[i] <= 122)){
					mshare->shm->data[i-j] = toupper(buffer[i]);
				}
				else{
					j++;
				}
			}
			if(j>0){
				mshare->shm->data[strlen(buffer)+2-j]='\0';
			}
			else{
				mshare->shm->data[strlen(buffer)+1]='\0';
			}
			printf("%s\n",mshare->shm->data);
		
		if(V(mshare->semidc) < 0){
			(void)merror(prog,"Failed to signal Semaphore to Caesar");
		}
	}

	if(P(mshare->semidr) < 0){
		(void)merror(prog,"Semaphore wait for state change failed");
	}

	mshare->shm->state = 1;
	
	if(V(mshare->semidc) < 0){
		(void)merror(prog,"Failed to signal Semaphore to Caesar");
	}
	
	(void)fclose(in);
	in = NULL;
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
		(void)fprintf(stderr,"Usage: %s <filename>\n",argv[0]);
		exit(EXIT_FAILURE); /*no cleanup nothing done up until now*/
	}
	mshare = check_first();
	(void)read_write(argv[1]);
	/*deletion of share will be handled by caesar*/

	exit(EXIT_SUCCESS);

}
