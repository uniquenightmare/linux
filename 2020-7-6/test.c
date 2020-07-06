#include<stdio.h>
#include<signal.h>
#include<unistd.h>
int count = 20;
void sigcallback(int signum)
{
  printf("i recv sig no is %d\n",signum);
  count--;
}
int main()
{
  signal(2,sigcallback);
  int i = 1;
  while(count--)
  {
    printf("%d\n",i);
    i++;
    sleep(1);
  }
  while(1)
  {
    printf("I am main thread\n");
    sleep(1);
  }
  return 0;
}
