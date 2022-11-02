
#include "kernel/types.h"
#include "user/user.h"
int main(int argc, char *argv[])
{
  //read lines from the standard input
  //run a command for each line supplying the lines as arguments to the command

  //use fork and exec to invoke the command on each line of input. 
  //Use wait in the parent to wait for the child to complete the command

  //To read individual lines of input, read a character at a time until a newline ('\n') appears.
  //kernel/param.h declares MAXARG, which may be useful if you need to declare an argv array.
  //Changes to the file system persist across runs of qemu; to get a clean file system run make
  //clean and then make qemu.

  for (int i=0; i<argc; i++)
  {
    printf("argv[%d] = %s", i, argv[i]);
  }
  
  exit(0);
}
