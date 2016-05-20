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
#include <pthread.h>

namespace ListExceptions{
	class ElementNotFound {};
}



using namespace std;


template<class T>
class Node{

public:
	T data;
	pthread_mutex_t* lock;
	Node<T>* next;
	Node<T>* prev;


	Node(const T newData){
		this->data = newData;
		this->next = NULL;
		MutexAttr attr;
		pthread_mutexattr_init(pthread_mutexattr_t *attr);
		pthread_mutexattr_settype(pthread_mutexattr_t *attr, int PTHREAD_MUTEX_ERRORCHECK);

		pthread_mutex_init(this->lock,attr);
		pthread_mutexattr_destroy(attr);

	}

	~Node(){
		pthread_mutex_destroy(this->lock);
	}

	T& operator*(){
		return T;
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
		this->current = NULL;
		this->size = 0;
	}

	~Lock_list()
	{
		Node<T>* current = this->first;
		Node<T>* nextNode = NULL;
		while (current != NULL){
			if (current->getNext() != NULL){
				nextNode = current->getNext();
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
	void insertBeforeCurrent(T& data, Node<T> curr);
	void insertAfterCurrent(T& data, Node<T> curr);
	
	//current wil point to prev after removal (lock prev and next)
	void removeCurrent();
	//removes a given node assuming it and its prev and next are locked by current thread.
	//returns a node to the prev unless removed first in which case returns NULL
	Node<T>* doRemove(Node<T>);


	// move iterator 
	//does not protect against corruption
	void iterFirst(Node<T>* curr);
	void iterLast(Node<T>* curr);
	void iterPrev(Node<T>* curr);
	void iterNext(Node<T>* curr);

	//locks node and returns a pointer to it.
	Node<T>* lockCurrent(Node<T>* curr);
	Node<T>* lockPrev(Node<T>* curr);
	Node<T>* lockNext(Node<T>* curr);
	Node<T>* lockFirst();
	Node<T>* lockNext();
	Node<T>* lockNode(Node<T>* node); // not protected against data corruption 

	// unlocks a given node or it prev,next
	void unlockCurrent(Node<T>*);
	void unlockPrev(Node<T>);
	void unlockNext(Node<T>);
	void unlockFirst();
	void unlockNext();
	void unlockNode(Node<T> node); // not protected against data corruption 


};




template<class T>
void Lock_list<T>::insertFirst(T& data){
	Node<T> first = lockFirst();
	insertBeforeCurrent(data);
}

template<class T>
void Lock_list<T>::insertLast(T& data){
	getLast();
	insertAfterCurrent(data);
}

template<class T>
void Lock_list<T>::insertBeforeCurrent(T& data){
	//if has previous aquire current and prev lock in order
	//note that if this node is first it will remain locked after the if statement
	if (current->prev != NULL){
		pthread_mutex_unlock(current->lock);
		pthread_mutex_lock(current->prev->lock);
		pthread_mutex_lock(current->lock);
	}

	//create new node
	Node<t> *newNode =new Node<t>(data);

	//lock it
	pthread_mutex_lock(newNode->lock);

	// link it to current and its prev if it exists
	if (current->prev != NULL){
		current->prev->next = newNode;
		newNode->prev = current->prev;
	}
	else{
		newNode->prev = NULL;
	}

	current->prev = newNode;
	newNode->next = current;
	current = newNode;

	// unlock current and update it to new node

	if (newNode->prev != NULL){
		pthread_mutex_unlock(newNode->prev->lock);
	}
	pthread_mutex_unlock(newNode->next->lock);
	
}

template<class T>
void Lock_list<T>::insertAfterCurrent(T& data){
	//if has next aquire next lock
	//note that if this node is last it will remain locked after the if statement
	if (current->next != NULL){
		pthread_mutex_lock(current->next->lock);
	}

	//create new node
	Node<t> *newNode = new Node<t>(data);

	//lock it
	pthread_mutex_lock(newNode->lock);

	// link it to current and its next if it exists
	if (current->next != NULL){
		current->next->prev = newNode;
		newNode->next = current->next;
	}
	else{
		newNode->next = NULL;
	}

	current->next = newNode;
	newNode->prev = current;
	current = newNode;

	// unlock current and update it to new node

	if (newNode->next != NULL){
		pthread_mutex_unlock(newNode->next->lock);
	}
	pthread_mutex_unlock(newNode->prev->lock);
}


template<class T>
void Lock_list<T>::removeCurrent(){
	//check if current has prevs or nexts
	int hasNext = (current->next != NULL);
	int hasPrev = (current->prev != NULL);
	
	//lock all 3(or less) in order
	if (hasPrev){// if has prev unlock current and aquire prev and current
		pthread_mutex_unlock(current->lock);
		pthread_mutex_lock(current->prev->lock);
		pthread_mutex_lock(current->lock);
	}

	if (hasNext){
		pthread_mutex_lock(current->next->lock);
	}

	//link next and prev to each other if they exist
	if (hasNext&&hasPrev){
		current->next->prev = current->prev;
		current->prev->next = current->next;
	}
	else if (hasNext){//removing first
		current->next->prev = NULL;
		first = current->next;
	}
	else if (hasPrev){//removing last
		current->prev->next= NULL;
		last = current->prev;
	}
	else{//removing only element
		last = first = NULL;
		
	}
	//unlock current and delete it
	//assign next to current

	pthread_mutex_unlock(current->lock);
	current = current->next;
	delete current;
	
	//unlock prev
	if (hasPrev){
		pthread_mutex_unlock(current->lock);
	}
	

}





#endif /* LIST_H_ */
