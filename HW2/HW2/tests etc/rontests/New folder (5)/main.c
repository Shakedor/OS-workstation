/*
 * main.c
 *
 *  Created on: Mar 27, 2016
 *      Author: root
 */
#include <sched.h>
#include "test_utilities_valka.h"
#include <assert.h>
#include "hw2_syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <sys/wait.h>
#include <asm/errno.h>
#include <sys/resource.h>

#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

void print_log(int PID){
	switch_t log[SCHED_SAVES_LIMIT];
	int n;
	int i;

	n = get_scheduling_statistic(log);
	printf("pid:%d - THERE ARE %d STATISTICS - errno = %d\n", PID, n, errno);
	for(i=0; i<n; ++i){
		int is_to_pid =  (log[i].next_pid == PID);
		int is_from_pid =  (log[i].previous_pid== PID);
		int is_not_pid =  (log[i].previous_pid != PID) && (log[i].next_pid != PID);

		// int is_overdue = (log[i].previous_type == BECAME_OVERDUE);
		// int is_overdue_to_short = (log[i].reason == OVERDUE_TO_SHORT);

		printf("%s", (is_to_pid ? GREEN : ""));
		printf("%s", (is_from_pid ? YELLOW : ""));
		// printf("%s", (is_overdue ? RED : ""));
		// printf("%s", (is_overdue_to_short ? BLUE : ""));
		printf("%s", (is_not_pid ? WHITE : ""));

		printf("%10d  |  ", log[i].time);
		printf("%5d - >%5d  |  ", log[i].previous_pid, log[i].next_pid);
		printf("%15s -> %15s  |  ", typeE2S(log[i].previous_type), typeE2S(log[i].next_type));
		printf("%s\n", swRsnE2S(log[i].reason));

	}
/*if (prev->reason == YEILDS) rq->switchInfo[csIndex].reason = YEILDS;
			else if (overdue_task(prev) && short_task(next)) rq->switchInfo[csIndex].reason = OVERDUE_TO_SHORT;
			else if (short_task(prev) && !short_task(next) && !rt_task(next)) rq->switchInfo[csIndex].reason = HIGHER_PRIORITY;
			else if (short_task(prev) && !short_task(next) && rt_task(next)) rq->switchInfo[csIndex].reason = BECAME_OVERDUE;
			else if (next->reason == CREATED) rq->switchInfo[csIndex].reason = CREATED;
			else if (prev->prio < next->prio) rq->switchInfo[csIndex].reason = HIGHER_PRIORITY;
			else if (prev->time_slice == 0) rq->switchInfo[csIndex].reason = TIMESLICE_ENDED;
			else rq->switchInfo[csIndex].reason = prev->reason;*/
}

/*you have to run flush log before forking !
and hope that the log is long enough to contail the overdues*/
int count_overdues_for_PID_in_logger(int PID){
	switch_t log[SCHED_SAVES_LIMIT];
	int n = get_scheduling_statistic(log);

	int count = 0;

	for(int i=0; i<n; ++i){
		if((log[i].previous_pid == PID) && (log[i].reason == BECAME_OVERDUE)){
			++count;
		}
	}

	return count;
}


/* TODO !

1.	It should be impossible for any user (or root) to change the number of cool off
	cycles of any SHORT-process to a different value than initial.

2.	It is allowed to change the requested time for a SHORT process,using
	sched_setparam(), the effect will take place only if the process gets a new
	time slice, e.g. let’s say we have a SHORT process with requested time 2000ms and
	2 cool off cycles. Assuming this process is asking for new requested time of
	3000ms, it will finish the 2000ms time slice, 2000ms cool off period and then get a
	3000ms time slice and so on.


*/

/******************************* SYS_CALLS ********************************
In case of an unsuccessful call, wrappers should return -1 and update errno
accordingly, like any other system call. In case the process is not a SHORT nor
Overdue-SHORT-process update errno to EINVAL.*/

/*The wrapper will return 1 if the given process is a SHORT-process, or 0 if it is
already overdue*/

