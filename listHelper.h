#ifndef LIST_HELPER_H
#define LIST_HELPER_H

#include "sorted-list.h"
#include "tokenizer.h"

int RecordComparator(struct node* p1, struct node* p2);
int ComparePathHelper(char* p1, char* p2);
void RecordDestructor(struct node* p1);
void PrintRecordSortedList(SortedListPtr list);

#endif
