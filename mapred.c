#include <stdio.h>
#include <stdlib.h>

void map();
void reduce();

int main(int args, char * argv[]){

  //check for process or thread implemenatation
  if(argv[5] == "procs"){ // do process implementation

    map();
    reduce();
  }
  else{ // do thread implementation

    map();
    reduce();

  }
  return 0;
}

void map(){



}


void reduce(){



}
