#include<signal.h>
#include<stdio.h>
#include <unistd.h>

void sigcallback(int signumber)
{
  printf("i am SIGINT\n");
  return;
}
int main()
{
  signal(SIGINT,sigcallback);
  while(1)
  {
    sleep(1);
  }
  return 0;
}
