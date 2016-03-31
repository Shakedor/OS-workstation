
#include <linux/mylist.h>
#include <linux/string.h>
#include <linux/slab.h>

typedef void* Data;

typedef struct node_t {
	Data data;
	struct node_t* next;
	struct node_t* previous;
}*Node;

struct List_t {
	Node head;
	Node node; // this is the iterator
	CopyListElement copyElement;
	FreeListElement freeElement;

};

typedef void(*printListElement)(ListElement element);

static ListElement listGoToFirst(List list){
	if(!list){
		return NULL;
	}
	while(list->node->previous!=NULL){
		list->node=list->node->previous;
	}

	return list->node->data;
}
  
static ListResult listMoveToPlace(List list,int place){

	//if place is 0 we put null in iterator
	if(place==0){
		list->node=NULL;
		return LIST_SUCCESS;
	}
	//list GET first, set counter to 1
	// till we get to original place, list GEt next.
	listGetFirst(list);
	for(int i=0;i<place-1;i++){
		listGetNext(list);
	}
	return LIST_SUCCESS;

}

// destroys current node on iterator
static void destroyNode(List list){
	list->freeElement(list->node->data);
	kfree(list->node);
}


//Destroys all Nodes, starting with the node currently pointed on by iterator
static void NodesDestroy(Node head,List list){

	if(listGetSize(list)==0){
		return;
	}
	Node previous=head->previous;
	// if had aprevious, erase it's next
	if(previous){
		previous->next=NULL;
	}

	Node next= head->next;
	if (next==NULL){
		list->freeElement(head->data);
		kfree(head);
		return;
	}
	// recursive call
	NodesDestroy(next,list);
	list->freeElement(head->data);
	kfree(head);
	return;
}

static ListElement listGetLast(List list){
	if(list==NULL){
		return NULL;
	}
	if(list->head==NULL){
		return NULL;
	}
	list->node=list->head;
	while (list->node->next!=NULL){
		list->node=list->node->next;
	}
	return list->node->data;
}


// prints List and saves place of iterator.



static ListResult nodesCopy(List list, List listCopy){
 	if (list ==NULL || listCopy==NULL){
		return LIST_NULL_ARGUMENT;
	}
	Node temporaryNode=list->node;
	// copies first node
	int badMalloc=0;
	listGetFirst(list);
	ListResult result=listInsertFirst(listCopy,list->node->data);

	//make sure node has head so getNext works
	listGetFirst(listCopy);

	if(result!=LIST_SUCCESS){
		destroyNode(listCopy);
		badMalloc=1;
	}
	//copies rest of nodes

	while(listGetNext(list)&&!badMalloc){
		result = listInsertLast(listCopy,list->node->data);
		if(result!=LIST_SUCCESS){
			badMalloc=1;
			break;
		}

	}

	//if listInsertAfterCurrent, destroy all copied nodes
	if(badMalloc){
		LIST_FOREACH(Node,node,listCopy){
			destroyNode(listCopy);
		}
		list->node=temporaryNode;
		return LIST_OUT_OF_MEMORY;
	}

	//resets Nodes to original location
	listGetFirst(list);
	listGetFirst(listCopy);
	if(temporaryNode==NULL){
		list->node=NULL;
		listCopy->node=NULL;
		return LIST_SUCCESS;
	}
	while(temporaryNode->previous!=list->node->previous){
		listGetNext(list);
		listGetNext(listCopy);
	}

	return LIST_SUCCESS;
}


static ListResult nodeSwitch(Node target1,Node target2){

	if(!target1 || !target2){
		return LIST_NULL_ARGUMENT;
	}
	// gets the neighbors of the switched nodes could be voids
	Node target1LastNeighbor = target1->previous;
	Node target2NextNeighbor = target2->next;

	if(target1LastNeighbor!=NULL){
		target1LastNeighbor->next = target2;
	}
	if(target2NextNeighbor!=NULL){
		target2NextNeighbor->previous = target1;
	}


	target1->next =target2NextNeighbor;
	target1->previous = target2;

	target2->next =target1;
	target2->previous = target1LastNeighbor;

	return LIST_SUCCESS;
}


