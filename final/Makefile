CC = gcc
CFLAGS = -pedantic -errors -Wall -c -std=gnu99 

all: client server servant

client: client.o
	$(CC) client.o -o client -lpthread -lrt

client.o: client.c
	$(CC) $(CFLAGS) client.c 

server: server.o
	$(CC) server.o -o server -lpthread -lrt

server.o: server.c
	$(CC) $(CFLAGS) server.c 

servant: servant.o
	$(CC) servant.o -o servant -lpthread -lrt

servant.o: servant.c
	$(CC) $(CFLAGS) servant.c 

clean:
	rm -rf *o client server servant
