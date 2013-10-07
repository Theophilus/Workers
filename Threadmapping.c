#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <errno.h>
#include <unistd.h>

struct wordchain{
	char *word;
	struct wordchain *next;
};
typedef struct wordchain wordlist;

wordlist *root=NULL;
wordlist *runner=NULL;

void *thread_mapping( void *ptr );
void mapper();
int i;
FILE *f_in, *f_out;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv){

	char *app, *impl, *maps, *reduces, *input, *output;
	int num_maps, num_reduces;

	if (argc < 7) {
		fprintf(stderr, "usage: %s [wordcount/sort] [procs/threads] [#ofmaps] [#ofreduces] [input] [output]\n", argv[0]);
		return -1;
	}

	app 	= argv[1];
	impl	= argv[2];
	maps	= argv[3];
	reduces	= argv[4];
	input 	= argv[5];
	output 	= argv[6];

	if (strcmp(app,"wordcount")!=0 && strcmp(app,"sort")!=0){
		fprintf(stderr, "Please choose 'wordcount' or 'sort' for app\n");
		return -2;
	}
	if (strcmp(impl,"procs")!=0 && strcmp(impl,"threads")!=0) {
		fprintf(stderr, "Please choose 'procs' or 'threads' for impl\n");
		return -3;
	}
	/*if (maps!=) {
		//Check if input is a  number
		return -4;
	}*/
	num_maps = atoi(maps);
	/*if (reduces!=) {
		//Check if input is a  number
		return -5;
	}*/
	num_reduces = atoi(reduces);
	f_in = fopen(input, "r");
	if (f_in == NULL) {
		fprintf(stderr, "Could not open input file\n");
		return -6;
	}
	f_out = fopen(output, "w");
	if (f_out == NULL) {
		fprintf(stderr, "Could not open output file\n");
		return -7;
	}
	
   	if ((strcmp(impl,"threads"))==0){ // do thread implementation
		pthread_t *thread = malloc(sizeof(pthread_t)*num_maps);
		int *threadcount  = malloc(sizeof(int)*num_maps);
		while(!feof(f_in)){
			for (i=0;i<num_maps;i++){
				if (feof(f_in))
					break;
				threadcount[i] = pthread_create( &thread[i], NULL, thread_mapping, NULL);//(void*) f_in););
			}
			for (i=0;i<num_maps;i++)
				pthread_join(thread[i], NULL);
		}
		free(thread);
		free(threadcount);
	}
	//Shared Memory implementation

	int shm = shm_open("/myshm", O_RDWR | O_CREAT, 0777);
	if (shm < 0) {
		perror("Could not open shm:");
		return 0;
	}

	int counter=0;
	int size=0;
	int len=0;
	runner = root;
	
	while(runner!=NULL){		
		len+=strlen(runner->word)+1;
		counter++;
		runner = runner -> next;
	}
	size = len+counter*sizeof(wordlist);//+counter*sizeof(wordlist->word)+counter*sizeof(wordlist->next);
	write(shm, root, size));
	//printf("Result: %d\n", write(shm, root, strlen(word_in) + 1));
	//write(shm, word_in, strlen(word_in) + 1)

	//printf("%d\n%d\n",size,len);
/*
	runner = root;
	while(runner){
		printf("%s\n", runner->word);
		runner = runner -> next;
	}
	runner = root;
	wordlist *p;
	wordlist *nexter;
	for (p = runner; p!=NULL; p=nexter){
		free(p->word);
		nexter = p->next;
		free(p);
	}
*/	
	exit(0);
}

void *thread_mapping( void *ptr ){
	mapper();	
}
void mapper()
{	
	FILE *str;
	//str = (FILE*) ptr;	
	str=f_in;	
	char newline[150];
	fgets(newline,150,str);
	char *endptr = strchr(newline,'\n');
	if (endptr != NULL)
		*endptr = '\0';
	char *line_in=strdup(newline);
	
	if (feof(str)){
		return;
	}
	char *tempstr, *token, *saveptr;
	for (tempstr=line_in;;tempstr=NULL){
		token = strtok_r(tempstr," ",&saveptr);
		if (token == NULL)
			break;	
		pthread_mutex_lock(&mutex);		//not sure yet if mutex is necessary here, or anywhere
			runner = (wordlist *) malloc (sizeof(wordlist));
			runner->word = (char*) malloc(sizeof(token));
			strcpy(runner->word,token);
			runner->next = root;
			root = runner;
		pthread_mutex_unlock(&mutex);
	}
	free(line_in);		//frees strdup malloc'd variable
}

//Ignore following: They are just notes
	//char *LineBuffer = malloc(length * sizeof(char));
	//char LineBuffer[150];	
	/*int length = 100; //initial size
	char * name = malloc(length * sizeof(char)); //allocate mem for 100 chars
	int count = 0; //to keep track of how many chars have been used
	char c; // to store the current char 
	while((c = getchar()) != '\n'){ //keep reading until a newline
		if(count >= length)
			name = realloc(name, (length += 10) * sizeof(char)); //add room for 10 more chars
		name[count++] = c
	}
	*/
	//fgets(LineBuffer, 150, 
     	//char *message;
     	//message = (char *) ptr;
     	//printf("%s \n", message);
