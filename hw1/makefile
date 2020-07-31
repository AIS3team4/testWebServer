all:webserver_fork webserver_select
	
webserver_fork:webserver_fork.o
	gcc webserver_fork.o -o webserver_fork
webserver_fork.o:webserver_fork.c
	gcc -c webserver_fork.c
webserver_select:webserver_select.o
	gcc webserver_select.o -o webserver_select
webserver_select.o:webserver_select.c
	gcc -c webserver_select.c
clean:
	rm -f webserver_fork webserver_select *.o *.out
