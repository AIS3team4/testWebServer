#include "webserver.h"

bool compare(const char* answer,const char* output){
	int answer_size=strlen(output),output_size=strlen(output),i;
	if(answer_size!=output_size)
		return false;
	for(i=0;i<answer_size;i++){
	//	printf("%c",answer[i]);
		if(answer[i]!=output[i])
			return false;
		
	}
	return true;
}
int main(int argc,char** argv){
	int f_size,fd_answer,fd_spec_input,fd_spec_output,s,header_lens,w_fd;
	char* answer,*output,*input;
	int input_size,answer_size,count=1;
	char *h="HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
	char str[80];
	const char *test_str="123";
	FILE* fptr;
	int l,n;
	fd_spec_input=open("spec_input",O_RDONLY);
	input_size=fsize("spec_input");
	input=(char*)malloc((input_size+1)*sizeof(char));
	read(fd_spec_input,input,input_size);
	

	fd_answer=open("index.html",O_RDONLY);
	answer_size=fsize("index.html");
	header_lens=strlen(h);
        answer=(char*)malloc((answer_size+header_lens+1)*sizeof(char));
	answer[0]='\0';
	strcat(answer,h);
        read(fd_answer,answer+header_lens,answer_size);
	output=p(0,&s);
	fptr=fopen("unit_test_report","w+");
	
//	w_fd=open("unit_test_report.txt",O_CREAT,S_IWGRP|S_IWOTH|S_IROTH|S_IRUSR|S_IWUSR);
	//printf("priv : %u\n",S_IRUSR|S_IWUSR);
	//printf("file_des : %d\n",w_fd);
	/*l=strlen(test_str);
	while((n=(write(w_fd,test_str,l)))>=0){
		printf("write !!\n");
		l-=n;
		test_str+=n;
	}*/
	if(compare(answer,output)){
		printf("coorect!\n");
		sprintf(str,"Case %d: Pass\n",count);
	//	write(w_fd,str,(size_t)strlen(str));
		fwrite(str,strlen(str),1,fptr);
		return 0;
	}
	else {
		printf("error!\n");
		sprintf(str,"Case %d: Fail\n",count);
		write(w_fd,str,(size_t)strlen(str));
		fwrite(str,strlen(str),1,fptr);
		return 1;
	}
	return 0;
}
