CC=gcc
CFLAGS=-I.

all: tcpclient tcpserver
tcpclient :
	$(CC) -o tcpclient  tcpclient.c $(CFLAGS)

tcpserver :
	$(CC) -o tcpserver tcpserver.c $(CFLAGS)

clean :
	rm Mult.o tcpserver tcpclient