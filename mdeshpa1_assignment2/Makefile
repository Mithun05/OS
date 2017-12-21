
obj-m := mdeshpa1_sys_module.o


KERNELDIR = /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KERNELDIR) SUBDIRS=$(PWD) modules
	$(CC) mdeshpa1_user_module.c -o mdeshpa1_user_module
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.symvers *.order mdeshpa1_user_module


