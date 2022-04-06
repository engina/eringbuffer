all: test

e_ringbuffer.o: e_ringbuffer.c e_ringbuffer.h
	$(CC) -W -Wall -std=c89 -pedantic -c $(CFLAGS) -o e_ringbuffer.o e_ringbuffer.c

e_ringbuffer.test.exe: e_ringbuffer.test.c e_ringbuffer.o
	$(CC) -W -Wall -std=c99 -pedantic $(CFLAGS) -o e_ringbuffer.test.exe e_ringbuffer.test.c e_ringbuffer.o -lcunit

test: e_ringbuffer.test.exe
	./e_ringbuffer.test.exe

clean:
	rm -vf *~ *.o *.exe