#include "webserver.h"

int main(int argc,char **argv){
	struct sockaddr_in serv_addr,cli_addr;
	int sockfd,connectfd,pid;
	int fdimg;
	socklen_t cli_len;
	char buffer[2048];
	printf("%s\n",resp_header);
	bzero(&serv_addr,sizeof(serv_addr));   //initialized

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
		printf("socket error!\n");
	printf("port : %d\n",atoi(argv[1]));
	serv_addr.sin_family=AF_INET;  //set family
	serv_addr.sin_port=htons(atoi(argv[1]));  //set port
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);  //set 可接收的addr

	bind(sockfd,(SA*)&serv_addr,sizeof(serv_addr));

	if((listen(sockfd,0))<0)
		printf("listen erre!\n");

	cli_len=sizeof(cli_addr);
	signal(SIGCHLD,sigChid);//收回zombie signal
	for( ; ; ){
		if((connectfd=accept(sockfd,(SA*)&cli_addr,&cli_len))<0)
			printf("connect error!\n");
		else{
			if((pid=fork())<0)   {        //new process
				printf("fork error!\n");
				exit(1);
			}

			if(pid==0){
				//make this context become function need para connectfd buffer
				char* out;
				int size;
				close(sockfd);                              //child close sockfd
				
				out=p(connectfd,&size);
				write(connectfd,out,size);
				if(out!=NULL){		
					free(out);
					out=NULL;
				}	
       				close(connectfd); 
       				exit(0);      //exit child process
    			}
   			else{
    				close(connectfd); //parent close connectfd
   			}
		}
 	}
return 0;
} 
  
 




