#include "mylist.h"
#include <stdlib.h>

int main(){
  List l = listCreate(stringCopy, stringDestroy);
  listDestroy(l);
  return 0;
}