bool TEST_SYS_SET_SCHEDULER() {
	int param[3] = {0, 500, 5};
	int invalid_prio[3] = {-1, 100, 5};
	int invalid_requested_time_min[3] = {0, 0, 5};
	int invalid_requested_time_max[3] = {0, 3001, 5};
	int invalid_cooloffs_min[3] = {0, 100, -1};
	int invalid_cooloffs_max[3] = {0, 100, 6};

	ASSERT_TEST(sched_setscheduler(-1, 5, param)==-1); //Ivalid pid
	ASSERT_TEST(errno == EINVAL); //Ivalid pid

	for(int n=0; n<20; ++n){
		int pid = fork();
		if(pid == 0){
			for(int i=0; i<10000000; ++i){}
			_exit(EXIT_SUCCESS);
		} else {
			errno=0;
			ASSERT_TEST(sched_setscheduler(pid, 5, invalid_prio)==-1);
			ASSERT_TEST(errno == EINVAL);
			errno=0;
			ASSERT_TEST(sched_setscheduler(pid, 5, invalid_requested_time_min)==-1);
			ASSERT_TEST(errno == EINVAL);
			errno=0;
			ASSERT_TEST(sched_setscheduler(pid, 5, invalid_requested_time_max)==-1);
			ASSERT_TEST(errno == EINVAL);
			errno=0;
			ASSERT_TEST(sched_setscheduler(pid, 5, invalid_cooloffs_max)==-1);
			ASSERT_TEST(errno == EINVAL);
			errno=0;
			ASSERT_TEST(sched_setscheduler(pid, 5, param)==0); //making pid SHORT
			//printf("\n is_SHORT(%d) returns %d\n",pid,is_SHORT(pid));
			//printf("\n number_of_cooloffs: %d\n", remaining_cooloffs(pid));
			ASSERT_TEST(is_SHORT(pid)==1);
			errno=0;
			ASSERT_TEST(sched_setscheduler(pid, 5, param)==-1); //but can't make it twice
			ASSERT_TEST(errno == EPERM);
		}
	}
 	return true;
}

bool TEST_SYS_IS_SHORT() {
	int cooloffs = 5;
	int param[3] = {0, 100, cooloffs};
	int pid = fork();

	if(pid == 0){
		for(int i=0; i<10000000; ++i){}
		for(int i=0; i<10000000; ++i){}
		for(int i=0; i<10000000; ++i){}
		for(int i=0; i<10000000; ++i){}
		for(int i=0; i<10000000; ++i){}
		_exit(EXIT_SUCCESS);
	} else {
		errno = 0;
		ASSERT_TEST(is_SHORT(pid)==-1);
		ASSERT_TEST(errno = EINVAL);
		sched_setscheduler(pid, 5, param);
		ASSERT_TEST(is_SHORT(pid) == 1);

	// int is_overdue = 1;
	// int shorts_counter = 0;
	// /*nasty trick to count cooloffs*/
	// while(1){//while child exists
	// 	int changed_state_to_short = is_overdue && (is_SHORT(pid)==1);
	// 	if(changed_state_to_short){
	// 		is_overdue = 0;
	// 		++shorts_counter;
	// 	}
	// 	if(is_SHORT(pid) == 0){
	// 		is_overdue = 1;
	// 	}
	// }
	// ASSERT_TEST(shorts_counter == (cooloffs + 1));
	}
 	return true;
}

/*The wrapper will return the time left for the process at the current time slice.
For an overdue process, it should return the reminded cool off time.*/
bool TEST_SYS_REMAINING_TIME() {
	errno = 0;
	ASSERT_TEST(remaining_time(-1)==-1);
	ASSERT_TEST(errno == EINVAL);

	int cooloffs = 5;
	int requested_time = 500;

	int param[3] = {0, requested_time, cooloffs};
	int pid = fork();

	if(pid == 0){
		for(int i=0; i<10000000; ++i){}
		for(int i=0; i<10000000; ++i){}
		for(int i=0; i<10000000; ++i){}
		_exit(EXIT_SUCCESS);
	} else {
		sched_setscheduler(pid,5,param);
		//should be equal +- when going to overdue
		//printf("\n remaining_time(%d) returns %d\n",pid,remaining_time(pid));
		//remaining_time(pid);
		//ASSERT_TEST(remaining_time(pid)==requested_time);

		for(int i=0; i<10000; ++i){
			int child_time = remaining_time(pid);
			/*trying my best..*/
			ASSERT_TEST((0 <= child_time) && (child_time <= requested_time));
			if(i%5==0) sched_yield();
		}
	}
 	return true;
}

