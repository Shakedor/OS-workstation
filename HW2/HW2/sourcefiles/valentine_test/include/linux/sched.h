#define other_task(p)	(((p)->policy != SCHED_SHORT) && ((p)->prio >= MAX_RT_PRIO))
#define short_task(p)	(((p)->policy == SCHED_SHORT) && ((p)->is_overdue == 0))
#define overdue_task(p)	(((p)->policy == SCHED_SHORT) && ((p)->is_overdue == 1))

/****************  HW2 monitor ********************/
#define SCHED_MONITORING_LIMIT 100
#define SCHED_SAVES_LIMIT 500
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
struct switch_info {
	int previous_pid;
	int previous_type;
	int next_pid;
	int next_type;

	unsigned long time;
	int reason;
};
enum taskType{
	OTHER,				//		0
	RT_FIFO,			//		1
	RT_RR,				//		2
	SHORT_NOT_OVERDUE,	//		5
	SHORT_OVERDUE, 		//		5
	ERR
};

int getSwitchTotal();
int getSwitchIndex();
struct switch_info* getSwitchInfo();
/************************* HW2 monitor *********************/

struct task_struct {    
	/* HW2 monitor */
	enum switchReason reason;
	/* HW2 monitor */
};