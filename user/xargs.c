
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  // read lines from the standard input
  // run a command for each line supplying the lines as arguments to the command

  // use fork and exec to invoke the command on each line of input.
  // Use wait in the parent to wait for the child to complete the command

  // To read individual lines of input, read a character at a time until a newline ('\n') appears.
  // kernel/param.h declares MAXARG, which may be useful if you need to declare an argv array.
  // Changes to the file system persist across runs of qemu; to get a clean file system run make
  // clean and then make qemu.

  //Read lines from the standard input:
  //buf is gonna be a string that contains all the input lines.
  char buf[512];
  //input is gonna be the directory to read from the standard input.
  char input;
  //index used to initialize the standard input into buffer. 
  int bufIndex = 0;
  while(read(0, &input, 1))
  {
    //if the input is not empty, then add it to the buffer.
    if(input != '\n')
    {
      buf[bufIndex] = input;
      bufIndex++;
    }else {

      // do the exec for the current lineCommand
      //32 is MAXARG.
      char *newArgv[32];
      //We ignore the first argument from argv (since that is xargs)
      //and we start from the second argument and copy into newArgv
      for (int i = 0; i < argc; i++)
      {
        newArgv[i] = argv[i + 1];
      }
      //We add the command to the end of the newArgv.
      newArgv[argc - 1] = buf;

      if (fork() == 0) //child
      {
        //We execute the second index (name of a command fx grep, or echo)
        //with the new arguments (including the command from the line).
        exec(argv[1], newArgv);
      }
      else
      {
        //parents wait for child to be done and then we reset the lineCommandIndex
        //and the lineCommand array.
        wait(0);
      }

      // reset buffer
      memset(buf, 0, sizeof(buf));
      bufIndex = 0;
    }
    
  }

  exit(0);
}
