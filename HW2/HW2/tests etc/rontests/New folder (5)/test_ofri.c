#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "hw2_syscalls.h"
#include "test_utilities_michael.h"

#define UPPER_DIV(x) (((x)+1)/2)
#define LOWER_DIV(x) ((x)/2)
#define SCHED_SHORT 5
#define DELTA 10
#define MAX_PID_SIZE 32768 
#define ASSERT_DELTA_INNER(b, c) ASSERT_LE(b-c, DELTA)
#define ASSERT_DELTA(b,c) do { \
		ASSERT_DELTA_INNER(b,c);\
		ASSERT_DELTA_INNER(c,b);\
	} while (0)
		
typedef struct sched_param {
	int sched_priority;
	int requested_time;
	int cooloffs;
} sched_param_t;

int test_fork_functionality(){
	int cooloffs = 5;
	int requested_time = 50;
	sched_param_t params = {0, 200, 5};
	// printf("line: %d\n",__LINE__);
	//check son of not short is not short
	if(fork() == 0){
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		exit(0);
	} else {
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		wait(NULL);
	}
	// printf("line: %d\n",__LINE__);
	//check son of short is short
	if(fork() ==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		if(fork() ==0){
			ASSERT_EQUALS(is_SHORT(getpid()),1);
		} else{
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	// printf("line: %d\n",__LINE__);
	
	//check son of overdue short is overdue short
	if(fork() ==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		while(is_SHORT(getpid()));
		if(fork() ==0){
			ASSERT_EQUALS(is_SHORT(getpid()),0);
		} else{
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	// printf("line: %d\n",__LINE__);
	//check remaining time and cooloffs of son and father
	if(fork() ==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		if(fork() ==0){
			ASSERT_EQUALS(is_SHORT(getpid()),1);
			ASSERT_EQUALS(remaining_cooloffs(getpid()),3);
			ASSERT_DELTA(remaining_time(getpid()),100);
		} else{
			ASSERT_EQUALS(remaining_cooloffs(getpid()),2);
			ASSERT_DELTA(remaining_time(getpid()),100);
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	// printf("line: %d\n",__LINE__);
	//checking fork with 1 tick in time slice left
	if(fork() ==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		while((remaining_time(getpid())*512)/1000 > 1); //wait until have 1 tick left
		if(fork() ==0){
			ASSERT_EQUALS(is_SHORT(getpid()),1);
			ASSERT_EQUALS(remaining_time(getpid()),1);
		} else{
			ASSERT_EQUALS(remaining_time(getpid()),1);
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	
	return 1;
}

int test_fork_order(){
	int cooloffs = 5;
	int requested_time = 50;
	sched_param_t params = {0, 200, 5};
	// printf("line: %d\n",__LINE__);
	//check son of not short is not short
	if(fork() == 0){
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		exit(0);
	} else {
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		wait(NULL);
	}
	// printf("line: %d\n",__LINE__);
	//check son of short is short
	if(fork() ==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		if(fork() ==0){
			ASSERT_EQUALS(is_SHORT(getpid()),1);
		} else{
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	// printf("line: %d\n",__LINE__);
	
	//check son of overdue short is overdue short
	if(fork() ==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		while(is_SHORT(getpid()));
		if(fork() ==0){
			ASSERT_EQUALS(is_SHORT(getpid()),0);
		} else{
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	// printf("line: %d\n",__LINE__);
	//check remaining time and cooloffs of son and father
	if(fork() ==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		if(fork() ==0){
			ASSERT_EQUALS(is_SHORT(getpid()),1);
			ASSERT_EQUALS(remaining_cooloffs(getpid()),3);
			ASSERT_DELTA(remaining_time(getpid()),100);
		} else{
			ASSERT_EQUALS(remaining_cooloffs(getpid()),2);
			ASSERT_DELTA(remaining_time(getpid()),100);
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	// printf("line: %d\n",__LINE__);
	//checking fork with 1 tick in time slice left
	if(fork() ==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		while((remaining_time(getpid())*512)/1000 > 1); //wait until have 1 tick left
		if(fork() ==0){
			ASSERT_EQUALS(is_SHORT(getpid()),1);
			ASSERT_EQUALS(remaining_time(getpid()),1);
		} else{
			ASSERT_EQUALS(remaining_time(getpid()),1);
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	
	return 1;
}
	
int test_fork_functionality_2(){
	int cooloffs = 5;
	int requested_time = 50;
	sched_param_t params = {0, 200, 5};
	// printf("line: %d\n",__LINE__);
	//check son of not short is not short
	if(fork() == 0){
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		exit(0);
	} else {
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		wait(NULL);
	}
	// printf("line: %d\n",__LINE__);
	//check son of short is short
	if(fork() ==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		if(fork() ==0){
			ASSERT_EQUALS(is_SHORT(getpid()),1);
		} else{
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	// printf("line: %d\n",__LINE__);
	
	//check son of overdue short is overdue short
	if(fork() ==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		while(is_SHORT(getpid()));
		if(fork() ==0){
			ASSERT_EQUALS(is_SHORT(getpid()),0);
		} else{
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	// printf("line: %d\n",__LINE__);
	//check remaining time and cooloffs of son and father
	if(fork() ==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		if(fork() ==0){
			ASSERT_EQUALS(is_SHORT(getpid()),1);
			ASSERT_EQUALS(remaining_cooloffs(getpid()),3);
			ASSERT_DELTA(remaining_time(getpid()),100);
		} else{
			ASSERT_EQUALS(remaining_cooloffs(getpid()),2);
			ASSERT_DELTA(remaining_time(getpid()),100);
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	// printf("line: %d\n",__LINE__);
	//checking fork with 1 tick in time slice left
	if(fork() ==0){
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		while((remaining_time(getpid())*512)/1000 > 1); //wait until have 1 tick left
		if(fork() ==0){
			ASSERT_EQUALS(is_SHORT(getpid()),1);
			ASSERT_EQUALS(remaining_time(getpid()),1);
		} else{
			ASSERT_EQUALS(remaining_time(getpid()),1);
			wait(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	
	return 1;
}


#define ASSERT_DELTA(b,c) do { \
		ASSERT_DELTA_INNER(b,c);\
		ASSERT_DELTA_INNER(c,b);\
	} while (0)

		
int test_fork_permitions(){
	//enable permittions logging
	system("sed  \"318s/.*/  echo -n $\\\"OK\\\"/\" /etc/init.d/functions > /etc/init.d/functions1");
	int cooloffs = 5;
	int requested_time = 50;
	sched_param_t params = {0, 2000, 5};
	sched_param_t paramSet;
	int i, j;
	if(fork()==0) {
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params)); //now we are short
		for(i=0;i<300;i++){
			if(fork()==0){
					for(j=0;j<30;j++){
						if(fork()==0){
							int temp = (is_SHORT(getpid()) == 1) ^ (is_SHORT(getpid()) == 0);
							ASSERT_EQUALS(temp,1);
							if(i==4 && j == 3){
								//print_runqueue();
							}
							exit(0);
						} else {
							while(wait(0)>0);
						}
					}
					exit(0);
			}
			else {
				while(wait(0)>0);
			}
		}
		exit(0);
	} else {
		while(wait(0)>0);
	}
	
	//check in the logs for invalid access
	system("sed  \"317s/.*/  \\[ \\\"\\$BOOTUP\\\" = \\\"color\\\" \\] \\&\\& \\$SETCOLOR_SUCCESS/\" /etc/init.d/functions1 > /etc/init.d/functions2");
	//clean log file
	system("rm -f /etc/init.d/functions1");
	//disable logging
	system("mv -f /etc/init.d/functions2 /etc/init.d/functions");
	return 1;
}
		
int test_multiple_forks(){
	int cooloffs = 5;
	int requested_time = 50;
	sched_param_t params = {0, 2000, 5};
	sched_param_t paramSet;
	int i, j;
	if(fork()==0) {
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params)); //now we are short
		for(i=0;i<300;i++){
			if(fork()==0){
					for(j=0;j<30;j++){
						if(fork()==0){
							int temp = (is_SHORT(getpid()) == 1) ^ (is_SHORT(getpid()) == 0);
							ASSERT_EQUALS(temp,1);
							if(i==4 && j == 3){
								//print_runqueue();
							}
							exit(0);
						} else {
							while(wait(0)>0);
						}
					}
					exit(0);
			}
			else {
				while(wait(0)>0);
			}
		}
		exit(0);
	} else {
		while(wait(0)>0);
	}
	return 1;
}


	
int main(){
	RUN_TEST(test_fork_permitions);
	RUN_TEST(test_fork_functionality);
	RUN_TEST(test_fork_functionality_2);
	RUN_TEST(test_fork_order);
	RUN_TEST(test_multiple_forks);
	return 0;
}
