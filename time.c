#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{
  int start, pid;

  if(argc < 2){
    printf(STDERR, "usage: time command [args...]\n");
    exit();
  }

  start = uptime();
  pid = fork();
  if(pid < 0){
    printf(STDERR, "time: fork failed\n");
    exit();
  }
  if(pid == 0){
    exec(argv[1], &argv[1]);
    printf(STDERR, "time: exec %s failed\n", argv[1]);
    exit();
  }

  setforegroundpid(pid);
  wait();
  setforegroundpid(0);
  printf(2, "%f seconds\n", ((float)uptime() - (float)start)/100.0f);
  exit();
}
