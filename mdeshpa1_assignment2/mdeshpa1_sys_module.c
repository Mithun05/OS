

/**
  The source code implements a Linux loadable kernel module.
  This module maps to /dev/process_list char driver and comes with a 
  helper C program that can be run in Linux user space to communicate 
  with this the kernel module.
  The Kernel module returns to the user space program all currently running 	
  processes.   	
*/

#include <linux/init.h>			// Macros used to mark up functions e.g __init __exit
#include <linux/module.h>		// Core header for loading module into the kernel
#include <linux/miscdevice.h>		// Header file to support kernel Driver Model
#include <linux/kernel.h>		// contains types, macros, functions for the kernel	
#include <linux/fs.h>			// file operation structure which allows use to open/close, read/write to device
#include <asm/uaccess.h>		// copy_to_user;copy_from_user
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include "data.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CHARACTER DEVICE");

Data *data;

Data data_info;

int process_counter = 0;
int record_counter = 0;
int newcounter = 0;


static int device_open(struct inode *inode, struct file *filp)
{

	process_counter = 0;
	record_counter = 0;
	newcounter = 0;
	printk(KERN_INFO "\nmdeshpa1_sys_module : opened device");

        struct task_struct *task_list;

        for_each_process(task_list)
        {
                process_counter++;
        }

        data = kmalloc(sizeof(Data)*process_counter, GFP_KERNEL);

        for_each_process(task_list) {
                data[record_counter].pid =  task_list -> pid;
                data[record_counter].ppid = task_list -> parent -> pid;
   		data[record_counter].cpu = task_cpu(task_list);
		data[record_counter].state = task_list -> state;
                record_counter++;
        }

	printk(KERN_INFO "\nTotal processes %d", record_counter);

	return 0;
}

static ssize_t device_read(struct file *filp, char *bufStoreData, size_t bufCount, loff_t *curOffset)	
{
	int ret;

	if(newcounter < record_counter)
	{
		printk(KERN_INFO "\nCounter %d", newcounter);
		data_info.pid = data[newcounter].pid;
		data_info.ppid = data[newcounter].ppid;
		data_info.cpu = data[newcounter].cpu;
		data_info.state = data[newcounter].state;
		newcounter++;	

		        ret = copy_to_user(bufStoreData, &data_info, sizeof(Data));


			 if(ret == 0)
        	 	 {
		                printk(KERN_INFO "\nmdeshpa1_sys_module : Reading from device");
                		return ret;
        		 }
        		 else
        		 {
                		printk(KERN_ALERT "\nmdeshpa1_sys_module : Failed reading");
                		return -EFAULT;
        		 }
	} 
	else
	{
		return -1;
	}

}

static int device_close(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "\nmdeshpa1_sys_module : closed device");
	return 0;
}

static struct file_operations fops = {
	.open = device_open,
	.read = device_read,
	.release = device_close
};


static struct miscdevice sample_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "process_list",
	.fops = &fops,
};

static int __init driver_entry(void) {

	printk(KERN_INFO "\nInitialize");
	
	printk(KERN_INFO "\nDevice registered");

	int error;

	error = misc_register(&sample_device);
	if(error)
	{
		printk(KERN_ALERT "can't register\n");
		return error;
	}
	
	printk(KERN_INFO "\nDevice created ");
	return 0;
}

static void __exit driver_exit(void)
{
	misc_deregister(&sample_device);
	kfree(data);
	newcounter = 0;
	printk(KERN_INFO "\nmdeshpa1_sys_module : unloaded module");
}
	

module_init(driver_entry);
module_exit(driver_exit);

