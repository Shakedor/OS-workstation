
#include <linux/mylist.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/blocker.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>



// bucnh of includes
// include list so we can user static functions


// implement utility functions for blockedList


int sys_block_program(const char *name, unsigned int name_len){
   // check name for NULL or other param errors
  if(!name || name_len <= 0 )
    return EINVAL;
  if(!blockedPrg){
    blockedPrg = listCreate(stringCopy, stringDestroy);
    if(!blockedPrg)
      return -1;
  }	//check null terminated string- are you sure about that?
	//if error return -EINVAL
	
	// check if in list, the function checks if the list is empty
  if(isInList(blockedPrg, name) == LIST_IS_IN)
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
	// if so remove and return prog was unblocked, the function checks if the list is empty
	if(removeString(blockedPrg, name) == LIST_SUCCESS)
	  return 
	    // I HAVE NO IDEA WHAT ARE THE RETURN VALUES
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
	return isBlocked(name);
}

int sys_get_blocked_count(void){
	return(ListGetSize(blockedPrg));
}


int sys_get_forbidden_tries(int pid, char log[][256], unsigned int n){
// if n<=0 ret -EINVAL
	if(n<=0){
		return EINVAL;
	}
	// check in processDS if process num pid exists
	// if not return -ESRCH
	task_struct *currStruct = find_task_by_pid(pid);
	
	if(currStruct==NULL){
		return(ESRCH);
	}	
	
	// get the stack start and end of said process
	unsigned long stack_start = (unsigned long)get_current();
	unsigned long stack_end = (unsigned long)(get_current()|0xefff);
	unsigned long log_position= (unsigned long)log;
	// check if log is legally within the process' stack else return 'EFAULT'
	if (!(stack_start<=log && log<=stack_end)){
		return(EFAULT);
	}
	
	// get the process' forbidden access log
	List forbiddenLog=currStruct->forbidenList;
	unsigned int logSize=listGetSize(forbiddenLog);
	unsigned int minSize=mymin(logSize,n);
	unsigned int maxSize=mymax(logSize,n);
	// for min(n,len of log)
	// put in char** param the log messages (Actually copy perhaps using the kernel copy functions)
	// put NULL in remaining spots
	
	for( int i=0,ListElement curr=listGetFirst(forbiddenLog); i<minSize;i++,listGetNext(forbiddenLog)){
		res=strcpy(log[i],curr);
		if(res==NULL){
			return(-1);
		}
	}
	for(int i=minSize; i<n; i++){
		log[i]=NULL;
	}
	
	//return number of entries copied
	return(minSize);
	
}


int isBlocked(const char *name){
  if(!name)
    return EINVAL;
  //check if in list
  if(isInList(blockedPrg, name) == LIST_IS_IN)
    return 1;
  return 0;
}




