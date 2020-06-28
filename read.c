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
  char buf[1024] = {0};
  read(fd,buf,sizeof(buf)-1);
  printf("%s\n",buf);
  while(1)
  {
    sleep(1);
  }
  return 0;
}
