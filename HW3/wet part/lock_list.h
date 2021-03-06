/*
* list.h
*
*  Created on: Jun 10, 2015
*      Author: Shaked
*/

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <map>

#include <sys/types.h>
#include <pthread.h>

namespace ListExceptions{
	class ElementNotFound {};
}

namespace mutexExceptions{
	class mutex_attr_init_failed {};
	class mutex_attr_setType_failed {};
	class mutex_init_failed {};
}

using namespace std;


template<class T>
class Node{

public:
	T data;
	pthread_mutex_t* lock,mutex;
	Node<T>* next;
	Node<T>* prev;


	Node(const T newData){
		this->data = newData;
		this->next = NULL;
		this->prev = NULL;
		this->lock = &mutex;
		int res = 0;
		pthread_mutexattr_t attr;

		res = pthread_mutexattr_init(&attr);
		if (res != 0){
			throw mutexExceptions::mutex_attr_init_failed();

		}

		res = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
		if (res != 0){
			throw mutexExceptions::mutex_attr_setType_failed();

		}

		res = pthread_mutex_init(lock,&attr);
		if (res != 0){
			throw mutexExceptions::mutex_init_failed();

		}

		pthread_mutexattr_destroy(&attr);

	}

	~Node(){
		pthread_mutex_destroy(lock);
		
	}

	T& operator*(){
		return data;
	}

};



template<class T>
class Lock_list{
	friend class Node<T>;
	Node<T>* first;
	Node<T>* last;
	int size;

public:
	Lock_list()
	{
		this->first = NULL;
		this->last = NULL;
		
		this->size = 0;
	}

	~Lock_list()
	{
		Node<T>* current = this->first;
		Node<T>* nextNode = NULL;
		while (current != NULL){
			if (current->next != NULL){
				nextNode = current->next;
				delete current;
				current = nextNode;
			}
			else{
				delete current;
				current = NULL;
			}

		}
	}

	/**
	* returns the size of the list
	*/
	int getSize()
	{
		return this->size;
	}

	
	
	// insertions
	void insertFirst(T& data);
	void insertLast(T& data);
	void insertBeforeCurrent(T& data, Node<T>* curr);
	void insertAfterCurrent(T& data, Node<T>* curr);
	
	//current wil point to prev after removal (lock prev and next)
	Node<T>* remove(Node<T>* curr);
	//removes a given node assuming it and its prev and next are locked by current thread.
	//returns a node to the prev unless removed first in which case returns NULL
	Node<T>* doRemove(Node<T>* curr);


	// move iterator 
	//does not protect against corruption
	void iterFirst(Node<T>** curr);
	void iterLast(Node<T>** curr);
	void iterPrev(Node<T>** curr);
	void iterNext(Node<T>** curr);

	//locks node and returns a pointer to it.
	Node<T>* lockCurrent(Node<T>* curr);
	Node<T>* lockPrev(Node<T>* curr);
	Node<T>* lockNext(Node<T>* curr);
	Node<T>* lockFirst();
	Node<T>* lockLast();


	// unlocks a given node or it prev,next
	void unlockCurrent(Node<T>* curr);
	void unlockPrev(Node<T>* curr);
	void unlockNext(Node<T>* curr);
	void unlockFirst();
	void unlockLast();
	


};




template<class T>
void Lock_list<T>::insertFirst(T& data){
	// if no first make only node
	//if first lock it and then make new node and link it.
	int isOnlyNode = (first == NULL);
	if (!isOnlyNode){
		pthread_mutex_lock(first->lock);
	}

	
	//create new node
	Node<T> *newNode = new Node<T>(data);

	//lock it
	pthread_mutex_lock(newNode->lock);

	if (isOnlyNode){// if onlynode update last
		last = newNode;
	}

	if (!isOnlyNode){//if not alone link 
		newNode->next = first;
		first->prev = newNode;
		
	}
	//update first
	first = newNode;
	//unlock
	pthread_mutex_unlock(newNode->lock);
	if (!isOnlyNode){
		pthread_mutex_unlock(newNode->next->lock);
	}

	size++;
	return;
}

template<class T>
void Lock_list<T>::insertLast(T& data){
	// if no last make only node
	//if last lock it and then make new node and link it.
	int isOnlyNode = (last == NULL);
	if (!isOnlyNode){
		pthread_mutex_lock(last->lock);
	}


	//create new node
	Node<T> *newNode = new Node<T>(data);

	//lock it
	pthread_mutex_lock(newNode->lock);

	if (isOnlyNode){// if onlynode update last
		first = newNode;
	}

	if (!isOnlyNode){//link 
		newNode->prev = last;
		last->next = newNode;
		
	}
	//update last
	last = newNode;

	//unlock

	if (!isOnlyNode){
		pthread_mutex_unlock(newNode->prev->lock);
	}
	pthread_mutex_unlock(newNode->lock);
	size++;
	return;
}

template<class T>
void Lock_list<T>::insertBeforeCurrent(T& data, Node<T>* curr){
	//if has previous aquire current and prev lock in order
	//note that if this node is first it will remain locked after the if statement
	if (curr->prev != NULL){
		pthread_mutex_unlock(curr->lock);
		pthread_mutex_lock(curr->prev->lock);
		pthread_mutex_lock(curr->lock);
	}

	//create new node
	Node<T> *newNode =new Node<T>(data);

	//lock it
	pthread_mutex_lock(newNode->lock);

	// link it to current and its prev if it exists
	if (curr->prev != NULL){
		curr->prev->next = newNode;
		newNode->prev = curr->prev;
	}
	else{
		newNode->prev = NULL;
	}
	
	// unlock current and update it to new node
	curr->prev = newNode;
	newNode->next = curr;
	curr = newNode;



	if (newNode->prev != NULL){
		pthread_mutex_unlock(newNode->prev->lock);
	}
	pthread_mutex_unlock(newNode->next->lock);
	size++;
	return;
	
}

