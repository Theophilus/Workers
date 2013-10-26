#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int main()
{
  int fd = 0;
  int ret = 0;
  char buff[80] = "";
  
  fd = open("/dev/cryptctl",O_RDONLY);
  printf("%d\n",fd);
  fprintf(stderr, "fopen() failed: %s\n", strerror(errno));
  close(fd);
}
