#include<unp.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<fcntl.h>


//set webpage
char webpage[]=
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title>Chen Che website</title>\r\n"
"<style>body (background-color:#FFFF00)</style></head>\r\n"
"<body><center><h1>作業一 陳徹 405410121</h1><br>\r\n"
"<img src=\"favicon.jpg\"></center></body></html>\r\n";



unsigned long fsize(char * file){                  //count file size
	FILE *f=fopen(file,"r");
	fseek(f,0,SEEK_END);
	unsigned long len=(unsigned long)ftell(f);
	fclose(f);
	return len;
}

int main(int argc,char **argv)
{
	struct sockaddr_in serv_addr,cli_addr;
	int listenfd,sockfd,connectfd,i,j,maxi,maxfd;
 	int fdimg,client[FD_SETSIZE],nready;
 	ssize_t n;
 	fd_set allset,rset;
 	socklen_t cli_len;
 	char buffer[2048];

 	bzero(&serv_addr,sizeof(serv_addr));   //initialized

 	if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0)
   		printf("socket error!\n");

	serv_addr.sin_family=AF_INET; 			//set family
 	serv_addr.sin_port=htons(80);			//set port
 	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); 	//set 接受的addr

 	bind(listenfd,(SA*)&serv_addr,sizeof(serv_addr));

 	if((listen(listenfd,LISTENQ))<0)
   		printf("listen erre!\n");
 
	maxfd=listenfd;
 	maxi=-1;

	for(i=0;i<FD_SETSIZE;i++)	//set cilent to empty
		client[i]=-1;

 	FD_ZERO(&allset);	 	//initialized allset
 	FD_SET(listenfd,&allset);	//add listenfd to allset

	for( ; ; ){
   		rset=allset;
   		nready=select(maxfd+1,&rset,NULL,NULL,NULL);  //有多少個連線

   		if(FD_ISSET(listenfd,&rset)){
			cli_len=sizeof(cli_addr);
	
  			if((connectfd=accept(listenfd,(SA*)&cli_addr,&cli_len))<0)
    				printf("connect error!\n");

  			for(i=0;i<FD_SETSIZE;i++){
    				if(client[i]<0){
        				client[i]=connectfd;	//save 可用的連線
        				break;
     			}
  			}
  			if(i==FD_SETSIZE)
    				printf("too many clients");
  
   			FD_SET(connectfd,&allset); //add connectfd to allset

   			if(connectfd>maxfd)
				maxfd=connectfd;

   			if(i>maxi)
				maxi=i;

   			if(--nready<=0)          //檢查是否還有連線
				continue;
  		}
  		for(i=0;i<=maxi;i++){
			if((sockfd=client[i])<0)
				continue;

			if(FD_ISSET(sockfd,&rset)){
				memset(buffer,0,2048); 	//initialized
			if((n=read(sockfd,buffer,2047))==0){  //檢查client是否關閉
				close(sockfd);
				FD_CLR(sockfd,&rset);
				client[i]=-1;
			}
			else{
				if(!strncmp(buffer,"GET /favicon.jpg",16)){ 	//web request
        				fdimg=open("favicon.jpg",O_RDONLY);
        				sendfile(sockfd,fdimg,NULL,fsize("favicon.jpg"));
        				close(fdimg);
       				}
       				else{
					
       					write(sockfd,webpage,sizeof(webpage)-1); //write webpage
       			
				}
			}
			if(--nready<=0)	////檢查是否還有連線
				break;	
			}		
  		}
	}
return 0;
} 
  
 




