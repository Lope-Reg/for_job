arch?=arm
modname?=driver

ifeq ($(arch),arm)
 KERNELDIR:=/home/linux/linux-5.10.61/
else
 KERNELDIR:=/lib/modules/$(shell uname -r)/build/
endif

PWD:=$(shell pwd)

.PHONY:all
all:
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc -o user_app demo.c
.PHONY:clean
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm -f user_app
obj-m:=$(modname).o