#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "tokenizer.h"
#include "sorted-list.h"
//#include "listHelper.h"

int RecordComparator(struct node* p1, struct node* p2){
  struct node* r1 = (struct node*)p1;
  struct node* r2 = (struct node*)p2;

  char* word1 = (char*)r1->value;
  char* word2 = (char*)r2->value;

  int x = strcmp(word1, word2);
  if(x > 0) //Iterate forward
    return -1;
  else if(x == 0) //Special case
    return ComparePathHelper(r1->filepath, r2->filepath);
  else if(x < 0) //Found spot
    return 1;
}

int ComparePathHelper(char* p1, char* p2){
  int x = strcmp(p1, p2);

  if(x < 0) //Case where path is less
    return -1;
  else if(x == 0)
    return 0;
  else if(x > 0) //Case where path is greater
    return 1;
}

void RecordDestructor(struct node* p1){
  free((struct node*)p1);
  return;
}

void PrintRecordSortedList(SortedListPtr list){
  if(list == NULL){
    printf("Trying to print empty list\n");
    return;
  }

  struct node* ptr = list->head;
  while(ptr != NULL){
    printf("%s resides in path: \"%s\"  with count %d\n", ptr->value, ptr->filepath,ptr->refCount); //%p is void format specifier
    ptr = ptr->next;
  }
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

int directory_handler(char* path, SortedListPtr sortedlist){

  DIR* dir = opendir(path);
  if(dir == NULL){ //Error opening dir, return
    return 0;
  }

  struct dirent* dp;
  while( (dp = readdir(dir)) != NULL ){
    //Not NUll; get type of entry

    //Continue when d_name == . or ..
    if( strcmp(".", dp->d_name) == 0 || strcmp("..", dp->d_name) == 0 )
	continue;

    char* newpath = ConcatPath(path, dp->d_name); //Conc. the path w/ name
    //If file...call file handler on "name+nameoffile"
    if(dp->d_type == DT_REG){
      printf("FILE: %s\n", dp->d_name);
      file_handler(newpath, sortedlist);
    }
    //If dir...call directory handler on "name+nameofdir"
    else if(dp->d_type == DT_DIR){
      printf("DIR: %s\n", dp->d_name);
      directory_handler(newpath, sortedlist);
    }
    //free(newpath);

  }
  closedir(dir);
  return 1;
}

int file_handler(char* path, SortedListPtr sortedlist){
  //Open file
    //Tokenize words
    //Put words into index by filename

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
  TKFN(buffer, sortedlist, path);
  // printf("%s",buffer);

  fclose(fp);
  //free(buffer);

  return 1;
}

/*Create new file named argv[1]
 *Take in path from argv[2]
 */
int main(int argc, char** argv){
  //Check for 2 arguments, i.e. if (argc != 3)
  /*is argv[2] a directory or file?
   *something = opendir(argv[2]);
   *check error codes...
   *if ENOTDIR, file ; no errors, directory
   *
   *if no errno == ENOTDIR, run file handler
   *else if no other error, run directory handler
   */

  //TKFN("This @ only tokenizes words");
  SortedListPtr sortedlist = SLCreate(RecordComparator, RecordDestructor);
  directory_handler("./homedir", sortedlist);
  sortedlist = finalSort(sortedlist);
  PrintRecordSortedList(sortedlist);
  SLDestroy(sortedlist);
  return 0;
}
