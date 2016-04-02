int IntComparator(void* p1, void* p2);
void IntDestructor(void* p1);
void PrintSortedList(SortedListPtr list);
int* NewIntObj(int value);
void Insert(int value, SortedListPtr list);
void Remove(int value, SortedListPtr list);

int StringComparator(void* p1, void* p2);
void StringDestructor(void* p1);
void PrintStringSortedList(SortedListPtr list);
char* NewStringObj(char* str);
void InsertString(char* str, SortedListPtr list);
void RemoveString(char* str, SortedListPtr list);
