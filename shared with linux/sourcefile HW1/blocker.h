//ifndef and stuff


// add blocked list data strucutre

// add utility functions to blockdList





















/** wrapper prototype
int my_system_call (int p1, char *p2,int p3)
{
unsigned int res;
__asm__
(
 "int $0x80;"
 : "=a" (res)
 : "0" (244) ,"b" (p1) ,"c" (p2), "d" (p3)
 : "memory"
);
 if (res>= (unsigned long)(-125))
 {
 errno = -res;
 res = -1;
 }
return (int) res;
}
**/


int block_program(const char *name, unsigned int name_len) {
	
}

int unblock_program (const char *name, unsigned int name_len){
	
} 

int is_program_blocked(const char *name, unsigned int name_len){

}

int get_blocked_count(void){
	
}

int get_forbidden_tries(int pid,char log[][256],unsigned int n){
	
}