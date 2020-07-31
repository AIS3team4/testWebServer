all:webserver_fork unit_test
	
webserver_fork:webserver_fork.o func.o
	gcc webserver_fork.o func.o -o webserver_fork
webserver_fork.o:webserver_fork.c webserver.h
	gcc -c webserver_fork.c 
func.o:func.c webserver.h
	gcc -c func.c
unit_test:unit_test.o func.o
	gcc unit_test.o func.o -o unit_test
unit_test.o:unit_test.c webserver.h
	gcc -c unit_test.c
clean:
	rm -f webserver_fork unit_test *.o
test:
	./unit_test < spec_input
