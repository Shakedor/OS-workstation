

#include <stdlib.h>
#include <pthread.h>
#include "lock_list.h"

using namespace std;

//the arguments for the sieve algorithm.

typedef struct ArgThread {
	int N;
	List<int> list;
	FILE* prime;
	FILE* out;
}ArgThread;

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
	List<int> list = new List<int>();
	Node<int>* current;

	//initialize the list with the values [1...N].
	for (int i = 2; i <= N; i++){
		current = list.insertLast(i); 
		list.unlockCurrent(current);
	}
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
		pthread_create(threadArray[i], NULL, threadFunc, (void *)arg);
	}
	for (int i = 1; i <= T; i++) {
		pthread_join(threadArray[i], NULL);
	}
	//TOCHECK where are you waiting for the threads to finish?
	//why are you closing the prime IO and the last out IO.

	delete[] threadArray;
	delete list;
	delete current;
	fclose(prime);

	return 0;
}
