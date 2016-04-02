#include "sorted-list.h"
struct TokenizerT_{
  char* inputstr;
  char* ptr;
};

typedef struct TokenizerT_ TokenizerT;

TokenizerT *TKCreate( char * ts  );
void TKDestroy( TokenizerT * tk  );
char * TKGetNextToken( TokenizerT * tk  );
int wordNextChar(char * nextChar);
int getchartype(char* str);
int PrintToken(char* str);
SortedListPtr TKFN(char* input);
