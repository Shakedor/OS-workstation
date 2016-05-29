#include "lock_list.h"
#include "testsMacros.h"

int test_size(){
	Lock_list<int> list;
	int x = 5;
	ASSERT_TEST(list.getSize()== 0);
	list.insertFirst(x);
	ASSERT_TEST(list.getSize() == 1);
	list.insertFirst(x);
	ASSERT_TEST(list.getSize() == 2);
	list.insertFirst(x);
	ASSERT_TEST(list.getSize() == 3);
	list.insertFirst(x);
	list.insertFirst(x);
	ASSERT_TEST(list.getSize() == 5);
	Node<int>* node;
	list.iterLast(node);
	list.doRemove(node);
	ASSERT_TEST(list.getSize() == 4);
	list.doRemove(node);
	list.doRemove(node);
	ASSERT_TEST(list.getSize() == 2);
	list.doRemove(node);
	list.doRemove(node);
	ASSERT_TEST(list.getSize() == 0);

}


int test_insertion(){
	Lock_list<int> list;
	Node<int>* node;
	for (int i = 0; i < 10; i++){
		list.insertLast(i);
	}
	list.iterFirst(node);
	for (int i = 0; i < 10; i++){
		ASSERT_TEST(**node== i);
		list.iterNext(node);
	}

	ASSERT_TEST(node==NULL);
}





int main(){


	return 0;
}