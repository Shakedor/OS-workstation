
#define MY_MODULE_NAME "MY_MODULE"

MODULE_LICENSE( "GPL" );

MODULE_AUTHOR( "Dean and Aviad" );

/* globals */

int entropy_count=0;
char* entropy_pool = NULL;
//TODO maybe lock the pool and count

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
	my_major = register_chrdev( my_major, MY_MODULE_NAME, &my_fops );
	if( my_major < 0 ) {
		printk( KERN_WARNING "can't get dynamic major\n" );
		return my_major;
	}
	
	// allocate entropy pool
	entropy_pool=kmalloc(512*sizeof(char),GFP_KERNEL);
	
	//check malloc
	
	// init it and counter to 0
	for (int i =0; i<512;i++){
		entropy_pool[i]=0;
	}
	
	entropy_count=0;
	
	
	
	return 0;
}


void cleanup_module( void ) {
	//change to static unloading
	unregister_chrdev( my_major, MY_MODULE_NAME);
	
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
	
	// if entropy_count less than zero, add yourself to waitqueue
	// upon waking up, check that the entropy count is right and thus you were 
	//wakened by correct signal, else return -ERESTARTSYS.

	
	//now we have at least 8 entropy
	// get min of buf and entropy bytes
	
	//subtract 8n from entropy count
	
	//dividing buffer to 20 bytes chunks
	//get num of chunks
	//get last chunk size
	//make tmp buffer of size 21
	
	//loop for each chunk in buffer
		//hash_pool
		
		//mix tmp 20 pooldata
		
		//copy tmp to chunk of buf with correct size
		//if failed return -EFAULT
		
	//return sum of size of chunks

}

ssize_t my_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
	//divide buf to 64 bytes chunks
	//get num of chunks
	//get last chunk size
	//loop for each chunk in buffer
	//copy from user the chunk
	//if copy from user failed return -EFAULT
	
	//mix each chunk

	//TODO check desired state of pooldata if non first chunk failed to copy
}


int my_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg) {
	switch( cmd ) {//TODO change ioctl
		case MY_OP1:
		//handle op1;
		break;
		case MY_OP2:
		//handle op2;
		break;
		default: return -ENOTTY;
	}
	return 0;
}

static int get_current_entropy(int* p){
	// check if valid user space
	//else return EFAULT
	
	// put entropy count in p
	
	//return 0
	
	
}


static int clear_pool(){
	// check if you are admin privilage
	//if not return _EPERM
	
	// reset entropy count // maybe need lock
	
	
}


static int add_entropy(struct inode *inode, struct file *filp,struct rand_pool_info *p){
	// check if you are admin privilage
	//if not return _EPERM
	
	//increase entropy_count by p->entropy_count
	//ceil it to 4096
	
	// do write with p->buff and p->buff size
	// reset entropy count // maybe need lock
	
	
}

















