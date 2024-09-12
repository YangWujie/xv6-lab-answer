#include "kernel/types.h"
#include "user/user.h"

void do_work(int fd) {
  int p[2];
  int prime, n;

  if (fd > 0) {
    if (read(fd, &prime, sizeof(int)) <= 0)
      return;
    printf("prime %d\n", prime);
  }

  if (pipe(p) != 0) {
    printf("pipe error\n");
    return;
  }

  int pid = fork();
  if (pid > 0) {
    close(p[0]);
    if (fd < 0) {
      for (int i = 2; i <= 35; i++) {
        write(p[1], &i, sizeof(int));
      }
    } else {
      while (read(fd, &n, sizeof(int)) > 0) {
        if (n % prime != 0)
          write(p[1], &n, sizeof(int));
      }
    }
    close(p[1]);
    wait(0);
  } else if (pid == 0) {
    close(p[1]);
    do_work(p[0]);
  } else {
    printf("fork error\n");
  }
}


int
main(int argc, char *argv[])
{
  do_work(-1);

  exit(0);
}
