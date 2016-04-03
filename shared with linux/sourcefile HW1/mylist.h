/*
 * OpSystemsList.h
 *
 *  Created on: Apr 1, 2016
 *      Author: mac
 */

#ifndef MYLIST_H_
#define MYLIST_H_

#include <linux/string.h>

/** Type used for returning error codes from list functions */
typedef enum ListResult_t {
  LIST_SUCCESS = 1,
  LIST_NULL_ARGUMENT = 22,
  LIST_OUT_OF_MEMORY = 12,
  LIST_INVALID_INPUT,
  LIST_DOESNT_EXIST,
  LIST_IS_IN
} ListResult;

typedef struct list_t *List;

List listCreate(void);

ListResult listAddString(List l, const char *string);

int listGetSize(List l);

ListResult listClear(List l);

ListResult listRemoveString(List l, const char *string);

void listDestroy(List l);

ListResult isInList(List l, const char* string);

char* listGetFirst(List l);

char* listGetNext(List l);

unsigned int mymax(unsigned int  a, unsigned int b);

unsigned int mymin(unsigned int  a, unsigned int b);

#endif /* MYLISTLIST_H_ */
