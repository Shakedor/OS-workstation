

#include <stdlib.h>
#include <map>
#include <pthread.h>
#include "lock_list.h"


using namespace std;



void SievePerThread(Lock_list<int> lock_list, int N) {
	Node<int>* current;
	lock_list.iterFirst(current);
	Node<int>* goback = lock_list.lockNode(current);
	int p = *current;
	bool sqr = false;    // remember if we passed p^2 
	while (current && *current <= N) {
		// check if the current number is divided by the prime candidate 
		if ((*current % p) == 0) {
			if (*current == (p*p)) {
				sqr = true;
			}
			lock_list.removeCurrent();
		}
		// if the sqr is gone then move to the next prime candidate
		if (*current > (p*p) && sqr == false) {
			lock_list.iterNext(goback);
			goback = lock_list.lockNode(current);
			int p = *current;
		}
		lock_list.iterNext(current);
	}

}