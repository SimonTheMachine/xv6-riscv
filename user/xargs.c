
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
  char input[512];
  //to check if input is empty or not.
  int n;
  //index used to initialize the standard input into buffer. 
  int bufIndex = 0;
  for (;;)
  {
    //reset input since we use the same array to read from 
    //standard input each time. 
    memset(input, 0, sizeof(input));
    //read from standard input.
    n = read(0, input, sizeof(input));
    if (n == 0)
    {
      break;
    }
    //Copy the new input into buf. 
    for (int i = 0; i < 512; i++)
    {
      if (input[i] != 0)
      {
        buf[bufIndex] = input[i];
        bufIndex++;
      }
      else
      {
        break;
      }
    }
  }
  printf("\nBuf is:\n \n%s \n \n", buf);

  //To store the command (before \n)
  char lineCommand[512];
  //so we know which index to put new characters into the command array.
  int lineCommandIndex = 0;
  for (int i = 0; i < sizeof(buf); i++)
  {
    //If we reach a new line, we know that we have a command.
    if (buf[i] == '\n')
    {
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
      newArgv[argc - 1] = lineCommand;

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

      // reset lineCommand
      memset(lineCommand, 0, sizeof(lineCommand));
      lineCommandIndex = 0;
    }
    else
    {
      //If we haven't reached a new line, we add the character to the lineCommand.
      lineCommand[lineCommandIndex] = buf[i];
      lineCommandIndex++;
    }
  }

  exit(0);
}
