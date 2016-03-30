#include "blocker.h"
#include <unistd.h>
#include <string.h>
#include <sys/types.h>


int main{
	int res =1;
	// inititalize some files and arguments
	const char *test2 = "2_test.c";
   const char *file1 = "/bin/date";
   const char *file2 = "/bin/mkdir";
   const char *file3 = "/bin/more";
   const char *file4 = "/bin/pwd";
   unsigned int lent2 = strlen(test2);
   unsigned int len1 = strlen(file1);
   unsigned int len2 = strlen(file2);
   unsigned int len3 = strlen(file3);
   unsigned int len4 = strlen(file4);
	char *argt2[] = {"2_test.c", NULL};
   char *arg1[] = {"/bin/date", NULL};
   char *arg2[] = {"/bin/mkdir", NULL};
   char *arg3[] = {"/bin/more", NULL};
   char *arg4[] = {"/bin/pwd", NULL};
   char* files[]={file1,file2,file3,file4}
   int lens[]={len1,len2,len3,len4}
   char **args[]={arg1,arg2,arg3,arg4}
 
	char log[10][256]={0};
	
	if(is_program_blocked(file1,len1)!=1){
		printf("Err 51 file1 should remain blocked after execv\n");
		res=0;
	}
	if(get_blocked_count()!=2){
		printf("Err 51 2 blocked files should remain blocked after execv\n");
		res=0;
	}
	
	block_program(file3,len3);
	
	// fourth forbidden access 1st time to file3
	execv(file3,arg3);
	pid_t my_pid=getpid();
	get_forbidden_tries(my_pid,log,5);
	if(strcmp(file1,log[0])!=0 || strcmp(file1,log[1])!=0 || \
		strcmp(file2,log[2])!=0 || strcmp(file3,log[3])!=0 || log[4][0]!=0){
		printf("ERR 52, forbidden access not logged properly\n");
		res=0;

	}
	
	get_forbidden_tries(my_pid,log,1);
	if(strcmp(file1,log[0])!=0 ||  log[1][0]!=0{
		printf("ERR 53, forbidden access log not truncated properly for smaller ns\n");
		res=0;
	}
	
	   // unblock all programs
	for(int i=0,i<4,i++){
		unblock_program(files[i],lens[i]);
    }
	
	return(res);
}