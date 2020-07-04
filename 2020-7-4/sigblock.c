#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sigcallback(int signum)
{
  printf("signum:%d\n",signum);
}
int main()
{
  signal(2,sigcallback);
  signal(40,sigcallback);
  sigset_t set,oldset;
  sigemptyset(&set);
  sigemptyset(&oldset);
  sigfillset(&set);
  sigfillset(&oldset);
  sigprocmask(SIG_BLOCK,&set,&oldset);
  getchar();
  sigprocmask(SIG_UNBLOCK,&set,NULL);
  
  while(1)
  {
    sleep(1);
  }
  return 0;
}
