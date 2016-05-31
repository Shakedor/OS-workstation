

#include <stdlib.h>
#include <pthread.h>
#include "lock_list.h"
#include <stdio.h>

using namespace std;

//the arguments for the sieve algorithm.

typedef struct ArgThread {
	int N;
	Lock_list<int>* list;
	FILE* prime;
	FILE* out;
}ArgThread;

// get a lock list and a numebr N (th emaximum element of the list)
void SievePerThread(Lock_list<int>* list, int N,FILE* out, FILE* prime) {
	Node<int>* current;
	Node<int>* candidate;
	candidate = current = list->lockFirst();
	int p = **current;
	bool sqr = false;
	bool firstTime = true;
	while ((**candidate)*(**candidate)<=N) {//for each prime candidate until ceil of sqrt N
		current = list->lockCurrent(candidate);
		p = **current;

		while (current){//for each current in a candidate run
			current = list->lockNext(current);
			if (current == NULL){// if end of list
				list->unlockLast();
				break;
			}

			if ((**current % p) == 0){// if current divides p
				if (**current == (p*p)) { // if we reached p^2 mark it and go on
					sqr = true;
					fprintf(out, "prime %d\n", p);
				}
				//get the next and remove node
				fprintf(out, "%d\n", **current);

				list->lockNext(current);
				current = list->doRemove(current);
				list->unlockNext(current);
			}

			//if we passed p^2 without seeing it then other thread handling this candidate
			if (sqr == false && (**current > (p*p))){
				list->unlockPrev(current);
				list->unlockCurrent(current);
				break;
			}

		}//end of current while meaning we reached end of list


		//update candidate
		candidate = list->lockCurrent(candidate);
		candidate = list->lockNext(candidate);
		list->unlockPrev(candidate);
		list->unlockCurrent(candidate);
		sqr = false;

	}

}

void* threadFunc(void* arg);

//wraper for the sieve algorithm.

void* threadFunc(void* arg) {
	ArgThread argTh = *((ArgThread*) arg);
	SievePerThread(argTh.list, argTh.N, argTh.out, argTh.prime);
	fclose(argTh.out);
	return NULL;
}

int main(int argc, char* argv[]) {
	//the number of values
	int N = atoi(argv[1]);

	//the number of threads.
	int T = atoi(argv[2]);
	Lock_list<int>* list = new Lock_list<int>;
	Node<int>* current;

	//initialize the list with the values [1...N].
	for (int i = 2; i <= N; i++){
		list->insertLast(i);
		//TOCHECK  list.unlockCurrent(current);
	}
	//TODO add io channel check
	FILE* prime = fopen("primes.txt", "w");
	FILE* out;

	//an array of all the threads id's.
	pthread_t* threadArray = new pthread_t[T];

	//creating T threads with the right arguments.
	for (int i = 1; i <= T; i++) {
		char filename[sizeof ("thread-0.txt")];
		sprintf(filename, "thread-%d.txt", i);
		out = fopen(filename,"w");
		ArgThread arg = {N, list, prime, out};
		pthread_create((pthread_t*)&threadArray[i], NULL, threadFunc, (void *)&arg);
	}
	for (int i = 1; i <= T; i++) {
		pthread_join(threadArray[i], NULL);
	}
	//TOCHECK where are you waiting for the threads to finish?
	//why are you closing the prime IO and the last out IO.
	
	//Loop over entire list and print primes to prime.txt

	int x = 0;
	Node<int> **p_curr=&current;
	list->iterFirst(p_curr);
	while ((*p_curr)!=NULL) {
		x = ***p_curr;
		fprintf(prime, "%d\n", x);
		list->iterNext(p_curr);
	}

	

	delete[] threadArray;
	delete list;
	
	fclose(prime);

	return 0;
}
