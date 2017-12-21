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
	 $(CC) ConfigDetailsUserModule.c -o ConfigDetailsUserModule	 	
	 $(CC) OtherAppTestCase1.c -o OtherAppTestCase1
	 $(CC) TestCase2.c -o TestCase2
	 $(CC) IOTestCase3.c -o IOTestCase3
	 $(CC) ComputeItensiveTestCase4.c -o ComputeItensiveTestCase4
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.symvers *.order ConfigDetailsUserModule OtherAppTestCase1 TestCase2 IOTestCase3 ComputeItensiveTestCase4
endif
