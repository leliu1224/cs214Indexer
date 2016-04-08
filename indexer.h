#ifndef INDEXER_H
#define INDEXER_H

#include "sorted-list.h"

int ComparePathHelper(struct node* p1, struct node* p2);
int RecordComparator(struct node* p1, struct node* p2);
void RecordDestructor(struct node* p1);
void PrintRecordSortedList(SortedListPtr list);
char* ConcatPath(char* s1, char* s2);
int directory_handler(char* path, SortedListPtr sortedlist);
int file_handler(char* filename, char* path, SortedListPtr sortedlist);
void WRITEFILE( FILE* OUTPUT, SortedListPtr list);

#endif
