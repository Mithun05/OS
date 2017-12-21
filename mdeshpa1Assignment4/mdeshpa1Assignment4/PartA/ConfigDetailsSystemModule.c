
 /**
 * @file   ConfigDetailsSystemModule.c
 * @author Mithun Deshpande
 * @date   30 April 2017
 * @brief  The Source Code implements a Linux Loadable Kernel Module.
 * This module logs a Page Fault for a particular process Id to a system log.
 */

 /**
  * Custom header file
  */
  #include "ConfigDetails.h"


 /**
 * @brief The LKM initialization function
 * The static keyword restricts the visibility of the function to within this C file. The __init        
 * macro means that for a built-in driver (not a LKM) the function is only used at the initialization time
 * and that it can be discarded and its memory freed up after that point.
 * @return 0 if successful
 */
 static int __init driver_entry(void)
 {
        pr_info("Config Module : Initializing the ConfigDetailsSystemModule\n");

	/**
         * Registering proc device 
         */
        error = register_jprobe(&device_jprobe);  // register handler
        if(error < 0)
        {
                pr_err("Config Module : Jprobe can't register and returned %d\n", error);
                return -1;
        }

        pr_info("Config  Module : Jprobe Planted at         = %p\n", device_jprobe.kp.addr); 
	pr_info("Config  Module : Jprobe Handler Address at = %p\n", device_jprobe.entry);
        pr_info("Config  Module : Initialization Done Successfully\n");

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
        pr_info("Config Module : Jprobe unregistered at = %p\n", device_jprobe.kp.addr);
        pr_info("Config Module : Unloaded ConfigDetailsSystemModule\n");
 }

 /**
  * Page fault handler routine
  */
 int handle_mm_fault(struct mm_struct *mm, struct vm_area_struct *vma, unsigned long address, unsigned int flags)
 {
	/**
	 * Check for process ID given
	 */
	if(processId == current -> pid)
	{
		pr_info("Page Fault : Process ID = %d Address = 0x%lx \n", current -> pid, address);
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

