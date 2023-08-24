obj-m += simple.o hello.o

KBUILD=/lib/modules/$(shell uname -r)/build/

all:
	@mkdir -p bin
	make -C $(KBUILD) M=$(PWD) modules

clean:
	make -C $(KBUILD) M=$(PWD) clean

test-simple:
	sudo dmesg -C
	sudo insmod simple.ko
	sudo rmmod simple.ko
	sudo dmesg

test-hello:
	sudo dmesg -C
	sudo insmod hello.ko
	ls -l /proc/hello
	cat /proc/hello
	sudo rmmod hello.ko
	sudo dmesg
