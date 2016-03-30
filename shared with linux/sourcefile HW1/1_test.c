#include "blocker.h"
#include <unistd.h>

int main(int argc, char* argv[])
{
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
	   if(0 != is_program_blocked(files[i],lens[i])){
		   printf(" Err1 programs not blocked initially\n");
	   }
   }
   
   if( get_blocked_count()!=0){
	   printf("Err2 program counter not initialized properly \n");
   }
   
   if( block_program(file1,0)!=-1){
	   printf("Err3 should fail for param reason\n");
   }
   if (erno != EINVAL){
	   printf("Err 3.5 erno should be 1\n")
   }
   
   if( block_program(NULL,1)!=-1){
	   printf("Err4 should fail for param reason\n");
   }
   
   if (erno != EINVAL){
	   printf("Err 3.5 erno should be 1\n")
   }
   
   if( block_program(file1,len2)!=-1){
	   printf("Err4.6 should fail for string size violation reason\n");
   }
   
   if( block_program(file1,len1)!=1){
	   printf("Err5 should have succeeded\n");
   }
   
   if( block_program(file1,len)!=0){
	   printf("Err6 should know prog was already blocked\n");
   }
   
   if( block_program(file3,len3)!=1){
	   printf("Err7 should have succeeded\n");
   }
   
   if( block_program(file3,len3)!=0){
	   printf("Err8 should know prog was already blocked\n");
   }
   
   for(int i=0,i<4,i++){
	   if( (is_program_blocked(files[i],lens[i])+i%2)%2 !=0 ){
		   printf(" Err9 programs not blocked correctly\n");
	   }
   }
   
   
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