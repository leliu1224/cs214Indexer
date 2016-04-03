/*sorted-list.c*/
/*modified to insert for a struct node for Asst3*/


#include "sorted-list.h"
#include <stdlib.h>
#include <string.h>

/*returns a linked list node with value*/
struct node* CreateRecord(char* newObj, char* pathname ){

  struct node* newnode = (struct node*)malloc(sizeof(struct node));
    if(newnode == NULL)
        return NULL;
    
    
    newnode->value = malloc(sizeof(strlen(newObj)) + 1 );
    newnode->filepath = malloc(sizeof(strlen(pathname))  + 1);

    strcpy(newnode->value, newObj);
    newnode->value[strlen(newObj)] = '\0';
    strcpy(newnode->filepath, pathname);
    newnode->filepath[strlen(pathname)] = '\0';
    //newnode->value = newObj;
    //newnode->filepath = pathname;
    newnode->refCount = 1;
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

    df(ptr);
   
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

int SLInsert(SortedListPtr list, char* newObj, char* pathname){
  struct node* newnode = CreateRecord(newObj, pathname);

    if(newnode == NULL)
        return 0;

    // If there is nothing in the SortedList yet
    if(list->head == NULL){
        list->head = newnode;
        list->head->refCount = 1;
        return 1;
    }

    // Check for duplicate in start of list
    if(list->COMPARATOR(list->head, newnode) == 0){ 
        list->DESTRUCTOR(newnode);
	list->head->refCount++;
        return 0;
    }

    struct node* ptr = list->head;
    struct node* prev = NULL;
    while(ptr != NULL && list->COMPARATOR(newnode, ptr) == -1  ){ // //First argument is smaller, iterates ptr to spot of insertion
        prev = ptr;
        ptr = ptr->next;
    }

    if(ptr != NULL && list->COMPARATOR(ptr, newnode) == 0 ){ //
        list->DESTRUCTOR(newnode);
	ptr->refCount++;
        return 0; //Increment refCount, already exists in list
    }

    //Doesnt exist
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

SortedListPtr finalSort(SortedListPtr list){
  if(list == NULL || list->head == NULL)
    return NULL;

  struct node * prev = NULL;
  struct node * current = list->head;
  struct node * newHead;
  while(current->next != NULL){
    //check within the same word section
    struct node * temp = current;
    while((temp->next != NULL) && strcmp(temp->value, temp->next->value) == 0){
      //bubble sort base on frequency value
      while((temp->next != NULL) && (temp->refCount < temp->next->refCount)){
        //flip the values
        if(prev == NULL){ // starting at the head
          newHead =  temp->next;
          temp->next = temp->next->next;
          newHead->next = temp;
          list->head = newHead;
        }
        else{
          prev->next = temp->next;
          temp->next =  temp;
        }
        prev =  temp;
        temp = temp->next;
      }
      //bubble sort based on file path
      // printf("%s\n", temp->filepath);
      // printf("%s\n", temp->next->filepath);
      // only do it when the different paths have the same frequency
      while((temp->next != NULL) && (temp->refCount == temp->next->refCount) && (strcmp(temp->filepath , temp->next->filepath) > 0 )){

        //flip the values
        if(prev == NULL){ // starting at the head
          newHead =  temp->next;
          temp->next = temp->next->next;
          newHead->next = temp;
          list->head = newHead; // set a new head for the list
        }
        else{
          prev->next = temp->next;
          temp->next =  temp;
        }
        prev =  temp;
        temp = temp->next;
      }
      prev = temp;
      temp = temp->next;
    }
    prev = current;
    current = current-> next;
  }
  return list;
}
