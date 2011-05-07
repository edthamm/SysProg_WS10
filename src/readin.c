/**
@file readin.c
@author Eduard Thamm 0525087
@brief Reads in Ascii-files handed as parameter
@details Reads in the file handed as parameter. File may contain only ASCII characters and lines must be shorter than 1023 symbols.
[A-Z] will be unchanged [a-z] will be converter tu upper case. All other symbols will be ignored. Content of file will be writen to a shared memory for processing by caesar.
@date 19.05.11
*/

#define STR_MAX 1024

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

int main(int argc,char* argv[]){

}
