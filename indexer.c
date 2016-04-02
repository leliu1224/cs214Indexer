#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "tokenizer.h"

char* ConcatPath(char* s1, char* s2){
  //2 extra bytes to add '/' and Null terminate
  char* newstring = calloc(strlen(s1)+strlen(s2)+1+1, 1);
  strcpy(newstring, s1);
  newstring[strlen(newstring)] = '/';
  strcat(newstring, s2);
  newstring[strlen(newstring)] = '\0';
  return newstring; //Remember to free this
}

int directory_handler(char* path){

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
      file_handler(newpath);
    }
    //If dir...call directory handler on "name+nameofdir"
    else if(dp->d_type == DT_DIR){ 
      printf("DIR: %s\n", dp->d_name);
      directory_handler(newpath);
    }
    free(newpath);

  }
  closedir(dir);
  return 1;
}

int file_handler(char* path){
  //Open file
    //Tokenize words
    //Put words into index by filename
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
  directory_handler("./homedir");
  return 0;
}
