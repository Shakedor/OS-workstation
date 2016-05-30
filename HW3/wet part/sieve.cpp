

#include <stdlib.h>
#include <pthread.h>
#include "lock_list.h"
#include <stdio.h>


using namespace std;
/*
// get a lock list and a numebr N (th emaximum element of the list)
void SievePerThread(Lock_list<int> list, int N,FILE* out, FILE* prime) {
	Node<int>* current;
	Node<int>* candidate;
	candidate = current = list.lockFirst();
	int p = **current;
	bool sqr = false;
	bool firstTime = true;
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
					if (firstTime == true) {
						fprintf(out, "prime %d", p);
						firstTime = false;
					}else {
						fprintf(out, "\nprime %d", p);
					}
					if (p == 2) {
						fprintf(prime, "%d", p);
					}else {
						fprintf(prime, "\n%d", p);
					}
				}
				//get the next and remove node
				fprintf(out, "\n%d", **current);

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
*/
