
 /**
 * The ConfigDetails header source file defines a structure. 
 * The Structure holds a buffer which acts as a FIFO queue
 * to store number of lines specified as a module parameter on the command line
 * and synchronization primitive to prevent race condition and deadlock.
 */

#ifndef CONFIGDETAILS_H
#define CONFIGDETAILS_H

 /**
  * The standard header files 
  */
 #include <linux/init.h>                 // Macros used to mark up functions e.g __init __exit
 #include <linux/module.h>               // Core header for loading module into the kernel
 #include <linux/kernel.h>               // Contains types, macros, functions for the kernel     
 #include <asm/uaccess.h>                // Required for copy from kernel to user space buffer function support
 #include <linux/sched.h>                // Header file
 #include <linux/slab.h>                 // Header file
 #include <linux/moduleparam.h>          // Header file for Kernel module to have parameter as a command line argument
 #include <linux/proc_fs.h>	         // Header file for the PROC filesystem */
 #include <linux/kprobes.h>		 // Header file to support Kprobes, Jprobes
 #include <linux/ktime.h>		 // Header file to support kernel time
 #include <linux/memory.h> 		 // Header file 	
 #include <linux/memcontrol.h> 		 // Header file
 #include <linux/limits.h>		 // Header file

 /**
 * Kernel Module Information 
 */
 MODULE_LICENSE("GPL");                         	 // The license type -- Avoid kernel taints warning     
 MODULE_AUTHOR("Mithun Deshpande");                      // The author       -- visible when you use modinfo
 MODULE_DESCRIPTION("Tracking Page Fault in  Process");  // The description  -- see modinfo
 MODULE_VERSION("4.0");                                  // The version no   -- to inform users
 
 /**
  * Configuration Data Structure
  */
 typedef struct ConfigData {
	
	long disTime;			// time 
	unsigned long virAddress;	// virtual address

 } ConfigData;

 /**
 * Proc file name
 */ 
 #define PROC_SPEC_FILE "data"


 /**
  * Max Config Buffer Initial Capacity
  */
 #define BUFFER_CAPACITY 1000

 /**
 * The Variable used to hold process ID
 */
 pid_t processId = 1;

/**
 * The Variable used to fill up buffer counter 
 */
 int bufSizeCounter = 0;

/**
 * Dealing with an error scenerio
 */ 
 int error = 0;

 /**
  * Module Parameter Initialization and description
  */
 module_param(processId, int, 0000);
 MODULE_PARM_DESC(processId, "Process ID");

 /**
  * The Prototype functions for the proc file system
  */
 static int  device_open(struct inode *, struct file *);
 static int  device_close(struct inode *, struct file *);
 static ssize_t  device_read(struct file *, char *, size_t, loff_t *);

 /** 
 *   @brief Devices are represented as file structure in the kernel. The file_operations structure from
 *   /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *   using a C99 syntax structure. proc fs usually implement open, read, write, release and
 *   lseek calls
 */
 static struct file_operations fops = {
        .open = device_open,                    // open  device operation
        .read = device_read,                    // read  device operation
        .release = device_close                 // close device operation
 };

 /**
 * @brief JProbe representation.
 */
 static struct jprobe device_jprobe = {

	.entry = handle_mm_fault,
	.kp = {
		.symbol_name = "handle_mm_fault",
	},
 };

/**
 * Information about the proc file system
 */
 struct proc_dir_entry *procEntry;

#endif
