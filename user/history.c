#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(2, "Usage: history <number of commands>\n");
    exit(1);
  }
  int n = atoi(argv[1]);
  if (n < 1)
  {
    fprintf(2, "Usage: history <number of commands>\n");
    exit(1);
  }
  // get history from history.txt file and print the last n lines of file
  int fd = open("history.txt", O_RDONLY);
  if (fd < 0)
  {
    fprintf(2, "history: cannot open history.txt\n");
    exit(1);
  }
  char buf[512];
  n = read(fd, buf, sizeof(buf));
  if (n < 0)
  {
    fprintf(2, "history: cannot read history.txt\n");
    exit(1);
  }
  for (int i = 0; i<n; i++) {
    if (buf[i] != 0) {
      fprintf(1, "%c", buf[i]);
    }
  }

  

  
  exit(0);
}
