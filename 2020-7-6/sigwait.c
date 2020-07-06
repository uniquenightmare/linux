#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
void sigcallback(int signum)
{
  printf("recv signum is %d\n",signum);
  wait(NULL);
}
int main()
{
  signal(SIGCHLD,sigcallback);
  int pid = fork();
  if(pid < 0)
  {
    perror("fork");
    return 0;
  }
  else if(pid == 0){
    sleep(3);
    printf("i am child ,i am exit\n");
    _exit(0);
  }
  else{
    while(1)
    {
     
      printf("I am father\n");
      sleep(3);
    }
  }
  return 0;
}