List listCreate(CopyListElement copyElement, FreeListElement freeElement) {

	if (copyElement == NULL || freeElement == NULL) {
		return NULL;
	}

	List list = kmalloc(sizeof(*list));
	if (list == NULL) {
		return NULL;
	}
	list->node=NULL;
	list->head=NULL;
	list->copyElement=copyElement;
	list->freeElement=freeElement;

	return list;
}

List listCopy(List list){
	if(list==NULL){
		return NULL;
	}

	List listCopy=listCreate(list->copyElement,list->freeElement);
	if (listCopy==NULL){
		return NULL;
	}
	ListResult nodeCopied=nodesCopy(list,listCopy);
	if(nodeCopied==LIST_OUT_OF_MEMORY){
		kfree(listCopy);
		return NULL;
	}
	listCopy->copyElement=list->copyElement;
	listCopy->freeElement=list->freeElement;

	return listCopy;

}

int listGetSize(List list){
	if(!list){
		return -1;
	}
	Node savePlace=list->node;
	int size=0;
	if(listGetFirst(list)==NULL){
		list->node=savePlace;
		return size;
	}
	size++;

	while (listGetNext(list)!=NULL){
		size++;
	}

	list->node=savePlace;
	return size;

}

ListElement listGetFirst(List list){
	if(list==NULL){
		return NULL;
	}
	if(list->head==NULL){
		return NULL;
	}
	list->node=list->head;
	return list->head->data;
}

ListElement listGetNext(List list){
	if(list==NULL){
		return NULL;
	}
	if(list->node==NULL){
		return NULL;
	}
	if(list->node->next==NULL){
		return NULL;
	}
	list->node=list->node->next;
	return list->node->data;
}

ListElement listGetCurrent(List list){
	if(list==NULL){
		return NULL;
	}
	if(list->node==NULL){
		return NULL;
	}
	return list->node->data;

}

Node createNode(CopyListElement copyElement,ListElement element){

	if(!copyElement||!element){
		return NULL;
	}
	Node newNode=kmalloc(sizeof(*newNode));
	if (newNode == NULL) {
		return NULL;
	}

	newNode->data=copyElement(element);
	if(newNode->data==NULL){
		kfree (newNode);
		return NULL;
	}
	newNode->next=NULL;
	newNode->previous=NULL;
	return newNode;
}

ListResult listInsertFirst(List list, ListElement element){
	if(!list){
		return LIST_NULL_ARGUMENT;
	}
	Node newNode= createNode(list->copyElement,element);
	if(!newNode){
		return LIST_OUT_OF_MEMORY;
	}

	// if no head exists (new list)
	if(!list->head){
		list->head=newNode;
		return LIST_SUCCESS;
	}else{//a head exists, not new list
		list->head->previous=newNode;
		newNode->next=list->head;
		list->head=newNode;
		return LIST_SUCCESS;
	}

}

ListResult listInsertLast(List list, ListElement element){
	if(!list){
		return LIST_NULL_ARGUMENT;
	}
	//save current place
	Node tempNode=list->node;
	if(listGetLast(list)==NULL){
		ListResult res= listInsertFirst(list,element);
		list->node=tempNode;
		return res;
	}

	Node newNode=createNode(list->copyElement,element);
	if(!newNode){
		return LIST_OUT_OF_MEMORY;
	}
	//moves iterator to end of list
	listGetLast(list);
	//links newNode to LastNode
	list->node->next=newNode;
	newNode->previous=list->node;

	//reset iterator to original place
	list->node=tempNode;
	return LIST_SUCCESS;

}
ListResult listInsertBeforeCurrent(List list, ListElement element){
	if(!list){
		return LIST_NULL_ARGUMENT;
	}


	//save current place
	Node tempNode=list->node;

	if(!list->node){
		return LIST_INVALID_CURRENT;
	}

	Node newNode=createNode(list->copyElement,element);
	if(!newNode){
		return LIST_OUT_OF_MEMORY;
	}

	if(!list->node){
		kfree(newNode);
		return LIST_INVALID_CURRENT;
	}


	//if current had a previous
	if(list->node->previous){
		list->node->previous->next=newNode;
		newNode->previous=list->node->previous;
	}

	//link newNode and currentNode
	newNode->next=list->node;
	list->node->previous=newNode;

	// in case of mishap, reset current
	list->node=tempNode;

	//if original node was head, we have to set head
	if(list->node==list->head){
		list->head=newNode;
	}

	return LIST_SUCCESS;

}


