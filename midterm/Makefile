CC = gcc
CFLAGS = -pedantic -errors -Wall -c -std=gnu99 

all: client serverY serverZ

client: client.c
	$(CC) client.c -o client

serverY: serverY.c
	$(CC) serverY.c -o serverY

serverZ: serverZ.c
	$(CC) serverZ.c -o serverZ -lpthread -lrt

clean:
	rm -rf *o client serverY serverZ
