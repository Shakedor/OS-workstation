#include "blocker.h"
#include <string.h>
   
int test_block_program(){
	
	int res =1;
	// inititalize some files and arguments
   const char *file1 = "/bin/date";
   const char *file2 = "/bin/mkdir";
   const char *file3 = "/bin/more";
   const char *file4 = "/bin/pwd";
   unsigned int len1 = strlen(file1);
   unsigned int len2 = strlen(file2);
   unsigned int len3 = strlen(file3);
   unsigned int len4 = strlen(file4);
   char *arg1[] = {"/bin/date", NULL};
   char *arg2[] = {"/bin/mkdir", NULL};
   char *arg3[] = {"/bin/more", NULL};
   char *arg4[] = {"/bin/pwd", NULL};
   const char* files[]={file1,file2,file3,file4};
   int lens[]={len1,len2,len3,len4};
   char **args[]={arg1,arg2,arg3,arg4};
   
   // check all files are unblocked
   int i;
   for(i=0;i<4;i++){
	   if(0 != is_program_blocked(files[i],lens[i])){
		   printf(" Err1 programs not blocked initially\n");
		   res=0;
	   }
   }
   
   if( get_blocked_count()!=0){
	   printf("Err2 program counter not initialized properly \n");
	   res=0;
   }
   
   if( block_program(file1,0)!=-1){
	   printf("Err3 should fail for param reason\n");
	   res=0;
   }
   if (erno != EINVAL){
	   printf("Err 3.5 erno should be 1\n");
	   res=0;
   }
   
   if( block_program(NULL,1)!=-1){
	   printf("Err4 should fail for param reason\n");
	   res=0;
   }
   
   if (erno != EINVAL){
	   printf("Err 3.5 erno should be 1\n");
	   res=0;
   }
   
   if( block_program(file1,len2)!=-1){
	   printf("Err4.6 should fail for string size violation reason\n");
	   res=0;
   }
   
   if( block_program(file1,len1)!=1){
	   printf("Err5 should have succeeded\n");
	   res=0;
   }
   
   if( block_program(file1,len)!=0){
	   printf("Err6 should know prog was already blocked\n");
	   res=0;
   }
   
   if( block_program(file3,len3)!=1){
	   printf("Err7 should have succeeded\n");
	   res=0;
   }
   
   if( block_program(file3,len3)!=0){
	   printf("Err8 should know prog was already blocked\n");
	   res=0;
   }
   
   for(int i=0,i<4,i++){
	   if( (is_program_blocked(files[i],lens[i])+i%2)%2 !=0 ){
		   printf(" Err9 programs not blocked correctly\n");
		   res=0;
	   }
   }
   
   unblock_program(file1,len1);
   unblock_program(file3,len3);
   
   return res;
   
}



int test_unblock_program(){
	
	int res =1;
	// inititalize some files and arguments
   const char *file1 = "/bin/date";
   const char *file2 = "/bin/mkdir";
   const char *file3 = "/bin/more";
   const char *file4 = "/bin/pwd";
   unsigned int len1 = strlen(file1);
   unsigned int len2 = strlen(file2);
   unsigned int len3 = strlen(file3);
   unsigned int len4 = strlen(file4);
   char *arg1[] = {"/bin/date", NULL};
   char *arg2[] = {"/bin/mkdir", NULL};
   char *arg3[] = {"/bin/more", NULL};
   char *arg4[] = {"/bin/pwd", NULL};
   char* files[]={file1,file2,file3,file4}
   int lens[]={len1,len2,len3,len4}
   char **args[]={arg1,arg2,arg3,arg4}
   
   // check all files are unblocked
	for(int i=0,i<4,i++){
		block_program(files[i],lens[i]);
   }
   
   for(int i=0,i<4,i++){
	   if(1 != is_program_blocked(files[i],lens[i])){
		   printf(" Err11 programs should be blocked \n");
		   res=0;
	   }
   }
   
   
   if( unblock_program(NULL,1)!=-1){
	   printf("Err12 should fail for param reason\n");
	   res=0;
   }
   
   if (erno != EINVAL){
	   printf("Err 12.5 erno should be 1\n");
	   res=0;
   }

   if( unblock_program(file1,0)!=-1){
	   printf("Err13 should fail for param reason\n");
	   res=0;
   }
   
   if(unblock_program(file1,len1)!=1 ){
	   printf("Err 14 unblock should succseed \n");
   }
   
   if(unblock_program(file1,len1)!=0 ){
	   printf("Err 15 should be already unblocked \n");
   }
   
   if(is_program_blocked(file1,len1)!=0){
	   printf("Err 16 we unblocked file 1");
   }
   
   if(unblock_program(file3,len3)!=1 ){
	   printf("Err 17 should be already unblocked \n");
   }
  
  
   for(int i=0,i<4,i++){
	   if( (is_program_blocked(files[i],lens[i])+i%2)%2 !=1 ){
		   printf(" Err9 programs not blocked correctly\n");
		   res=0;
	   }
   }
   
   unblock_program(file2,len2);
   unblock_program(file4,len4);
   return res;
   
}

