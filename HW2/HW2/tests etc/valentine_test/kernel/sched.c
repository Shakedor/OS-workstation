struct runqueue {
    /* HW2 monitor */
	struct switch_info switchInfo[SCHED_SAVES_LIMIT];
	int switchCounter, switchIndex, switchTotal;
    /* HW2 monitor */
} ____cacheline_aligned;

static int try_to_wake_up(task_t * p, int sync)
{
		if (p->prio < rq->curr->prio) {
            /* HW2 monitor */
            current->reason = HIGHER_PRIORITY;
            /* HW2 monitor */
			resched_task(rq->curr);
        }
		success = 1;
	}
}

void scheduler_tick(int user_tick, int system)
{
	//put wherever you think it should be
            /* HW2 monitor */
			current->reason = TIMESLICE_ENDED;
			/* HW2 monitor */

            /* HW2 monitor */
            current->reason = BECAME_OVERDUE;
            /* HW2 monitor */

            /* HW2 monitor */
            current->reason = OVERDUE_TO_SHORT;
            /* HW2 monitor */

	}
}

/************************* HW2 monitor *********************/
int get_task_type(task_t* p){
	if(p->policy == SCHED_FIFO) return RT_FIFO;
	if(p->policy == SCHED_RR) return RT_RR;
	if(p->policy == SCHED_OTHER) return OTHER;
	if(overdue_task(p)) return SHORT_OVERDUE;
	if(short_task(p)) return SHORT_NOT_OVERDUE;
	return ERR;
}
/************************* HW2 monitor *********************/

asmlinkage void schedule(void)
{

/* I WOULD PUT IT SOMEWHERE THERE.. ITS YOUR CALL ;) */
switch_tasks:
    /* HW2 monitor */
	int csIndex = rq->switchIndex >= SCHED_SAVES_LIMIT ? 0 : rq->switchIndex;
	if (prev->reason == CREATED ||	prev->reason == ENDED) {
		rq-> switchCounter = 0;
	}
	if (rq->switchCounter < SCHED_MONITORING_LIMIT) {
		rq->switchInfo[csIndex].previous_pid = prev->pid;
		rq->switchInfo[csIndex].previous_type = get_task_type(prev);
		rq->switchInfo[csIndex].next_pid = next->pid;
		rq->switchInfo[csIndex].next_type = get_task_type(next);

		rq->switchInfo[csIndex].time = jiffies;
		rq->switchInfo[csIndex].reason = prev->reason;
		rq->switchIndex = csIndex + 1;
		(rq->switchTotal)++;
		(rq->switchCounter)++;
	}
	/* HW2 monitor */
}

/* HW2 monitor */
int getSwitchTotal() {
	runqueue_t *rq = this_rq_lock();
	int tmp = rq->switchTotal;
	rq_unlock(rq);
	return tmp;
}
int getSwitchIndex() {
	runqueue_t *rq = this_rq_lock();
	int tmp = rq->switchIndex;
	rq_unlock(rq);
	return tmp;
}
struct switch_info* getSwitchInfo() {
	runqueue_t *rq = this_rq_lock();
	struct switch_info* tmp = rq->switchInfo;
	rq_unlock(rq);
	return tmp;
}
/* HW2 monitor */

asmlinkage long sys_sched_yield(void)
{
    /* HW2 monitor */
    current->reason = YEILDS;
    /* HW2 monitor */
}

void __init sched_init(void)
{
	for (i = 0; i < NR_CPUS; i++) {
        /* HW2 monitor */
		rq->switchCounter = 0;
		rq->switchIndex = 0;
		rq->switchTotal = 0;
		/* HW2 monitor */
	}
}

asmlinkage int sys_get_scheduling_statistic(struct switch_info *user_switch_info){
	if (!user_switch_info) { return -EINVAL; }
	if (getSwitchTotal() >= SCHED_SAVES_LIMIT) {
		int partSize = (sizeof(struct switch_info))*(SCHED_SAVES_LIMIT - getSwitchIndex());
		copy_to_user(user_switch_info, getSwitchInfo() + getSwitchIndex() , partSize);
		partSize = (sizeof(struct switch_info))*getSwitchIndex();
		copy_to_user(user_switch_info + (SCHED_SAVES_LIMIT - getSwitchIndex()), getSwitchInfo() , partSize);
		return SCHED_SAVES_LIMIT;
	} else {
		int partSize = (sizeof(struct switch_info))*(getSwitchIndex());
		copy_to_user(user_switch_info, getSwitchInfo() , partSize);
		return getSwitchTotal();
	}
}

asmlinkage int sys_flush_scheduling_statistic(){
	runqueue_t *rq = this_rq_lock();
	rq->switchTotal = 0;
	rq->switchIndex = 0;
	rq->switchCounter = 0;
	rq_unlock(rq);
	return 0;
}