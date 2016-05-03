#include  <stdio.h>
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


int isShortTest() {
	ASSERT_EQUALS(is_SHORT(getpid()),-1);
	ASSERT_EQUALS(errno, EINVAL);
	int cooloffs = 5;
	int requested_time = 50;
	sched_param_t params = {1, requested_time, cooloffs};
	
	if(fork() == 0){
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()),1);
		while(is_SHORT(getpid()) || remaining_cooloffs(getpid()));
		ASSERT_EQUALS(is_SHORT(getpid()),0);
		exit(0);
	} else {
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		wait(NULL);
	}
	ASSERT_EQUALS(is_SHORT(getpid()),-1);
	ASSERT_EQUALS(errno, EINVAL);
	return 1;
}

int sysCallsErrorTest() {
	int invalidPID = getpid() + 1;
	
	ASSERT_EQUALS(is_SHORT(getpid()),-1);
	ASSERT_EQUALS(errno, EINVAL);
	ASSERT_EQUALS(is_SHORT(invalidPID),-1);
	ASSERT_EQUALS(errno, EINVAL);
	
	ASSERT_EQUALS(remaining_time(getpid()),-1);
	ASSERT_EQUALS(errno, EINVAL);
	ASSERT_EQUALS(remaining_time(invalidPID),-1);
	ASSERT_EQUALS(errno, EINVAL);
	
	ASSERT_EQUALS(remaining_cooloffs(getpid()),-1);
	ASSERT_EQUALS(errno, EINVAL);
	ASSERT_EQUALS(remaining_cooloffs(invalidPID),-1);
	ASSERT_EQUALS(errno, EINVAL);
	
	return 1;
}

int generalSysCallsTest() {
	ASSERT_EQUALS(is_SHORT(getpid()),-1);
	ASSERT_EQUALS(errno, EINVAL);
	if(fork()==0){
		int cooloffs = 5;
		int requested_time = 50;
		sched_param_t params = {1, requested_time, cooloffs};
		sched_param_t resParamChild, resParamParent;
		sched_param_t paramSet;
		int time;
		
		
		params.requested_time = 1000;
		params.cooloffs = 4;
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()), 1);
		paramSet.requested_time = 500;
		paramSet.cooloffs = 0; // should be ignored
		paramSet.sched_priority = 1;
		ASSERT_ZERO(sched_setparam(getpid(), &paramSet));
		time = remaining_time(getpid());
		ASSERT_DELTA(time, 1000);
		
		if(fork() != 0) {
			// short
			ASSERT_EQUALS(is_SHORT(getpid()), 1);
			ASSERT_EQUALS(remaining_cooloffs(getpid()),2);
			time = remaining_time(getpid());
			ASSERT_DELTA(time, 500);
			ASSERT_ZERO(sched_getparam(getpid(), &resParamParent));
			ASSERT_EQUALS(resParamParent.requested_time, 500);
			ASSERT_EQUALS(resParamParent.cooloffs, 4);
			
			while(is_SHORT(getpid()));
			// overdue
			time = remaining_time(getpid());
			ASSERT_DELTA(time, 1000);
			ASSERT_EQUALS(remaining_cooloffs(getpid()),1);
			ASSERT_ZERO(sched_getparam(getpid(), &resParamParent));
			ASSERT_EQUALS(is_SHORT(getpid()), 0);
			ASSERT_EQUALS(resParamParent.requested_time, 500);
			ASSERT_EQUALS(resParamParent.cooloffs, 4);
			
			while(!is_SHORT(getpid()));
			// short
			ASSERT_EQUALS(is_SHORT(getpid()), 1);
			time = remaining_time(getpid());
			ASSERT_DELTA(time, 500);
			ASSERT_EQUALS(remaining_cooloffs(getpid()),1);
			ASSERT_ZERO(sched_getparam(getpid(), &resParamParent));
			ASSERT_EQUALS(resParamParent.requested_time, 500);
			ASSERT_EQUALS(resParamParent.cooloffs, 4);
			
			wait(NULL);
		} else {
			// short
			ASSERT_EQUALS(is_SHORT(getpid()), 1);
			ASSERT_EQUALS(remaining_cooloffs(getpid()),2);
			time = remaining_time(getpid());
			ASSERT_DELTA(time, 500);
			ASSERT_ZERO(sched_getparam(getpid(), &resParamChild));
			ASSERT_EQUALS(resParamChild.requested_time, 500);
			ASSERT_EQUALS(resParamChild.cooloffs, 4);
			
			while(is_SHORT(getpid()));
			// overdue
			time = remaining_time(getpid());
			ASSERT_DELTA(time, 1000);
			ASSERT_EQUALS(remaining_cooloffs(getpid()),1);
			ASSERT_EQUALS(is_SHORT(getpid()), 0);
			ASSERT_ZERO(sched_getparam(getpid(), &resParamChild));
			ASSERT_EQUALS(resParamChild.requested_time, 500);
			ASSERT_EQUALS(resParamChild.cooloffs, 4);
			
			while(!is_SHORT(getpid()));
			// short
			ASSERT_EQUALS(is_SHORT(getpid()), 1);
			time = remaining_time(getpid());
			ASSERT_DELTA(time, 500);
			ASSERT_EQUALS(remaining_cooloffs(getpid()),1);
			ASSERT_EQUALS(resParamChild.requested_time, 500);
			ASSERT_EQUALS(resParamChild.cooloffs, 4);
			
			exit(0);
		}
		
		
		exit(0);
	} else {
		wait(NULL);
	}
	return 1;
}

