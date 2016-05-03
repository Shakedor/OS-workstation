#include "hw2_syscalls.h"
#include <assert.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#ifndef	errno
extern int errno;
#endif
 

 
int main() {

	int res = 0;
	int lp[3] = {0, 0, 0};
	int lp2[3] = {0, 0, 0};
	/*trying to change other to short from child*/
	int pid1 = fork();
	if(pid1 == 0){
	int i=0;
		for(;i<100000;i++);
		exit(1);	
	}
	lp[1] = 1000;
	lp[2] = 4;
	res = sched_setscheduler(pid1,5,&lp);
	sched_getparam(pid1,&lp2);
	assert(lp2[1] == 1000 && is_SHORT(pid1));
	


	

	/*trying to put wronge parms in short*/
	lp[1] = 4000;
	lp[2] = 4;
	res = sched_setscheduler(getpid(),5,&lp);
	assert(errno == EINVAL && res == -1);
	lp[1] = 2000;
	lp[2] = 7;
	res = sched_setscheduler(getpid(),5,&lp);
	assert(errno == EINVAL && res == -1);
	/*succese*/
	lp[1] = 1000;
	lp[2] = 3;
	res = sched_setscheduler(getpid(),5,&lp);
	assert(res==0);
	/**/
	/*trying to change back to short*/
	res = sched_setscheduler(getpid(),5,&lp);
	assert(errno == EPERM && res == -1);
	/**/
	/*trying to change back to other*/
	res = sched_setscheduler(getpid(),0,&lp);
	assert(errno == EPERM && res == -1);	
	/**/
	/*trying to change the cooloffs for short*/
	lp[1] = 1000;
	lp[2] = 4;
	res = sched_setparam(getpid(),&lp);
	sched_getparam(getpid(),&lp2);
	assert(lp2[2] == 3);
	/*trying to change the prio to */
	sched_getparam(getpid(),&lp2);
	int preprio = lp2[0];
	lp[0] =120;
	res = sched_setparam(getpid(),&lp);
	sched_getparam(getpid(),&lp2);
	assert(lp2[0] == preprio);
 
    return 0;

}
