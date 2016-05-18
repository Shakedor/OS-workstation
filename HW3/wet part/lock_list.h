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
class List;

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


};



template<class T>
class Lock_list{
	friend class Node<T>;
	Node<T>* first;
	Node<T>* last;
	Node<T>* current;
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

	T& getFirst();
	T& getLast();
	T& getNext();
	T& getPrev();
	void unlockCurrent();
	// current wil point to new data
	void insertFirst(T& data);
	void insertLast(T& data);
	void insertBeforeCurrent(T& data);
	void insertAfterCurrent(T& data);
	//current wil point to next after removal
	void removeCurrent();




};

template<class T>
T& Lock_list<T>::getFirst(){
	// lock first node and get data
	
	if (first == NULL){
		throw ListExceptions::ElementNotFound;
	}
	current = first;
	pthread_mutex_lock(first->lock);
	return first->data;
}

template<class T>
T& Lock_list<T>::getLast(){
	// lock first node and get data

	if (last == NULL){
		throw ListExceptions::ElementNotFound;
	}
	current = last;
	pthread_mutex_lock(last->lock);
	return last->data;
}

template<class T>
T& Lock_list<T>::getNext(){
	//update current than hand over hand lock
	if (current->next == NULL){
		pthread_mutex_unlock(current->lock);
		throw ListExceptions::ElementNotFound;
	}
	current = current->next;
	pthread_mutex_lock(current->lock);
	pthread_mutex_unlock(current->prev->lock);
	return current->data;
}

template<class T>
T& Lock_list<T>::getprev(){
	//update current than hand over hand lock
	if (current->prev == NULL){
		pthread_mutex_unlock(current->lock);
		throw ListExceptions::ElementNotFound;
	}
	current = current->prev;
	pthread_mutex_lock(current->lock);
	pthread_mutex_unlock(current->next->lock);
	return current->data;
}

template<class T>
void Lock_list<T>::unlockCurrent(){
	//unlock current
	if (current == NULL){
		return;
	}
	pthread_mutex_unlock(current->lock);

}

template<class T>
void Lock_list<T>::insertFirst(T& data){
	getFirst();
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
