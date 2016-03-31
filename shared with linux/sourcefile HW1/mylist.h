#ifndef MYLIST_H_
#define MYLIST_H_

/** Type for defining the list */
typedef struct List_t *List;

/** Type used for returning error codes from list functions */
typedef enum ListResult_t {
  LIST_SUCCESS = 1,
  LIST_NULL_ARGUMENT = 22,
  LIST_OUT_OF_MEMORY = 12,
  LIST_INVALID_CURRENT,
  LIST_DOESNT_EXIST,
  LIST_IS_IN
} ListResult;

/** Element data type for list container */
typedef void* ListElement;

/**
* Type of function for copying an element of the list.
*
* Such a function should be supplied to a list to allow it to copy its
* elements. The function supplied should be able to dynamically copy the
* object and return a pointer to a newly allocated object.
* If this function fails for some reason, it should return NULL.
*
* For example, here is a proper function for copying a string that can be
* used for storing strings in a list:
* @code
* ListElement copyString(ListElement str) {
*   assert(str);
*   char* copy = malloc(strlen(str) + 1);
*   return copy ? strcpy(copy, str) : NULL;
* }
* @endcode
*/
typedef ListElement(*CopyListElement)(ListElement);

/** Type of function for deallocating an element of the list */
typedef void(*FreeListElement)(ListElement);



/**
* Allocates a new List.
*
* Creates a new empty list. This function receives the functions which will be
* used for copying elements into the list and freeing them when needed.
* For example, in order to create a list of strings one need to define these
* two functions:
* @code
* ListElement copyString(ListElement str);
* void freeString(ListElement str);
* @endcode
* And then create a list as follows:
* @code
* List listOfStrings = listCreate(copyString, freeString);
* @endcode
*
* @param copyElement Function pointer to be used for copying elements into
* the list or when copying the list.
* @param freeElement Function pointer to be used for removing elements from
* the list.
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new List in case of success.
*/
extern List listCreate(CopyListElement copyElement, FreeListElement freeElement);

/**
* Creates a copy of target list.
*
* The new copy will contain all the elements from the source list in the same
* order and will use the same functions as the original list for copying and
* freeing elements.
*
* @param list The target list to copy
* @return
* NULL if a NULL was sent or a memory allocation failed.
* A List containing the same elements with same order as list otherwise.
*/
extern List listCopy(List list);

/**
* Returns the number of elements in a list
*
* @param list The target list which size is requested.
* @return
* -1 if a NULL pointer was sent.
* Otherwise the number of elements in the list.
*/
extern int listGetSize(List list);

/**
* Sets the internal iterator to the first element and retrieves it.
*
* The list has an internal iterator (also called current element) to allow
* iteration over the list's elements. This function sets the iterator to point
* to the first element in the list and return it.
* Use this to start iteraing over the list, searching from the beginning of
* the list and/or get the first element in the list.
* (To continue iteration use listGetNext)
* @code
* void f(List list) {
*   ListElement first = listGetFirst(list);
*   printf("The first element is at address %x\n", first);
* }
* @endcode
*
* @param list The list for which to set the iterator and return the first
* element.
* @return
* NULL is a NULL pointer was sent or the list is empty.
* The first element of the list otherwise
*/
extern ListElement listGetFirst(List list);

/**
* Advances the list's iterator to the next element and return it
*
* @param list The list for which to advance the iterator
* @return
* NULL if reached the end of the list, the iterator is at an invalid state or
* a NULL sent as argument
* The next element on the list in case of success
*/
extern ListElement listGetNext(List list);

/**
* Returns the current element (pointed by the iterator)
*
* @param list The list for which to get the iterator
* @return
* NULL if the iterator is at an invalid state or a NULL sent as argument
* The current element on the list in case of success
*/
extern ListElement listGetCurrent(List list);

/**
* Adds a new element to the list, the new element will be the first element.
*
* @param list The list for which to add an element in its start
* @param element The element to insert. A copy of the element will be
* inserted as supplied by the copying function which is stored in the list
* @return
* LIST_NULL_ARGUMENT if a NULL was sent as list
* LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* an element failed)
* LIST_SUCCESS the element has been inserted successfully
*/
extern ListResult listInsertFirst(List list, ListElement element);

