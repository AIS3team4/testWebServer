#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<netdb.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#define SA struct sockaddr
//設定網頁
char webpage[]=                                            
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title>Chen Che website</title>\r\n"
"<style>body (background-color:#FFFF00)</style></head>\r\n"
"<body><center><h1>作業一 陳徹 405410121</h1><br>\r\n"
"<img src=\"favicon.jpg\"></center></body></html>\r\n";


void sigChid(int sig){  //signal function,clean zombie
	pid_t pid;
 	int stat;
 	while((pid=waitpid(-1,&stat,WNOHANG))>0){
//	wait(&stat);	
		printf("kill zombie\n");
	}
}

unsigned long fsize(char * file){  //看檔案的大小
 	FILE *f=fopen(file,"r");
 	fseek(f,0,SEEK_END);
 	unsigned long len=(unsigned long)ftell(f);
 	fclose(f);
 	return len;
}
int main(int argc,char **argv){
	struct sockaddr_in serv_addr,cli_addr;
	int sockfd,connectfd,pid;
	int fdimg;
	socklen_t cli_len;
	char buffer[2048];

	bzero(&serv_addr,sizeof(serv_addr));   //initialized

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
		printf("socket error!\n");

	serv_addr.sin_family=AF_INET;  //set family
	serv_addr.sin_port=htons(80);  //set port
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
				close(sockfd);                              //child close sockfd
				memset(buffer,0,2048);                      //initialized
				read(connectfd,buffer,2047);                //read request
				if(!strncmp(buffer,"GET /favicon.jpg",16)){ //GET request
					fdimg=open("favicon.jpg",O_RDONLY);
        				sendfile(connectfd,fdimg,NULL,fsize("favicon.jpg"));
        				close(fdimg);
       				}
       				else{
        				write(connectfd,webpage,sizeof(webpage)-1); //write website
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
  
 




