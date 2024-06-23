arch?=x86
modname?=mycdev
PWD:=$(shell pwd)

ifeq ($(arch),arm)
 KERNELDIR:=/home/linux/linux-5.10.61/
else
 KERNELDIR:=/lib/modules/$(shell uname -r)/build/
endif
.PHONY:all
all:
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc write.c -o write
	gcc read.c -o read
.PHONY:clean
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm write
	rm read
obj-m:=$(modname).o