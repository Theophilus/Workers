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
void *run_job_scheduler(int maps);

char IN_FILE[30];
const char OUT_FILE[30];
int schedule[100];
int job_num;

//pthread_mutex_t lock;

#define SHMSZ 1024

typedef struct word_key{
 	char *word;
	int count;
	struct word_key *nextPtr;
} Word_key;


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
	strcpy(IN_FILE, argv[10]);
 	 // printf("file name: %s\n",IN_FILE);
 	 strcpy(OUT_FILE, argv[12]);
 	 
  	/* create shared memory segment. */
  	 shmid = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
  
	/* configure size */
	ftruncate(shmid,SHMSZ);
	
	int num_map= atoi(argv[6]);
  	int num_red= atoi(argv[8]);
  	int f_size,i ;
  	
  	/* create job junks */
	run_job_scheduler(num_map);
		
	for(i=0;i<=num_map;i++){
		printf("%d array has: %d start schedule\n",i,schedule[i]);
			
	}
	
	//printf("file size : %d chunk size :%d\n",f_size,filechunk_size);
	
  	/*check for process or thread implementation */
  	if((strcmp(argv[4],"procs"))==0){ // do process implementation
  
 		//printf(" in process\n");
  		//printf("num procs: %d\n",num_map);
  	
  		/* map shared memory to object*/
		filemap=mmap(0,SHMSZ,PROT_WRITE,MAP_SHARED,shmid,0);
  
  
		//printf("after map creator\n");
  		
  		pid_t pid;
   		for( i=0; i < num_map;i++){
    
   			pid_t pid=fork();
    		if((pid) == 0){
    			job_num=0+i;
    			//printf("start: %d end:%d\n",job_start_ptr,job_end_ptr);
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
  	//printf("I was in map\n");
  
  	/*
 	 int shmid;
 	 int *shm;
 	 int *filemap;
 	 int start_line;
	*/
	
	/* word key pointer*/
	Word_key *main_ptr;
	Word_key *wk_ptr;
	Word_key *search_ptr;
	Word_key *curr_ptr;
	
	int word_exist=-1;
	
	//main_ptr=malloc(sizeof(Word_key));
	
	/* word buff*/
	//char str[80];
	//printf("size of string :%d\n", sizeof(str));
	
	/* get file pointer number*/
	// printf("Job number =>%d\n",job_num);
	
	int start_pos=schedule[job_num];
	int end_pos=schedule[job_num+1];
	
	printf("%d got start number =>%u and end number =>%u\n",getpid(),start_pos, end_pos);
	
	//char buffer=malloc(filechunk_size);
	
	FILE *fp;
	char line[20];
	char *str;
	// printf("before file loop");
	if( (fp=fopen(IN_FILE,"r")) == NULL){
		printf("%s does not exist\n",IN_FILE);
     	exit(1);
    }
	else{
		rewind(fp);
		fseek(fp,start_pos,SEEK_CUR);
		printf("fptr pos =>%d\n",ftell(fp));
		
	 	while( ftell(fp) != end_pos){
	  		/* get a word from file */
			fscanf(fp,"%s",&str);
			printf("word scanned =>%s\n",&str);
			
			search_ptr=main_ptr;
			while( search_ptr != NULL ){
				//printf("in loop with word: %s\n",search_ptr->word);
				
				if( str == search_ptr->word ){
					printf("word exists!!!!!\n");
					word_exist=0;
					search_ptr->count++;
					printf("word: [%s] count:[%d]\n",&search_ptr->word,search_ptr->count);
					
					break;
				}
				
				search_ptr=search_ptr->nextPtr;
			}
			
			if( word_exist = -1 ){
			    printf("in new word\n");
			    wk_ptr=malloc(sizeof(Word_key));
				wk_ptr->word=str;
				wk_ptr->count=1;
			    
			    if(main_ptr == NULL){
			    
			    	printf("in  NULL new word\n");
			    	main_ptr = curr_ptr = wk_ptr;
			    }
			    else{
			    	printf("in next new word\n");
					curr_ptr->nextPtr= wk_ptr;
					curr_ptr= curr_ptr->nextPtr;	
				}
			}
				
	 	}
	 
	}
      fclose(fp);
      
    free(wk_ptr);
	
}


void reduce(){



}

void *run_job_scheduler(int maps){

	//printf(" I was in scheduler\n");
	
	int f_chunk_size,i,f_size;
	char c;
	schedule[0]=0;
	FILE *fp;
	
  	if( (fp=fopen(IN_FILE,"r")) == NULL){
      printf("%s does not exist\n",IN_FILE);
      exit(1);
    }
	else{
	  fseek(fp, 0L, SEEK_END);
		f_size = ftell(fp);
		//printf(" fp position before => %d char %c\n",ftell(fp),fgetc(fp));
		
		f_chunk_size=f_size/maps;
		//printf(" file size: %d chunk size => %d \n",f_size,f_chunk_size);
	
		for(i=1;i<= maps;i++){
			rewind(fp);
		//	printf("rewind position => %d\n",ftell(fp));
		
			fseek(fp,i*f_chunk_size-1,SEEK_CUR);
		
			//printf(" fp position before => %d\n",ftell(fp));
			//printf(" fp position char => %c\n",fgetc(fp));
		
			while((c=fgetc(fp)) != '\n'){
		//		printf(" while loop fp position char => %c\n",c);
		
			}
		//printf(" fp position  at end of line => %d\n",ftell(fp));
		schedule[i]=1+(int) ftell(fp);
		}
	
	fclose(fp);
	}
	
}

void add_map_to_shm(char *result){




}

