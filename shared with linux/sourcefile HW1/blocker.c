
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/kerBlocker.h>
#include <asm/uaccess.h>
#include <linux/mylist.h>

// bucnh of includes
// include list so we can user static functions


// implement utility functions for blockedList


int sys_getfive(void){
	return 5;
}




int isBlocked(const char *name){
	#ifdef MYBDBG
	printk("entering is blocked, name is %s \n",name);
	#endif

  if(!name){
	#ifdef MYBDBG
	printk("bad name! name is %s \n",name);
	#endif
    return -EINVAL;
  }
  //if list is empty
  if(!blockedPrg){
	#ifdef MYBDBG
	printk("no blocked prog list so no prog is blocked \n");
	#endif	  
	  return 0;
  }
  //check if in list
  if(isInList(blockedPrg, name) == LIST_IS_IN){
		    #ifdef MYBDBG
			printk("%s is in list return 1 from isBlocked\n",name);
			#endif
		return 1;

  }
	#ifdef MYBDBG
	printk("%s is NOT in list return 0 from isBlocked\n",name);
	#endif
  
  return 0;
}

ListResult listAddFromUser(List l, const char *userStr, int str_len){
	
  #ifdef MYBDBG_list
  printk(" entering listAddFromUser  add string (failed unless a success message is printed) l57\n");
  #endif	
  if(!userStr || !l)
    return LIST_NULL_ARGUMENT;
  char* temp = kmalloc(sizeof(char)*(str_len + 1 ),GFP_KERNEL);
  if(!temp)
    return LIST_OUT_OF_MEMORY;
  if(copy_from_user(temp, userStr, str_len + 1)){
    kfree(temp);
    return LIST_COPY_FAIL;
   }
  if( listAddString(l, temp) != LIST_SUCCESS ){
    kfree(temp);
    return LIST_OUT_OF_MEMORY;
  }
  kfree(temp);
      
  #ifdef MYBDBG_list
  printk("listAddFromUser add successful l74\n");
  #endif
		
  return LIST_SUCCESS;
}

int sys_block_program(const char *name, unsigned int name_len){
   // check name for NULL or other param errors
  if(!name || name_len <= 0 ){
    #ifdef MYBDBG
    printk("sys_block_program name is NULL or name len <= 0 line 24\n");
    #endif
    return -EINVAL;
  }
  if(!blockedPrg){
    blockedPrg = listCreate();
    if(!blockedPrg)
      return -1;
  }	//check null terminated string- are you sure about that?
	//if error return -EINVAL
	
	// check if in list, the function checks if the list is empty
  if(isInList(blockedPrg, name) == LIST_IS_IN){
    #ifdef MYBDBG
    printk("sys_block_program program %s  is already blocked l 38\n", name);
    #endif
    return 1;
  }
	//if already in list return appropriate return value
        //if not in list
	// add to blockedList
	ListResult Res=listAddFromUser(blockedPrg, name,name_len);
	
  if(Res == LIST_OUT_OF_MEMORY){
    #ifdef MYBDBG
    printk("sys_block_program adding program %s to forbidden list failed, return -1  line 47\n", name);
    #endif
    return -ENOMEM;
  }
  
  if(Res==LIST_COPY_FAIL){
	#ifdef MYBDBG
    printk("sys_block_program adding program %s to forbidden list copied from forbidden area, return -1  line 47\n", name);
    #endif
    return -EFAULT ;
  }

   #ifdef MYBDBG
    printk("sys_block_program adding program %s to forbidden list succedeed, return 1  line 53\n", name);
    #endif
	//return blocked ret value
  return 0;
}


