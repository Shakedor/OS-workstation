#include <linux/ctype.h>
#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>  	
#include <linux/slab.h>
#include <linux/fs.h>       		
#include <linux/errno.h>  
#include <linux/types.h> 
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/types.h> 		// needed for
#include <linux/signal.h>		// using signals
#include <linux/ioctl.h>		// for ioctl constants
#include <asm/termios.h>
#include <linux/random.h>
#include "my_module.h"
#include "sha1.h"
#include "mix.h"
#include <linux/wait.h>
#include <stdint.h>


#define MY_MODULE_NAME "MY_MODULE"

#define MY_MAJOR 62

MODULE_LICENSE("GPL");

MODULE_AUTHOR( "Dean and Aviad" );

/* globals */

int entropy_count=0;
char* entropy_pool = NULL;
DECLARE_WAIT_QUEUE_HEAD(my_waitqueue);
//TODO maybe lock the pool and count



#define READ_CHUNK_SIZE 20
#define WRITE_CHUNK_SIZE 64


//declaration of functions
int init_module(void);
void cleanup_module(void);
int my_open( struct inode *inode, struct file *filp );
int my_release( struct inode *inode, struct file *filp );
ssize_t my_read( struct file *filp, char *buf, size_t count, loff_t *f_pos );
ssize_t my_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
int my_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
static int get_current_entropy(int* p);
static int clear_pool();
static int add_entropy(struct inode *inode, struct file *filp,struct rand_pool_info *p);

struct file_operations my_fops = {
.open= my_open,
.release=my_release,
.read= my_read,
.write= my_write,
.llseek= NULL,
.ioctl= my_ioctl,
.owner= THIS_MODULE,
};



int init_module( void ) {
	//change to static loading
	register_chrdev( MY_MAJOR, MY_MODULE_NAME, &my_fops );
	
	// allocate entropy pool
	entropy_pool=kmalloc(512*sizeof(char),GFP_KERNEL);
	
	//check malloc
	if (entropy_pool == NULL) {
		return -EFAULT;
	}
	
	// init it and counter to 0
	int i;
	for (i =0; i<512;i++){
		entropy_pool[i]=0;
	}

	init_waitqueue_head(&my_waitqueue);
	// filp->private_data = &my_waitqueue;
	
	entropy_count=0;
	
	
	
	return 0;
}


void cleanup_module( void ) {
	//change to static unloading

	unregister_chrdev( MY_MAJOR, MY_MODULE_NAME);
	
	// close the wait queue
	wake_up_interruptible(&my_waitqueue);

	//free entropy pool
	kfree(entropy_pool);
	return;
}

int my_open( struct inode *inode, struct file *filp ) {
	//initialize waitqueue
	// and condition
	return 0;
}

int my_release( struct inode *inode, struct file *filp ) {
	//destroy and clear waitqueue
	// and condition
	return 0;
}


ssize_t my_read( struct file *filp, char *buf, size_t count, loff_t *f_pos ) {
	//if size is 0 return 0
	if(count <=0){
		return 0;
	}
	
	// if entropy_count less than 8, add yourself to waitqueue
	if (entropy_count < 8){
		 wait_event_interruptible(my_waitqueue, entropy_count>=8);
	}
	// upon waking up, check that the entropy count is right and thus you were 
	//wakened by correct signal, else return -ERESTARTSYS.

	if(signal_pending(current)){
		return -ERESTARTSYS;
	}

	if(entropy_count<=8){
		return -ERESTARTSYS;
	}
	
	//now we have at least 8 entropy
	// get min of buf and entropy bytes
	
	int E = entropy_count/8 ; // int division so it will floor the result
	E =  (E < count ) ? E : count ;
	
	//subtract 8n from entropy count
	entropy_count -= 8*E;
	
	//dividing buffer to 20 bytes chunks
	//get num of chunks
	int num_chunks= E/READ_CHUNK_SIZE + (READ_CHUNK_SIZE - 1 + E%READ_CHUNK_SIZE)/READ_CHUNK_SIZE ; 
	//get last chunk size
	int last_chunk_size=E%READ_CHUNK_SIZE;
	//make tmp buffer of size 21
	char tmp[READ_CHUNK_SIZE+1]={0}; //TODO check this initializes buffer properly
	int status=0;
	int chunk_size=READ_CHUNK_SIZE;
	//loop for each chunk in buffer
	int i,j;
	for(i = 0; i < num_chunks; i++){
		//reset tmp
		for(j=0;j<READ_CHUNK_SIZE+1;j++){
			tmp[j]=0;
		}
		
		//hash_pool
		hash_pool (entropy_pool, tmp);
		//mix tmp 20 pooldata
		mix (tmp, READ_CHUNK_SIZE, entropy_pool);
		
		
		//copy tmp to chunk of buf with correct size
		//if failed return -EFAULT		
		if(i+1==num_chunks){//last chunk
			chunk_size=last_chunk_size;
		}
		status=copy_to_user((void*)(intptr_t)buf[i+READ_CHUNK_SIZE], (void*)(intptr_t)tmp, chunk_size);
		
		if(status<0){
			return -EFAULT;
		}
	}		
	//return sum of size of chunks
	
	return E;

}

