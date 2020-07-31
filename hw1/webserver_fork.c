#include<netinet/in.h>
#include<stdbool.h>
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

enum{
	GET,POST
};
enum{
	HTTP1_0,HTTP1_1,HTTP2_0
};
struct http_format{
	char* first_line,*header,*body;
};
struct http_action{
	int action;
	int http_version;
	char* file_name;
	char* para;
};
struct http_response{
	char* header;
	char* data;
};

char resp_header[]={"HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"};

unsigned long fsize(char * file);
bool parse(char* buffer,struct http_format* http_fmt){
	int i=0,lens=strlen(buffer),l=0;
	int flag=0,pre=0;
	char * temp1=NULL,*temp2=NULL,*temp3=NULL;
	http_fmt->first_line=http_fmt->header=http_fmt->body=NULL;

	for(i=0;i<lens;i++){
		if(buffer[i]=='\r' && i+1<lens && buffer[i+1]=='\n'){ //do process just for first time
			if(!flag){
				l=i;
				temp1=(char*)malloc((i+1)*sizeof(char));
				flag=1;
				strncpy(temp1,buffer,i);
				temp1[i]='\0';
				http_fmt->first_line=temp1;
				pre=i+2;
			}
			else{
		//		printf("i: %d lens: %d\n",i,lens);
				if(i+3<lens && buffer[i+2]=='\r' && buffer[i+3]=='\n'){
					printf("have header\n");
					int k,j=0;
					l=i-pre;
					temp2=(char*)malloc((l+1)*sizeof(char));
					for(k=pre ,j=0;k<i;k++,j++){
						temp2[j]=buffer[k];
						
					}
					temp2[j]='\0';
					http_fmt->header=temp2;
					//printf("header %s\n",http_fmt->header);
					pre=i+4;
					break;
				}
			}
		}
	}
	//TODO maybe need to handle the data that is not satisfied htpp protocl
	if(pre<lens){
		int k,j=0;
		l=lens-pre+1;
		temp3=(char*)malloc((l+1)*sizeof(char));
		for(k=pre,j=0;k<lens;k++,j++){
			temp3[j]=buffer[k];
		}
		temp3[j]='\0';
		http_fmt->body=temp3;
	}
	if(http_fmt->first_line)	
		printf("fisrt line %s\n",http_fmt->first_line);
	if(http_fmt->header)
		printf("header : %s :header\n",http_fmt->header);
	if(http_fmt->body)
		printf("body %s\n",http_fmt->body);

	if(http_fmt->first_line==NULL || http_fmt->header==NULL)
		return false;
	else 
		return true;

}
bool determine_action(struct http_format* fmt,struct http_action *act){
	char *first=fmt->first_line;
	char* action=NULL,*path=NULL,*http_version=NULL,*temp=NULL;
	
	printf("start deter %s\n",first);
	int i=0;
	temp=strtok(first," ");

	while(temp!=NULL){
		if(i==0)
			action=temp;
		else if(i==1)
			path=temp;
		else 
			http_version=temp;
		temp=strtok(NULL," ");
		printf("strtok data : %s\n",temp);
		i++;
	}
	if(action==NULL || path==NULL || http_version==NULL) //http format error
		return false;

	if(action!=NULL && strcmp(action,"GET")==0){ //determine which http action to do 
		printf("action is GET\n");
		act->action=GET;
	}
	else{
		printf("action is POST\n");
		act->action=POST;
	}
				//determine which http version it use
	if(http_version!=NULL && strcmp(http_version,"HTTP/1.0")==0){
		act->http_version=HTTP1_0;
	}
	else if(http_version!=NULL && strcmp(http_version,"HTTP/1.1")==0){
		act->http_version=HTTP1_1;
	}
	else
		act->http_version=HTTP2_0;

	act->file_name=&path[1];

	act->para=fmt->body;

	return true;
}
void sigChid(int sig){  //signal function,clean zombie
	pid_t pid;
 	int stat;
 	while((pid=waitpid(-1,&stat,WNOHANG))>0){
//	wait(&stat);	
		printf("kill zombie\n");
	}
}
char* proccess(struct http_action* act,int *s,int connectfd){
	char *output=NULL;
	if(act->action==GET){
		if(act->file_name!=NULL){
			if(strstr(act->file_name,".html")!=NULL){
			int fd=open(act->file_name,O_RDONLY),total_size=0;
			char f_buf[2048];

			if(fd<=0){
				printf("no file exist!\n");
				return NULL; //maybe can change to some error page
			}
			int f_size=fsize(act->file_name),n;

			total_size=strlen(resp_header);
			total_size+=f_size;
			output=(char*)malloc((total_size+1)*sizeof(char));
			strcpy(output,resp_header);

			while((n=read(fd,f_buf,f_size))!=0){
			//	printf(" buff %s\n",f_buf);
				strcat(output,f_buf);
				f_size-=n;
			}

			*s=total_size;
			write(connectfd,output,total_size);
			printf("response : %s\n",output);
                        close(fd);
			}
			else if(strstr(act->file_name,".jpg")!=NULL){
				int fd=open(act->file_name,O_RDONLY);
                        	if(fd<=0){
                                	printf("no file exist!\n");
                                	return NULL; //maybe can change to some error page
                        	}
                        	int f_size=fsize(act->file_name);
				sendfile(connectfd,fd,NULL,f_size);
				close(fd);
			}
			return output;
		}
	}
	else{
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
	printf("%s\n",resp_header);
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
				struct http_format http;
				struct http_action act;
				char* send_to_client;
				int size;
				bool parse_fine=false;
				close(sockfd);                              //child close sockfd
				memset(buffer,0,2048);                      //initialized
				read(connectfd,buffer,2048);                //read request
//				printf("%s\n",buffer);

				
				if(parse(buffer,&http) && determine_action(&http,&act)){
				
					send_to_client=proccess(&act,&size,connectfd);
					if(send_to_client!=NULL){		
						free(send_to_client);
						send_to_client=NULL;
					}
				}
				else{
					//TODO error handle
				}

				//destory 
				if(http.first_line){
					free(http.first_line);
					http.first_line=NULL;
				}
				if(http.header){
                                        free(http.header);
                                        http.header=NULL;
                                }
				if(http.body){
                                        free(http.body);
                                        http.body=NULL;
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
  
 




