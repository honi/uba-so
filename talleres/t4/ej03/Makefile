obj-m += letras123.o

KBUILD=/lib/modules/$(shell uname -r)/build/

all:
	make -C $(KBUILD) M=$(PWD) modules

clean:
	make -C $(KBUILD) M=$(PWD) clean
	-rmmod letras123.ko

test: all
	g++ -g -Wall -pedantic -pthread -std=c++0x test.cpp -o test
	-rmmod letras123.ko
	insmod letras123.ko
	-./test
	rmmod letras123.ko

testpy: all
	-rmmod letras123.ko
	insmod letras123.ko
	-./test.py
	rmmod letras123.ko
