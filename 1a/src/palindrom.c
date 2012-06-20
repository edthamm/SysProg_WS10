/**
@file palindrom.c
@author Eduard Thamm 0525087
@brief Will check if given string is a palindrom.
@detail On call -i will specify to ignore cases while -s will ignore spaces during the check. Max. length of string to be checked is 40 char.
@date 07-08-2011
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char str[42];

static void compare(void){
	int i = 0;
	for(i = strlen(str); i>1; i--){
		if(str[strlen(str) - i] != str[i-2]){
			printf("%sis not a palindrome!\n",str);
			exit(0);
		}
	}
	printf("%sis a palindrome!\n",str);
	exit(0);
}

static void read(void){
	(void)printf("Enter text now:\n");
	fgets(str, 42, stdin);
	if(strlen(str) == 41){
		fprintf(stderr, "String is to long!");
		exit(1);
	}
}


static void ignoreall(void){
	int i = 0, j = 0;
	char str2[strlen(str)];
	(void)read();
	for ( i = 0; i<strlen(str); i++){
		str[i]=toupper(str[i]);
	}

	for ( i = 0; i<strlen(str); i++){
		if(str[i]!=' '){
			str2[j] = str[i];
			j++;
		}
	}
	(void)strcpy(str,str2);
	(void)compare();
	
}

static void ignorecase(void){
	int i = 0;

	(void)read();
	for ( i = 0; i<strlen(str); i++){
		str[i]=toupper(str[i]);
	}
	(void)compare();
}

static void noopt(){
	(void)read();
	(void)compare();
}

static void ignorespace(void){
	int i = 0, j = 0;	
	char str2[strlen(str)];
	read();
	for ( i = 0; i<strlen(str); i++){
		if(str[i]!=' '){
			str2[j] = str[i];
			j++;
		}
	}
	(void)strcpy(str,str2);
	(void)compare();
}




int main(int argc, char *argv[]){
	int iflag = 0, sflag = 0,opt;
	

	if(argc > 3){/*do some argument handling*/
			(void)fprintf(stderr, "Usage: %s [-s] [-i] \n", argv[0]);
			exit(EXIT_FAILURE);
	}
	while((opt=getopt(argc,argv,"si "))!=-1){
		switch(opt){
			case 'i':
				iflag = 1;
			break;
			case 's':
				sflag = 1;
			break;
			default:
				(void)fprintf(stderr, "Error in argument handling! Can not continue...\n");
			exit(EXIT_FAILURE);				
		}
	}

	if(iflag == 1 && sflag == 1){
		(void)ignoreall();
	}
	if(iflag == 1 && sflag == 0){
		(void)ignorecase();
	}
	if(iflag == 0 && sflag == 1){
		(void)ignorespace();
	}
	else{
		(void)noopt();
	}

	(void)fprintf(stderr, "An internal flag Error has occured...\n");
	exit(EXIT_FAILURE);
}
