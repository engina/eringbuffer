all: test

test_commands.txt:
	php test_cmd_gen.php 128 100000

e_ringbuffer.o: e_ringbuffer.c e_ringbuffer.h
	$(CC) -W -Wall -ansi -pedantic -c $(CFLAGS) -o e_ringbuffer.o e_ringbuffer.c

test_rbuf.exe: e_ringbuffer.o
	$(CC) -W -Wall -std=c99 -pedantic $(CFLAGS) -o test_rbuf.exe test_rbuf.c e_ringbuffer.o

test: test_rbuf.exe test_commands.txt
	php test_rbuf.php
	./test_rbuf.exe
	diff c_output.txt php_output.txt && echo TEST PASSED

clean:
	rm -vf *~ *.o *.exe c_output.txt php_output.txt test_commands.txt
