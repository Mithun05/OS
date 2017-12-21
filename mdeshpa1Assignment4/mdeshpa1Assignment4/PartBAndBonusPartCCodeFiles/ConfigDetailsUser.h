/**
 * The ProcessInfo header source file defines a structure. 
 * The struct contains the process ID, process parent ID,
 * CPU it's running on, and it's current status or state.
 */

#ifndef CONFIGDETAILSUSER_H
#define CONFIGDETAILSUSER_H    

/**
  * Configuration Data Structure
  */
 typedef struct ConfigData {

        long disTime;                   // time 
        unsigned long virAddress;       // virtual address

 } ConfigData;

 /**
 * Proc file name
 */
 #define PROC_SPEC_FILE "/proc/data"


 /**
  * Max Config Buffer Initial Capacity
  */
 #define BUFFER_CAPACITY 1000

#endif