/*The wrapper will return the number of cool off periods left for the SHORT
process. Whenever a cool off period has started it is no longer consider as a
remaining cool off period. For example, let p be a SHORT process with
requested time 100ms and 1 cool off period. After running for 100ms, it will
go to a cool off period, during that time remaining_cooloffs should return 0.*/
/*bool TEST_SYS_REMAINING_COOLOFFS() {
	errno = 0;
	ASSERT_TEST(remaining_cooloffs(-1)==-1);
	ASSERT_TEST(errno == EINVAL);

    int i=0;
	int j=0;
	int cooloffs = 5;
	int requested_time = 1000;
	int param[3] = {0, requested_time, cooloffs};
	int pid = fork();*/

	// if(pid == 0){
        // // while(remaining_cooloffs(getpid())){
        // while(1){
            // if (is_SHORT(getpid()) == 0) {
                // printf("remaining_cooloffs(pid)=%d\n", remaining_cooloffs(getpid()));
            // }
			// sched_yield();
			// for(j=0; j<9000000; ++j);
			// ++i;
			// if(i == 20) return 0;
		// }
		// // for(int i=0; i<10000000; ++i){}
		// // _exit(EXIT_SUCCESS);
        // // return 0;
	// } else {
		// errno = 0;
		// ASSERT_TEST(remaining_cooloffs(pid)==-1);
		// ASSERT_TEST(errno == EINVAL);
		// sched_setscheduler(pid, 5, param);
		// // ASSERT_TEST(remaining_cooloffs(pid)==cooloffs);
	// }
    
    /*if(pid == 0){
		while(1){
//			printf("remaining time is %d\n", remaining_time(getpid()));
			printf("is_SHORT(%d)= %d - timeslice:%d\n", getpid(), is_SHORT(getpid()), remaining_time(getpid()));
			for(j=0; j<9000000; ++j);
			++i;
			if(i == 20) return 0;
		}
	} else {
		int param[3] = {0, 100, 5};
		sched_setscheduler(pid, 5, param);
	}
 	return true;
}*/

bool TEST_SYS_NICE() {
    errno = 0;
    // in order to check the user (not root) can't lower NICE, run this check. runs as a user called user and not root
    // su user -c './asd'
    // printf("nice = %d\n", nice(-1));
    // printf("errno = %d\n", errno);
	// ASSERT_TEST(nice(-1)==-1);
	// ASSERT_TEST(errno == EPERM);

    int which = PRIO_PROCESS;
    int nice_val;
    int new_nice;
	int cooloffs = 5;
	int requested_time = 10;
	int param[3] = {0, requested_time, cooloffs};
	int pid = fork();

	if(pid == 0){
		for(int i=0; i<10000000; ++i){}
		_exit(EXIT_SUCCESS);
	} else {
		errno = 0;
		// ASSERT_TEST(nice(-1)==-1);
        // ASSERT_TEST(errno == EPERM);
		// sched_setscheduler(pid, 5, param);
        // ASSERT_TEST(nice(-1)==-1);
        // ASSERT_TEST(errno == EPERM);
		
        nice_val = getpriority(which, 0);
        // printf("nice_val = %d\n", nice_val);
        // printf("nice = %d\n", nice(15));
        
        // ASSERT_TEST(nice(10)==0);
        // printf("prio = %d\n", getpriority(which, 0));
        ASSERT_TEST(nice(nice_val+10) == getpriority(which, 0));
        new_nice = getpriority(which, 0);
        ASSERT_TEST(nice(new_nice+10) == getpriority(which, 0));
	}
    
 	return true;
}

