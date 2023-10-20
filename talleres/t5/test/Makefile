CC=g++
CFLAGS=-g -Wall -pedantic -I../include/

all: testhdd

testhdd: testhdd.o ../src/hdd.o ../src/mbr.o ../src/pentry.o ../src/ext2fs.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

testhdd.o: testhdd.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

clean:
	rm -rf *.o
	rm -rf testhdd
