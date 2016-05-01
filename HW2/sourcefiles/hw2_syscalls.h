
#ifndef NEW_SYS_H_
#define NEW_SYS_H_


#include <linux/unistd.h>
#include <linux/types.h>
#include <stdio.h>


int is_SHORT(int pid){
	unsigned int res;
	__asm__
	(
	 "int $0x80;"
	 : "=a" (res)
	 : "0" (243) ,"b" (pid) 
	);
	 if (res>= (unsigned long)(-125))
	 {
		 errno = -res;
		 res = -1;
	 }
	return (int) res;
}  

int remaining_time(int pid){
	unsigned int res;
	__asm__
	(
	 "int $0x80;"
	 : "=a" (res)
	 : "0" (244) ,"b" (pid) 
	);
	 if (res>= (unsigned long)(-125))
	 {
		 errno = -res;
		 res = -1;
	 }
	return (int) res;
}  

int remaining_cooloffs(int pid){
	unsigned int res;
	__asm__
	(
	 "int $0x80;"
	 : "=a" (res)
	 : "0" (245) ,"b" (pid) 
	);
	 if (res>= (unsigned long)(-125))
	 {
		 errno = -res;
		 res = -1;
	 }
	return (int) res;
}  








#endif
