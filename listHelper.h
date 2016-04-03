#include "sorted-list.h"
#include "tokenizer.h"

int RecordComparator(void* p1, void* p2);
int ComparePathHelper(char* p1, char* p2);
void RecordDestructor(void* p1);
void PrintRecordSortedList(SortedListPtr list);
