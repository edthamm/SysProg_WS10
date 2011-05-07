/**
@file caesar.c
@author Eduard Thamm 0525087
@brief Encrypts/Decrypts Caesar Ciphers. Valid input value 1-25.
@details This programm takes the output of readin from shared memory and right-shifts every symbol by the distance given in the argument. Overflow will be handled modulo wise. Output will be generated to stdout.
@date 19.5.11
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
