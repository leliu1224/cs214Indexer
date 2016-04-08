#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "tokenizer.h"
#include "sorted-list.h"

//When the tokens are equal, compare paths
int ComparePathHelper(struct node* p1, struct node* p2){

  int x = strcmp(p1->filepath, p2->filepath);
  //int x = p1->refCount - p2->refCount;

  if(x < 0) //Case where path is less
    return -1;
  else if(x == 0)
    return 0;
  else//Case where path is greater
    return 1;
}

int RecordComparator(struct node* p1, struct node* p2){

  char* word1 = (char*)p1->value;
  char* word2 = (char*)p2->value;

  int x = strcmp(word1, word2);
  if(x > 0) //Iterate forward
    return -1;
  else if(x == 0) //Special case
    return ComparePathHelper(p1, p2);
  else //Found spot
    return 1;
}

void RecordDestructor(struct node* p1){
  free(p1->value); //Free members before freeing the node itself
  free(p1->filepath);
  free(p1);
  return;
}

void PrintRecordSortedList(SortedListPtr list){
  if(list == NULL || list->head == NULL){
    printf("Trying to print empty list\n");
    return;
  }
  struct node* prev = NULL;
  struct node* ptr = list->head;
  printf("{\"list\" : [\n");  //start list
  printf("\t{\"%s\" : [\n", ptr->value);

  do{ //Fix commas...
    if(prev != NULL){
      if( strcmp(prev->value, ptr->value) != 0 ){ //Unique word, new subset
	printf("\t]},\n");
	printf("\t{\"%s\" : [\n", ptr->value);
	if((ptr->next != NULL) && (strcmp(ptr->value, ptr->next->value) == 0))
	  printf("\t\t{\"%s\" : %d},\n", ptr->filepath, ptr->refCount);
	else
	  printf("\t\t{\"%s\" : %d}\n", ptr->filepath, ptr->refCount);
	prev = ptr;
	ptr = ptr->next;
	continue;
      }
    }
    if((ptr->next != NULL) && (strcmp(ptr->value, ptr->next->value) == 0))
      printf("\t\t{\"%s\" : %d},\n", ptr->filepath, ptr->refCount);
    else
      printf("\t\t{\"%s\" : %d}\n", ptr->filepath, ptr->refCount);
    prev = ptr;
    ptr = ptr->next;


    if(ptr == NULL)
      printf("\t]}\n");
}while(ptr != NULL);

  printf("]}\n");
  //printf("%s resides in path: \"%s\"  with count %d\n", ptr->value, ptr->filepath,ptr->refCount); //%p is void format specifier
}


char* ConcatPath(char* s1, char* s2) {
  //2 extra bytes to add '/' and Null terminate
  char* newstring = calloc(strlen(s1)+strlen(s2)+1+1, 1);
  strcpy(newstring, s1);
  newstring[strlen(newstring)] = '/';
  strcat(newstring, s2);
  newstring[strlen(newstring)] = '\0';
  return newstring; //Remember to free this
}

int file_handler(char* filename, char* path, SortedListPtr sortedlist){
  //Takes in filename, the path to get to the file, and the list as args
  //Open file, Tokenize words, Put words into index by filename
  FILE *fp;
  long lSize;
  char *buffer;

  fp = fopen ( path , "rb" );
  if( !fp ){
    perror(path);return 0;
  }

  fseek( fp , 0L , SEEK_END);
  lSize = ftell( fp );
  rewind( fp );

  /* allocate memory for entire content */
  buffer = calloc( 1, lSize+1 );
  if( !buffer ){ //Error check
    fclose(fp);printf("malloc failed in %s at line %d\n", __FILE__,__LINE__);return 0; }

  /* copy the file into the buffer */
  if( 1!=fread( buffer , lSize, 1 , fp) ){ //Error check
    fclose(fp),free(buffer);printf("fread failed in %s at line %d, caused by filepath: \"%s\"\n", __FILE__,__LINE__, path);return 0; }

  /* do your work here, buffer is a string contains the whole text */  
  TKFN(buffer, sortedlist, filename);
  // printf("%s",buffer);
  
  free(buffer);
  fclose(fp);
  return 1;
}

int directory_handler(char* path, SortedListPtr sortedlist){

  DIR* dir = opendir(path);
  if(dir == NULL){ //Error opening dir, return
    printf("Failed to opendir\n");
    return 0;
  }

  struct dirent* dp;
  while( (dp = readdir(dir)) != NULL ){ //if not null, get type of entry
    //Continue when d_name == . or ..
    if( strcmp(".", dp->d_name) == 0 || strcmp("..", dp->d_name) == 0 )
	continue;

    char* newpath = ConcatPath(path, dp->d_name); //Conc. the path w/ name
    //If file...call file handler on "name+nameoffile"
    if(dp->d_type == DT_REG){
      printf("FILE: %s\n", dp->d_name);
      file_handler(dp->d_name, newpath, sortedlist);
    }
    //If dir...call directory handler on "name+nameofdir"
    else if(dp->d_type == DT_DIR){
      printf("DIR: %s\n", dp->d_name);
      directory_handler(newpath, sortedlist);
    }
    free(newpath);

  }
  closedir(dir);
  return 1;
}

void WRITEFILE( FILE* OUTPUT, SortedListPtr list){
  if(list == NULL || list->head == NULL){
    printf("Trying to print empty list\n");
    return;
  }
  struct node* prev = NULL;
  struct node* ptr = list->head;
  fprintf(OUTPUT, "{\"list\" : [\n");  //start list
  fprintf(OUTPUT, "\t{\"%s\" : [\n", ptr->value);

  do{ //Fix commas...
    if(prev != NULL){
      if( strcmp(prev->value, ptr->value) != 0 ){ //Unique word, new subset
	fprintf(OUTPUT, "\t]},\n");
	fprintf(OUTPUT, "\t{\"%s\" : [\n", ptr->value);
	if((ptr->next != NULL) && (strcmp(ptr->value, ptr->next->value) == 0))
	  fprintf(OUTPUT, "\t\t{\"%s\" : %d},\n", ptr->filepath, ptr->refCount);
	else
	  fprintf(OUTPUT, "\t\t{\"%s\" : %d}\n", ptr->filepath, ptr->refCount);
	prev = ptr;
	ptr = ptr->next;
	continue;
      }
    }
    if((ptr->next != NULL) && (strcmp(ptr->value, ptr->next->value) == 0))
      fprintf(OUTPUT, "\t\t{\"%s\" : %d},\n", ptr->filepath, ptr->refCount);
    else
      fprintf(OUTPUT, "\t\t{\"%s\" : %d}\n", ptr->filepath, ptr->refCount);
    prev = ptr;
    ptr = ptr->next;


    if(ptr == NULL)
      fprintf(OUTPUT, "\t]}\n");
}while(ptr != NULL);

  fprintf(OUTPUT, "]}\n");
  //printf("%s resides in path: \"%s\"  with count %d\n", ptr->value, ptr->filepath,ptr->refCount); //%p is void format specifier

  printf("WRITEFILE success... check your output file\n");
  return;
}
