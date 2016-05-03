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
	lp2[0] = 0;
	lp2[1] = 0;
	lp2[2] =0;
	lp[0] = 0;
	lp[1] = 600;
	lp[2] =5;
	sched_setscheduler(getpid(),5,&lp);
	printf("remaining time father 1 %d\n",remaining_time(getpid()));
	int father_pid = getpid();
	if (fork() == 0) {
		printf("remaining time father 2 %d\n",remaining_time(father_pid));
		printf("cooloffs father %d\n",remaining_cooloffs(father_pid));
		lp[0] = 0;
		lp[1] = 2000;
		lp[2] =4;
		sched_setparam(father_pid,&lp);
		exit(1);
	} 
	printf("remaining time father 3 %d ~~ 300\n",remaining_time(getpid()));
	while(is_SHORT(getpid()));
	printf("remaining time overdue father %d ~~ 600\n",remaining_time(getpid()));
	while(!is_SHORT(getpid()));
	printf("remaining time father 4 %d ~~ 2000\n",remaining_time(getpid()));
	if(fork()==0){
		lp[0] = 0;
		lp[1] = 2000;
		lp[2] =0;
		sched_setparam(getpid(),&lp);
		printf("errno %d\n",errno);
		printf("cooloffs father %d\n",remaining_cooloffs(father_pid));
		printf("remaining time son1 %d\n",remaining_time(getpid()));
		printf("cooloffs son %d == 1\n",remaining_cooloffs(getpid()));
		printf("\nSON ENTERING OVERDUE\n");
		while(is_SHORT(getpid()));
		printf("cooloffs overdue son %d == 0\n",remaining_cooloffs(getpid()));
		printf("\nSON EXITING OVERDUE\n");
		while(!is_SHORT(getpid()));	
		printf("cooloffs overdue son %d == 0\n",remaining_cooloffs(getpid()));
		printf("remaining time son1 %d ~~ 2000\n",remaining_time(getpid()));
		printf("\nSON ENTERING OVERDUE\n");
		while(is_SHORT(getpid()));
		printf("remaining time son1 %d ~~ 0\n",remaining_time(getpid()));
		exit(1);
	}
	printf("remaining time father 4 %d\n",remaining_time(father_pid));
	int i=0;
	for(;i<=500000000;i++){
		if (!(i % 100000000)) {
				printf("father\n");
				printf("remaining time father %d\n",remaining_time(father_pid));
		}
	}

}