ssize_t my_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
	int E = entropy_count/8 ;
	//divide buf to 64 bytes chunks
	//get num of chunks
	int num_chunks= E/WRITE_CHUNK_SIZE + (WRITE_CHUNK_SIZE - 1 + E%WRITE_CHUNK_SIZE)/WRITE_CHUNK_SIZE ; 
	//get last chunk size
	int last_chunk_size=E%WRITE_CHUNK_SIZE;
	//make tmp buffer of size 65
	char tmp[WRITE_CHUNK_SIZE+1]={0}; //TODO check this initializes buffer properly
	int status=0;
	int chunk_size=WRITE_CHUNK_SIZE;
	
	//loop for each chunk in buffer
	int i;
	for(i = 0; i < num_chunks; i++){
		//copy from user the chunk
		if(i+1==num_chunks){//last chunk
			chunk_size=last_chunk_size;
		}
		status = copy_from_user((void*)(intptr_t)tmp, (void*)(intptr_t)buf[i + READ_CHUNK_SIZE], chunk_size);
		//if copy from user failed return -EFAULT
		if(status<0){
			return -EFAULT;
		}
		//mix each chunk
		mix (tmp, READ_CHUNK_SIZE, entropy_pool);
		
	}
	//return n
	return count;
	
	
	//TODO check desired state of pooldata if non first chunk failed to copy
}


int my_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg) {
	int status=0;
	switch( cmd ) {//TODO change ioctl
		case RNDGETENTCNT:
		status=get_current_entropy((int*)arg);
		break;
		case RNDCLEARPOOL:
		status=clear_pool();
		break;
		case RNDADDENTROPY:
		status=add_entropy(inode,filp,(struct rand_pool_info*)arg);
		break;
		default: 
		return -EINVAL;
	}
	
	return status;
}

static int get_current_entropy(int* p){
	// check if valid user space
	//else return EFAULT	
	// put entropy count in p
	//return 0
	
	int status = 0;
	status=copy_to_user(p,&entropy_count,sizeof(int));
	if(status<0){
		return -EFAULT;
	}
	
	return status;
	
}


static int clear_pool(){
	// check if you are admin privilage
	//if not return _EPERM
	if(capable(CAP_SYS_ADMIN)==0){
		return -EPERM;
	}
	
	// reset entropy count // maybe need lock
	
	entropy_count=0;
	return 0;
	
	
}


static int add_entropy(struct inode *inode, struct file *filp,struct rand_pool_info *p){
	// check if you are admin privilage
	//if not return _EPERM
	//printk("in add_entropy\n");
	if(capable(CAP_SYS_ADMIN)==0){
		//printk("failed ADMIN TEST \n");
		return -EPERM;
	}
	//read data from rand_pool_info, check that its valid.
	// else return -EFAULT
	struct rand_pool_info my_p;
	
	int status = 0;
	status=copy_from_user(&my_p,p,sizeof(struct rand_pool_info));
	if(status<0){
		//printk("failed copy to usr \n");
		return -EFAULT;
	}
	
	
	
	if(my_p.entropy_count < 0){
		printk("failed entropy count param \n");
		return -EINVAL;
	}
	
	//increase entropy_count by p->entropy_count
	//ceil it to 4096
	entropy_count+=my_p.entropy_count;
	if(entropy_count>4096){
		entropy_count=4096;
	}
	
	// do write with p->buff and p->buff size
	status = my_write(filp, (const char*)p->buf, p->buf_size, &(filp->f_pos));
		
	//wake up waiting processes	
	wake_up_interruptible(&my_waitqueue);
	//printk("failed performed my write, status is %d \n",status);
	return status;
	
}
















