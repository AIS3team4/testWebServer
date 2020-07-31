#include "webserver.h"


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
char* proccess(struct http_action* act,int *s){
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

				while((n=read(fd,f_buf,f_size))>0){
		
					strcat(output,f_buf);
					f_size-=n;
				}

				*s=total_size;
				output[total_size]='\0';	
				printf("response : %s\n",output);
                        	close(fd);
			}
			else if(strstr(act->file_name,".jpg")!=NULL){
				int fd=open(act->file_name,O_RDONLY),n;
                        	if(fd<=0){
                                	printf("no file exist!\n");
                                	return NULL; //maybe can change to some error page
                        	}
                        	int f_size=fsize(act->file_name);
				*s=f_size;
				output=(char*)malloc((f_size)*sizeof(char));
				output[0]='\0';

				read(fd,output,f_size);
				close(fd);
			}
			return output;
		}
	}
	else{
		printf("error!\n");
		//TODO error handle
	}
}
char* p(int connectfd,int *s){
	char buffer[2048];
	struct http_format http;
        struct http_action act;
        char* send_to_client;
        int size;

	memset(buffer,0,2048);                      //initialized
        read(connectfd,buffer,2048);                //read request

       	if(parse(buffer,&http) && determine_action(&http,&act)){

      		send_to_client=proccess(&act,&size);
		*s=size;
      	}
       	else{
		printf("error!\n");
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
	return send_to_client;
}
unsigned long fsize(char * file){  //看檔案的大小
 	FILE *f=fopen(file,"r");
 	fseek(f,0,SEEK_END);
 	unsigned long len=(unsigned long)ftell(f);
 	fclose(f);
 	return len;
}
