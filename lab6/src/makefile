CC=gcc
CFLAGS=-I.

all: client server
client : Mult.o Mult.h
	$(CC) -o client Mult.o client.c $(CFLAGS) -pthread

server : Mult.o Mult.h
	$(CC) -o server Mult.o server.c $(CFLAGS) -pthread

Mult.o : Mult.h
	$(CC) -o Mult.o -c Mult.c $(CFLAGS)

clean :
	rm Mult.o server client