#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

void map();
void reduce();
int get_Fptr_num();

char IN_FILE[20];

pthread_mutex_t lock;

#define SHMSZ 1024

typedef struct word_key{
  char word;
  int count;
}word_key;


int main(int args, char * argv[]){

  /* check for correct command line args
   if(args !=15 || args !=10){
    printf("Please check number of arguments\n");
    return 1;
    }*/
    
  int *filemap;
  int shmid;
  const char *shm_name="OS";
  /* connect file name to global file variable */
  //IN_FILE= argv[10];
  
  
   
  	 /* create shared memory segment. */
  shmid = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    //perror("shm_open");
    //  exit(1);
  

	/* configure size */
	ftruncate(shmid,SHMSZ);
	
	//printf("before\n");
	
  /*check for process or thread implementation */
  if((strcmp(argv[4],"procs"))==0){ // do process implementation
  
 //   printf(" in process\n");
    int num_map= atoi(argv[6]);
  	int num_red= atoi(argv[8]);
  	
  	/* map shared memory to object*/
	filemap=mmap(0,SHMSZ,PROT_WRITE,MAP_SHARED,shmid,0);
  
  //printf("before map creator\n");
   int i;
  for(i=0; i < num_map ; i++){
  	*filemap = 1+i;
  	filemap++;
  }
  *filemap=NULL;
  

//  printf("after map creator\n");
  /* connect map to shared memory*/
  pid_t pid;
    for( i=0; i<num_map;i++){
     
    	if((pid=fork()) == 0){
    	//	map();
    	printf("fork fptr number %d\n",getpid());
   		}
   		else{
   			//printf("There was a fork error in %d count\n",i);
   		}
    }
    //reduce();
    /*
     if ((shmd = shmctl(shmid,)) == (int *) -1) {
        perror("shmat");
        exit(1);
    }*/
  }
  else if ((strcmp(argv[4],"threads"))==0){ // do thread implementation

   // map();
   // reduce();

  }
  else if((strcmp(argv[4],"extra"))==0){ // do extra implementation


  }
  return 0;
}


void map(){
  //printf("I was in map\n");
  
  /*
  int shmid;
  key_t key;
  int *shm;
  int *filemap;

  key = shm_key;

  /*
   * Locate shared memory segment.
   
  if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
    perror("shmget");
    exit(1);
  }

  /*
   * attach the segment to our data space.
   
  if ((shm = shmat(shmid, NULL, 0)) == (void *) -1) {
    perror("shmat");
    exit(1);
  }

  /*
   * read what the server put in the memory.
   

  for (filemap = shm; *filemap != NULL; filemap++){
  printf("Value: %d\n",*filemap);
   
  }
	printf("I was at the end of map\n");
*/
	int fptrnum= get_Fptr_num();
	printf("%d got fptr number %d\n",getpid(), fptrnum);
	
}


void reduce(){



}


int get_Fptr_num(){
	//printf("I was in get function\n");
	
  int shmid;
  int *fptr;
  const char *shm_name="OS";
  
  //pthread_mutex_lock(&lock);
  
	 /* create shared memory segment. */
  shmid = shm_open(shm_name, O_RDWR,  0666);
    //perror("shm_open");

  
	
  fptr=mmap(0,SHMSZ,PROT_READ | PROT_WRITE,MAP_SHARED,shmid,0);
	
	int ptr_num;
	
	for(; *fptr != NULL ; fptr++){
		
	if( *fptr == -1){
			//continue;
			//printf("I was in continue\n");
		}
		else{
			//printf("I was in mapper\n");
			/* read number and write "*" */
			ptr_num = *fptr;
			*fptr = (-1);
		 }
	}
	 
	//pthread_mutex_unlock(&lock);
	
	return ptr_num;
}

