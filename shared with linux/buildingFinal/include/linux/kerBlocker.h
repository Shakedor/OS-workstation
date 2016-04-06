#ifndef _COUNTER_FUNCTIONS_H
#define _COUNTER_FUNCTIONS_H

#include <linux/sched.h>

extern List blockedPrg = {0};

int isBlocked(const char *name);

int sys_block_program(const char *name, unsigned int name_len);
int sys_unblock_program(const char *name, unsigned int name_len);
int sys_is_program_blocked(const char *name, unsigned int name_len);
int sys_get_blocked_count(void);
int sys_get_forbidden_tries(int pid,char log[][256],unsigned int n);

#endif
