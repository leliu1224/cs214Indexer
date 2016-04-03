#ifndef SORTED_LIST_H
#define SORTED_LIST_H
/*
 * sorted-list.h
 */
#include <stdlib.h>
#include <stdio.h>

/*
 * Your list is used to store data items of an unknown type, which you need to sort.
 * Since the type is opaque to you, you do not know how to directly compare the data.
 *
 * You can presume though that a user will supply your code with a comparator function
 * that knows how to compare the data being sorted, but your code must do the rest
 * of the bookkeeping in a generic manner.
 *
 * The comparator function will take pointers to two data items and will return -1 if the 1st
 * is smaller, 0 if the two are equal, and 1 if the 2nd is smaller.
 *
 * The user will also supply a destruct function will take a pointer to a single data item
 *	and knows how to deallocate it. If the data item does not require deallocation, the user's
 *  destruct function will do nothing.
 */

typedef int (*CompareFuncT)( void *, void * );
typedef void (*DestructFuncT)( void * );

/*
 * Sorted list type that will hold all the data to be sorted.
 */

struct node{
  char* value;
  struct node* next;
  int refCount;
  char * filepath;
};

struct SortedList
{
    struct node* head;
    CompareFuncT COMPARATOR;
    DestructFuncT DESTRUCTOR;
    int size;
	//Do we want unique items or allow duplicate items in list?
};
typedef struct SortedList* SortedListPtr;

struct node* CreateRecord(char* newObj, char* pathname);
void FreeLinkedList(struct node* ptr, DestructFuncT df);

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df);
void SLDestroy(SortedListPtr list);

/*
 * SLInsert inserts a given data item 'newObj' into a SortedList while maintaining the
 *   order and uniqueness of list items.
 *
 * SLInsert should return 1 if 'newObj' is not equal to any other items in the list and
 *   was successfully inserted.
 * SLInsert should return 0 if 'newObj' is equal to an item already in the list or it was
 *   not successfully inserted
 *
 * Data item equality should be tested with the user's comparator function *
 */

int SLInsert(SortedListPtr list, char* newObj, char* pathname);

/*
 * SLRemove should remove 'newObj' from the SortedList in a manner that
 *   maintains list order.
 *
 * SLRemove must not modify the data item pointed to by 'newObj'.
 *
 * SLRemove should return 1 on success, and 0 on failure.
 */

int SLRemove(SortedListPtr list, char* newObj);


#endif