ListResult listInsertAfterCurrent(List list, ListElement element){
	if(!list){
		return LIST_NULL_ARGUMENT;
	}

	//save current place
	Node tempNode=list->node;

	if(!list->node){
		return LIST_INVALID_CURRENT;
	}

	Node newNode=createNode(list->copyElement,element);
	if(!newNode){
		return LIST_OUT_OF_MEMORY;
	}





	// if current had a next, link to it
	if(list->node->next){
		list->node->next->previous=newNode;
		newNode->next=list->node->next;
	}

	// linked newNode and Current
	newNode->previous=list->node;
	list->node->next=newNode;

	// in case of mishap, reset current
	list->node=tempNode;

	return LIST_SUCCESS;

}


ListResult listRemoveCurrent(List list){
	if(!list){
		return LIST_NULL_ARGUMENT;
	}

	if(!list->node){
		return LIST_INVALID_CURRENT;
	}
	if(list->node==list->head){
		list->head=list->node->next;

	}

	Node previous=list->node->previous;
	Node next=list->node->next;

	if(!next && !previous){
		destroyNode(list);
		list->node=NULL;
		return LIST_SUCCESS;
	}

	//if next exists put prev in it
	if(next){
		next->previous=previous;
	}
	// if previous exists put nex tin it
	if(previous){
		previous->next=next;
	}
	destroyNode(list);
	// set node to valid location after destruction
	list->node=NULL;

	return LIST_SUCCESS;


}
ListResult listClear(List list){
	if(!list){
		return LIST_NULL_ARGUMENT;
	}
	listGetFirst(list);
	NodesDestroy(list->node,list);
	//reset node after all are erased
	list->node=NULL;
	list->head=NULL;
	return LIST_SUCCESS;
}

void listDestroy(List list){
	if(!list){
		return;
	}

	listClear(list);
	kfree(list);
	return;
}

ListElement stringCopy(ListElement string){
  if(!string){
    return NULL;
  }
  char* strCopy = kmalloc(sizeof(char)* strlen(string) + 1 );
  if(!strCopy)
    return NULL;
  strcpy(strCopy, string);
  return strCopy ? strCopy : NULL;
}

void stringDestroy(ListElement string){
  if(!string)
    return;
  kfree(string);
}

ListResult removeString(List l, ListElement string){
  if( !l || !string )
    return LIST_NULL_ARGUMENT;
  for(Node t = l->head; t != NULL; t = t->next){
    if(!strcmp((char*)(t->data), string)){
      (t->previous)->next = t->next;
      (t->next)->previous = t->previous;
      kfree(t);
      return LIST_SUCCESS;
    }
  }
  return LIST_DOESNT_EXIST;
}

ListResult isInList(List l, char* string){
  if(!l || !string)
    return LIST_NULL_ARGUMENT; 
   LIST_FOREACH(char*, itr, l){
    if(!strcmp(itr, string))
      return LIST_IS_IN;
  }
   return LIST_DOESNT_EXIST;
}


unsigned int max(unsigned int  a, unsigned int b){
  return a >= b ? a : b; 
}

unsigned int min(unsigned int  a, unsigned int b){
  return a > b ? b : a; 
}
