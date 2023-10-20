CC=g++
CFLAGS=-g -Wall -pedantic -I../include/

all: hdd.o pentry.o mbr.o

hdd.o: hdd.cpp
	$(CC) $(CFLAGS) -c -o $@ $^
	
pentry.o: pentry.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

mbr.o: mbr.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf *.o
