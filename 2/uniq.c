FILE *write = NULL;
		FILE *read = NULL;
		char buf[1024];
		
		if(close(fd2[1]) < 0){
			(void)merror("Failed to close writing pipe in uniq_parent");
		}

		fd2[1] = 0;

		if(close(fd3[0]) < 0){
			(void)merror("Failed to close reading pipe in uniq_parent");
		}

		fd3[0] = 0;


		if((read = fdopen(fd2[0], "r")) == NULL){
			(void)merror("Could not open reading pipe in uniq_parent");
		}
		if((write = fdopen(fd3[1], "w")) == NULL){
			(void)merror("Could not open writing pipe in uniq_parent");
		}

		if((fin = malloc(sizeof(char))) == NULL){
			(void)merror("Allocating final buffer failed");
		}

		finnow = fin;

		/*writing to child goes here*/

		while(fgets(buf,1023,read) != NULL){
			fin = (char *) realloc(res,strlen(buf)*sizeof(char));

			if(fin == NULL){
				(void)merror("Reallocating final buffer failed");
			}

			(void)strncpy(finnow, buf, strlen(buf));
			/*copying w/o terminating \0, then moving position pointer*/
			finnow += strlen(buf);		
		}

		
		(void)wait(NULL);
		return;
	}
	else{/*this is the child*/
		
		if(close(fd2[0]) < 0){
			(void)merror("Failed to close reading pipe in uniq_child");
		}

		fd2[0] = 0;

		if(close(fd3[1]) < 0){
			(void)merror("Failed to close writing pipe in uniq_child");
		}

		fd3[1] = 0;



		if(dup2(fd2[1], fileno(stdout)) < 0){
			(void)merror("Could not redirect pipe to stdout in uniq_child");
		}

		if(dup2(fd3[0], fileno(stdin)) < 0){
			(void)merror("Could not redirect pipe to stdin in uniq_child");
		}

		/*here ill need to play around with set buff; plan is to first complete uniq -d and then write the output as a whole*/
		if(execl("/bin/sh","uniq",(char *)0) < 0){
			(void)merror("execl in child went wrong");
		}
