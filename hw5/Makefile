CC = gcc
CFLAGS = -pedantic -errors -Wall -c -std=gnu99 

all: hw5

hw5: orkun.o
	$(CC) orkun.o -o hw5 -lpthread -lrt -lm

orkun.o: orkun.c
	$(CC) $(CFLAGS) orkun.c  

clean:
	rm -rf *o hw5
