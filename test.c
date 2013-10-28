#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "crypt_ioctl.h"

int main()
{
  int fd = 0;
  int ret = 0;
  char buff[80] = "";
  
  fd = open("/dev/cryptctl",O_RDWR);
  
  ioctl(fd, CRYPT_READ);
  ioctl(fd, CRYPT_WRITE);
  
  close(fd);
}
