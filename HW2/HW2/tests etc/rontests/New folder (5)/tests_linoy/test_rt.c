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
	int pid1 =fork();
	
if (pid1 == 0) {
		printf("other\n");
		int i = 0;
		for(;i<=50000000;i++){
			if (!(i % 10000000)) 
				printf("son1\n");
		}
		exit(1);
	} else {
		int pid2 =fork();
		if (pid2 == 0) {
			lp[0] = 90;
			lp[1] = 0;
			lp[2] =0;
			sched_setscheduler(getpid(),1,&lp);
			printf("rt\n");
			int i = 0;
			if(fork()==0){
				printf("son2 goto sleep\n");
				usleep(50000);
				exit(1);
			}else{
				int a = 0;
				wait(&a);
					for(;i<=50000000;i++){
					if (!(i % 10000000)) 
						printf("son2\n");
				}
				exit(1);
			}
		}else{
		int pid3 =fork();
		if (pid3 == 0) {
			lp[0] = 0;
			lp[1] = 1000;
			lp[2] =3;
			sched_setscheduler(getpid(),5,&lp);
			if(errno){
				printf("errno2 %d\n",errno);
			}
			printf("short\n");
			int i = 0;
			for(;i<=50000000;i++){
				if (!(i % 10000000)) 
					printf("son3\n");
			}
			exit(1);
		} else {
			int pid4 =fork();
			if (pid4 == 0) {
			lp[0] = 0;
			lp[1] = 1000;
			lp[2] =3;
			sched_setscheduler(getpid(),5,&lp);
			printf("short\n");
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