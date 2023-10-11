obj-m += nulo.o

KBUILD=/lib/modules/$(shell uname -r)/build/

all:
	make -C $(KBUILD) M=$(PWD) modules

clean:
	make -C $(KBUILD) M=$(PWD) clean
	-rmmod nulo.ko

test: all
	g++ -g -Wall -pedantic -pthread -std=c++0x test.cpp -o test
	insmod nulo.ko
	-./test
	rmmod nulo.ko