bool TEST_SYS_GET_PARAM() {
    errno = 0;

	int cooloffs = 5;
	int requested_time = 10;
	int param[3] = {0, requested_time, cooloffs};
	int pid = fork();

	if(pid == 0){
		for(int i=0; i<10000000; ++i){}
		_exit(EXIT_SUCCESS);
	} else {
		errno = 0;
		sched_setscheduler(pid, 5, param);
        ASSERT_TEST(sched_getparam(-1, &param)==-1);
        ASSERT_TEST(errno == EINVAL);
        
        ASSERT_TEST(sched_getparam(pid, &param)==0);
        ASSERT_TEST(param[0]==0);
        //printf("param[1] = %d\n", param[1]);
        ASSERT_TEST(param[1]==requested_time);
        ASSERT_TEST(param[2]==cooloffs);
	}
    
 	return true;
}

bool TEST_SYS_SET_PARAM() {
	int cooloffs = 5;
	int requested_time = 10;
	int param[3] = {0, requested_time, cooloffs};
	int change_sched_prio[3] = {1, requested_time, cooloffs};
	int change_cooloffs[3] = {0, requested_time, cooloffs-1};
	int change_requested_time[3] = {0, requested_time+10, cooloffs};
	int pid = fork();

	if(pid == 0){
		for(int i=0; i<10000000; ++i){}
		_exit(EXIT_SUCCESS);
	} else {
		errno = 0;
		sched_setscheduler(pid, 5, param);
        ASSERT_TEST(sched_setparam(-1, &param)==-1);
        ASSERT_TEST(errno == EINVAL);
        errno = 0;
        // errno = 0;
        // ASSERT_TEST(sched_setparam(pid, &change_cooloffs)==-1);
        // ASSERT_TEST(errno == EINVAL);
        
        ASSERT_TEST(sched_setparam(pid, &change_requested_time)==0);
        ASSERT_TEST(sched_getparam(pid, &param)==0);
        ASSERT_TEST(param[0]==0);
        ASSERT_TEST(param[1]==(requested_time+10));
        ASSERT_TEST(param[2]==cooloffs);
	}
    
 	return true;
}

bool TEST_GET_SCHEDULER() {
    errno = 0;
    ASSERT_TEST(sched_getscheduler(-1)==-1);
    ASSERT_TEST(errno == EINVAL);
    
    int cooloffs = 5;
	int requested_time = 10;
	int param[3] = {0, requested_time, cooloffs};
	int pid = fork();

	if(pid == 0){
		for(int i=0; i<10000000; ++i){}
		_exit(EXIT_SUCCESS);
	} else {
		errno = 0;
		sched_setscheduler(pid, 5, param);
        ASSERT_TEST(sched_getscheduler(-1)==-1);
        ASSERT_TEST(errno == EINVAL);
        
        ASSERT_TEST(sched_getscheduler(pid)==5);
	}
    
 	return true;
}
/*******************************************************************************/






/*A real time process returned from waiting and is ready to run.*/
bool TEST_NEW_REALTIME_REPLACES_SHORT() {
 	return true;
}

/*Another SHORT-process returned from waiting or from a cool off period, and
it has higher priority.*/
bool TEST_SHORT_WITH_HI_PRIO_RETURNS() {
 	return true;
}

/*The SHORT-process goes out for waiting.
how to test that ? maybe with printf's ?*/
bool TEST_SHORT_GOES_OUT_FOR_WAIT() {
 	return true;
}

bool TEST_SHORT_BECOMES_OVERDUE() {
 	return true;
}

bool TEST_SHORT_TO_SHORT() {
 	return true;
}

bool TEST_NICE_CHANGES() {
 	return true;
}

/*In any case that a SHORT-process has left the CPU without finishing its time slice
you should remember the remained part, the process will use it in the next time it is
chosen to run.*/
bool TEST_TIMESLICE_BEFORE_AFTER() {
 	return true;
}

/*The SHORT-process yields the CPU.*/
bool TEST_SHORT_YIELD_FROM_OVERDUE() {
 	return true;
}

/*The SHORT-process yields the CPU.*/
bool TEST_SHORT_YIELD_FROM_SHORT() {
 	return true;
}

bool TEST_CREATE_SHORT_NEGATIVE_NICE() {
 	return true;
}

/*The SHORT-process forked, and created a child (see explanation in the next
section).*/
bool TEST_SHORT_FORK_SHORT() {
 	return true;
}

