
#include "mylist.h"
//HOW TO INCLUDE STRING.H??????

// bucnh of includes
// include list so we can user static functions


// implement utility functions for blockedList

extern


int sys_block_program(const char *name, unsigned int name_len){
   // check name for NULL or other param errors
  if(!name || name_len <= 0 )
    return EINVAL;
  
	//check null terminated string- are you sure about that?
	//if error return -EINVAL
	
	// check if in list
  if(isInList(blockedPrg, name))
    return 0;
	//if already in list return appropriate return value
        //if not in list
	// add to blockedList
  if(listInsertLast(blockedPrg, name) == LIST_OUT_OF_MEMORY)
    return -1;
	//return blocked ret value
  return 1;
}


int sys_unblock_program(const char *name, unsigned int name_len){
	// check name for NULL or other param errors
	//check null terminated string
	//if error return -EINVAL
	if(!name || name_len <= 0 )
	  return EINVAL;
	// check if in list
	// if so remove and return prog was unblocked
	if(removeString(blockedPrg, name) == LIST_SUCCESS)
	  return 
	    // I HSVE NO IDEA WHAT ARE THE RETURN VALUES
	//if not, return prog not blocked
}


int sys_is_program_blocked(const char *name, unsigned int name_len){
	// check name for NULL or other param errors
	//check null terminated string
	//if error return -EINVAL
	if(!name || name_len <= 0 )
	  return EINVAL;
	//check if in list
	//if yes return 0 (blocked)
	//else return 1(not blocked)
	
}

int sys_get_blocked_count(void){
	// return list counter
}


int sys_get_forbidden_tries (int pid, char log[][256], unsigned int n){
	// if n<=0 ret -EINVAL
	// check in processDS if process num pid exists
	// if not return -ESRCH
	
	// get the stack start and end of said process
	// check if log is legally within the process' stack
	
	// get the process' forbidden access log
	// for min(n,len of log)
	// put in char** param the log messages (Actually copy perhaps using the kernel copy functions)
	// put NULL in remaining spots
	
	
}








