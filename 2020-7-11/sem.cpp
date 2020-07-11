#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<vector>
#include <semaphore.h>

#define SIZE 1
#define THREADCOUNT 1
class RingQueue
{
  public:
    RingQueue()
      :vec(SIZE)
       ,Capacity(SIZE)
       ,PosR(0)
       ,PosW(0)
    {
      sem_init(&ProSem,0,SIZE);
      sem_init(&ConSem,0,0);
      sem_init(&Lock,0,1);
    }
    ~RingQueue()
    {
      sem_destroy(&ProSem);
      sem_destroy(&ConSem);
      sem_destroy(&Lock);
    }
    void push(int &data)
    {
      sem_wait(&ProSem);
      sem_wait(&Lock);
      vec[PosW] = data;
      PosW = (PosW+1)%Capacity;
      sem_post(&Lock);
      sem_post(&ConSem);
    }
    void pop(int *data)
    {
      sem_wait(&ConSem);
      sem_wait(&Lock);
      *data = vec[PosR];
      PosR = (PosR+1)%Capacity;
      sem_post(&Lock);
      sem_post(&ProSem);
    }
  private:
    std::vector<int> vec;
    size_t Capacity;
    int PosW;
    int PosR;
    sem_t ProSem;
    sem_t ConSem;
    sem_t Lock;
};
void *ConsumeStart(void* arg)
{
  RingQueue* rq = (RingQueue*)arg;
  int data;
  while(1)
  {
    rq->pop(&data);
    printf("ConsumeStart[%p][%d]\n",pthread_self(),data);
  }
  return NULL;
}
void *ProductStart(void* arg)
{
  RingQueue* rq = (RingQueue*)arg;
  int i = 0;
  while(1)
  {
    rq->push(i);
    printf("ProductStart[%p][%d]\n",pthread_self(),i);
    i++;
  }
  return NULL;
}
int main()
{
  RingQueue* rq = new RingQueue();

  pthread_t com_tid[THREADCOUNT],pro_tid[THREADCOUNT];
  int i = 0;
  for(;i < THREADCOUNT;i++)
  {

    int ret =pthread_create(&com_tid[i],NULL,ConsumeStart,(void*)rq);
    if(ret < 0)
    {
      perror("pthread_create");
      return 0;
    }
    ret =pthread_create(&pro_tid[i],NULL,ProductStart,(void*)rq);
    if(ret < 0)
    {
      perror("pthread_create");
      return 0;
    }
  }
  for(i = 0;i < THREADCOUNT;i++)
  {
    pthread_join(pro_tid[i],NULL);
    pthread_join(com_tid[i],NULL);
  }
  delete rq;
  rq = NULL;
  return 0;
}
