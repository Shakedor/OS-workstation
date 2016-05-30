#include "lock_list.h"
#include "testsMacros.h"

int test_size(){
	printf("a\n");
	Lock_list<int> list;
	Node<int>* node = NULL;
	printf("a\n");
	int x = 5;
	ASSERT_TEST(list.getSize()== 0);
	list.insertFirst(x);
	printf("a\n");
	ASSERT_TEST(list.getSize() == 1);
	list.insertFirst(x);
	ASSERT_TEST(list.getSize() == 2);
	list.insertFirst(x);
	ASSERT_TEST(list.getSize() == 3);
	list.insertFirst(x);
	list.insertFirst(x);
	ASSERT_TEST(list.getSize() == 5);
	list.iterLast(&node);
	ASSERT_TEST(node!=NULL);
	ASSERT_TEST(**node == 5);
	list.doRemove(node);
	ASSERT_TEST(list.getSize() == 4);
	list.iterLast(&node);
	list.doRemove(node);
	list.iterLast(&node);
	list.doRemove(node);
	ASSERT_TEST(list.getSize() == 2);
	list.iterLast(&node);
	list.doRemove(node);
	list.iterLast(&node);
	list.doRemove(node);
	ASSERT_TEST(list.getSize() == 0);

	return 1;

}


int test_insertion1(){
	Lock_list<int> list;
	Node<int>* node;
	for (int i = 0; i < 10; i++){
		list.insertLast(i);
	}
	list.iterFirst(&node);
	for (int i = 0; i < 10; i++){
		ASSERT_TEST(**node== i);
		list.iterNext(&node);
	}

	ASSERT_TEST(node==NULL);

	return 1;
}

int test_insertion2(){
	Lock_list<int> list;
	Node<int>* node;
	for (int i = 0; i < 10; i++){
		list.insertFirst(i);
	}
	list.iterFirst(&node);
	for (int i = 9; i >=0; i--){
		ASSERT_TEST(**node== i);
		list.iterNext(&node);
	}

	ASSERT_TEST(node==NULL);
	
	return 1;
}


int test_insertion3(){
	Lock_list<int> list;
	Node<int>* node;
	int x = 1;
	
	list.insertFirst(x);
	node = list.lockFirst();
	//1
	ASSERT_TEST(**node == 1);
	list.unlockCurrent(node);
	x = 3;
	list.insertBeforeCurrent(x, node);
	node = list.lockPrev(node);
	//3,1
	ASSERT_TEST(**node == 3);
	list.unlockCurrent(node);
	node = list.lockNext(node);
	ASSERT_TEST(**node == 1);
	list.unlockCurrent(node);

	x = 5;
	list.insertAfterCurrent(x, node);
	node = list.lockNext(node);
	//3,1,5
	ASSERT_TEST(**node == 5);
	list.unlockCurrent(node);

	return 1;



}

int test_doremove(){
	Lock_list<int> list;
	Node<int>* node;
	for (int i = 0; i < 10; i++){
		list.insertLast(i);
	}
	// 0, ... , 9

	node = list.lockFirst();
	list.unlockCurrent(node);
	node = list.lockNext(node);
	list.unlockCurrent(node);
	node = list.lockNext(node); // should point to 2
	ASSERT_TEST(**node == 2);
	// 0,1,3...,9
	node=list.doRemove(node);
	ASSERT_TEST(**node == 1);
	node = list.lockNext(node);
	ASSERT_TEST(**node == 3);
	list.unlockCurrent(node);
	
	node = list.lockLast();
	ASSERT_TEST(**node == 9);
	node = list.lockPrev(node);
	ASSERT_TEST(**node == 8);
	list.unlockNext(node);
	list.unlockCurrent(node);
	// 0,1,3,..,7,9
	node=list.doRemove(node);
	ASSERT_TEST(**node == 7);
	node = list.lockNext(node);
	ASSERT_TEST(**node == 9);
	list.unlockCurrent(node);

	node = list.lockFirst();
	ASSERT_TEST(**node == 0);
	node = list.doRemove(node);
	ASSERT_TEST(node==NULL);

	node = list.lockFirst();
	ASSERT_TEST(**node == 1);
	list.unlockCurrent(node);

	return 1;



}


int main2(){
	printf("0\n");
	RUN_TEST(test_size);
	printf("1\n");
	RUN_TEST(test_insertion1);
	printf("2\n");
	RUN_TEST(test_insertion2);
	printf("3\n");
	RUN_TEST(test_insertion3);
	printf("4\n");
	RUN_TEST(test_doremove);
	printf("5\n");
	return 0;
}
