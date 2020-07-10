#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

int g_noodle = 0;
pthread_mutex_t lock;
pthread_cond_t cond;
void* ComsumeStart(void* arg)
{
  (void)arg;
  while(1)
  {
    pthread_mutex_lock(&lock);
    if(!g_noodle)
    {
      pthread_cond_wait(&cond,&lock);
    }
    printf("i having noodles\n");
    sleep(1);
    g_noodle--;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);
  }
  return NULL;
}
void* ProductStart(void* arg)
{
  (void)arg;
  while(1)
  {
    pthread_mutex_lock(&lock);
    if(g_noodle)
    {
      pthread_cond_wait(&cond,&lock);
    }
    printf("i making noodles\n");
    sleep(1);
    g_noodle++;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);
  }
  return NULL;
}
int main()
{
  pthread_cond_init(&cond,NULL);
  pthread_mutex_init(&lock,NULL);
  pthread_t tid[2];
  int i = 0;
  int ret = pthread_create(&tid[0],NULL,ComsumeStart,NULL);
  if(ret < 0)
  {
    perror("pthread_create");
    return 0;
  }
 
  ret = pthread_create(&tid[0],NULL,ProductStart,NULL);
  if(ret < 0)
  {
    perror("pthread_create");
    return 0;
  }
  for(i =0;i < 2;i++)
  {
    pthread_join(tid[i],NULL);
  }
  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&lock);
  return 0;
}
