#include "types.h"
#include "stat.h"
#include "user.h"
#include "libc/stdint.h"

char buf[1024];

void
size(int fd)
{
  int n;
  uint32_t i=0;

  while((n = read(fd, buf, sizeof(buf))) > 0) {
    i+=n;
  }
  if(n < 0){
    printf(1, "size: read error\n");
    exit();
  }
  printf(STDIN, "%u bytes (%f MB)\n", i, ((double)i/(1024.0*1024.0)));
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    size(0);
    exit();
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "size: cannot open %s\n", argv[i]);
      exit();
    }
    size(fd);
    close(fd);
  }
  exit();
}
