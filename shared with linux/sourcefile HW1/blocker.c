


// bucnh of includes
// include list so we can user static functions


// implement utility functions for blockedList



int sys_block_program(const char *name, unsigned int name_len){
	// check name for NULL or other param errors
	//check null terminated string
	//if error return -EINVAL
	
	// check if in list
	//if already in list return appropriate return value
	//if not in list
	// add to blockedList
	
	//return blocked ret value
}


int sys_unblock_program(const char *name, unsigned int name_len){
	// check name for NULL or other param errors
	//check null terminated string
	//if error return -EINVAL
	
	// check if in list
	// if so remove and return prog was unblocked
	
	//if not, return prog not blocked
}


int sys_is_program_blocked(const char *name, unsigned int name_len){
	// check name for NULL or other param errors
	//check null terminated string
	//if error return -EINVAL
	
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