/**
* Adds a new element to the list, the new element will be the last element
*
* @param list The list for which to add an element in its end
* @param element The element to insert. A copy of the element will be
* inserted as supplied by the copying function which is stored in the list
* @return
* LIST_NULL_ARGUMENT if a NULL was sent as list
* LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* an element failed)
* LIST_SUCCESS the element has been inserted successfully
*/
extern ListResult listInsertLast(List list, ListElement element);
/**
* Adds a new element to the list, the new element will be place right before
* the current element (As pointed by the inner iterator of the list)
*
* @param list The list for which to add an element before its current element
* @param element The element to insert. A copy of the element will be
* inserted as supplied by the copying function which is stored in the list
* @return
* LIST_NULL_ARGUMENT if a NULL was sent as list
* LIST_INVALID_CURRENT if the list's iterator is in an invalid state (Does
* not point to a legal element in the list)
* LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* an element failed)
* LIST_SUCCESS the element has been inserted successfully
*/
extern ListResult listInsertBeforeCurrent(List list, ListElement element);

/**
* Adds a new element to the list, the new element will be place right after
* the current element (As pointed by the inner iterator be of the list)
*
* @param list The list for which to add an element after its current element
* @param element The element to insert. A copy of the element will be
* inserted as supplied by the copying function which is stored in the list
* @return
* LIST_NULL_ARGUMENT if a NULL was sent as list
* LIST_INVALID_CURRENT if the list's iterator is in an invalid state (Does
* not point to a legal element in the list)
* LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* an element failed)
* LIST_SUCCESS the element has been inserted successfully
*/
extern ListResult listInsertAfterCurrent(List list, ListElement element);

/**
* Removes the currently pointed element of the list using the stored freeing
* function
*
* @param list The list for which the current element will be removed
* @return
* LIST_NULL_ARGUMENT if list is NULL
* LIST_INVALID_CURRENT if the current pointer of the list is in invalid state
* LIST_SUCCESS the current element was removed successfully
*/
extern ListResult listRemoveCurrent(List list);
/**
* Removes all elements from target list.
*
* The elements are deallocated using the stored freeing function
* @param list Target list to remove all element from
* @return
* LIST_NULL_ARGUMENT - if a NULL pointer was sent.
* LIST_SUCCESS - Otherwise.
*/
extern ListResult listClear(List list);

/**
* listDestroy: Deallocates an existing list. Clears all elements by using the
* stored free function.
*
* @param list Target list to be deallocated. If list is NULL nothing will be
* done
*/
extern void listDestroy(List list);

// ---------------------------------- ADDED -- FOR LIST OF STRINGS---------------------------------------

extern ListElement stringCopy(ListElement string);

extern void stringDestroy(ListElement string);

// finds in a given list of strings a string that equal to the second parameter and, if exists, removes it from the list
// returns:
// LIST_NULL_ARGUMENT - if the arguments are invalid; LIST_SUCCESS - if the string was deleted; LIST_DOESNT_EXIST - if a string doesn't exist in the list
extern ListResult removeString(List l, ListElement string);


//returns LIST_IS_IN if a given string is in list, otherwise LIST_DOESNT_EXIST, in case of NULL pointer - LIST_NULL_ARGUMENT
extern ListResult isInList(List l, char* string);

//returns max(a, b)
extern unsigned int mymax(unsigned int a, unsigned int b);

//returns min(a, b)
extern unsigned int mymin(unsigned int a, unsigned int b);

/**
* Macro for iterating over a list.
*
* Declares a new variable to hold each element of the list.
* Use this macro for iterating through the list conveniently.
* Note that this mcaro modifies the internal iterator.
* For example, the following code will go through a list of strings and print
* them to the standard output:
* @code
* void printList(List listOfStrings) {
*   LIST_FOREACH(char*, str, listOfStrings) {
*     printf("%s\\n", str);
*   }
* }
* @endcode
*
* @param type The type of the elements in the list
* @param iterator The name of the variable to hold the next list element
* @param list the list to iterate over
*/
#define LIST_FOREACH(type,iterator,list) \
	type iterator;\
	for(iterator = listGetFirst(list) ; \
		iterator ;\
		iterator = listGetNext(list))

#endif /* MYLIST_H_ */
