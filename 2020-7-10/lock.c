#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#define THREADCOUNT 2
int g_noodle = 0;
pthread_mutex_t lock;
pthread_cond_t product_cond;
pthread_cond_t consume_cond;
void* ComsumeStart(void* arg)
{
  (void)arg;
  while(1)
  {
    pthread_mutex_lock(&lock);
    while(g_noodle == 0)
    {
      pthread_cond_wait(&consume_cond,&lock);
    }
    g_noodle--;
    printf("i having noodles:%d\n",g_noodle);
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&product_cond);
  }
  return NULL;
}
void* ProductStart(void* arg)
{
  (void)arg;
  while(1)
  {
    pthread_mutex_lock(&lock);
    while(g_noodle == 1)
    {
      pthread_cond_wait(&product_cond,&lock);
    }
    g_noodle++;
    printf("i making noodles:%d\n",g_noodle);
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&consume_cond);
  }
  return NULL;
}
int main()
{
  pthread_mutex_init(&lock,NULL);
  pthread_cond_init(&consume_cond,NULL);
  pthread_cond_init(&product_cond,NULL);
  pthread_t Consume_tid[2];
  pthread_t Product_tid[2];
  int i = 0;
  int ret = -1;
  for(;i < THREADCOUNT;i++)
  {
     ret = pthread_create(&Consume_tid[0],NULL,ComsumeStart,NULL);
    if(ret < 0)
    {
      perror("pthread_create");
      return 0;
    }
  }
 
  for(i = 0;i < 2;i++)
  {
     ret = pthread_create(&Product_tid[0],NULL,ProductStart,NULL);
    if(ret < 0)
    {
      perror("pthread_create");
      return 0;
    }
  }
  for(i =0;i < 2;i++)
  {
    pthread_join(Consume_tid[i],NULL);
    pthread_join(Product_tid[i],NULL);
  }
  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&consume_cond);
  pthread_cond_destroy(&product_cond);
  return 0;
}
