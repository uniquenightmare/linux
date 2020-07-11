#include<iostream>
#include<queue>
#include<unistd.h>
#include<pthread.h>
#include<stdio.h>
#define CAPACITY 10
#define THREADCOUNT 1
class BlockQueue
{
  public:
    BlockQueue(size_t _capacity)
      :Capacity(_capacity)
    {
      pthread_mutex_init(&Mutex_,NULL);
      pthread_cond_init(&consumeCond,NULL);
      pthread_cond_init(&productCond,NULL);
    }
    ~BlockQueue()
    {
      pthread_mutex_destroy(&Mutex_);
      pthread_cond_destroy(&productCond);
      pthread_cond_destroy(&consumeCond);
    }
    void push(int& data)
    {
      pthread_mutex_lock(&Mutex_);
      while(que.size() == Capacity)
      {
        pthread_cond_wait(&productCond,&Mutex_);
      }
      que.push(data);
      pthread_mutex_unlock(&Mutex_);
      pthread_cond_signal(&consumeCond);
    }
    void pop(int* data)
    {
      pthread_mutex_lock(&Mutex_);
      while(que.empty())
      {
        pthread_cond_wait(&consumeCond,&Mutex_);
      }
      *data = que.front();
      que.pop();
      pthread_mutex_unlock(&Mutex_);
      pthread_cond_signal(&productCond);
    }
  private:
    std::queue<int> que;
    pthread_mutex_t Mutex_;
    pthread_cond_t consumeCond;
    pthread_cond_t productCond;
    size_t Capacity;
};

void *ConsumeStart(void* arg)
{ 
  BlockQueue* bq = (BlockQueue*)arg;
  while(1)
  {
    int data;
    bq->pop(&data);

    printf("ConsumeStart[%p][%d]\n",pthread_self(),data);
  }
  return NULL;
}
void *ProductStart(void* arg)
{
  BlockQueue* bq = (BlockQueue*)arg;
  int i = 0;
  while(1)
  {
    bq->push(i);
    printf("ProductStart[%p][%d]\n",pthread_self(),i);
    i++;
  }
  return NULL;
}
int main()
{
  BlockQueue* bq = new BlockQueue(1);
  pthread_t com_tid[THREADCOUNT],pro_tid[THREADCOUNT];
  int i = 0;
  for(;i < THREADCOUNT;i++)
  {

    int ret =pthread_create(&com_tid[i],NULL,ConsumeStart,(void*)bq);
    if(ret < 0)
    {
      perror("pthread_create");
      return 0;
    }
    ret =pthread_create(&pro_tid[i],NULL,ProductStart,(void*)bq);
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
  delete bq;
  bq = NULL;
  return 0;
}
