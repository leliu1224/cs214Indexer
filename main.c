#include "indexer.h"
#include <dirent.h>
#include <errno.h>

/*Create new file named argv[1]
 *User must indicate whether to overwrite if file already exists 
 *Take in path from argv[2]
 *Will check if the given path is a valid file/dir
 */
int main(int argc, char** argv){
  if(argc != 3){
    printf("Error: wrong number of arguments caught in file:%s at line: %d\n", __FILE__, __LINE__);
    printf("Please give input in the format \"./index <output name> <dir or file>\"\n");
    return 0;}

  FILE* OUTPUT = NULL;
  FILE* closeme = NULL;
  if(  (closeme = (fopen(argv[1], "r"))) != NULL){ //Check if output file already exists, does user wish to overwrite? 

    printf("File already exists. Would you like to overwrite the file: %s?\n", argv[1]);
    printf("Enter '0' if you dont want to overwrite\n");printf("Enter '1' if you want overwrite the file\n");

    int userinput;
    scanf("%d", &userinput);
    switch(userinput){
    case 0:
      printf("Program will end because you chose not to overwrite. Rerun with a new argument\n");
      fclose(closeme);
      exit(1);
    case 1://Continue through exits
      break;
    default:
      printf("Program will end; you did not give proper input\n");
      fclose(closeme);
      exit(1);
    }
  }

  OUTPUT = fopen(argv[1], "w"); //Open argv[1] in writemode, referenced by FILE* OUTPUT.                                                       

  if(OUTPUT == NULL){
    printf("Failed to open output file in write mode\n");fclose(closeme);exit(1);
  }

  /*is argv[2] a directory or file?
   *something = opendir(argv[2]);
   *if no errno == ENOTDIR, run file handler 
   *else if no other error, run directory handler 
   */

  SortedListPtr sortedlist = SLCreate(RecordComparator, RecordDestructor);

  errno = 0;
  DIR* CHECKERRNO = opendir(argv[2]);
  if(errno == ENOTDIR){
    file_handler(argv[2], argv[2], sortedlist);
  }
  else if(errno == ENOENT){
    SLDestroy(sortedlist);fclose(OUTPUT);fclose(closeme);closedir(CHECKERRNO);
    printf("ENOENT Error. Cannot open NONEXISTING file/dir\n");exit(1);
  }
  else{
    directory_handler(argv[2], sortedlist);
  }

  //sortedlist = finalSort(sortedlist);
  FirstSort(sortedlist);
  SecondSort(sortedlist); 
  WRITEFILE(OUTPUT, sortedlist);
  PrintRecordSortedList(sortedlist);
  

  SLDestroy(sortedlist);
  if(OUTPUT)
    fclose(OUTPUT);
  if(closeme)
    fclose(closeme);
  if(CHECKERRNO)
    closedir(CHECKERRNO);
  return 0;
}
