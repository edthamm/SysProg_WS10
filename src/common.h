#ifndef COMMON_H
#define COMMON_H
/**
@file common.h
@author Eduard Thamm
@brief Contains all routine declarations common to both caesar and readin.
@detail Also contains defines and typedefs.
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


#define STR_MAX 1024
#define SHM_KEY 8795
#define SEM_KEY_R 4598
#define SEM_KEY_C 8954


typedef struct shm{
	int state;/*0 if there is still work to be done 1 othewise; is there a nice way to document this in doxy?*/
	char data[STR_MAX];
} shm;

typedef shm *shmp;

typedef struct share{
	int shmid;
	int semidr;
	int semidc;
	shm* shm;
} share;

typedef share *sharep;

void merror(char* prog,char *s);
share* check_first(void);
int free_share(share* del);
#endif /*COMMON_H*/
