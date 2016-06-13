
#include <pthread.h>

int number_of_readers;
pthread_cond_t readers_condition;
int number_of_writers;
pthread_cond_t writers_condition;
mutex_t global_lock;
// bool says if a thread was selected to become the only one that can upgrade.
// upgradable stores the thread id of the thread that is upgradable.
// we use bool instead of looking for a NULL like initial value for upgradeable.
pthread_t upgradeAble;
bool upgraded;
// this condition checks when we can upgrade
pthread_cond_t R_to_W_condition;

void readers_writers_init() {
	number_of_readers = 0;
	pthread_cond_init(&readers_condition, NULL);
	number_of_writers = 0;
	pthread_cond_init(&writers_condition, NULL);
	pthread_mutex_init(&global_lock, NULL);
	upgraded = false;
	
}

bool upgrade_to_write_lock()
{
	pthread_mutex_lock(&global_lock);
	//if no upgraAble was chosen, choose it
	if (!upgraded){
		upgradeAble = pthread_self();
		upgraded = true;
	}
	//if the current thread is not upgradable, return 0;
	if (!pthread_equal(upgradeAble, pthread_self())){
		pthread_mutex_unlock(&global_lock);
		return false;
	}

	if (number_of_writers > 0){
		// cant be a reader since there is a writer active so either we are non readers or are the writer itself
		return false;
	}

	while (number_of_readers != 1 && number_of_writers != 0){
		pthread_cond_wait(&R_to_W_condition, &global_lock);
	}

	number_of_writers++;
	number_of_readers--;
	pthread_mutex_unlock(&global_lock);
	return true;


}




void read_lock() {
	pthread_mutex_lock(&global_lock);
	while (number_of_writers > 0)
		pthread_cond_wait(&readers_condition, &global_lock);
	number_of_readers++;
	pthread_mutex_unlock(&global_lock);
}


void read_unlock() {
	pthread_mutex_lock(&global_lock);
	number_of_readers--;
	if (number_of_readers == 0)
		pthread_cond_signal(&writers_condition);
	//might need to signal to upgrade if only 1 reader
	else if (number_of_readers == 1)
		pthread_cond_signal(&R_to_W_condition);

	pthread_mutex_unlock(&global_lock);
}

void write_lock() {
	pthread_mutex_lock(&global_lock);
	while ((number_of_writers > 0) || (number_of_readers > 0))
		pthread_cond_wait(&writers_condition, &global_lock);
	number_of_writers++;
	pthread_mutex_unlock(&global_lock);
}

void write_unlock() {
	pthread_mutex_lock(&global_lock);
	number_of_writers--;
	if (number_of_writers == 0) {
		if (number_of_readers == 1)
			pthread_cond_signal(&R_to_W_condition);
		pthread_cond_broadcast(&readers_condition);
		pthread_cond_signal(&writers_condition);
	}
	pthread_mutex_unlock(&global_lock);
}
