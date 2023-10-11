obj-m += azar.o

KBUILD=/lib/modules/$(shell uname -r)/build/

all:
	make -C $(KBUILD) M=$(PWD) modules

clean:
	make -C $(KBUILD) M=$(PWD) clean
	-rmmod azar.ko

test: all
	g++ -g -Wall -pedantic -pthread -std=c++0x test.cpp -o test
	insmod azar.ko
	-./test
	rmmod azar.ko

testpy: all
	insmod azar.ko
	-./test.py
	rmmod azar.ko
