# make for 4981 assignment 3

CC=g++
CFLAGS=-Wall -ggdb

all: server

server: server.o	
	$(CC) $(CFLAGS) server.o -o server

clean:
	rm -f *.o *.bak server

server.o: server.cpp
	$(CC) $(CFLAGS) -O -c server.cpp

