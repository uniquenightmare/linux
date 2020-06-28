#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
  int fd = open("./a.txt",O_RDWR);
  if(fd < 0)
  {
    perror("open");
    return 0;
  }
  write(fd,"61-linux",8);

  while(1)
  {
    sleep(1);
  }
  return 0;
}