int getSetParamTest() {
	ASSERT_EQUALS(is_SHORT(getpid()),-1);
	ASSERT_EQUALS(errno, EINVAL);
	if(fork() == 0){
		int cooloffs = 5;
		int requested_time = 50;
		sched_param_t params = {1, requested_time, cooloffs};
		sched_param_t resParamChild, resParamParent;
		sched_param_t paramSet;
		
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params), -1);
		ASSERT_EQUALS(errno, EPERM);
		ASSERT_EQUALS(is_SHORT(getpid()), 1);
		
		if(fork() != 0){
			ASSERT_EQUALS(is_SHORT(getpid()), 1);
			while(remaining_cooloffs(getpid())!=1);
			ASSERT_ZERO(sched_getparam(getpid(), &resParamParent));
			ASSERT_EQUALS(resParamParent.cooloffs, cooloffs);
			ASSERT_EQUALS(resParamParent.requested_time, requested_time);
			ASSERT_EQUALS(resParamParent.sched_priority, 1);
			wait(NULL);
		} else {
			ASSERT_EQUALS(is_SHORT(getpid()), 1);
			while(remaining_cooloffs(getpid())!=1);	
			ASSERT_ZERO(sched_getparam(getpid(), &resParamChild));
			ASSERT_EQUALS(resParamChild.cooloffs, cooloffs);
			ASSERT_EQUALS(resParamChild.requested_time, requested_time);
			ASSERT_EQUALS(resParamChild.sched_priority, 1);
			exit(0);
		}
		
		while(is_SHORT(getpid()));
		ASSERT_ZERO(sched_getparam(getpid(), &resParamParent));
		ASSERT_EQUALS(resParamParent.cooloffs, cooloffs);
		ASSERT_EQUALS(resParamParent.requested_time, requested_time);
		ASSERT_EQUALS(resParamParent.sched_priority, 1);
		
		// setparam check
		paramSet.sched_priority = 0;
		paramSet.requested_time = 3500;
		while(remaining_cooloffs(getpid()));
		ASSERT_ZERO(remaining_cooloffs(getpid()));
		paramSet.cooloffs = 0;
		ASSERT_EQUALS(sched_setparam(getpid(), &paramSet), -1);
		ASSERT_EQUALS(errno, EINVAL);
		
		paramSet.requested_time = 500;
		paramSet.cooloffs = 6; // should be ignored
		ASSERT_ZERO(sched_setparam(getpid(), &paramSet));
		
		ASSERT_ZERO(sched_getparam(getpid(), &resParamParent));
		ASSERT_EQUALS(resParamParent.cooloffs, cooloffs); // original cooloffs
		ASSERT_EQUALS(resParamParent.requested_time, paramSet.requested_time);
		ASSERT_EQUALS(resParamParent.sched_priority, paramSet.sched_priority);
		exit(0);
	} else {
		wait(NULL);
	}
	
	return 1;
}

