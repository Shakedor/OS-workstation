

#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/syscall.h>
#include <linux/sched.h>

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
	if (p->is_overdue != 1 && p->policy != SCHED_SHORT) {
		return -EINVAL;
	}
	return p->remaining_time;
}

asmlinkage int sys_remaining_cooloffs(int pid) {
	task_t *p = find_task_by_pid(pid);
	if(p == NULL) {
		return -EINVAL;
	}
	if (p->is_overdue != 1 && p->policy != SCHED_SHORT) {
		return -EINVAL;
	}
	return p->remaining_cycles;
}