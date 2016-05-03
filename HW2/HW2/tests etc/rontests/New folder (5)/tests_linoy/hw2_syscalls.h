#ifndef _HW2_SYSCALLS_H
#define _HW2_SYSCALLS_H

#ifndef	errno
extern int errno;
#endif

/* syscall 243 */
int is_SHORT(int pid){
	unsigned int res;
	__asm__
	( 
		"int $0x80;"
		: "=a" (res)
		: "0" (243) ,"b" (pid)
		: "memory"
	);
	if (res>= (unsigned long)(-125))
	{
		errno = -res;
		res = -1;
	}
	return (int) res;
}


/* syscall 244 */
int remaining_time(int pid){
	unsigned int res;
	__asm__
	( 
		"int $0x80;"
		: "=a" (res)
		: "0" (244) ,"b" (pid)
		: "memory"
	);
	if (res>= (unsigned long)(-125))
	{
		errno = -res;
		res = -1;
	}
	return (int) res;
}


/* syscall 245 */
int remaining_cooloffs(int pid){
	unsigned int res;
	__asm__
	( 
		"int $0x80;"
		: "=a" (res)
		: "0" (245) ,"b" (pid)
		: "memory"
	);
	if (res>= (unsigned long)(-125))
	{
		errno = -res;
		res = -1;
	}
	return (int) res;
}

int get_scheduling_statistic(void* switchInfo) {
	long __res;
	__asm__ volatile (
		"movl $246, %%eax;"
		"movl %1, %%ebx;"
		"int $0x80;"
		"movl %%eax,%0"
		: "=m" (__res)
		: "m" (switchInfo)
		: "%eax","%ebx"
	);
	if ((unsigned long)(__res) >= (unsigned long)(-125)) {
		errno = -(__res); __res = -1;
	}
	return (int)(__res);
}

void flush_scheduling_statistic() {
	long __res;
	__asm__ volatile (
		"movl $247, %%eax;"
		"int $0x80;"
		: : : "%eax"
	);
	if ((unsigned long)(__res) >= (unsigned long)(-125)) {
		errno = -(__res); __res = -1;
	}
	// return (int)(__res);
}

#define SCHED_MONITORING_LIMIT 100
#define SCHED_SAVES_LIMIT 500

typedef struct switch_info {
	int previous_pid;
	int previous_type;
	int next_pid;
	int next_type;
	unsigned long time;
	int reason;
} switch_t;

enum switchReason{
	DEFUALT,
	CREATED,
	ENDED,
	YEILDS,
	BECAME_OVERDUE,
	PREVIOUS_WENT_WAITING,
	HIGHER_PRIORITY,
	TIMESLICE_ENDED,
	OVERDUE_TO_SHORT
};
enum taskType{
	OTHER,				//		0
	RT_FIFO,			//		1
	RT_RR,				//		2
	SHORT_NOT_OVERDUE,	//		5
	SHORT_OVERDUE, 		//		5
	ERR
};

char* swRsnE2S(int e){
	switch(e){
		case DEFUALT: return "DEFAULT";
		case CREATED: return "CREATED";
		case ENDED: return "ENDED";
		case YEILDS: return "YEILDS";
		case BECAME_OVERDUE: return "BECAME_OVERDUE";
		case PREVIOUS_WENT_WAITING: return "PREVIOUS_WENT_WAITING";
		case HIGHER_PRIORITY: return "HIGHER_PRIORITY";
		case TIMESLICE_ENDED: return "TIMESLICE_ENDED";
		case OVERDUE_TO_SHORT: return "OVERDUE_TO_SHORT";
	}
	return "ERROR?";
}

char* typeE2S(int e){
	switch(e){
		case OTHER: return "OTHER";
		case RT_FIFO: return "RT_FIFO";
		case RT_RR: return "RT_RR";
		case SHORT_NOT_OVERDUE: return "SHORT";
		case SHORT_OVERDUE: return "SHORT_OVERDUE";
		case ERR: return "ERR";
	}
	return "ERROR?";
}

#endif