template<class T>
void Lock_list<T>::insertAfterCurrent(T& data, Node<T>* curr){
	//if has next aquire next lock
	//note that if this node is last it will remain locked after the if statement
	if (curr->next != NULL){
		pthread_mutex_lock(curr->next->lock);
	}

	//create new node
	Node<T> *newNode = new Node<T>(data);

	//lock it
	pthread_mutex_lock(newNode->lock);

	// link it to current and its next if it exists
	if (curr->next != NULL){
		curr->next->prev = newNode;
		newNode->next = curr->next;
	}
	else{
		newNode->next = NULL;
	}
	// unlock current and update it to new node
	curr->next = newNode;
	newNode->prev = curr;
	curr = newNode;

	

	if (newNode->next != NULL){
		pthread_mutex_unlock(newNode->next->lock);
	}
	pthread_mutex_unlock(newNode->prev->lock);
	size++;
	return;
}


template<class T>
Node<T>* Lock_list<T>::remove(Node<T>* curr){
	//check if current has prevs or nexts
	int hasNext = (curr->next != NULL);
	int hasPrev = (curr->prev != NULL);
	
	//lock all 3(or less) in order
	if (hasPrev){// if has prev unlock current and aquire prev and current
		pthread_mutex_unlock(curr->lock);
		pthread_mutex_lock(curr->prev->lock);
		pthread_mutex_lock(curr->lock);
	}

	if (hasNext){
		pthread_mutex_lock(curr->next->lock);
	}

	return doRemove(curr);
	

}

template<class T>
Node<T>* Lock_list<T>::doRemove(Node<T>* curr){
	//check if current has prevs or nexts
	int hasNext = (curr->next != NULL);
	int hasPrev = (curr->prev != NULL);


	//link next and prev to each other if they exist
	if (hasNext&&hasPrev){
		curr->next->prev = curr->prev;
		curr->prev->next = curr->next;
	}
	else if (hasNext){//removing first
		curr->next->prev = NULL;
		first = curr->next;
	}
	else if (hasPrev){//removing last
		curr->prev->next = NULL;
		last = curr->prev;
	}
	else{//removing only element
		last = first = NULL;

	}
	//unlock current and delete it
	//assign next to current
	Node<T> *temp = curr->prev;		// will be null if no prev
	
	pthread_mutex_unlock(curr->lock);
	delete curr;
	size--;
	return temp;
}

template<class T>
void Lock_list<T>::iterFirst(Node<T>** curr){
	*curr = first;

}

template<class T>
void Lock_list<T>::iterLast(Node<T>** curr){
	*curr = last;

}

template<class T>
void Lock_list<T>::iterPrev(Node<T>** curr){
	*curr = (*curr)->prev;

}

template<class T>
void Lock_list<T>::iterNext(Node<T> **curr){
	*curr = (*curr)->next;

}

template<class T>
Node<T>* Lock_list<T>::lockCurrent(Node<T>* curr){
	if (!curr){
		return NULL;
	}
	pthread_mutex_lock(curr->lock);
	return curr;
}

template<class T>
Node<T>* Lock_list<T>::lockPrev(Node<T>* curr){
	if (!curr){
		return NULL;
	}
	if (!curr->prev){
		return NULL;
	}
	pthread_mutex_lock(curr->prev->lock);
	return curr->prev;
}

template<class T>
Node<T>* Lock_list<T>::lockNext(Node<T>* curr){
	if (!curr){
		return NULL;
	}
	if (!curr->next){
		return NULL;
	}
	pthread_mutex_lock(curr->next->lock);
	return curr->next;
}

template<class T>
Node<T>* Lock_list<T>::lockFirst(){
	if (!first){
		return NULL;
	}
	pthread_mutex_lock(first->lock);
	return first;
}

template<class T>
Node<T>* Lock_list<T>::lockLast(){
	if (!last){
		return NULL;
	}
	pthread_mutex_lock(last->lock);
	return last;
}

template<class T>
void Lock_list<T>::unlockCurrent(Node<T>* curr){
	if (!curr){
		return ;
	}
	pthread_mutex_unlock(curr->lock);
	return ;
}

template<class T>
void Lock_list<T>::unlockPrev(Node<T>* curr){
	if (!curr){
		return ;
	}
	if (!curr->prev){
		return ;
	}
	pthread_mutex_unlock(curr->prev->lock);
	return ;
}

template<class T>
void Lock_list<T>::unlockNext(Node<T>* curr){
	if (!curr){
		return ;
	}
	if (!curr->next){
		return ;
	}
	pthread_mutex_unlock(curr->next->lock);
	return ;
}

template<class T>
void Lock_list<T>::unlockFirst(){
	if (!first){
		return ;
	}
	pthread_mutex_unlock(first->lock);
	return ;
}

template<class T>
void Lock_list<T>::unlockLast(){
	if (!last){
		return ;
	}
	pthread_mutex_unlock(last->lock);
	return;
}




#endif /* LIST_H_ */
