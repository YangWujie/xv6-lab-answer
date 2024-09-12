#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_LINE_LENGTH 512

char buf[MAX_LINE_LENGTH];
char *args[MAXARG];

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(2, "Usage: xargs cmd [args]\n");
    exit(1);
  }

  args[0] = argv[1];
  char **cla = args + 1;
  for (int i = 2; i < argc; i++) {
    *cla = argv[i];
    cla++;
  }

  char *s = buf;
  char *e = buf;
  char **rla = cla;
  while (read(0, e, 1) != 0) {
    if (*e == ' ') {
      if (s == e)
        continue;
      *e = 0;
      *rla = s;
      rla++;
      e++;
      s = e;
    } else if (*e == '\n') {
      *e = 0;
      *rla = s;
      *(rla + 1) = 0;

      int pid = fork();
      if (pid > 0) {
        wait(0);
      } else if (pid == 0) {
        exec(argv[1], args);
        printf("exec error.\n");
        exit(1);
      } else {
        printf("fork error.\n");
        exit(1);
      }

      rla = cla;
      s = buf;
      e = buf;
    } else {
      e++;
    }
  }
  
  exit(0);
}
