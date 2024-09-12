#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2], p2[2];
  if (pipe(p1) != 0 || pipe(p2) != 0) {
    printf("pipe error\n");
    exit(1);
  }

  char buf;
  int pid = fork();
  if (pid > 0) {
    write(p1[1], &buf, 1);
    read(p2[0], &buf, 1);
    printf("%d: received pong\n", getpid());
    pid = wait((int *) 0);
  } else if (pid == 0) {
    read(p1[0], &buf, 1);
    printf("%d: received ping\n", getpid());
    write(p2[1], &buf, 1);
    exit(0);
  } else {
    printf("fork error\n");
  }

  exit(0);
}
