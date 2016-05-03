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


		
int test_runqueue_sanity(){
	int cooloffs = 5;
	int requested_time = 50;
	sched_param_t params = {0, 3000, 5};
	sched_param_t paramSet = {0, 1500, 5};
	int i, j;
	if(fork()==0) {
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params)); //now we are short
					if(fork()==0) {
						//son
						printf("printing runqueue - 1\n");
						print_runqueue();
						nice(-2);
						if(fork()==0) {
							//grandchild
							nice(1);
							//now father should run
							printf("we returned to here, after printing renqueue no 2\n");
							while(is_SHORT(getpid()));
							printf("grandchild is overdue\n");
							while(!is_SHORT(getpid()));
							exit(0);
						} else {
							//son
							printf("printing runqueue - 2\n");
							print_runqueue();
							ASSERT_ZERO(sched_setparam(getppid(), &paramSet));
							while(is_SHORT(getpid()));
							ASSERT_DELTA(remaining_time(getpid()),3000);
							printf("son is overdue\n");
							nice(-10);
							print_runqueue();
							while(!is_SHORT(getpid()));
							ASSERT_DELTA(remaining_time(getpid()),3000);
							print_runqueue();
							wait(0);
						}
						exit(0);
					} else {
						//father
						printf("we returned to father\n");
						print_runqueue();
						while(is_SHORT(getpid()));
						ASSERT_DELTA(remaining_time(getpid()),3000);
						printf("father is overdue\n");
						print_runqueue();
						while(!is_SHORT(getpid()));
						ASSERT_DELTA(remaining_time(getpid()),1500);
						print_runqueue();

						wait(0);
					}
			exit(0);
	}
	else {
		wait(0);
	}
	
	return 1;
}

int test_short_yield(){
		int cooloffs = 5;
	int requested_time = 50;
	sched_param_t params = {0, 3000, 5};
	sched_param_t paramSet = {0, 1500, 5};
	int i, j;
	if(fork()==0) {
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params)); //now we are short
		if(fork()==0){
			//son
			printf("\nson goes first - 1\n");
			printf("son yield       - 2\n");
			sched_yield();
			printf("son return       - 4\n");
			while(is_SHORT(getpid()));
			printf("son return  again     - 6\n");
			print_runqueue();
			exit(0);
		} else {
			//father
			printf("father goes now - 3\n");
			print_runqueue();
			while(is_SHORT(getpid()));
			print_runqueue();
			printf("father before yield    - 5\n");
			sched_yield();
			while(!is_SHORT(getpid()));
			wait(0);
		}
			exit(0);
	}
	else {
		wait(0);
	}

	
	return 1;
}

int test_short_sleep(){
		int cooloffs = 5;
	int requested_time = 50;
	sched_param_t params = {0, 3000, 5};
	sched_param_t paramSet = {0, 1500, 5};
	int i, j;
	if(fork()==0) {
		ASSERT_ZERO(sched_setscheduler(getpid(), SCHED_SHORT, &params)); //now we are short
		
		if(fork()==0){
			//son
			printf("\nson goes first - 1\n");
			while(is_SHORT(getpid()));
			printf("son goes overdue - 3\n");
			sleep(1);
			printf("son wakes up, still overdue - 6\n");
			print_runqueue(); //only son is here as an overdue
			while(!is_SHORT(getpid()));
			printf("son is now SHORT - 8\n");
			while(is_SHORT(getpid()));
			exit(0);
		} else {
			//father
			printf("father starts after son  - 2\n");
			while(is_SHORT(getpid()));
			printf("father goes overdue - 4\n");
			while(!is_SHORT(getpid()));
			printf("father goes SHORT - 5\n");
			sleep(1);
			printf("father wakes up as SHORT, steal CPU- 7\n");
			print_runqueue(); //father here as SHORT, son as overdue
			nice(-3);
			sleep(3);
			printf("father steals again CPU- 9\n");
			print_runqueue(); //father here as SHORT, son as short also
			
			wait(0);
		}
		exit(0);
	}
	else {
		wait(0);
	}

	
	return 1;
}


int main(){
	RUN_TEST(test_runqueue_sanity);
	RUN_TEST(test_short_yield);
	RUN_TEST(test_short_sleep);
	
	return 0;
}
