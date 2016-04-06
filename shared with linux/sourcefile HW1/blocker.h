#ifndef BLOCKER_H_
#define BLOCKER_H_

#include <linux/unistd.h>
#include <linux/types.h>
#include <stdio.h>


//#define DEBUG

int block_program(const char *name, unsigned int name_len) {
unsigned int res;
__asm__
(
 "int $0x80;"
 : "=a" (res)
 : "0" (243) ,"b" (name) ,"c" (name_len)
 : "memory"
);
 if (res>= (unsigned long)(-125))
 {
 errno = -res;
 res = -1;
 }
return (int) res;
}  

int getfive(void) {
unsigned int res;
__asm__
(
 "int $0x80;"
 : "=a" (res)
 : "0" (248) 
);
 if (res>= (unsigned long)(-125))
 {
 errno = -res;
 res = -1;
 }
return (int) res;
}  

int unblock_program (const char *name, unsigned int name_len){
  unsigned int res;
__asm__
(
 "int $0x80;"
 : "=a" (res)
 : "0" (244) ,"b" (name) ,"c" (name_len)
 : "memory"
);
 if (res>= (unsigned long)(-125))
 {
 errno = -res;
 res = -1;
 }
return (int) res;
}

int is_program_blocked(const char *name, unsigned int name_len){
unsigned int res;

__asm__
(
 "int $0x80;"
 : "=a" (res)
 : "0" (245) ,"b" (name) ,"c" (name_len)
);
 if (res>= (unsigned long)(-125))
 {
 errno = -res;
 res = -1;
 }
return (int) res;
}

int get_blocked_count(void){
	#ifdef DEBUG
     printf("entering get block count wrapper\n");
     #endif
   unsigned int res;
__asm__
(
 "int $0x80;"
 : "=a" (res)
 : "0" (246)
);

     #ifdef DEBUG
     printf("after assembler in get blocked count wrapper\n");
	 printf("res is %u \n",res);
     #endif
 if (res>= (unsigned long)(-125))
 {
 errno = -res;
 res = -1;
 }
return (int) res;
}

int get_forbidden_tries(int pid,char log[][256],unsigned int n){
  unsigned int res;
__asm__
(
 "int $0x80;"
 : "=a" (res)
 : "0" (247) ,"b" (pid) ,"c" (log), "d" (n)
 : "memory"
);
 if (res>= (unsigned long)(-125))
 {
 errno = -res;
 res = -1;
 }
return (int) res;
}


int isBlocked(const char *name);

#endif
