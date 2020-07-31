all:webserver_fork
	
webserver_fork:webserver_fork.o func.o
	gcc webserver_fork.o func.o -o webserver_fork
webserver_fork.o:webserver_fork.c webserver.h
	gcc -c webserver_fork.c 
func.o:func.c
	gcc -c func.c
clean:
	rm -f webserver_fork *.o 

test:all
	echo "kasdnaksncknjc" > test_report.txt