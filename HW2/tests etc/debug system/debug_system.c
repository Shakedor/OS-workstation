

////////////////////
/*  %%%%%%%%%%%%%%%
 
 This file includes all the changes and a simple tutorial on how to use 
 the debug system.
 
 Follow the steps in the comments and read the last one, it contains
 directions on how to use the system
 
 We assume your .h file of system calls is named "hw2_syscalls.h"
 as the assignment demands.
 you can change it in the main.c file if it's not the case.
 
 GOOD LUCK
 
 p.s: A lot of data is not printed but you can read the context_switch_info
 struct, and in the main.c you can edit the printf call to print your
 desired data
 
 Also note that we made some foundations for context switch reasons
 but we did not used it eventually,
 so if you want you can try and extend it yourself
 
 */
////////////////////


/*
 ######@@@@@@@@@@@
 
    1)  PUT INSIDE sched.c  SOMEWHERE ABOVE schdule(void) AND UNDER runqueue 
        definition
 
 ########@@@@@@@@@@
 */
//////////////////////////////////////
/////##############################    CHANGES FOR DEBUGGING!!!
/////##############################    CHANGES FOR DEBUGGING!!!
/////##############################    CHANGES FOR DEBUGGING!!!
/////##############################    CHANGES FOR DEBUGGING!!!

typedef enum {
    UNKNOWN,
    DUMMY,
    END_OF_TIME_SHORT,
    END_OF_TIME_OVERDUE,
    END_OF_TIME_OTHER,
    END_OF_TIME_RT,
    YIELD_SHORT,
    YIELD_OVERDUE,
    YIELD_OTHER,
    YIELD_RT,
    WAIT_SHORT,
    WAIT_OVERDUE,
    WAIT_OTHER,
    WAIT_RT,
    WAKEUP,
    WAKEUP_SHORT,
    WAKEUP_OVERDUE,
    WAKEUP_OTHER,
    WAKEUP_RT,
    FORK_SHORT,
    FORK_OVERDUE,
    FORK_OTHER,
    EXIT_SHORT,
    EXIT_OVERDUE,
    EXIT_OTHER,
    CHANGE_PRIO_SHORT,
    CHANGE_PRIO_OVERDUE,
    CHANGE_PRIO_OTHER,
    CHANGE_POLICY_TO_SHORT,
    CHANGE_POLICY_TO_OVERDUE,
    CHANGE_POLICY_TO_OTHER
} REASON_FOR_CALL;

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

#define MAX_LOG_SIZE 101
struct context_switches{
    Info all;
    Info only_short;
    struct context_switch_info p_all[MAX_LOG_SIZE];
    int len;
    int size_all, size_short;
};

typedef struct context_switches* ListInfo;

ListInfo list_info;

void info_init(REASON_FOR_CALL reason){
    struct context_switch_info info;
    if (!list_info || (list_info && list_info->size_all > 100)) {
        return;
    }
    
    info.counter = 0;
    info.reason = reason;
    list_info->p_all[list_info->size_all] = info;
    list_info->size_all++;
}

static void fill_info(task_t* prev, task_t* next, runqueue_t *rq){
    int i;
    Info info;
    
    if (!list_info || (list_info && list_info->size_all > 100)) {
        return;
    }
    if (prev->policy != SCHED_SHORT && next->policy != SCHED_SHORT) {
        return;
    }
    
    info_init(UNKNOWN);
    info = &list_info->p_all[list_info->size_all-1];
    
    
    info->p_short_state = prev->_short_state;
    info->n_short_state = next->_short_state;
    
    info->p_run_state = prev->_run_state;
    info->n_run_state = next->_run_state;
    
    info->p_cool_off_cycles = prev->_cool_off_cycles;
    info->n_cool_off_cycles = next->_cool_off_cycles;
    
    info->p_initial_cool_off_cycles = prev->_initial_cool_off_cycles;
    info->n_initial_cool_off_cycles = next->_initial_cool_off_cycles;
    
    info->p_requested_time = prev->_requested_time;
    info->n_requested_time = next->_requested_time;
    
    info->p_new_requested_time = prev->_new_requested_time;
    info->n_new_requested_time = next->_new_requested_time;
    
    info->p_pid = prev->pid;
    info->n_pid = next->pid;
    
    info->p_policy = prev->policy;
    info->n_policy = next->policy;
    
    info->p_prio = prev->prio;
    info->n_prio = next->prio;
    
    info->p_static_prio = prev->static_prio;
    info->n_static_prio = next->static_prio;
    
    info->p_timeslice = prev->time_slice;
    info->n_timeslice = next->time_slice;
    
    info->nr_running = rq->nr_running;
    info->time_stamp = jiffies;
    
    for(i=0 ; i < BITMAP_SIZE ; ++i){
        info->active_bitmap[i] = rq->active->bitmap[i];
        info->expired_bitmap[i] = rq->expired->bitmap[i];
        info->short_bitmap[i] = rq->shortArray.bitmap[i];
        info->overdue_bitmap[i] = rq->overdueArray.bitmap[i];
    }
}

