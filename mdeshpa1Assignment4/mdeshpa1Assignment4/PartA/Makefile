# If KERNELRELEASE is defined, we have been invoked from the 
# kernel build system and can use its language
ifneq ($(KERNELRELEASE),)
	obj-m := ConfigDetailsSystemModule.o

# Otherwise we were called directly from the command 
# line, invoke the kernel build system
else
	KERNELDIR = /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
	CC = gcc

default:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) modules
	$(CC) TestCase1.c -o TestCase1
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.symvers *.order TestCase1
endif
