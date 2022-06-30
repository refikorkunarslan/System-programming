CC = gcc
CFLAGS = -pedantic -errors -Wall -c -std=gnu99 

all: hw3named hw3unnamed

hw3named: orkun.o
	$(CC) orkun.o -o hw3named -lpthread -lrt

orkun.o: orkun.c
	$(CC) $(CFLAGS) orkun.c  

hw3unnamed: orkun1.o
	$(CC) orkun1.o -o hw3unnamed -lpthread -lrt

orkun1.o: orkun1.c
	$(CC) $(CFLAGS) orkun1.c 

clean:
	rm -rf *o hw3named hw3unnamed
