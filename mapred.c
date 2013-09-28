#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void map();
void reduce();

const int FILEMAP_KEY= 1234;
const int MAIN_KEY= 1235;
#define SHMSZ 1024

typedef struct word_key{
  char word[20];
  int count;

}word_key;


int main(int args, char * argv[]){

  /* check for correct command line args*/
  if(args !=15 || args !=10){
    printf("Please check number of arguments\n");
    return 1;
  }

  char c;
  int filemap_shmid;
  int  main_shmid;
  key_t main_mem_key;
  key_t filemap_mem_key;
  char *shm;
  int file_ptr_map[atoi(argv[2])];

  main_mem_key=MAIN_KEY;
  filemap_mem_key=FILEMAP_KEY;

  /* Create filemap memory segment. */
  
  if ( (filemap_shmid = shmget(filemap_mem_key, SHMSZ,  0666)) < 0){
    perror("shmget");
    // printf("shmget error");
    exit(1);
  }

  /* Create main memory segment.*/
 
 if ((main_shmid = shmget(main_mem_key, SHMSZ, IPC_CREAT | 0666)) < 0) {
   perror("shmget");
   //printf("shmget error"); 
   exit(1);
  }

  //check for process or thread implemenatation
  if((strcmp(argv[4],"procs"))==0){ // do process implementation
    printf("i was in process\n");
    map();
    reduce();
  }
  else if ((strcmp(argv[4],"threads"))==0){ // do thread implementation

    map();
    reduce();

  }
  else if((strcmp(argv[4],"extra"))==0){ // do extra implementation


  }
  return 0;
}

void map(){



}


void reduce(){



}
