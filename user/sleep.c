#include "kernel/types.h"
#include "user/user.h"
// Sleep is a malloc tester now
#include "memory_management.h"

int main(int argc, char *argv[])
{

    //Checks basic malloc expansion
    //int* array = 
    (int*) _malloc(100 * sizeof(int));
    int* array2 = (int*) _malloc(50 * sizeof(int));
    int* array3 = (int*) _malloc(50 * sizeof(int));
    int* array4 = (int*) _malloc(100 * sizeof(int));
    printf("Done allocating");
    //Checks if free function works (with the memory merge thing)
    _free(array3);
    _free(array2);
    
    //This is to check if the future proof memory merge works
    _free(array4);
    printf("Done freeing");
    //Checks if it has merged them correctly
    array2 = (int*) _malloc(170 * sizeof(int));
    array3 = (int*) _malloc(10 * sizeof(int));
    //Checks if it can allocate a new block even though some is free
    //int* array5 = 
    (int*) _malloc(30 * sizeof(int));

    printf("Done reallocating");
    exit(0);
}
/*
// function for usage informational message
void usage()
{
  fprintf(1, "usage: sleep clockticks\n");
}
int main(int argc, char *argv[])
{
  // declare int variable to hold number of ticks
  int ticks;
  // if fewer than 2 args call informational message
  if (argc < 2)
  {
    usage();
    // exit logging an error state
    exit(1);
  }
  // convert passed in arg to an integer
  ticks = atoi(argv[1]);
  // call sleep system call with ticks
  sleep(ticks);
  // exit logging success state
  exit(0);
}
*/


