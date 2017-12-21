
 /**
 * @file   ConfigDetailsSystemModule.c
 * @author Mithun Deshpande
 * @date   12 April 2017
 * @brief  The Source Code implements a Linux Loadable Kernel Module.
 * This module maps to "/dev/linepipe" and comes with a helper 
 * Shell Scripts that can be run in Linux user space to communicate 
 * with this the kernel module. The Kernel module implements all the Synchronization 
 * mechanism or primitive needed for the Producer-Consumer problem to avoid race conditions and 
 * deadlock.    
 */

 /**
  * Custom header file
  */
  #include "ConfigDetails.h"

 /**
  * Config Details structure variable
  */
  ConfigData configDetails;


 /**
 * @brief The LKM initialization function
 * The static keyword restricts the visibility of the function to within this C file. The __init        
 * macro means that for a built-in driver (not a LKM) the function is only used at the initialization time
 * and that it can be discarded and its memory freed up after that point.
 * @return 0 if successful
 */
 static int __init driver_entry(void)
 {
        printk(KERN_INFO "Config Module : Initializing the ConfigDetailsSystemModule LKM\n");
        
	/**
         * Registering char driver device 
         */
        error = misc_register(&miscProcessDevice);  // register device 
        if(error)
        {
                printk(KERN_ALERT "Config Module : Misc device can't register\n");
                return error;
        }

	/**
	 * To track how many items from the buffer has been consumed and produced
	 * Initialize it with given buffer size
	 */
        trackQueue = N;

	printk(KERN_INFO "Config Module : Queue Buffer Size(N) = %d\n", N);

	/**
	 * To control access to critical region for multiple consumers
	 */
	sema_init(&configDetails.semOneMutex, 1); 

	/**
	 * To control access to critical region for multiple producers
	 */
	sema_init(&configDetails.semTwoMutex, 1);


	/**
	 * To count empty buffer slots 
	 */
	sema_init(&configDetails.empty, N);

	/**
	 * To count full buffer slots
	 */
	sema_init(&configDetails.full, 0); 

 	 
	/**
	 * Allocate memory to hold line into the buffer dynamically
	 */
        configDetails.lineItem = kmalloc(N*LINE_SIZE*sizeof(char*), GFP_KERNEL);

	/**
	 * Handle error condition
	 */
        if(!configDetails.lineItem)
	{
                printk(KERN_ALERT "Config Module : Not Enough Memory\n");
	}

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
        kfree(configDetails.lineItem);
        misc_deregister(&miscProcessDevice);                    // unregister the miscellaneous device
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
 *  @brief This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. In this case, the producer generates data.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param length The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */
 static ssize_t device_write(struct file *filep, const char *buffer, size_t length, loff_t *offset)
 {

	/**
	 * The Variable used to hold bytes stored or counted
	 */
        int bytesCounter;

	/**
         * The Variable used to keep track of how many times it is being called for filling up the buffer
         */
        int breakCounter;

	/**
	 * The Variable used to hold line produced by the Producer
	 */
	char *line;
	
	/**
	 * Initialize local variables
	 */
	bytesCounter = 0;
	breakCounter = 0;
	line = NULL;


	/**
	 * Use this version instead of down so can terminate from user space
  	 * Lock for multiple producer : Enter into the critical region
	 */
        if(down_interruptible(&configDetails.semTwoMutex) < 0)	
	{
		printk(KERN_ALERT "Config Module : Exit From User\n");
		return -1;
	}       
        
	/**
	 * Use this version instead of down so can terminate from user space
	 * Lock for writing : Decrement empty count
	 */
	if(down_interruptible(&configDetails.empty) < 0)
	{
		printk(KERN_ALERT "Config Module : Exit From User\n");
                return -1;
	}

	/**
	 * The Variable used to hold the insertion position into the buffer size
	 */         
	insertQueue = insertQueue%N;

	 /**
	  * Allocate Memory for the Line generated by the producer
	  */
	line = kmalloc(sizeof(char)*length, GFP_KERNEL);

	/**
	 * Store line into the buffer 
	 */
         while(bytesCounter < length) 
	 {

			/**
			 * To keep track of buffer size
			 */
                        if(!(trackQueue > 0)) 
			{
                                breakCounter = 1;
                                break;
                        }

			/**
			 * To copy line from user space 
			 */	
                        else
                        {
                                line[bytesCounter] = buffer[bytesCounter];
                                bytesCounter++;
                                breakCounter = 0;
                        }
         }

	 /**
	  * Positional insert into the queue
	  */
         if(breakCounter != 1)
         {
                        configDetails.lineItem[insertQueue] = line;     
                        insertQueue++;
                        trackQueue--;
         }

	/**
	 * Relase lock for writing 
	 * Increment count of full slots
	 */
        up(&configDetails.full);                  

	/**
         * Release lock for producer
         * Exit the critical region
         */
        up(&configDetails.semTwoMutex);              
    
	return bytesCounter;
 }
 
 /** 
 *  @brief This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer to the user and captures any errors. 
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param bufCount The length of the b
 *  @param curOffset The offset if required
 */

 static ssize_t device_read(struct file *filp, char *buffer, size_t bufCount, loff_t *curOffset)
 {

	/**
         * The Variable used to hold bytes read
         */
        int bytesCounter;

	/**
         * The Variable used to keep track of how many times it is being called for removing the buffer
         */
        int breakCounter;

	/**
	 * Initialize the local variable
	 */
	bytesCounter = 0;
	breakCounter = 0;


        /**
         * Use this version instead of down so can terminate from user space
         * Lock for multiple consumer : Enter into the critical region
         */
        if(down_interruptible(&configDetails.semOneMutex) < 0)
        {
                printk(KERN_ALERT "Config Module : Exit From User\n");
                return -1;
        }

        /**
         * Use this version instead of down so can terminate from user space
         * Lock for reading : Decrement full count
         */
        if(down_interruptible(&configDetails.full) < 0)
        {
                printk(KERN_ALERT "Config Module : Exit From User\n");
                return -1;
        }

	/**
         * The Variable used to hold the removal position from the buffer size
         */
        removeQueue = removeQueue%N;


	/**
	 * This is used to consume the lines stored in the Queue or 
	 * buffer.
	 */ 	
        while(bytesCounter < LINE_SIZE) 
	{
		 /**
		  * To keep track of how many lines are being read so far
		  */
                 if(!(trackQueue < N))
                 {
                                breakCounter = 1;
                                break;
                 }

		 /**
		  * To copy from Queue into user space buffer
		  */	
                 else
                 {
                       copy_to_user(&buffer[bytesCounter], &configDetails.lineItem[removeQueue][bytesCounter], 1);
                       bytesCounter++;
                       breakCounter = 0;
                 }
        }
                
	/**
	 * Positional remove from the queue
	 */
	if(breakCounter != 1)
        {
                 removeQueue++;
                 trackQueue++;
        }
		
	/**
	 * Release lock for reading
	 * Increment the count of empty slots
	 */        
	up(&configDetails.empty);                    

	/**
	 * Release lock for consumer
	 * Exit the critical region
	 */
        up(&configDetails.semOneMutex);               

        return bytesCounter;
 }

 /** 
 *  @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
 module_init(driver_entry);
 module_exit(driver_exit);

