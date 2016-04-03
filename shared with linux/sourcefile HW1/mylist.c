
/*
#include <stdio.h>
#include <stdlib.h>
#include "mylist.h"
#include <assert.h>

*/
#include <linux/mylist.h>
#include <linux/slab.h>

typedef struct node_t* Node;

struct list_t{
	Node head;
        Node current;
};
struct node_t{
	char* name;
	struct node_t* next;
	struct node_t* previous;
};

static Node allocNode(const char *string);

static void deallocNode(Node node);

static Node listGetLast(List l);

List listCreate(void){
	//List l = malloc(sizeof(*l));
	List l = kmalloc(*l), GFP_KERNEL);
	if(!l)
		return NULL;
	l->head = NULL;
l->current = NULL;
	return l;
}
ListResult listAddString(List l, const char *string){
  if(!string || !l)
    return LIST_NULL_ARGUMENT;
  if(!l->head){
	  l->head = allocNode(string);
    if(!l->head)
    	return LIST_OUT_OF_MEMORY;
  } else {
    Node temp = listGetLast(l);
    temp->next = allocNode(string);
    (temp->next)->previous = temp;
  }
  return LIST_SUCCESS;
}

int listGetSize(List l){
	if(!l)
		return LIST_NULL_ARGUMENT;
  int count = 0;
  Node tmp = l->head;
  for( ;tmp; tmp = tmp->next){
    count++;
  }
  return count;
}

ListResult listClear(List l){
	if(!l){
	   return LIST_NULL_ARGUMENT;
	}
	Node t = listGetLast(l);
	for(; t != l->head ; ){
	  Node temp = t;
	  t = t->previous;
	  t->next = NULL;
	  deallocNode(temp);
	}
	l->head = NULL;
	deallocNode(t);
	return LIST_SUCCESS;
}

static Node listGetLast(List l){
  if(!(l->head))
    return NULL;
  Node t = l->head;
  for( ;t ; t = t->next){
    if(!t->next)
      break;
  }
  return t;
}

ListResult listRemoveString(List l, const char *string){
  if( !l || !string )
    return LIST_NULL_ARGUMENT;
  Node t = l->head;
  for( ; t != NULL; t = t->next){
    if(!strcmp(t->name, string)){
      if(t->previous)
    	(t->previous)->next = t->next;
      if(t->next)
    	  (t->next)->previous = t->previous;
      if( t == l->head )
    	  l->head = t->next;
      deallocNode(t);
      return LIST_SUCCESS;
    }
  }
  return LIST_DOESNT_EXIST;
}

void listDestroy(List l){
	if(!l){
	  return;
	}
	listClear(l);
	//assert(l);
	kfree(l);
	//free(l);
	return;
}


ListResult isInList(List l, const char* string){
  if(!l || !string)
    return LIST_NULL_ARGUMENT;
  Node t = l->head;
  for( ; t; t = t->next){
   if(!strcmp(t->name, string))
      return LIST_IS_IN;
  }
   return LIST_DOESNT_EXIST;
}

char* listGetFirst(List l){
  if(!l)
    return NULL;
  l->current = l->head;
  return l->current ? (l->current)->name : NULL;

}

char* listGetNext(List l){
  if(!l)
    return NULL;
  if(l->current)
    l->current = (l->current)->next;
  return l->current ? (l->current)->name : NULL;

}

unsigned int mymax(unsigned int  a, unsigned int b){
  return a >= b ? a : b;
}

unsigned int mymin(unsigned int  a, unsigned int b){
  return a > b ? b : a;
}

static Node allocNode(const char *string){
 if(!string)
   return NULL;
 Node tmp = kmalloc(sizeof(*tmp), GFP_KERNEL);
 //Node tmp = malloc(sizeof(*tmp));
 if(!tmp)
   return NULL;
 tmp->name = kmalloc(sizeof(char)*(strlen(string) + 1), GFP_KERNEL);
 //tmp->name = malloc(sizeof(char)*(strlen(string) + 1));
 if(!tmp->name){
   kfree(tmp);
	 //free(tmp);
	 return NULL;
 }
 strcpy(tmp->name, string);
 tmp->previous = NULL;
 tmp->next = NULL;
 return tmp;
}

static void deallocNode(Node node){
	if(!node)
		return;
	kfree(node->name);
	//free(node->name);
	kfree(node);
	//free(node);
}
