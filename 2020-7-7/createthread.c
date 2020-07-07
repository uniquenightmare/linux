#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
void* thread_start(void *arg)
{
  int *i = (int *)arg;
  while(1)
  {
    printf("i am new thread:%d\n",*i);
    sleep(1);

  }
  return NULL;
}
int main()
{
  pthread_t tid;
  int ret;
  {

     int i = 10;
     ret = pthread_create(&tid,NULL,thread_start,(void*)&i);
  }
  if(ret < 0)
  {
    perror("pthread_create");
    return 0;
  }
  while(1)
  {
    printf("i am main thread\n");
    sleep(1);
  }
  return 0;
}
