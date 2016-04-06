#ifndef SORTED_LIST_H
#define SORTED_LIST_H
/*
 * sorted-list.h
 */
#include <stdlib.h>
#include <stdio.h>

//No longer generic, modified for Asst3
struct node{
  char* value;
  char* filepath;
  int refCount;
  struct node* next;
};

//No longer generic, modified for Asst3
//Record comparator/destructor functions in indexer.c
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
int EqualValue_Count(struct node* p1, struct node* p2);
int SLInsert(SortedListPtr list, char* newObj, char* pathname);

SortedListPtr finalSort(SortedListPtr list);

#endif
