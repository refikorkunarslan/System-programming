CC = gcc
CFLAGS = -pedantic -errors -Wall -c -std=gnu99 

all: hw1

hw1: 151044063.o
	$(CC) 151044063.o -o hw1

151044063.o: 151044063.c
	$(CC) $(CFLAGS) 151044063.c

clean:
	rm -rf *o hw1
