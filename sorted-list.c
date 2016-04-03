/*sorted-list.c*/

#include "sorted-list.h"
#include <stdlib.h>
#include <string.h>

/*returns a linked list node with value*/
struct node* CreateRecord(char* newObj, char* pathname ){

    struct node* newnode = (struct node*)malloc(sizeof(struct node));
    if(newnode == NULL){
        return NULL;
    }
    newnode->value = newObj;
    newnode->refCount = 1;
    newnode->filepath = pathname;
    newnode->next = NULL;

    return newnode;
}


/*
 *Frees linked list that was malloced
 *Called in SLDestroy
 */
void FreeLinkedList(struct node* ptr, DestructFuncT df){
    if(ptr == NULL){
        return;
    }

    if(ptr->next != NULL){
        FreeLinkedList(ptr->next, df);
    }

    ptr->refCount--;
    if(ptr->refCount == 0){
        df(ptr->value);
	free(ptr);
    }
    return;
}

/*--------------------------------------------------------------------------------------------------
 * SLCreate creates a new, empty, 'SortedList'.
 *
 * SLCreate's parameters will be a comparator (cf) and destructor (df) function.
 *   Both the comparator and destructor functions will be defined by the user as per
 *     the prototypes above.
 *   Both functions must be stored in the SortedList struct.
 *
 * SLCreate must return NULL if it does not succeed, and a non-NULL SortedListPtr
 *   on success.
 */

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){
    SortedListPtr list = malloc(sizeof(struct SortedList));

    if(list == NULL){
        return NULL;
    }

    list->COMPARATOR = cf;
    list->DESTRUCTOR = df;
    list->head = NULL;

    return list;
}

/*--------------------------------------------------------------------------------------------
 * SLDestroy destroys a SortedList, freeing all dynamically-allocated memory.
 */
void SLDestroy(SortedListPtr list){
  if(list != NULL){
    FreeLinkedList(list->head, list->DESTRUCTOR); //Free Linked List before freeing SortedList
    free(list);
  }
}



//===0.2: List Insert/Remove

/*----------------------------------------------------------------------------------------------
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

//
int SLInsert(SortedListPtr list, char *newObj, char* pathname){
  struct node* newnode = CreateRecord(newObj, pathname);

    // Invalid node creation for some reason
    if(newnode == NULL){
        return 0;
    }

    // If there is nothing in the SortedList yet
    if(list->head == NULL){
        list->head = newnode;
        list->head->refCount = 1;
        list->head->filepath = pathname;
        return 1;
    }

    // Check for duplicates in start of list
    if(list->COMPARATOR(list->head->value, newnode->value) == 0){
        // if(list->COMPARATOR(list->head->filepath, newnode->filepath) == 0){
        //   newnode->refCount++;
        // }
        // else if(list->COMPARATOR(list->head->filepath, newnode->value) == -1){
        //   newnode->next = list->head;
        //   list->head = newnode;
        // }
        // else{
        //   list->head->next = newnode;
        // }
        //list->DESTRUCTOR(newnode->value);
        //free(newnode);
        list->DESTRUCTOR(newnode);
	list->head->refCount++;
        return 0;
    }

    // Normal insert
    struct node* ptr = list->head;
    struct node* prev = NULL;
    while(ptr != NULL && list->COMPARATOR(newnode->value, ptr->value) == -1 ){ //First argument is smaller, iterates ptr to spot of insertion
        prev = ptr;
        ptr = ptr->next;
    }

    if(ptr != NULL && list->COMPARATOR(ptr->value, newnode->value) == 0 ){ //Duplicate
        //list->DESTRUCTOR(newnode->value);
        //free(newnode);
        list->DESTRUCTOR(newnode->value);
	ptr->refCount++;
        return 0; //No duplicate items in list
    }

    if(prev == NULL){ //Head of list
      newnode->next = ptr;
      list->head = newnode;
    }
    else if(ptr != NULL){ //Middle of list
      prev->next = newnode;
      newnode->next = ptr;
    }
    else{ //End of list
      prev->next = newnode;
    }
    return 1;
}


/*---------------------------------------------------------------------------------------------
 * SLRemove should remove 'newObj' from the SortedList in a manner that
 *   maintains list order.
 *
 * SLRemove must not modify the data item pointed to by 'newObj'.
 *
 * SLRemove should return 1 on success, and 0 on failure.
 */

int SLRemove(SortedListPtr list, char *newObj){
    if( list->head == NULL){
        return 0;
    }

    struct node* ptr = list->head;
    if(list->COMPARATOR(list->head->value, newObj) == 0){
        struct node* temp = list->head;
        if(list->head->next != NULL){
            list->head = list->head->next;
            list->head->refCount--; //Subtract ref count
            if(list->head->refCount == 0)
                list->DESTRUCTOR(temp->value); //Destroy the node IF AND ONLY IF REFCOUNT IS 0
            free(temp);
	    return 1;
        }
        else{
            list->head->refCount--;
            if(list->head->refCount == 0){
                list->DESTRUCTOR(temp->value);
		free(temp);
	    }
            list->head = NULL;
            return 1;
        }
    }
    while(ptr->next != NULL ){
        if(list->COMPARATOR(ptr->next->value, newObj) == 0 ){//Found node to delete
            struct node* temp = ptr->next; //Node to be deleted
            ptr->next = temp->next; //Set ptr->next to the next of the to be deleted node
            temp->refCount--;
            if(ptr->next != NULL){
                ptr->next->refCount++;
            }
            if(temp->refCount == 0){
                list->DESTRUCTOR(temp->value);	//Destroy the note IF AND ONLY IF REFCOUNT IS 0
		free(temp);
	    }
            return 1;
        }
        ptr = ptr->next;
    }
    return 0; //Nothing to delete
}

SortedListPtr finalSort(SortedListPtr list){
  struct node * prev = NULL;
  struct node * current = list->head;
  struct node * newHead;
  while(current->next != NULL){
    //check within the same word section
    struct node * temp = current;
    while(strcmp(temp->value, temp->next->value) == 0){
      //bubble sort base on frequency value
      while((temp->next != NULL) && (temp->refCount < temp->next->refCount)){
        //flip the values
        if(prev = NULL){ // starting at the head
          newHead =  temp->next;
          newHead->next = temp;
          temp->next = temp->next->next;
        }
        else{
          prev->next = temp->next;
          temp->next =  temp;
        }
        prev =  temp;
        temp = temp->next;
      }
      //bubble sort based on word order
      while((temp->next != NULL) && (strcmp(temp->value , temp->next->value) == 1)){

        //flip the values
        if(prev = NULL){ // starting at the head
          newHead =  temp->next;
          newHead->next = temp;
          temp->next = temp->next->next;
        }
        else{
          prev->next = temp->next;
          temp->next =  temp;
        }
        prev =  temp;
        temp = temp->next;

      }
    }
    prev = current;
    current = current-> next;
  }
}