//////////////////////////////////////
/////##############################    END CHANGES FOR DEBUGGING!!!
/////##############################    END CHANGES FOR DEBUGGING!!!
/////##############################    END CHANGES FOR DEBUGGING!!!
/////##############################    END CHANGES FOR DEBUGGING!!!




/*
 ######@@@@@@@@@@@
 
 2)  PUT THIS CALL INSIDE  sched_init() :
 
 ########@@@@@@@@@@
 */


////############### SHORT_DEBUG
list_info = NULL;   // SHORT_DEBUG



/*
 ######@@@@@@@@@@@
 
 3)  PUT THIS FUNCTION CALL INSIDE  schdule(void) UNDER "switch_tasks:" label:
 
 ########@@@@@@@@@@
 */


//############################## SHORT_DEBUG
fill_info(prev, next, rq); // SHORT_DEBUG
//############################## SHORT_DEBUG



/*
 ######@@@@@@@@@@@
 
 4)  PUT THESE SYSTEM CALLS INSIDE sched.c AT THE BOTTOM OF THE FILE:
 
 ########@@@@@@@@@@
 */

///////
/////// SYSTEM CALLS:
///////

int sys_context_log(struct context_switch_info info[], int len) {
    Info tmp;
    unsigned int i=0,min;
    unsigned long res = 0;
    if (!list_info) {
        return -1;
    }
    //  run through the list and find the node:
    min = ((list_info->size_all < len) ? list_info->size_all : len);
    for (i=0; i<min; i++) {
        tmp = &list_info->p_all[i];
        printk("log number %d n_pid=%d\n",i,tmp->n_pid);
        res += copy_to_user(&info[i],tmp,sizeof(struct context_switch_info));
    }
    return res;
}

int sys_get_log_size(int* size) {
    //  SHORT_DEBUG
    *size = list_info->size_all;
    return (list_info) ? (list_info->size_all) : -1;
}

int sys_start_log(void) {
    //  SHORT_DEBUG
    if(!list_info) {
        list_info = (ListInfo) kmalloc(sizeof(*list_info), GFP_KERNEL);
        if (list_info == NULL) {
            return -ENOMEM;
        }
        list_info->size_all = 0;
        list_info->size_short = 0;
    } else {
        list_info->size_all = 0;
    }
    return 0;
}




/*
 ######@@@@@@@@@@@
 
 5)  PUT THESE SYSTEM CALLS DEFINITIONS INSIDE entry.S:
 
 ########@@@@@@@@@@
 */


.long SYMBOL_NAME(sys_get_log_size) /* out change 246 SHORT_DEBUG */
.long SYMBOL_NAME(sys_context_log) /* out change 247 SHORT_DEBUG */
.long SYMBOL_NAME(sys_start_log) /* out change 248 SHORT_DEBUG */

/*
 ######@@@@@@@@@@@
 
 6)  all the wrapper function are in main.c and are implemented inside code.
    all you need to do is use the runnable file as follows:
 
    compile the main.c using "gcc main.c -o debug"
    
    to start a new log of size 100 run the following command:
 
    debug start
 
    (this will aso reset an existing log)
 
 
    
    after starting a new log, the system will log all context switches that envolves
    a short process. so the way to use this system is to run the "debug start"
    command, then run your own test, and after the test is done fetch the log 
    and display it using:
 
    debug log
 
    if you want it is more convenient to export it to a file using:
 
    debug log > out.txt
 
 
    if you want, you can call the is_SHORT system call using
    debug is *insert pid here*
 
    the remaining_time system call using
    debug time *insert pid here*
 
    and the cooloff_cycles system call using
    debug cycles *insert pid here*
 
 
 ########@@@@@@@@@@
 */



