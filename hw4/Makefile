CC = gcc
CFLAGS = -pedantic -errors -Wall -c -std=gnu99 

all: hw4

hw4: orkun.o
	$(CC) orkun.o -o hw4 -lpthread -lrt

orkun.o: orkun.c
	$(CC) $(CFLAGS) orkun.c  

clean:
	rm -rf *o hw4
