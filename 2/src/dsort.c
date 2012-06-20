/**
@file dsort.c
@author Eduard Thamm 0525087
@brief Returns the duplicate output of the passed shell commands.
@details Stores the output of the two commands handed down by the user in to an array, sorts them via sort and runs them through "uniq -d" generating an output matching that of ($1;$2)|sort|uniq -d if run in a shell.
@date 04/05/2011
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <limits.h>

#define STR_MAX 1024

int fd[2],fd2[2];
char *res = NULL;
size_t res_size;
/**
@brief This function cleans up the mess on errors.
@details Is called by merror() and cleanupsig() to free all recources uses the globals fd, *res.
*/
static void cleanup(void){
	/*all that needs cleaning goes here*/
	if(fd[0] != 0){
		(void)close(fd[0]);
	}
	if(fd[1] != 0){
		(void)close(fd[1]);
	}
	if(fd2[0] != 0){
		(void)close(fd2[0]);
	}
	if(fd2[1] != 0){
		(void)close(fd2[1]);
	}
	if(res != NULL){
		(void)free(res);
	}

}

/**
@brief Eases the handling of errors throughout the programm.
@param s The String that shall be printed to stderr.
*/
static void merror(char *s){
	(void)fprintf(stderr,"A fatal error has occured: %s.\nCan not continue...\nExiting...\n",s);
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
@brief This function is called upon by parent to spawn a shell enviroment of uniq -d
@details Uses fd2.
*/
static void make_uniq(void){
	pid_t pid;
	int i;	
	

	if(pipe(fd2) < 0){
		(void)merror("Pipe in make_uniq went wrong");
	}
	
	if((pid=fork()) < 0){
		(void)merror("Fork in make_uniq went wrong");
	}
	else if (pid > 0){ /*this is the parent*/
		FILE *write;


		if(close(fd2[0]) < 0){
			(void)merror("Failed to close reading pipe in uniq parent");
		}
		fd2[0] = 0;

		if((write = fdopen(fd2[1], "w")) == NULL){
			(void)merror("Could not open writing pipe in uniq parent");
		}
		
		for(i = 0; i < res_size; i++){
			fputs((res+i*STR_MAX),write);
			(void)fflush(write);
		}
		fclose(write);/*same here as in parent*/
		close(fd2[1]);
		fd2[1]=0;

		(void)wait(NULL);
	}
	else{ /*this is the child*/

		if(close(fd2[1]) < 0){
			(void)merror("Failed to close writing pipe in uniq child");
		}
		fd2[1] = 0;
		
		if((dup2(fd2[0],fileno(stdin))) < 0){
			(void)merror("Failed to redirect to stdin in uniq child");
		}

		if(execl("/usr/bin/uniq","uniq","-d",(char *)0) < 0){
			(void)merror("execl in uniq child went wrong");
		}
		(void)fflush(stdout);
		
	}
return;
}

/**
@brief The compare function used by qsort.
@param p1 The first value to be compared.
@param p2 The second value to be compared.
@return An integer less than, equal to, or greater than zero if  p1  is  found,
       respectively, to be less than, to match, or be greater than p2.
@details Underlaying comparator is strcmp().
*/
static int cmpstringp(const void *p1, const void *p2)
       {
           /*The actual arguments to this function are "pointers to
              pointers to char", but strcmp() arguments are "pointers
              to char", hence the following cast plus dereference */
	const char * s1 = (const char*) p1;
	const char * s2 = (const char *) p2;

           return strcmp( s1, s2);
       }


/**
@brief The routines of the parent process.
@param flag An integer used for flow-control. It shows where in argv[] we are.
@details Use *res to allocate the solution string under, and fd.
*/
static void parent(int flag){
	FILE *read = NULL;
	char buf[STR_MAX];

	if(close(fd[1]) < 0){
		(void)merror("Failed to close writing pipe in parent");
	}
	fd[1] = 0;

	if((read = fdopen(fd[0], "r")) == NULL){
		(void)merror("Could not open reading pipe in parent");
	}
	if(flag == 1){
		if((res =(char *) malloc((STR_MAX*sizeof(char)))) == NULL){
			(void)merror("Allocating result buffer failed");
		}
		res_size = 0;
	}	
	
	while(fgets(buf,STR_MAX -1,read) != NULL){
		res = (char *) realloc(res,(((res_size+2)*STR_MAX*sizeof(char))));/*STR_MAX siehe angabe hinweise*/
		if(res == NULL){
			(void)merror("Reallocating result buffer failed");
		}
		buf[strlen(buf)+1]= '\0';
		(void)strncpy((res+res_size*STR_MAX), buf, (strlen(buf)+1));
		res_size++;		
	}
	fclose(read);/*which one of these???*/
	/*if(close(fd[0]) < 0){
		(void)merror("Failed to close reading pipe in parent");will this close read as well? it will says tut
	}*/

	if(flag == 2){

		(void)qsort(res,res_size,(STR_MAX),cmpstringp);

		(void)make_uniq();
	}
 
	(void)wait(NULL);
	return;
}

/**
@brief The routines of the child process.
@param s The current command.
@details Uses fd.
*/
static void child(char *s){ 
	char *cmd;
	
	if(close(fd[0]) < 0){
		(void)merror("Could not close reading pipe in child");	
	}
	fd[0] = 0;

	if(dup2(fd[1], fileno(stdout)) < 0){
		(void)merror("Could not redirect pipe to stdout");
	}
	(void)close(fd[1]);/*no checking here because it will not effect flow maybe check...*/
	
	if((cmd = malloc(sizeof(char)*(strlen(s)+1))) == NULL){
		(void)merror("Could not allocate bufferspace for command in child");
	}
	
	(void)snprintf(cmd,(strlen(s)+1),"%s",s);


	if(execl("/bin/sh","sh","-c", cmd,(char *)0) < 0){
		(void)merror("execl in child went wrong");
	}
	(void)fflush(stdout);
	return;
}

/**
@brief Sorting through the chaos. Doing all the administrative work.
@param argc Number of arguments passed
@param argv[] Argument String
@details Uses fd directly.
*/
int main(int argc, char *argv[]){

	int flag = 0;
	pid_t pid;


 
	(void)signal(SIGINT,cleanupsig);
	(void)signal(SIGQUIT,cleanupsig);
	(void)signal(SIGTERM,cleanupsig);
	(void)signal(SIGABRT,cleanupsig);

	if(argc != 3){
		(void)fprintf(stderr,"Usage: %s \"command1\" \"command2\"\n",argv[0]);
		exit(EXIT_FAILURE); /*no cleanup nothing done up until now*/
	}
	
	while (flag < 2){
		flag++;
		
		if(pipe(fd) < 0){
			(void)merror("Pipe went wrong");
		} 
		
		if((pid=fork()) < 0){
			(void)merror("Fork went wrong");
		}
		else if (pid > 0){ /*this is the parent*/
			(void)parent(flag);
		}
		else{/*this is the child*/
			(void)child(argv[flag]); break; /*w/o break child would fork*/
		}
	}
				
	cleanup();
	exit(EXIT_SUCCESS);
}
