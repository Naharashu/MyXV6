#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(void)
{
  int fd;
  char buf[2048];
  int i;

  memset(buf, 'A', sizeof(buf));

  fd = open("bigfile", O_CREATE | O_WRONLY);
  if(fd < 0){
    printf(2, "open failed\n");
    exit();
  }

  for(i = 0; i < 524; i++){
    if(write(fd, buf, 1024) != 1024){
      printf(2, "write failed at block %d\n", i);
      break;
    }
  }

  close(fd);
  printf(1, "wrote %d bytes\n", i * sizeof(buf));
  exit();
}
