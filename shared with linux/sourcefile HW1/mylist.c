
/*
#include <stdio.h>
#include <stdlib.h>
#include "mylist.h"
#include <assert.h>

*/
#include <linux/mylist.h>
#include <linux/slab.h>

typedef struct node_t* Node;

// flag for conditional compilation
//#define DEBUG_list



struct list_t{
	Node head;
        Node iter;
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

	#ifdef DEBUG_list
	printk("entering list create mylist.c l 38\n");
	#endif
	//List l = malloc(sizeof(*l));
	List l = kmalloc(sizeof(*l), GFP_KERNEL);
	if(!l)
	#ifdef DEBUG_list
	printk("Allcation failed exiting list create mylist.c l 44 \n");
	#endif
		return NULL;
	l->head = NULL;
l->iter = NULL;
	#ifdef DEBUG_list
	printk("allocation successful exiting list Create mylist.c l 50\n");
	#endif
	return l;
}
ListResult listAddString(List l, const char *string){
	
	#ifdef DEBUG_list
	printk(" entering list add string (failed unless a success message is printed) l57\n");
	#endif
		
	
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
  
	#ifdef DEBUG_list
	printk("list add successful l74\n");
	#endif
		
  return LIST_SUCCESS;
}



int listGetSize(List l){
	
	#ifdef DEBUG_list
	printk("entering list get size l83\n");
	#endif
	
	if(!l)
		return LIST_NULL_ARGUMENT;
  int count = 0;
  Node tmp = l->head;
  
	#ifdef DEBUG_list
	printk("counting number of nodes l92\n");
	#endif
	
  for( ;tmp; tmp = tmp->next){
    count++;
  }
  
	#ifdef DEBUG_list
	printk("exiting list get size l100\n");
	#endif
	
  return count;
}

ListResult listClear(List l){
	
	#ifdef DEBUG_list
	printk("entering list clear l109\n");
	#endif
	
	if(!l){
	   return LIST_NULL_ARGUMENT;
	}
	
	#ifdef DEBUG_list
	printk("deallocating nodes l117\n");
	#endif
	
	Node t = listGetLast(l);
	for(; t != l->head ; ){
	  Node temp = t;
	  t = t->previous;
	  t->next = NULL;
	  deallocNode(temp);
	}
	l->head = NULL;
	deallocNode(t);

	#ifdef DEBUG_list
	printk("exiting list clear l131 \n");
	#endif
	
	
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
	
	#ifdef DEBUG_list
	printk("entering listRemoveString l152 \n");
	#endif
	
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
	#ifdef DEBUG_list
	printk("successful removal exiting listremoveString l168 \n");
	#endif
	
      return LIST_SUCCESS;
    }
  }
	#ifdef DEBUG_list
	printk("Error, list doesnt exist cant remove. exiting listRemoveString l175 \n");
	#endif
	
  return LIST_DOESNT_EXIST;
}

void listDestroy(List l){
	#ifdef DEBUG_list
	printk("entering list Destroy l183 \n");
	#endif
	
	if(!l){
	  return;
	}
	listClear(l);
	//assert(l);
	kfree(l);
	//free(l);
	#ifdef DEBUG_list
	printk("exiting list Destroy l194 \n");
	#endif
	
	return;
}


ListResult isInList(List l, const char* string){
	#ifdef DEBUG_list
	printk("entering is in list l203\n");
	#endif
	
  if(!l || !string)
    return LIST_NULL_ARGUMENT;
  Node t = l->head;
  
	#ifdef DEBUG_list
	printk("looking for string l211 \n");
	#endif
	
  for( ; t; t = t->next){
   if(!strcmp(t->name, string))
	#ifdef DEBUG_list
	printk("string found! exiting is in list l217 \n");
	#endif
	
      return LIST_IS_IN;
  }
  
  
	#ifdef DEBUG_list
	printk("string not found, exiting is in list l225 \n");
	#endif
	
   return LIST_DOESNT_EXIST;
}

char* listGetFirst(List l){
	#ifdef DEBUG_list
	printk("entering list get first l233 \n");
	#endif
	
  if(!l)
    return NULL;
  l->iter = l->head;
	#ifdef DEBUG_list
	printk("returning head exiting list get first l240 \n");
	#endif
	
  return l->iter ? (l->iter)->name : NULL;

}

char* listGetNext(List l){
	#ifdef DEBUG_list
	printk("entering listGEtnext l249 \n");
	#endif
	
  if(!l)
    return NULL;
  if(l->iter)
    l->iter = (l->iter)->next;
	#ifdef DEBUG_list
	printk("returning next exiting list ger next l257 \n");
	#endif
	
  return l->iter ? (l->iter)->name : NULL;

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
