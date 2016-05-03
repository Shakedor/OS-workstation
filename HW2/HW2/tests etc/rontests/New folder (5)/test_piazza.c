#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "hw2_syscalls.h"
#include "test_utilities_michael.h"

#define TRUE 1
#define FALSE 0
#define UPPER_DIV(x) (((x)+1)/2)
#define LOWER_DIV(x) ((x)/2)
#define SCHED_SHORT 5
#define DELTA 10
#define MAX_PID_SIZE 32768
#define MAX(a,b) ((a)>(b) ? (a) : (b)) 
#define ASSERT_DELTA_INNER(b, c) ASSERT_LE(b-c, DELTA)
#define ASSERT_DELTA(b,c) do { \
		ASSERT_DELTA_INNER(b,c);\
		ASSERT_DELTA_INNER(c,b);\
	} while (0)
		
typedef struct sched_param {
	int sched_priority;
	int requested_time;
	int number_of_cooloffs;
} sched_param_t;


//https://piazza.com/class/iliabemchdwbj?cid=233
int test_getscheduler(){
	int cooloffs = 5;
	int requested_time = 300;
	sched_param_t params = {0, 300, 5};
	if(fork()==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		ASSERT_EQUALS(sched_getscheduler(getpid()),SCHED_SHORT);
		while(is_SHORT(getpid()));
		ASSERT_EQUALS(sched_getscheduler(getpid()),SCHED_SHORT);
		if(fork() ==0){
			ASSERT_EQUALS(sched_getscheduler(getpid()),SCHED_SHORT);
			ASSERT_EQUALS(is_SHORT(getpid()),0);
			ASSERT_EQUALS(sched_getscheduler(getpid()),SCHED_SHORT);
		} else{
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	return 1;
}

//https://piazza.com/class/iliabemchdwbj?cid=115
//checks that setscheduler only works when it needs to
int test_setscheduler_on_SHORT(){
	int cooloffs = 5;
	int requested_time = 300;
	sched_param_t params = {0, requested_time, cooloffs};
	//you should ignore sched_priority
	sched_param_t params2 = {1, requested_time, cooloffs};
	//we'll mark the current process as A
	if(fork()==0){
		//we'll mark the current process as B
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		errno=0;
		ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params),-1);
		ASSERT_EQUALS(errno,EPERM);
		errno=0;
     	ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params2),-1);
		ASSERT_EQUALS(errno,EPERM);
		errno=0;
		if(fork()==0){
			//we are now the son of a short process
			int forked_cooloffs = (cooloffs+1)>>1;
			ASSERT_EQUALS(is_SHORT(getpid()),1);
			ASSERT_EQUALS(is_SHORT(getppid()),1);
			ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params),-1);
			ASSERT_EQUALS(errno,EPERM);
			errno=0;
			ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params2),-1);
			ASSERT_EQUALS(errno,EPERM);
			errno=0;
			while (is_SHORT(getpid())) ;
			ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params),-1);
			ASSERT_EQUALS(errno,EPERM);
			errno=0;
			ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params2),-1);
			ASSERT_EQUALS(errno,EPERM);
			errno=0;       
	        for (int i = 1; i <= forked_cooloffs; ++i) {
	            while (!is_SHORT(getpid()));
	         	ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params),-1);
				ASSERT_EQUALS(errno,EPERM);
				errno=0;
				ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params2),-1);
				ASSERT_EQUALS(errno,EPERM);
				errno=0;   
	            while (is_SHORT(getpid()));	
	            ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params),-1);
				ASSERT_EQUALS(errno,EPERM);
				errno=0; 
				ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params2),-1);
				ASSERT_EQUALS(errno,EPERM);
				errno=0;  
	        }
	        ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params),-1);
			ASSERT_EQUALS(errno,EPERM);
			errno=0;  
			ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params2),-1);
			ASSERT_EQUALS(errno,EPERM);
			errno=0;
		} else {
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	return 1;
}

int test_247(){
	int cooloffs = 5;
	int requested_time = 300;
	sched_param_t params = {0, 300, 5};
	errno=0;
	if(fork()==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		errno=0;
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		ASSERT_EQUALS((is_SHORT(0) == is_SHORT(getpid()) || is_SHORT(0) == -1),TRUE);
		ASSERT_EQUALS((errno == 0 || errno == EINVAL), TRUE);
		exit(0);
	} else {
		wait(NULL);
	}
	return 1;
}

int test_245(){
	int cooloffs = 5;
	int requested_time = 300;
	sched_param_t params = {0, 300, 5};
	errno=0;
	if(fork()==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		errno=0;
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		while(is_SHORT(getpid()));
		//we are now overdue
		ASSERT_EQUALS(remaining_cooloffs(getpid()),4);
		while(!is_SHORT(getpid()));
		//we are now short again
		ASSERT_EQUALS(remaining_cooloffs(getpid()),4);
		while(is_SHORT(getpid()));
		//we are now overdue again
		ASSERT_EQUALS(remaining_cooloffs(getpid()),3);
		ASSERT_ZERO(sched_setparam(getpid(),&params));
		ASSERT_EQUALS(remaining_cooloffs(getpid()),3);
		exit(0);
	} else {
		wait(NULL);
	}
	return 1;
}

