#ifndef SORTED_LIST_H
#define SORTED_LIST_H
/*
 * sorted-list.h
 */
#include <stdlib.h>
#include <stdio.h>

struct node{
  char* value;
  //char value[256];
  char* filepath;
  int refCount;
  struct node* next;
};

typedef int (*CompareFuncT)( struct node *, struct node * );
typedef void (*DestructFuncT)( struct node * );

struct SortedList
{
    struct node* head;
    CompareFuncT COMPARATOR;
    DestructFuncT DESTRUCTOR;
    int size;
	
};
typedef struct SortedList* SortedListPtr;

struct node* CreateRecord(char* newObj, char* pathname);
void FreeLinkedList(struct node* ptr, DestructFuncT df);

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df);
void SLDestroy(SortedListPtr list);
int SLInsert(SortedListPtr list, char* newObj, char* pathname);

SortedListPtr finalSort(SortedListPtr list);

#endif
