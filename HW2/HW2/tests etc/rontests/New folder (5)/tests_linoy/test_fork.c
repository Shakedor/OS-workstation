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
	lp2[0]= 0;
	lp2[1] = 0;
	lp2[2] =0;
	lp[0]= 0;
	lp[1] = 800;
	lp[2] =5;
	sched_setscheduler(getpid(),5,&lp);
	if (fork() == 0) {
		assert(is_SHORT(getpid()) ==1);
		assert(remaining_cooloffs(getpid()) ==3);
		printf("son1\n");
		exit(1);
		
	}
	printf("father\n");
	while(is_SHORT(getpid()));
	if (fork() == 0) {
		assert(is_SHORT(getpid()) ==0);
		printf("remaining_cooloffs %d\n",remaining_cooloffs(getpid()));
		//assert(remaining_cooloffs(getpid()) ==1);
		printf("son2\n");
		exit(1);
		
	}
	printf("father\n");
	
}