int test_is_program_blocked(){
	
	int res =1;
	// inititalize some files and arguments
   const char *file1 = "/bin/date";
   const char *file2 = "/bin/mkdir";
   const char *file3 = "/bin/more";
   const char *file4 = "/bin/pwd";
   unsigned int len1 = strlen(file1);
   unsigned int len2 = strlen(file2);
   unsigned int len3 = strlen(file3);
   unsigned int len4 = strlen(file4);
   char *arg1[] = {"/bin/date", NULL};
   char *arg2[] = {"/bin/mkdir", NULL};
   char *arg3[] = {"/bin/more", NULL};
   char *arg4[] = {"/bin/pwd", NULL};
   char* files[]={file1,file2,file3,file4}
   int lens[]={len1,len2,len3,len4}
   char **args[]={arg1,arg2,arg3,arg4}
 
    if( is_program_blocked(NULL,1)!=-1){
	   printf("Err21 should fail for param reason\n");
	   res=0;
   }
   
   if (erno != EINVAL){
	   printf("Err 21.5 erno should be EINVAL\n");
	   res=0;
   }

   if( is_program_blocked(file1,0)!=-1){
	   printf("Err22 should fail for param reason\n");
	   res=0;
   }
 
  for(int i=0,i<4,i++){
	   if(0 != is_program_blocked(files[i],lens[i])){
		   printf(" Err23 programs should be not blocked \n");
		   res=0;
	   }
   }
 
   // check all files are unblocked
	for(int i=0,i<4,i++){
		block_program(files[i],lens[i]);
   }
   
   for(int i=0,i<4,i++){
	   if(1 != is_program_blocked(files[i],lens[i])){
		   printf(" Err24 programs should be blocked \n");
		   res=0;
	   }
   }
   
   for(int i=0,i<4,i++){
	   unblock_program(files[i],lens[i]);
	   if(0 != is_program_blocked(files[i],lens[i])){
		   printf(" Err24 programs should be blocked \n");
		   res=0;
	   }
   }
   

   return res;
   
}



int test_get_blocked_count(){
	
	int res =1;
	// inititalize some files and arguments
   const char *file1 = "/bin/date";
   const char *file2 = "/bin/mkdir";
   const char *file3 = "/bin/more";
   const char *file4 = "/bin/pwd";
   unsigned int len1 = strlen(file1);
   unsigned int len2 = strlen(file2);
   unsigned int len3 = strlen(file3);
   unsigned int len4 = strlen(file4);
   char *arg1[] = {"/bin/date", NULL};
   char *arg2[] = {"/bin/mkdir", NULL};
   char *arg3[] = {"/bin/more", NULL};
   char *arg4[] = {"/bin/pwd", NULL};
   char* files[]={file1,file2,file3,file4}
   int lens[]={len1,len2,len3,len4}
   char **args[]={arg1,arg2,arg3,arg4}
 
	// 0 0 0 0
	if( get_blocked_count()!=0){
	   printf("Err 31 should be no blocked progs\n");
	   res=0;
	}
	
	// 1 0 0 0
	block_program(files1,len1);
	
	if( get_blocked_count()!=1){
	   printf("Err 31 should be no blocked progs\n");
	   res=0;
	}
	
	// 1 1 0 0 
	block_program(files2,len2);
	
	if( get_blocked_count()!=2){
	   printf("Err 31 should be no blocked progs\n");
	   res=0;
	}

	// 1 1 0 0 
	block_program(files2,len2);
	
	if( get_blocked_count()!=2){
	   printf("Err 31 should be no blocked progs\n");
	   res=0;
	}

	// 1 1 1 0
	block_program(files3,len3);
	
	if( get_blocked_count()!=3){
	   printf("Err 31 should be no blocked progs\n");
	   res=0;
	}

	// 0 1 1 0
	unblock_program(files1,len1);
	
	if( get_blocked_count()!=2){
	   printf("Err 31 should be no blocked progs\n");
	   res=0;
	}

	// 0 1 1 0
	unblock_program(files4,len4);
	
	if( get_blocked_count()!=2){
	   printf("Err 31 should be no blocked progs\n");
	   res=0;
	}		
   
   // 1 1 1 0
   	block_program(files1,len1);
	
	// 1 1 1 1
	block_program(files4,len4);
	
	if( get_blocked_count()!=4){
	   printf("Err 31 should be no blocked progs\n");
	   res=0;
	}

	unblock_program(files4,len4);
	unblock_program(files4,len4);
	if( get_blocked_count()!=3){
	   printf("Err 31 should be no blocked progs\n");
	   res=0;
	}
	unblock_program(files3,len3);
	unblock_program(files3,len3);
	if( get_blocked_count()!=2){
	   printf("Err 31 should be no blocked progs\n");
	   res=0;
	}
	unblock_program(files2,len2);
	unblock_program(files2,len2);
	if( get_blocked_count()!=1){
	   printf("Err 31 should be no blocked progs\n");
	   res=0;
	}
	unblock_program(files1,len1);
	unblock_program(files1,len1);
	if( get_blocked_count()!=0){
	   printf("Err 31 should be no blocked progs\n");
	   res=0;
	}
	
	return res;

}


