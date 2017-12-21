
/**
 * @file   ConfigDetailsSystemModule.c
 * @author Mithun Deshpande
 * @date   30 April 2017
 * @brief  The Source Code implements a Linux Loadable Kernel Module.
 * This module maps to "proc" filesystem and returns page fault for a particular process.
 */

/**
 * Custom header file
 */
#include "ConfigDetails.h"

/**
 * Config Details structure variable
 */
ConfigData configDetails[BUFFER_CAPACITY];

/**
 * Retrive and send data to user space application
 */
int counter = 0;	

/**
 * @brief The LKM initialization function
 * The static keyword restricts the visibility of the function to within this C file. The __init        
 * macro means that for a built-in driver (not a LKM) the function is only used at the initialization time
 * and that it can be discarded and its memory freed up after that point.
 * @return 0 if successful
 */
static int __init driver_entry(void)
{
	printk(KERN_INFO "Config Module : Initializing the ConfigDetailsSystemModule\n");

	/**
	 * Registering proc device 
	 */
	error = register_jprobe(&device_jprobe);  // register device 
	if(error < 0)
	{
		printk(KERN_ALERT "Config Module : Jprobe can't register and returned %d\n", error);
		return -1;
	}

	/**
	 * Creating proc file entry
	 */
	procEntry = proc_create(PROC_SPEC_FILE, 0, NULL, &fops);
	if(procEntry == NULL)
	{
		remove_proc_entry(PROC_SPEC_FILE, NULL);
		printk(KERN_ALERT "Config Module : Proc Entry Failed\n");
		return -1;
	}

	printk(KERN_INFO "Config  Module : Jprobe planted at = %p\n", device_jprobe.kp.addr);
	printk(KERN_INFO "COnfig  Module : Jprobe handler at = %p\n", device_jprobe.entry);
	printk(KERN_INFO "Config  Module : Initialization Done Successfully\n");

	return 0;
}


/** 
 *   @brief The LKM cleanup function
 *   Similar to the initialization function, it is static. The __exit macro notifies that if this
 *   code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit driver_exit(void)
{
	unregister_jprobe(&device_jprobe);                    // unregister the jprobe handler
	remove_proc_entry(PROC_SPEC_FILE, NULL);
	printk(KERN_INFO "Config Module : Jprobe unregistered at = %p\n", device_jprobe.kp.addr);
	printk(KERN_INFO "Config Module : Unloaded ConfigDetailsSystemModule\n");
}

/** 
 *  @brief The device open function that is called each time the device is opened
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int device_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Config Module : Opened Device Successfully\n");
	return 0;
} 

/** 
 *  @brief The device release function that is called whenever the device is closed/released by
 *  the userspace program.
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int device_close(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Config Module : Device Successfully Closed\n");
	return 0;
}

/** 
 *  @brief This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer to the user and captures any errors. 
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buf  The pointer to the buffer to which this function writes the data
 *  @param bufCount The length of the b
 *  @param curOffset The offset if required
 */

static ssize_t device_read(struct file *filp, char *buf, size_t bufCount, loff_t *curOffset)
{
	/**
	 * Return number of bytes
	 */
	int ret;

	/**
	 * Initialize them
	 */
	ret = 0;

	if(counter < bufSizeCounter)
	{
		ret = copy_to_user(buf, &configDetails[counter], sizeof(configDetails[counter]));
		if(ret < 0)
		{
			printk(KERN_ALERT "Config Module : Copy to user failed\n");
			ret = -1;
		}

		counter++;
	}
	else
	{
		ret = -1;
	}

	return ret;
}

/**
 * Handler routine
 */	
int handle_mm_fault(struct mm_struct *mm, struct vm_area_struct *vma, unsigned long address, unsigned int flags)
{
	/**
	 * Time structure
	 */
	struct timespec aTime;  	

	/**
	 * Check for process ID given
	 */
	if(processId == current -> pid)
	{
		if(bufSizeCounter < BUFFER_CAPACITY)
		{
			aTime = current_kernel_time();
			configDetails[bufSizeCounter].disTime = aTime.tv_nsec;
			configDetails[bufSizeCounter].virAddress = address; 
			printk(KERN_INFO "Process Id = %d Counter = %d Time (nsec) = %ld Address = 0x%lx\n", current -> pid, bufSizeCounter, configDetails[bufSizeCounter].disTime, configDetails[bufSizeCounter].virAddress);
			bufSizeCounter++;
		}
		else
		{
			aTime = current_kernel_time();
			printk(KERN_INFO "Process Id = %d Time (nsec) = %ld Address = 0x%lx\n", current -> pid, aTime.tv_nsec, address);
		}
	}

	jprobe_return();
	return 0;
}


/** 
 *  @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(driver_entry);
module_exit(driver_exit);

