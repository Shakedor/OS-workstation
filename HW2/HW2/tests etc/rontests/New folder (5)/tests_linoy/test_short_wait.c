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
 
/*first the father run and activate son1 that start and finsh to run then the 
father start son2,and give the cpu for son2, son2 do yeild and go to the end of 
the que,after the father.
the father run and start son3, that start and finisher (and the father, that gave the cpu, 
go to the end of the que, after son2.
its son2 tor to run and finish and then the father run and start son4
so its working fine */
 
int main() {
	int res = 0;
	int lp[3] = {0, 0, 0};
	int lp2[3] = {0, 0, 0};
	lp2[0] = 0;
	lp2[1] = 0;
	lp2[2] =0;
	lp[0] = 0;
	lp[1] = 2999;
	lp[2] =3;
	sched_setscheduler(getpid(),5,&lp);
	int pid1 =fork();
	
if (pid1 == 0) {
		int i = 0;
		for(;i<=50000000;i++){
			if (!(i % 10000000)) 
				printf("son1\n");
		}
		exit(1);
	} else {
		int pid2 =fork();
		if (pid2 == 0) {
			int i = 0;
			for(;i<=25000000;i++){
				if (!(i % 10000000)) 
				printf("son2\n");
			}
			int pid3 = fork();
			if(pid3==0){
				sched_yield();
				i =0;
				for(;i<=2500000;i++){
					if (!(i % 100000)) 
						printf("son2.1\n");
				}
				exit(1);
			}
			printf("son2 goto wait %d\n",remaining_time(getpid()));
			int a = 0;
			wait(&a);
			printf("son2 back from wait %d\n",remaining_time(getpid()));
			i = 0;
			for(;i<=25000000;i++){
				if (!(i % 10000000)) 
				printf("son2\n");
			}
		exit(1);
		}else{
		int pid3 =fork();
		if (pid3 == 0) {
			int i = 0;
			for(;i<=50000000;i++){
				if (!(i % 10000000)) 
					printf("son3\n");
			}
			exit(1);
		} else {
			int pid4 =fork();
			if (pid4 == 0) {
			int i = 0;
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