int test_get_forbidden_tries(){
	
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
	
	int status1=fork();
	if(status1<0){
		printf("forking num1 failed\n");
	}
	//father code
	if(status1!=0){
		res=wait();
		int status2=fork();
		if(status2!=0){
			printf("forking num2 failed\n");
		}
		// father code
		if(status!=0){
			res=wait();	
			return(res);
		}
		//second son code
		else{
			pid_t my_pid=getpid();
			if(get_blocked_count()!=0){
				printf("Err 61 blocked list not emptied before 2nd son\n");
				res=0;
			}
			// no forbidden access
			get_forbidden_tries(my_pid,log,1);
			if(strcmp(log[0][0]!=NULL){
				printf("ERR 40, forbidden access not initialized properly\n");
				res=0;
			}
			block_program(file1,len1);
			block_program(file2,len2);
			
			// forbidden acess to file 2
			execv(file2,arg2);
			get_forbidden_tries(my_pid,log,2);
			if(strcmp(file2,log[0])!=0 || log[1][0]!=NULL){
				printf("ERR 41, forbidden access not logged properly\n");
				res=0;
			}
			//  forbidden to file1
			execv(file1,arg1);
			get_forbidden_tries(my_pid,log,2);
			if(strcmp(file2,log[0])!=0 || strcmp(file1,log[0])!=0){
				printf("ERR 42, forbidden access not logged properly\n");
				res=0;
			}
			
			
			// unblock all programs
			for(int i=0,i<4,i++){
				unblock_program(files[i],lens[i]);
			}
			return(res);
		
		}

	}
	//first son code
	else{
		// no forbidden access
		pid_t my_pid=getpid();
		get_forbidden_tries(my_pid,log,1);
		if(strcmp(log[0][0]!=NULL){
			printf("ERR 40, forbidden access not initialized properly\n");
		}
		block_program(file1,len1);
		block_program(file2,len2);
		
		// forbidden acess to file 1
		execv(file1,arg1);
		get_forbidden_tries(my_pid,log,2);
		if(strcmp(file1,log[0])!=0 || log[1][0]!=NULL){
			printf("ERR 41, forbidden access not logged properly\n");
		}
		// 2nd forbidden to file1
		execv(file1,arg1);
		get_forbidden_tries(my_pid,log,2);
		if(strcmp(file1,log[0])!=0 || strcmp(file1,log[0])!=0){
			printf("ERR 42, forbidden access not logged properly\n");
		}
		// forbidden to file2
		execv(file2,arg2);
		get_forbidden_tries(my_pid,log,4);
		if(strcmp(file1,log[0])!=0 || strcmp(file1,log[1])!=0 || \
			strcmp(file2,log[2])!=0) || log[3][0]!=0{
			printf("ERR 42, forbidden access not logged properly\n");
		}
		
		//legal access to test2
		execv(test2,argt2);
		return(0);
	}

}

int main(int argc, char* argv[])
{

	if (test_block_program()!=1){
		printf("FAILED BLOCK PROGRAM TEST!!!! \n\n\n");
	}
	if (test_unblock_program()!=1){
		printf("FAILED UNBLOCK PROGRAM TEST!!!! \n\n\n");
	}
	if (test_is_program_blocked()!=1){
		printf("FAILED IS PROGRAM BLOCKED TEST!!!! \n\n\n");
	}	
	if (test_get_blocked_count()!=1){
		printf("FAILED GET BLOCKED COUNT TEST!!!! \n\n\n");
	}
	if (test_get_forbidden_tries()!=1){
		printf("FAILED GET FORBIDDEN TRIES TEST!!!! \n\n\n");
	}		

	printf("if you got here with no other messages you passed the test\n");
	return 0;
}




 ////////////////////////////////////////////////////////////////////
 
 
   printf("-before block\n");   
   block_program(file, len);
   printf("-after block\n");
   printf("is_program_blocked=%d\n", is_program_blocked(file, len)); //Should be 1
   printf("\n");
   
   execve(file, arg, NULL); //Shuld fail
   printf("/bin/date is blocked\n");
   printf("\n");
   
   printf("-before unblock\n");
   unblock_program(file, len);
   printf("-after unblock\n");
   printf("is_program_blocked=%d\n", is_program_blocked(file, len)); //Should be 0
   printf("\n");
   
   execve(file, arg, NULL); //This execve shuld succeed
   printf("ERROR\n");
}