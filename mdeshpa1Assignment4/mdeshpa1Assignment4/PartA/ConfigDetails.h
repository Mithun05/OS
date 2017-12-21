
 /**
 * The ConfigDetails header source file defines a module parameter 
 * which is a process Id and register jprobe handler routine
 * to display a page fault of process using that particular ID.
 */

/**
 * Avoid multiple inclusion of a same header file
 */
#ifndef CONFIGDETAILS_H
#define CONFIGDETAILS_H

 /**
  * The standard header files 
  */
 #include <linux/init.h>		 // Header file to support __init or __exit
 #include <linux/module.h>               // Core header for loading module into the kernel
 #include <linux/kernel.h>               // Contains types, macros, functions for the kernel     
 #include <linux/kprobes.h>		 // Header file to support Kprobes, Jprobes

 /**
 * Kernel Module Information 
 */
 MODULE_LICENSE("GPL");                         	 // The license type -- Avoid kernel taints warning     
 MODULE_AUTHOR("Mithun Deshpande");                      // The author       -- visible when you use modinfo
 MODULE_DESCRIPTION("Tracking Page Fault in  Process");  // The description  -- see modinfo
 MODULE_VERSION("4.0");                                  // The version no   -- to inform users
 
 /**
 * The Variable used to hold process ID
 */
 pid_t processId = 1;

 /**
 * The Variable used to deal with an error scenerio. 
 */
 int error;

 /**
  * Module Parameter Initialization and description
  */
 module_param(processId, int, 0000);
 MODULE_PARM_DESC(processId, "Process ID");

 /**
 * @brief JProbe representation.
 */
static struct jprobe device_jprobe = {

	.entry = handle_mm_fault,
	.kp = {
		.symbol_name = "handle_mm_fault",
	},
};

#endif
