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
/*4 short process with diff prios */ 

 
int main() {
	int res = 0;
	int lp[3] = {0, 0, 0};
	int lp2[3] = {0, 0, 0};
	lp2[0] = 0;
	lp2[1] = 0;
	lp2[2] =0;
	lp[0] = 0;
	lp[1] = 50;
	lp[2] =5;
	sched_setscheduler(getpid(),5,&lp);
	nice(-10);
	int i = 0;
	
if (fork()== 0) {
		nice(1);
		printf("son1 nice 1\n");
		for(;i<=50000000;i++){
			if (!(i % 10000000)) 
				printf("son1\n");
		}
		exit(1);
	} else {
		printf("short\n");
		int pid2 =fork();
		if (pid2 == 0) {
			nice(19);
			i=0;
			printf("son2 nice 19\n");
			for(;i<=50000000;i++){
				if (!(i % 10000000)) 
					printf("son2\n");
			}
			exit(1);
		}else{
		int pid3 =fork();
		if (pid3 == 0) {
			nice(4);
			printf("son3 nice 4\n");
			i = 0;
			for(;i<=50000000;i++){
				if (!(i % 10000000)) 
					printf("son3\n");
			}
			exit(1);
		} else {
			int pid4 =fork();
			if (pid4 == 0) {
			nice(5);
			printf("son4 nice 5\n");
			i = 0;
			for(;i<=50000000;i++){
				if (!(i % 10000000)) 
					printf("son4\n");
			}
			_exit(1);
			}
		}
	}
}
}