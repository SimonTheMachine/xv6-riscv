#include "kernel/types.h"
#include "user/user.h"
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
