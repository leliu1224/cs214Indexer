/*word tokenizer library of functions
 *Call 'TKFN' to use the tokenizer
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokenizer.h"
#include "sorted-list.h"
#include "listHelper.c"

TokenizerT *TKCreate( char * ts  ) {

  if(ts == NULL)
    return NULL;

  TokenizerT* tokenizer = (TokenizerT*)malloc(sizeof(TokenizerT));
  if(tokenizer == NULL){     //Check if malloc was successfull
    printf("MALLOC FAILED\n");
    return NULL;
  }

  /* fill out the TokenizerT and establish the pointers */
  tokenizer->inputstr = (char*)malloc( (sizeof(char)*strlen(ts)) + 1);     //Add one for room to null terminate
  strcpy(tokenizer->inputstr, ts);
  tokenizer->inputstr[strlen(ts)] = '\0';
  tokenizer->ptr = tokenizer->inputstr;
  return tokenizer;
}


void TKDestroy( TokenizerT * tk  ) {
  free(tk->inputstr);
  free(tk);
}

char * TKGetNextToken( TokenizerT * tk  ) {

  //keep track of the current token string
  char* tokenString = NULL;
  int tokenType = 0;
  //keep track of the sub conditions of each large case
  int subCondition = 0;

  /* if tokenType is 0 run the first letter check*/
  if(tokenType == 0){
    subCondition = 1;
    tokenType = getchartype(tk->ptr); //Get first char type
    if(tokenType == 5){ //Bad token
      tokenString = (char*)malloc(sizeof(char)*2);
      strncpy(tokenString, tk->ptr, 1);
      tokenString[1] = '\0';
      tk->ptr++;
      return tokenString;
    }
    tokenString = (char*)malloc(sizeof(char)*2);
    tokenString[0] = tk->ptr[0];
    tokenString[1] = '\0';
    tk->ptr++;    //Advance pointers
  }

  /* loop through the string using pointer */
  char *currentPtr;
  for (currentPtr = tk->ptr; *currentPtr != '\0'; currentPtr++){
    switch (tokenType) {
      //word case
    case 3:
      subCondition = wordNextChar(currentPtr);
      break;
      // space or new token
    default:
      return tokenString;
      break;
    }
    /* if the next char is not part of the previous token */
    if(subCondition == 0){
      tk->ptr++;
      return tokenString;
    }
    else{
      // add the char into the existing string
      size_t len = strlen(tokenString);
      tokenString = (char*)realloc(tokenString, len+2);
      tokenString[len++] = *currentPtr;
      tokenString[len++] = '\0';
      tk->ptr++;
    }
  }
  return tokenString;

}

/* function to check if the char is part of a word */
int wordNextChar(char * nextChar){
  if(isalpha(nextChar[0]) || isdigit(nextChar[0])){
    return 1;
  }
  return 0;
}


/* check the type using the first letter */
int getchartype(char* str){
  if(str == NULL){
    return 0;
  }
  if( isalpha(str[0])){
    return 3; //Token will be alphanumeric
  }
  /* type of word */
  else{
    return 5; //Token will not be alphanumeric
  }
}

/* function for printing */
int PrintToken(char* str){
  if(str == NULL){
    return 1;
  }

  /* Convert to lowercase and print word */
  if( isalpha(str[0]) ){
    char* lowercasestr;
    int len = strlen(str);
    int i = 0;
    for(i; i < len; i++)
      lowercasestr[i] = tolower(str[i]);
    printf("%s\n", lowercasestr);
    return 0;
  }
  else{
    return 1;
  }
}

/*
 * tokenizerfn will have a string input.
 * The string input contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 * Instantiate and return the sorted list
 */
SortedListPtr TKFN(char* input){
  if(input == NULL){
    printf("Trying to tokenize NULL String\n");
    return NULL;
  }

  TokenizerT* tokenizer = TKCreate(input);

  char* returnedtoken = NULL;
  // int returnTokenLen = 0;
  // int notErrorToken = 1;
  /* loop through the token string */
  while(tokenizer->ptr[0] != '\0'){
    returnedtoken = TKGetNextToken(tokenizer);
    if(returnedtoken == NULL)
      continue;

    PrintToken(returnedtoken);

    if(returnedtoken != NULL)
      free(returnedtoken);
  }

  SortedListPtr sortedList =  SLCreate(StringComparator, StringDestructor);
  TKDestroy(tokenizer);
  return sortedList;
}

void processToken(char * token, SortedListPtr list, char * filepath){
// takes token, check if it's in sorted list
  // increment the count by 1 or add a new node to the sorted list
//}
