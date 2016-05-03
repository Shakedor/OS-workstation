

#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/syscall.h>
#include <linux/sched.h>


#define DEBUG_SYS

asmlinkage int sys_is_SHORT(int pid) {
	task_t *p = find_task_by_pid(pid);
	if(p == NULL) {
		return -EINVAL;
	}
	if (p->policy != SCHED_SHORT) {
		return -EINVAL;
	}
	if(p->is_overdue == 1) {
		return 0;
	}
	return 1;
}

asmlinkage int sys_remaining_time(int pid) {
	task_t *p = find_task_by_pid(pid);
	if(p == NULL) {
		return -EINVAL;
	}
	if (p->policy != SCHED_SHORT) {
		return -EINVAL;
	}
	#ifdef DEBUG_SYS
	printk("remaining time is %d, requested is %d, NCreq is %d\n",p->remaining_time,p->requested_time,p->NCrequested_time);
	#endif
	if(p->remaining_cycles <= -1){
		return 0;
	}
	return (p->remaining_time*1000)/HZ;
}

asmlinkage int sys_remaining_cooloffs(int pid) {
	task_t *p = find_task_by_pid(pid);
	if(p == NULL) {
		return -EINVAL;
	}
	if (p->policy != SCHED_SHORT) {
		return -EINVAL;
	}
	if(p->remaining_cycles <=  -1){
		return 0;
	}
	return p->remaining_cycles;
}