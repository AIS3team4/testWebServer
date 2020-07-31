#ifndef WEBSERVER_H

#define WEBSERVER_H

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

static char resp_header[]={"HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"};

unsigned long fsize(char * file);
bool parse(char*,struct http_format*);
bool determine_action(struct http_format* ,struct http_action *);
void sigChid(int );
char* proccess(struct http_action* ,int *);
char* p(int ,int *);
unsigned long fsize(char * );
#endif
