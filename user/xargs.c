
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
  char buf[512];
  int n;
  for (;;)
  {
    n = read(0, buf, sizeof(buf));
    if (n < 0)
    {
      fprintf(2, "xargs: read error\n");
      exit(1);
    }
    if (n == 0)
    {
      break;
    }
    if (write(1, buf, n) != n) {
      fprintf(2, "xargs: write error\n");
      exit(1);
    }
  }

  char *newArgv[32];
  for (int i = 0; i < argc; i++)
  {
    newArgv[i] = argv[i + 1];
  }
  newArgv[argc] = buf;
  //print buf
  fprintf(1, "buf: %s\n", buf);

  for (int i = 0; i < argc; i++)
  {
    printf("argv[%d] = %s \n", i, argv[i]);
  }
  printf("result: \n");
  exec(argv[1], newArgv);
  fprintf(2, "xargs: exec %s failed\n", argv[1]);

  exit(0);
}