int setschedulerTest() {
	if(fork() == 0){
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		int cooloffs = -1;
		int requested_time = 50;
		sched_param_t params = {0, requested_time, cooloffs};
		sched_param_t resParamChild, resParamParent;
		sched_param_t paramSet;
		int i;
		
		ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params), -1);
		ASSERT_EQUALS(errno, EINVAL);
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		
		params.cooloffs = 10;
		ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params), -1);
		ASSERT_EQUALS(errno, EINVAL);
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		
		params.cooloffs = 5;
		params.requested_time = -1;
		ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params), -1);
		ASSERT_EQUALS(errno, EINVAL);
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		
		params.requested_time = 3001;
		ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params), -1);
		ASSERT_EQUALS(errno, EINVAL);
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		
		params.cooloffs = 5;
		params.requested_time = 100;
		params.sched_priority = -5;
		ASSERT_EQUALS(sched_setscheduler(getpid(), SCHED_SHORT, &params), -1);
		ASSERT_EQUALS(errno, EINVAL);
		ASSERT_EQUALS(is_SHORT(getpid()),-1);
		ASSERT_EQUALS(errno, EINVAL);
		
		params.requested_time = 1000;
		params.cooloffs = 2;
		params.sched_priority = 0;
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params));
		ASSERT_EQUALS(is_SHORT(getpid()), 1);
		paramSet.requested_time = 500;
		paramSet.cooloffs = 0; // should be ignored
		paramSet.sched_priority = 1;
		ASSERT_ZERO(sched_setparam(getpid(), &paramSet));
		i = 100;
		while(--i);
		
		if(fork() != 0) {
			while(is_SHORT(getpid()));
			ASSERT_ZERO(sched_getparam(getpid(), &resParamParent));
			ASSERT_EQUALS(resParamParent.cooloffs, params.cooloffs ); // original cooloffs
			ASSERT_EQUALS(is_SHORT(getpid()), 0);
			ASSERT_EQUALS(resParamParent.requested_time, paramSet.requested_time);
			ASSERT_EQUALS(resParamParent.sched_priority, paramSet.sched_priority);
			
			wait(NULL);
		} else {
			while(is_SHORT(getpid()));
			ASSERT_ZERO(sched_getparam(getpid(), &resParamParent));
			ASSERT_EQUALS(resParamParent.cooloffs, params.cooloffs ); // original cooloffs
			ASSERT_EQUALS(is_SHORT(getpid()), 0);
			ASSERT_EQUALS(resParamParent.requested_time, paramSet.requested_time);
			ASSERT_EQUALS(resParamParent.sched_priority, paramSet.sched_priority);
			
			exit(0);
		}
		exit(0);
	} else {
		wait(NULL);
	}
	return 1;
}

void doNothing(){
	return;
}

int infiniteLoopTest(){
	while(1){
		doNothing();
		doSomething();
	}
	return 1;
}

int main(){
	RUN_TEST(sysCallsErrorTest);
	RUN_TEST(isShortTest);
	RUN_TEST(generalSysCallsTest);
	RUN_TEST(getSetParamTest);
	RUN_TEST(setschedulerTest);
	RUN_TEST(infiniteLoopTest);
	
	return 0;
}
