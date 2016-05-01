//
//  main.c
//  wet2
//
//  Created by macbook on 4/16/16.
//  Copyright © 2016 LiorCohen. All rights reserved.
//

#include <stdio.h>

#include "hw2_syscalls.h"

#define MAX_USER_RT_PRIO	100
#define MAX_RT_PRIO		MAX_USER_RT_PRIO

#define MAX_PRIO		(MAX_RT_PRIO + 40)
#define BITMAP_SIZE ((((MAX_PRIO+1+7)/8)+sizeof(long)-1)/sizeof(long))




struct context_switch_info{
    
    unsigned short p_short_state, n_short_state;
    unsigned short p_run_state, n_run_state;
    short p_cool_off_cycles, n_cool_off_cycles;
    unsigned short p_initial_cool_off_cycles, n_initial_cool_off_cycles;
    unsigned int p_requested_time, n_requested_time;
    unsigned int p_new_requested_time, n_new_requested_time;
    
    int p_pid, p_policy, p_prio, p_static_prio, p_timeslice;
    int n_pid, n_policy, n_prio, n_static_prio, n_timeslice;
    
    int counter;
    
    unsigned long nr_running;
    unsigned long time_stamp;
    
    unsigned long active_bitmap[BITMAP_SIZE];
    unsigned long expired_bitmap[BITMAP_SIZE];
    unsigned long short_bitmap[BITMAP_SIZE];
    unsigned long overdue_bitmap[BITMAP_SIZE];
    
    int reason;
};

typedef struct context_switch_info* Info;

int get_log_size(int* size) {
    
    unsigned int res;
    __asm__
    (
     "int $0x80;"
     : "=a" (res)
     : "0" (246) ,"b" (size)
     : "memory"
     );
    
    if (res >= (unsigned long)(-125))
    {
        errno = -res;
        res = -1;
    }
    return (int) res;
    
}

int get_CSLog(struct context_switch_info info[], int len) {
    
    unsigned int res;
    __asm__
    (
     "int $0x80;"
     : "=a" (res)
     : "0" (247) ,"b" (info), "c" (len)
     : "memory"
     );
    
    if (res >= (unsigned long)(-125))
    {
        errno = -res;
        res = -1;
    }
    return (int) res;
    
}

int start_CSLog(void) {
    
    unsigned int res;
    __asm__
    (
     "int $0x80;"
     : "=a" (res)
     : "0" (248)
     : "memory"
     );
    
    if (res >= (unsigned long)(-125))
    {
        errno = -res;
        res = -1;
    }
    return (int) res;
    
}


int main(int argc, const char * argv[]) {
    // insert code here...
    char* op_is = "is";
    char* op_time = "time";
    char* op_cycles = "cycles";
    char* op_start = "start";
    char* op_log = "log";
    //struct sched_param param;
    unsigned long pid;
    int* size;
    int i,j,logRes;
#define MAX_SIZE 1000
    struct context_switch_info info[MAX_SIZE];
    
    if (argc > 1) {
        if (!strcmp(argv[1],op_is)) {
            printf("is_SHORT = %d\n",is_SHORT(atoi(argv[2])));
        }
        if (!strcmp(argv[1],op_time)) {
            printf("remaining time = %d\n",remaining_time(atoi(argv[2])));
        }
        if (!strcmp(argv[1],op_cycles)) {
            printf("remaining cooloffs = %d\n",remaining_cooloffs(atoi(argv[2])));
        }
        if (!strcmp(argv[1],op_start)) {
            printf("starting logs = %d\n",start_CSLog());
        }
        if (!strcmp(argv[1],op_log)) {
            logRes = get_log_size(size);
            printf("fetching logs = %d, of size=%d\n", logRes, *size);
            logRes=get_CSLog(info, *size);
            printf("result of copying = %d\n",logRes);
            
            for (i=0; i< *size; i++) {
                printf("log %d: reason=%d\n",i,info[i].reason);
                printf("Prev: \tid=%d, \tpolicy=%d, \ts_state=%d, \tcool=%d \tTimeSlice=%d \n",info[i].p_pid,info[i].p_policy,info[i].p_short_state,info[i].p_cool_off_cycles,info[i].p_timeslice);
                printf("Next: \tid=%d, \tpolicy=%d, \ts_state=%d, \tcool=%d \tTimeSlice=%d \n",info[i].n_pid,info[i].n_policy,info[i].n_short_state,info[i].n_cool_off_cycles,info[i].n_timeslice);
                printf("SHORT BITMAP: \t\t");
                for (j=0; j<BITMAP_SIZE; j++) {
                    printf(" %d ",info[i].short_bitmap[j]);
                }
                printf("\n");
                printf("OVERDUE BITMAP: \t");
                for (j=0; j<BITMAP_SIZE; j++) {
                    printf(" %d ",info[i].overdue_bitmap[j]);
                }
                printf("\n");
                printf("ACTIVE BITMAP: \t\t");
                for (j=0; j<BITMAP_SIZE; j++) {
                    printf(" %d ",info[i].active_bitmap[j]);
                }
                printf("\n");
                printf("EXPIRED BITMAP: \t");
                for (j=0; j<BITMAP_SIZE; j++) {
                    printf(" %d ",info[i].expired_bitmap[j]);
                }
                printf("\n");
                printf("Counter: %d\n",info[i].counter);
                printf("\n------------------------------------------------------------\n");
            }
        }
    }
    
    return 0;
}
