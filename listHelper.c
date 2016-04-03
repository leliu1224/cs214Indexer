#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sorted-list.h"
#include "tokenizer.h"

int RecordComparator(struct node* p1, struct node* p2){
  struct node* r1 = (struct node*)p1;
  struct node* r2 = (struct node*)p2;
  char* word1 = (char*)r1->value;
  char* word2 = (char*)r2->value;
  int x = strcmp(word1, word2);
  if(x > 0)
    return -1;
  else if(x == 0)
    return ComparePathHelper(r1->filepath, r2->filepath);
  else if(x < 0)
    return 1;
}

int ComparePathHelper(char* p1, char* p2){
  int x = strcmp(p1, p2);
  
  if(x < 0)
    return -1;
  else if(x == 0)
    return 0;
  else if(x > 0)
    return 1;
}

void RecordDestructor(struct node* p1){
  free((struct node*)p1);
}

void PrintRecordSortedList(SortedListPtr list){
  if(list == NULL){
    printf("Trying to print empty list\n");
    return;
  }
  
  struct node* ptr = list->head;
  while(ptr != NULL){
    char* print = ptr->value;
    printf("%s -> ", print); //%p is void format specifier
    ptr = ptr->next;
  }
  printf("\n");
}

/*
 * Call Insert(<value>, list) to insert into list
 * Call Remove(<value>, list) to remove from list
 * Call InsertString(<string>, stringlist) to insert into string list
 * Call RemoveString(<string>, stringlist) to insert into string list
 */

/*
int main(int argc, char** argv){
  
    SortedListPtr list = SLCreate(IntComparator, IntDestructor); //store fn ptrs in list

    printf("Inserting 100\n");
    Insert(100, list);

    printf("Inserting 10, 70, 80, 30, 20 in descending order...\n");
    Insert(10, list);
    Insert(70, list);
    Insert(80, list);
    Insert(30, list);
    Insert(20, list);

    PrintSortedList(list);

    printf("Removing head, 20, 70\n");
    Remove(100, list);
    Remove(20, list);
    Remove(70, list);
    PrintSortedList(list);

    SLDestroy(list);
  
    SortedListPtr stringlist = SLCreate(StringComparator, StringDestructor);
    InsertString("Aa", stringlist);
    InsertString("Baa", stringlist);
    //InsertString("Boo", stringlist);
    //InsertString("cat", stringlist);
    //InsertString("dog", stringlist);
    //InsertString("name", stringlist);
    PrintStringSortedList(stringlist);
    SLDestroy(stringlist);
    return 0;
}
*/
