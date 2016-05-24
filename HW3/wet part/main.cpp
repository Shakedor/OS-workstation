

#include <stdlib.h>
#include <pthread.h>
#include "lock_list.h"

using namespace std;

struct ArgThread {
	int N;
	
};

int main(int argc, char* argv[]) {
	int N = atoi(argv[1]);
	int T = atoi(argv[2]);
	List<int> list = new List<int>();
	Node<int>* current;
	for (int i = 2; i <= N; i++){
		current = list.insertLast(i); 
		list.unlockCurrent(current);
	}
	FILE* prime = fopen("prime", "w");
	for (int i = 1; i <= T; i++) {

	}
	return 0;
}