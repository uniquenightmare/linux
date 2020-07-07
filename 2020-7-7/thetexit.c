#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
void* thread_start(void* arg)
{
  (void)arg;
  printf("i am new thread\n");
  sleep(20);
  printf("pthread_exit\n");
  pthread_exit(NULL);
  while(1)
  {
    printf("11111\n");
  }
   return NULL;
}
int main()
{
  pthread_t tid;
  pthread_create(&tid,NULL,thread_start,NULL);
  printf("%p\n",pthread_self());
  pthread_cancel(pthread_self());
  while(1)
  {
    printf("i am main thread\n");
    sleep(1);

  }
  return 0;
}
