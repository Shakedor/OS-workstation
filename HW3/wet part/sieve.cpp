

#include <stdlib.h>
#include <map>
#include <pthread.h>
#include "lock_list.h"


using namespace std;



// get a lock list and a numebr N (th emaximum element of the list)
void SievePerThread(Lock_list<int> list, int N,FILE* out) {
	Node<int>* current;
	Node<int>* candidate;
	candidate = current = list.lockFirst();
	int p = **current;
	bool sqr = false;

	while (candidate) {//for each prime candidate
		current = list.lockCurrent(candidate);
		p = **current;
		
		while (current){//for each current in a candidate run
			current = list.lockNext(current);
			if (current == NULL){// if end of list
				list.unlockLast();
				break;
			}
			
			if ((**current % p) == 0){// if current divides p
				if (**current == (p*p)) { // if we reached p^2 mark it and go on
					sqr = true;
					fprintf(out, "prime %d\n", p);
				}
				//get the next and remove node
				fprintf(out, "%d\n", **current);

				list.lockNext(current);
				list.doRemove(current);
				list.unlockNext(current);
			}

			//if we passed p^2 without seeing it then other thread handling this candidate
			if (sqr == false && (**current > (p*p))){
				list.unlockPrev(current);
				list.unlockCurrent(current);
				break;
			}
				
		}//end of current while

		//update candidate
		candidate = list.lockCurrent(candidate);
		candidate = list.lockNext(candidate);
		list.unlockPrev(candidate);
		list.unlockCurrent(candidate);
		
	}

}