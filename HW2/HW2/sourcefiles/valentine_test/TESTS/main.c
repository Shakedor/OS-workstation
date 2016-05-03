/*
 * main.c
 *
 *  Created on: Mar 27, 2016
 *      Author: root
 */
#include <sched.h>
#include "test_utilities.h"
#include <assert.h>
#include "short_sys.h"
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
		int is_to_short_pid =  log[i].next_type == SHORT_NOT_OVERDUE && log[i].next_pid == PID;
		int is_to_short_notpid =  log[i].next_type == SHORT_NOT_OVERDUE && log[i].next_pid != PID;
		int became_overdue_pid = (log[i].reason == BECAME_OVERDUE && log[i].previous_pid == PID);
		int became_overdue_notpid = (log[i].reason == BECAME_OVERDUE && log[i].previous_pid != PID);

		printf("%s", (is_to_pid ? BOLDGREEN : ""));
		printf("%s", (is_from_pid ? BOLDYELLOW : ""));
		printf("%s", (became_overdue_pid ? BOLDRED : ""));
		printf("%s", (is_to_short_pid ? BOLDBLUE : ""));
		printf("%s", (is_not_pid ? WHITE : ""));
		printf("%s", (is_to_short_notpid ? CYAN : ""));
		printf("%s", (became_overdue_notpid ? MAGENTA : ""));

		printf("%10d  |  ", log[i].time);
		printf("%5d - >%5d  |  ", log[i].previous_pid, log[i].next_pid);
		printf("%15s -> %15s  |  ", typeE2S(log[i].previous_type), typeE2S(log[i].next_type));
		printf("%s\n", swRsnE2S(log[i].reason));

	}
}

int main(){
	flush_scheduling_statistic();
	int retval;
	int pid = fork();
	if(pid == 0){
		for(int i=0; i<20; ++i){
			for(int j=0; j<9000000; ++j);
			++i;
		}
		_exit(EXIT_SUCCESS);
	} else {
		int cooloffs = 5;
		int requested_time = 25;
		struct sched_param* param = (struct sched_param*)malloc(sizeof(struct sched_param));
		param->sched_priority=0;
		param->requested_time=requested_time;
		param->requested_cycles=cooloffs;
		sched_setscheduler(pid, 5, param);
	}

	wait(&retval);
	print_log(pid);
	return 0;
}
