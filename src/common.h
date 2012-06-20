#ifndef COMMON_H
#define COMMON_H
/**
@file common.h
@author Eduard Thamm
@brief Contains all routine declarations common to both caesar and readin.
@details Also contains defines and typedefs.
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
#include <sem182.h>


#define STR_MAX 1024 /**< Defines the maximum string length. E.g. The max. length per line read.*/
#define SHM_KEY 8795 /**< Defines the key for shared memory.*/
#define SEM_KEY_R 4598 /**< Defines the key for readin semaphore.*/
#define SEM_KEY_C 8954 /**< Defines the key for caesar semaphore.*/

/**
@brief The struct for shared memory.
*/
typedef struct shm{
	int state;/**< Indicates the phase of work curently going on. 0 if still working, 1 if finished.*/
	char data[STR_MAX];/**< Contains the data to be transfered between the two parties.*/
} shm;

/**
@brief The pointer to a shared memory struct.
*/
typedef shm *shmp;

/**
@brief This struct contains all that is needed for collaboration.
@details Id's for 2 semaphores and 1 shared memory and a pointer to shared memory are contained here in.
*/
typedef struct share{
	int shmid; /**< The ID of the shared memory.*/
	int semidr;/**< The ID of the semaphore used by readin.*/
	int semidc;/**< The ID of the semaphore used by caesar.*/
	shm* shm;  /**< The pointer to shared memory*/
} share;

/**
@brief The pointer to a share struct.
*/
typedef share *sharep;

/**
@brief Eases the handling of errors throughout the programm.
@param s The String that shall be printed to stderr.
@param prog The name of the programm causing the error.
*/
void merror(char* prog,char *s);

/**
@brief Checks if share has been created before if so connects to resources else creates resources.
@return Returns a newly created share if no instance of share has been started connects to existing and returns it otherwise.
*/
share* check_first(void);

/**
@brief Frees a share.
@details Detaches shm than deletes shm than deletes sem, checks if they exist first. Handles NULL input.
@return 0 in case of success,-1 on failing to free a sem, -2 on failing to detach shm, -3 on failing to delete shm.
*/
int free_share(share* del);
#endif /*COMMON_H*/