int test_244(){
	int cooloffs = 5;
	int requested_time = 300;
	sched_param_t params = {0, 300, 1};
	errno=0;
	if(fork()==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		errno=0;
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		while(is_SHORT(getpid()));
		//we are now overdue with cooloffs remaining = 0
		ASSERT_EQUALS(remaining_cooloffs(getpid()),0);
		if(fork()==0){
			//son of an overdue process with cooloffs remaining = 0
			//so the son should get remaining_CO = 0
			ASSERT_EQUALS(remaining_cooloffs(getpid()),0);
			ASSERT_EQUALS((remaining_time(getpid())>0),TRUE);
		} else {
			ASSERT_EQUALS(remaining_cooloffs(getpid()),0);
			ASSERT_EQUALS(remaining_time(getpid()),0);
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	return 1;
}

int test_206(){
	int cooloffs = 5;
	int requested_time = 300;
	sched_param_t params = {0, 300, 3};
	sched_param_t bad_params = {0, -1, 3};
	sched_param_t bad_params2 = {0, 300, 5};
	errno=0;
	if(fork()==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		errno=0;
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		ASSERT_EQUALS(sched_setparam(getpid(),&bad_params),-1);
		ASSERT_EQUALS(errno,EINVAL);
		errno=0;
		ASSERT_EQUALS(sched_setparam(getpid(),&bad_params2),0);
		ASSERT_EQUALS(remaining_cooloffs(getpid()),3);
		exit(0);
	} else {
		wait(NULL);
	}
	return 1;
}

int test_175(){
	int cooloffs = 5;
	int requested_time = 300;
	sched_param_t params = {0, 300, 5};
	errno=0;
	if(fork()==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		errno=0;
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		while(is_SHORT(getpid()));
		//we are now overdue
		ASSERT_EQUALS(remaining_cooloffs(getpid()),4);
		while(!is_SHORT(getpid()));
		//we are now short again
		ASSERT_EQUALS(remaining_cooloffs(getpid()),4);
		while(is_SHORT(getpid()));
		//we are now overdue again
		ASSERT_EQUALS(remaining_cooloffs(getpid()),3);
		sched_param_t ret;
		sched_getparam(getpid(),&ret);
		ASSERT_EQUALS(ret.requested_time,300);
		ASSERT_EQUALS(ret.number_of_cooloffs,5);
		exit(0);
	} else {
		wait(NULL);
	}
	return 1;
}

int test_173(){
	int cooloffs = 5;
	int requested_time = 300;
	sched_param_t params = {0, 300, 3};
	errno=0;
	if(fork()==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		errno=0;
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		ASSERT_EQUALS(is_SHORT(-1),-1);
		ASSERT_EQUALS(errno,EINVAL);
		errno=0;
		ASSERT_EQUALS(remaining_time(-1),-1);
		ASSERT_EQUALS(errno,EINVAL);
		errno=0;
		ASSERT_EQUALS(remaining_cooloffs(-1),-1);
		ASSERT_EQUALS(errno,EINVAL);
		errno=0;
		exit(0);
	} else {
		wait(NULL);
	}
	return 1;
}

int single_fork_template(){
	int cooloffs = 5;
	int requested_time = 300;
	sched_param_t params = {0, 300, 3};
	errno=0;
	if(fork()==0){
		exit(0);
	} else {
		wait(NULL);
	}
	return 1;
}

/*
interesting stuff we need to check:

ask ofri and elad about https://piazza.com/class/iliabemchdwbj?cid=194

is yield working properly? some things don't look right
https://piazza.com/class/iliabemchdwbj?cid=172
https://piazza.com/class/iliabemchdwbj?cid=162
https://piazza.com/class/iliabemchdwbj?cid=161
https://piazza.com/class/iliabemchdwbj?cid=160

can this happen?
https://piazza.com/class/iliabemchdwbj?cid=159

is the context switch done right at "needs resched" and in general, do we immediately resched or do we turn on the flag?

when forking a short(not overdue) process, does the son run and the parent goes to the end of the line?
when forking an overdue process, does the son run immediately? (says waits for its turn to run), also, where does the parent go to?

CONFLICTING ANSWERS FOR OVERDUE SHORT:
https://piazza.com/class/iliabemchdwbj?cid=150
https://piazza.com/class/iliabemchdwbj?cid=145

stopped reading at:
https://piazza.com/class/iliabemchdwbj?cid=142
*/
int main(){
	RUN_TEST(test_getscheduler);
	RUN_TEST(test_setscheduler_on_SHORT);
	RUN_TEST(test_247);
	RUN_TEST(test_245);
	RUN_TEST(test_244);
	RUN_TEST(test_206);
	RUN_TEST(test_175);
	RUN_TEST(test_173);
	return 0;
}