int sys_unblock_program(const char *name, unsigned int name_len){
	// check name for NULL or other param errors
	//check null terminated string
	//if error return -EINVAL
  if(!name || name_len <= 0 ){
	  #ifdef MYBDBG
	  printk("sys_unblock_program name is NULL or name len <= 0 line 66\n");
          #endif
	  return -EINVAL;
  }
	// check if in list
	// if so remove and return prog was unblocked, the function checks if the list is empty
  if(!blockedPrg){
     #ifdef MYBDBG
    printk("sys_unblock_program the forbidden list is empty for program %s line 74\n", name);
      #endif
    return 1;
  }
  if(listRemoveString(blockedPrg, name) == LIST_SUCCESS){
     #ifdef MYBDBG
     printk("sys_unblock_program removing of program %s from the forbidden list succedeed line 80\n",name);
     #endif
    return 0;
  }

   #ifdef MYBDBG
   printk("sys_unblock_program program %s wasn't blocked  line 86\n",name);
   #endif
   return 1; //the programm wasn't blocked
	    // I HAVE NO IDEA WHAT ARE THE RETURN VALUES
}


int sys_is_program_blocked(const char *name, unsigned int name_len){
	// check name for NULL or other param errors
	//check null terminated string
	//if error return -EINVAL
  if(!name || name_len <= 0 ){
     #ifdef MYBDBG
     printk("sys_is_program_blocked name is NULL or name len <= 0 line 99\n");
     #endif
    return -EINVAL;
  }
	//check if in list
	//if yes return 1 (blocked)
	//else return 0(not blocked)
	#ifdef MYBDBG
     printk("sys_is_program_blocked name is legit, checking if its blocked\n");
     #endif
	return isBlocked(name);
}

int sys_get_blocked_count(void){
	 #ifdef MYBDBG
     printk("entering sys get blocked_count\n");
     #endif
  if(!blockedPrg){
     #ifdef MYBDBG
     printk("sys_get_blocked_count the forbidden list is empty! 112\n");
     #endif
    return 0;
  }
	return(listGetSize(blockedPrg));
}


int sys_get_forbidden_tries(int pid, char log[][256], unsigned int n){
// if n<=0 ret -EINVAL
	if(n<=0){
	  #ifdef MYBDBG
	  printk("sys_get_forbidden_tries n <= 0 line 124\n");
          #endif
		return -EINVAL;
	}
	// check in processDS if process num pid exists
	// if not return -ESRCH
	struct task_struct *currStruct = find_task_by_pid(pid);
	
	if(currStruct==NULL){
	  #ifdef MYBDBG
	  printk("sys_get_forbidden_tries there is no process with pid line 134 %d\n", pid);
          #endif
	  return(-ESRCH);
	}	
	
	int emptyList=0;
	// get the process' forbidden access log
	if(currStruct->forbidenList==NULL){
		emptyList=1;
	}
	
	List forbiddenLog=currStruct->forbidenList;
	
	unsigned int logSize;
	if(emptyList==1){
		logSize=0;
	}else{
		logSize=listGetSize(forbiddenLog);
	}	
	unsigned int minSize=mymin(logSize,n);
	
	if(access_ok(VERIFY_WRITE,log,minSize*256)==0){
		return -EFAULT;
	}
	
	if(emptyList){
		return 0;
	}
	
	//unused Variable
	//
	//unsigned int maxSize=mymax(logSize,n);
	//
	
	// for min(n,len of log)
	// put in char** param the log messages (Actually copy perhaps using the kernel copy functions)
	// put NULL in remaining spots
	
	 int i;
	 char* curr=NULL;
	 int res=0;
	  #ifdef MYBDBG
	  printk("sys_get_forbidden_tries about to start copy the log of process pid line 169 %d\n", pid);
          #endif
	for( i=0, curr=listGetFirst(forbiddenLog); i<minSize;i++,curr=listGetNext(forbiddenLog)){
		#ifdef MYBDBG
		printk(" curr is %s \n", curr);
        #endif
		res==copy_to_user(log[i],curr,strlen(curr)+1);
		if(res!=0){
			return(-EFAULT);
		}
	}
	 #ifdef MYBDBG
	  printk("sys_get_forbidden_tries finishing copy the log of process pid line 178 %d\n", pid);
          #endif
	for( i=minSize; i<n; i++){
		log[i][0]=0;
	}
	
	//return number of entries copied
	return(minSize);
	
}






