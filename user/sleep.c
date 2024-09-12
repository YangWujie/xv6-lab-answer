#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(2, "Usage: sleep duration\n");
    exit(1);
  }

  int d = atoi(argv[1]);
  sleep(d);

  exit(0);
}
