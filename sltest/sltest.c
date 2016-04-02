#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sorted-list.h"

/*Integer Sorted List Functions*/
int IntComparator(void* p1, void* p2){
    int* x1 = (int*)p1;
    int* x2 = (int*)p2;

    if(*x1 < *x2)
        return -1;
    else if(*x1 > *x2)
        return 1;
    else
        return 0;
}

void IntDestructor(void* p1){
    free((int*)p1);
}

void PrintSortedList(SortedListPtr list){
    if(list == NULL){
        printf("Trying to print empty list\n");
        return;
    }

    struct node* ptr = list->head;
    //printf("Printing integer sorted list...\n");
    while(ptr != NULL){
        int * print = ptr->value;
        printf("%d -> ", *print); //%p is void format specifier
        ptr = ptr->next;
    }
    printf("\n");
    return;
}

int* NewIntObj(int value){
  int* newvalue = (int*)malloc(sizeof(int));
  *newvalue = value;
  return newvalue;
}

void Insert(int value, SortedListPtr list){
  int* newvalue = NewIntObj(value);
  SLInsert(list, (void*)newvalue);
  return;
}

void Remove(int value, SortedListPtr list){
  int* newvalue = NewIntObj(value);
  SLRemove(list, (void*)newvalue);
  return;
}


/*
 * Call Insert(<value>, list) to insert from list
 * Call Remove(<value>, list) to remove from list
 */
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
    return 0;
}