/*The SHORT-process forked, and created a child (see explanation in the next
section).*/
bool TEST_SHORT_FORK_OVERDUE() {
 	return true;
}

bool TEST_NO_SHORT_WITH_OVERDUE_ON() {
 	return true;
}

bool TEST_CREATE_NEGATIVE_NICE() {
 	return true;
}

bool TEST_STRESS_OTHERS_ONLY() {
 	return true;
}

bool TEST_STRESS_SHORTS() {
 	return true;
}

bool TEST_OVERDUE_TO_SHORT() {
 	return true;
}

bool TEST_STRESS_SHORTS_ZERO_COOLOFF() {
 	return true;
}


//bool VALKA_TEST(){
//	int i=0;
//	int j=0;
//	flush_scheduling_statistic();
//	print_log(1,1,1,-1);
//	int pid = fork();
//	if(pid == 0){
//		while(1){
////			printf("remaining time is %d\n", remaining_time(getpid()));
////			printf("is_SHORT(%d)= %d - timeslice:%d\n", getpid(), is_SHORT(getpid()), remaining_time(getpid()));
//			for(j=0; j<9000000; ++j);
//			++i;
//			if(i == 20) return 0;
//		}
//	} else {
//		int param[3] = {0, 100, 5};
//		sched_setscheduler(pid, 5, param);
//	}
//
//
//	wait(0);
//	print_log(1,1,1,pid);
//
//	return true;
//}

// bool VALKA_TEST2(){
	// int i=0;
	// int j=0;
	// flush_scheduling_statistic();
	// int pid = fork();
	// if(pid == 0){
		// while(1){
			// for(j=0; j<9000000; ++j);
			// ++i;
			// if(i == 20) _exit(EXIT_SUCCESS);
		// }
	// } else {
		// int cooloffs = 5;
		// int requested_time = 25;
		// int param[3] = {0, requested_time, cooloffs};
		// sched_setscheduler(pid, 5, param);
	// }

	// wait(0);
	// print_log(pid);

	// return true;
// }

int main(){
	nice(-40);
	RUN_TEST(TEST_SYS_SET_SCHEDULER);
	RUN_TEST(TEST_SYS_IS_SHORT);
	RUN_TEST(TEST_SYS_REMAINING_TIME);
	//RUN_TEST(TEST_SYS_REMAINING_COOLOFFS);
	RUN_TEST(TEST_SYS_GET_PARAM);
    RUN_TEST(TEST_SYS_SET_PARAM);
	RUN_TEST(TEST_GET_SCHEDULER);
	RUN_TEST(TEST_SYS_NICE);
//	RUN_TEST(TEST_NEW_REALTIME_REPLACES_SHORT);
//	RUN_TEST(TEST_SHORT_WITH_HI_PRIO_RETURNS);
//	RUN_TEST(TEST_SHORT_GOES_OUT_FOR_WAIT);
//	RUN_TEST(TEST_SHORT_BECOMES_OVERDUE);
//	RUN_TEST(TEST_SHORT_TO_SHORT);
//	RUN_TEST(TEST_NICE_CHANGES);
//	RUN_TEST(TEST_TIMESLICE_BEFORE_AFTER);
//	RUN_TEST(TEST_SHORT_YIELD_FROM_OVERDUE);
//	RUN_TEST(TEST_SHORT_YIELD_FROM_SHORT);
//	RUN_TEST(TEST_CREATE_SHORT_NEGATIVE_NICE);
//	RUN_TEST(TEST_SHORT_FORK_SHORT);
//	RUN_TEST(TEST_SHORT_FORK_OVERDUE);
//	RUN_TEST(TEST_NO_SHORT_WITH_OVERDUE_ON);
//	RUN_TEST(TEST_STRESS_OTHERS_ONLY);
//	RUN_TEST(TEST_STRESS_SHORTS);
//	RUN_TEST(TEST_OVERDUE_TO_SHORT);
//	RUN_TEST(TEST_STRESS_SHORTS_ZERO_COOLOFF);
//	RUN_TEST(VALKA_TEST);
	// RUN_TEST(VALKA_TEST2);
	return 0;
}
