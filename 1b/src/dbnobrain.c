/*
Dicebot
@author Eduard Thamm 0525087
@brief A simple Bot playing a game of dice.
@detail This bot connects to a hardcoded gameserver and plays a simple game of dice against 2 opponents remembering his own and their scores and reporting them on BYE. Terminates with 0 on success and 1 on failure. For detail see STDERR.
@date 

gameserver: sysprog-dicer.tilab.tuwien.ac.at:9001 (128.130.60.189)

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

int sid;

/*cleanup on signals will go here*/

static void cleanup(void){
	(void)close(sid);
	exit(EXIT_FAILURE);
}

static void auth(char *whom){
	char buffer[128];	
	(void)read(sid,buffer,127);
	if(strncmp(buffer,"HELO",4)==0){
		(void) snprintf(buffer, 127,"AUTH %s\n",whom);
		(void) write(sid,buffer,strlen(buffer));
		(void) printf("Authenticated\n");
	}
	else{
		(void)fprintf(stderr, "ERROR while authenticating with server. Recieved:%s\n",buffer);
		(void)cleanup();
	}
}
/*Need to try clearing the buffer after every rcv funny things happen*/
static void play(void){
	long int p0=0,p1=0,p2=0,i;
	long int l,k;
	long int p0tmp=0,p1tmp=0,p2tmp=0;
	char d1[2],d2[2],id[2];
	char buffer[128];

	while(1==1){
		(void)read(sid,buffer,127);
		if(strncmp(buffer,"TURN",4) == 0){
			p0 += p0tmp; p0tmp = 0;
			p1 += p1tmp; p1tmp = 0;
			p2 += p2tmp; p2tmp = 0;
			i = 11;/*one roll outside the loop, than 5 rolls but 6 answering thrw with no "snake eyes"*/
			(void)write(sid,"ROLL\n",5);
			(void)printf("Your turn!\n");
			
			/*gamelogic goes here and is now roll 6 times then save; might add save if over certain value(maybe 36)[will add an if similar to 				the i==0 if recv a turn send a save, break]*/
			/*counting for myself will be done here*/
			/*while(i>0){
				(void)read(sid,buffer,127);
				if(strncmp(buffer,"TURN",4) == 0){
					(void)write(sid,"ROLL\n",5);
					--i;
				}
				if(strncmp(buffer,"THRW",4) == 0){
					strncpy(id,&buffer[5],1);; id[2]='\0';		
					strncpy(d1,&buffer[7],1); d1[2]='\0';
					strncpy(d2,&buffer[9],1);; d2[2]='\0';

					l = strtol(d1,NULL,10);
					k = strtol(d2,NULL,10);
					--i;
					(void)printf("Player %s threw %ld %ld.\n",id,l,k);
					if(k != l){
						p2tmp += (k+l);
					}
				
					if(k == l){
						p2tmp = 0;
						break;
					}
				}
				if(strncmp(buffer,"ERR",3) == 0){
				(void)fprintf(stderr,"Recived ERROR from Server:\n%s\nQuiting...\n",buffer);
				(void)cleanup();
				}
			}
			if(i == 0){
				(void)read(sid,buffer,127);
				if(strncmp(buffer,"TURN",4) == 0){
					(void)write(sid,"SAVE\n",5);
					(void)printf("Saved points.\n");
				}
			}*/
			
		}
		
		if(strncmp(buffer,"THRW",4) == 0){
			strncpy(id,&buffer[5],1);; id[2]='\0';/*\0 termination needed for all strings or strange things will happen*/
			strncpy(d1,&buffer[7],1);; d1[2]='\0';
			strncpy(d2,&buffer[9],1);; d2[2]='\0';
			l = strtol(d1,NULL,10);
			k = strtol(d2,NULL,10);
			(void)printf("Player %s threw %ld %ld.\n",id,l,k);
			if(strncmp(id,"0",1) == 0 && k != l){
				p0tmp += (k+l);
			} 
			if(strncmp(id,"0",1) == 0 && k == l){
				p0tmp = 0;
			}
			if(strncmp(id,"1",1) == 0 && k != l){
				p1tmp += (k+l);
			} 
			if(strncmp(id,"1",1) == 0 && k == l){
				p1tmp = 0;
			}
/*something is wrong with the count could cause the timeouts*/
		}
		
		if(strncmp(buffer,"ERR",3) == 0){
			(void)fprintf(stderr,"Recived ERROR from Server:\n%s\nQuiting...\n",buffer);
			(void)cleanup();
		}
	
		if(strncmp(buffer,"DEF",3) == 0 || strncmp(buffer,"WIN",3) == 0){
			p0 += p0tmp;
			p1 += p1tmp;
			p2 += p2tmp;
			/*reset temps after add not necessary since game is over and no futher adds will be made*/
			(void)snprintf(buffer,127,"BYE %ld %ld %ld\n",p2,p0,p1);
			(void)write(sid,buffer,strlen(buffer));
			(void)printf("Game ended! Your score: %ld, Opponent 0: %ld, Opponent 1: %ld\n",p2,p0,p1);
			return;
		}

	}				
}

int main(int argc, char *argv[]){
	int n,opt;
	char *port;
	char *botname;
	char *srvname;
	struct addrinfo *server;
	struct addrinfo hints;
	port = "9001";
	


	if(argc < 4 || argc > 6){/*do some argument handling*/
		(void)fprintf(stderr, "Usage: %s -n <botname> [-p <portnummer>] <servername>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	while((opt=getopt(argc,argv," n:p:"))!=-1){
		switch(opt){
			case 'n':
				botname = optarg;
			break;
			case 'p':
				port = optarg;
			break;/*need to find a way to verify that the passed arguments are good*/
			default:
				(void)fprintf(stderr, "switch went wrong");
			exit(EXIT_FAILURE);				
		}
	}
	if(argc == 4){
		srvname = argv[3];
	}
	else{
		srvname = argv[5];/*tested this works*/
	}
	(void)printf("Trying to connect to: %s:%s\n",srvname,port);
	
	memset(&hints, 0, sizeof(struct addrinfo));/*set hints*/
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_flags = 0;
	hints.ai_protocol = 0;          /* Any protocol */

	
	n = getaddrinfo(srvname,port,&hints,&server); /*get server ip fit args*/
		if (n != 0) {
			
			(void)fprintf(stderr, "ERROR retrieving server ip\n");
			exit(EXIT_FAILURE);
           	}



	sid = socket(server->ai_family, server->ai_socktype,server->ai_protocol);
    		if (sid < 0) {
        		(void)fprintf(stderr,"ERROR opening socket\n");
			exit(EXIT_FAILURE);
		}

	/* tested til here need to figure out why connect does not work - works if ip is entered
	*/
		
	if (connect(sid,server->ai_addr, server->ai_addrlen) == -1){/* connect to server*/
		 		
		(void)fprintf(stderr,"ERROR connecting to server\n");
		(void)cleanup();
	}
	freeaddrinfo(server);
	(void)printf("Connected\n");
	/*do the gaming part*/
	(void)auth(botname);
	(void)play();
	(void)close(sid);
		
return 0;
}
