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
	printf("\naaa\n");
	int res = 0;
	int lp[3] = {0, 0, 0};
	int lp2[3] = {0, 0, 0};
	lp2[0] = 0;
	lp2[1] = 0;
	lp2[2] =0;
	lp[0] = 0;
	lp[1] = 2999;
	lp[2] =5;
	if(fork()==0){
		printf("OK\n");
		sched_setscheduler(getpid(),5,&lp);
		while(is_SHORT(getpid()));
		if(fork()==0){
			assert(remaining_cooloffs(getpid()) ==2);
			assert(remaining_time(getpid())<2999 &&remaining_time(getpid())>2990);
			printf("OK\n");
			exit(1);
		}
		assert(remaining_cooloffs(getpid()) ==2);
		assert(remaining_time(getpid())<2999 &&remaining_time(getpid())>2990);
		printf("OK\n");
		exit(1);
		
	}
	if(fork()==0){
		sched_setscheduler(getpid(),5,&lp);
		if(fork()==0){
			assert(remaining_cooloffs(getpid()) ==3);
			assert(remaining_time(getpid())<=1500 &&remaining_time(getpid())>1490);
			while(is_SHORT(getpid()));
			assert(remaining_cooloffs(getpid()) ==2);
			assert(remaining_time(getpid())<=2999 &&remaining_time(getpid())>2990);
			while(!is_SHORT(getpid()));
			assert(remaining_cooloffs(getpid()) ==2);
			assert(remaining_time(getpid())<2999 &&remaining_time(getpid())>2990);
			printf("OK\n");
			exit(1);
		}
		assert(remaining_cooloffs(getpid()) ==2);
		assert(remaining_time(getpid())<1500 &&remaining_time(getpid())>1490);
		while(is_SHORT(getpid()));
		assert(remaining_cooloffs(getpid()) ==1);
		assert(remaining_time(getpid())<2999 &&remaining_time(getpid())>2990);
		while(!is_SHORT(getpid()));
		assert(remaining_cooloffs(getpid()) ==1);
		assert(remaining_time(getpid())<2999 &&remaining_time(getpid())>2990);
		printf("OK\n");
		exit(1);
		
	}
	if(fork()==0){
		lp[0] = 0;
		lp[1] = 1000;
		lp[2] =1;
		printf("OK\n");
		sched_setscheduler(getpid(),5,&lp);
		while(is_SHORT(getpid()));
		if(fork()==0){
			assert(remaining_cooloffs(getpid()) ==0);
			assert(remaining_time(getpid())<=1000 &&remaining_time(getpid())>=990);
			while(!is_SHORT(getpid()));
			assert(remaining_cooloffs(getpid()) ==0);
			assert(remaining_time(getpid())<=1000 && remaining_time(getpid())>=990);
			printf("OK-son\n");
			exit(1);
		}
		assert(remaining_cooloffs(getpid()) ==0);
		assert(remaining_time(getpid()) ==0);
		printf("errno%d\n",errno);
		printf("OK-father\n");
		exit(1);
		
	}
	
}