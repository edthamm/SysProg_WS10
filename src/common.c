/**
@file common.c
@author Eduard Thamm
@brief Contains all routines common to both caesar and readin.
@detail Makes my life easier.
@date 20.5.11
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

extern void cleanup(void);
extern char* prog;

/**
@brief Eases the handling of errors throughout the programm.
@param s The String that shall be printed to stderr.
*/
void merror(char* prog,char *s){
	(void)fprintf(stderr,"%s: A fatal error has occured: %s.\nCan not continue...\nExiting...\n",prog,s);
	(void)cleanup();
	exit(EXIT_FAILURE);
}

/**
@brief Initializes a new share struct.
@return Share struct all values NULL.
*/
static share* new_share(void){
	share* new = (share*) malloc(sizeof(share));
	
	new->semidr = 0;
	new->semidc = 0;
	new->shmid = 0;
	new->shm = NULL;
	
	return new;
}


/**
@brief Trys to attatch to a shared memory and semaphore.
@detail This function impl. assumes that if a shm could be attached thet has to be a sem and throws an error otherwise.
@return Returns NULL if no share was found share* otherwise.
*/
static share* attach(void){
	share* ret = new_share();

	if((ret->shmid = shmget(SHM_KEY,sizeof(shm),0666)) <0 ){
		(void)free(ret);
		return (share*)NULL;
	}

	if((ret->shm = (shm*) shmat(ret->shmid,NULL,0) ) == (struct shm*)-1){
		(void)free(ret);
		(void)merror(prog,"Could not attach share");
	}
	else{
		if((ret->semidr = semgrab(SEM_KEY_R))<0){
			(void)free(ret);
			(void)merror(prog,"Got SHM but failed to aquire SEM_R");
		}
		if((ret->semidc = semgrab(SEM_KEY_C))<0){
			(void)free(ret);
			(void)merror(prog,"Got SHM but failed to aquire SEM_C");
		}
	}
	return ret;

}

/**
@brief Creates a shared memory segment and a semaphore.
@detail Creates semapores first then shm see attach.
@return A share containing 2 semaphores and one shared memory segment, or terminates.
*/
static share* create(void){
	share* ret = new_share();

	if((ret->semidc = seminit(SEM_KEY_C,666,0)) < 0){
		(void)free(ret);
		(void)merror(prog,"Failed to initialize SEM_C");
	}
	if((ret->semidr = seminit(SEM_KEY_R,666,1)) < 0){
		(void)free_share(ret);
		(void)merror(prog,"Failes to initalize SEM_R");
	}
	if((ret->shmid = shmget(SHM_KEY,sizeof(shm),(IPC_CREAT|0400|0200))) < 0){
		(void)free_share(ret);
		(void)merror(prog,"Failed to initialize SHM");
	}
	if((ret->shm = (shm*) shmat(ret->shmid,NULL,0)) == (shm*) -1){
		(void)free_share(ret);
		(void)merror(prog,"Failed to attach SHM");
	}
	/*indicates working state*/
	ret->shm->state = 0;		
 
	return ret;
}

/**
@brief Frees a share.
@detail Detaches shm than deletes shm than deletes sem, checks if they exist first. Handles NULL input.
@return 0 in case of success,-1 on failing to free a sem, -2 on failing to detach shm, -3 on failing to delete shm.
*/
int free_share(share* del){

	if(del != (share*) NULL){
		if(del->shm != NULL){
			if(shmdt((void*)del->shm) < 0){
				return -2;
			}
		}
		if(del->shmid != 0){
			if(shmctl(del->shmid,IPC_RMID,NULL) < 0){
				return -3;
			}
		}
		if(del->semidc != 0){
			if(semrm(del->semidc) < 0){
				return -1;	
			}
		}
		if(del->semidr != 0){
			if(semrm(del->semidr) < 0){
				return -1;	
			}
		}		
		(void)free(del);
	}
	return 0;
}

/**
@brief Checks if share has been created before if so connects to resources else creates resources.
@return Returns a newly created share if no instance of share has been started connects to existing and returns it otherwise.
*/
share* check_first(void){
	share* ret = (share*) NULL;

	if((ret = attach()) != (share*) NULL){
		return ret;
	}
	else{
		ret = create();
		return ret;
	}
}


