#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

void map();
void reduce();
void add_map_to_shm();

char IN_FILE[20];
char OUT_FILE[20];
int file_ptr;
int filechunk_size;
//pthread_mutex_t lock;

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
    
  int *filemap,shm;
  int shmid;
  const char *shm_name="filemap";
  
  /* connect file name to global file variable */
  IN_FILE= argv[10];
  //OUT_FILE= argv[0];
   
  	 /* create shared memory segment. */
  shmid = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
  

	/* configure size */
	ftruncate(shmid,SHMSZ);
	
	
	int num_map= atoi(argv[6]);
  	int num_red= atoi(argv[8]);
  	
	filechunk_size=(sizeof(IN_FILE)/num_map;
	
	//printf("before\n");
	
  /*check for process or thread implementation */
  if((strcmp(argv[4],"procs"))==0){ // do process implementation
  
 //   printf(" in process\n");
  	printf("num procs: %d\n",num_map);
  	
  	/* map shared memory to object*/
	filemap=mmap(0,SHMSZ,PROT_WRITE,MAP_SHARED,shmid,0);
  
  
//  printf("after map creator\n");
  /* connect map to shared memory*/
  int i;
  pid_t pid;
    for( i=0; i<num_map;i++){
     pid_t pid=fork();
    	if((pid) == 0){
    	file_ptr=1+i;
    		map();
    	//printf("fork fptr number %d\n",getpid());
    	exit(0);
   		}
   		else if(pid<0){
   			printf("There was a fork error in %d count\n",i);
   		}
   		else{
   		
   		wait(NULL);
   		}
    }
    //reduce();
   
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
  printf("I was in map\n");
  
  /*
  int shmid;
  int *shm;
  int *filemap;
  int start_line;
*/

	/* get file pointer number*/
	int fptr_num= file_ptr;
	printf("%d got fptr number %d\n",getpid(), fptr_num);
	
	char buffer=malloc(filechunk_size);
	if(fptr_num ==1){
		start_line=1;
	
	}
	else{
		start_line= (filechunk_size*(fptr_num-1))+1;
	
	}
	FILE *input= IN_FILE;
	char line[130];
  if( (input=fopen(inputfile,"r")) == NULL){
      printf("%s does not exist\n",inputfile);
      exit(1);
    }
	else{
	  while(!feof(input)){
	  	fseek(
	    //fread(buff,sizeof(buff),1,input);
	    //printf("%s file read",buff);
	    //fwrite(buff,sizeof(buff),1,output);
	    
	    fgets(buff,sizeof(buff),input);
	    fputs(buff,output);
	  }
	  fclose(output);
	}
      fclose(input);
      
    }
	
}


void reduce(){



}

void add_map_to_shm(char *result){



}

