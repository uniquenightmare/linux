#include<stdio.h>
#include<signal.h>
volatile int g_val = 1;
void sigcallback(int signum)
{
 // printf("signum:%d\n",signum);
  g_val = 0;
  printf("signum:%d,%d",signum,g_val);
}
int main()
{
  signal(2,sigcallback);
  while(g_val)
  {
    //printf("g_val:%d\n",g_val);
   // sleep(1);
  }
 // printf("g_val:%d\n",g_val);
  return 0;
}
