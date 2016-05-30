

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
}

int main(int argc, char* argv[]) {
	//the number of values
	int N = atoi(argv[1]);

	//the number of threads.
	int T = atoi(argv[2]);
	List<int> list = new List<int>();//TOCHECK new int returns int*
	Node<int>* current;

	//initialize the list wwith the values [1...N].
	for (int i = 2; i <= N; i++){
		current = list.insertLast(i); 
		list.unlockCurrent(current);
	}
	FILE* prime = fopen("prime", "w");
	FILE* out;

	//an array of all the threads id's.
	int* threadArray = new int[T];//TOCHECK same here about the new

	//creating T threads with the right arguments.
	for (int i = 1; i <= T; i++) {
		char filename[sizeof ("file100.txt")];
		sprintf(filename, "thread-%d.txt", i);
		out = fopen(filename,"w");
		ArgThread arg = {N, list, prime, out};
		pthread_create(threadArray[i], NULL, threadFunc, (void *)ArgThread); //TOCHECK shouldnt you pass the arg instance of ArgThread
	}

	//TOCHECK where are you waiting for the threads to finish?
	//why are you closing the prime IO and the last out IO.

	delete[] threadArray;
	fclose(prime);
	fclose(out);
	return 0;
}
