#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <queue>
#include<stdlib.h>
//线程池 = 线程安全队列 + 一大堆的线程
//线程安全队列
//    元素 = 数据 + 处理数据的函数地址

#define THREADCOUNT 4

typedef void* (*Handler_t)(int);
class ThreadTask
{
    public:
        ThreadTask(int data, Handler_t handler)
        {
            data_ = data;
            handler_ = handler;
        }

        ~ThreadTask()
        {
            //
        }

        //使用函数，处理数据
        void Run()
        {
            handler_(data_);
        }
    private:
        int data_;
        Handler_t handler_; //返回值为void* 参数为int




};

class ThreadPool
{
    public:
        ThreadPool()
        {
            capacity_ = 10;

            thread_capacity_ = THREADCOUNT;

            pthread_mutex_init(&lock_, NULL);
            pthread_cond_init(&cond_, NULL);

            //创建线程
            bool is_create = ThreadCreate();
            if(!is_create)
            {
                printf("Threadpool Create thread failed\n");
                exit(1);
            }

            IsExit = false;

            cur_thread_count_ = THREADCOUNT;
        }

        ~ThreadPool()
        {

        }

        bool Push(ThreadTask* tt)
        {
            pthread_mutex_lock(&lock_);
            if(IsExit)
            {
                pthread_mutex_unlock(&lock_);
                return false;
            }
            que_.push(tt);
            pthread_mutex_unlock(&lock_);

            //当插入数据之后 通知线程池当中的线程（消费者）
            pthread_cond_signal(&cond_);
            return true;
        }

        bool Pop(ThreadTask** tt)
        {
            *tt = que_.front();
            que_.pop();
            return true;
        }

        void ThreadJoin()
        {
            for(int i = 0; i < THREADCOUNT; i++)
            {
                pthread_join(tid_[i], NULL);
            }
        }

        // 如果直接退出线程，则有可能队列当中当中还有数据没有处理完毕；
        // 我们不应该去调用这样的接口来结束我们的线程
        void ThreadExit()
        {
            for(int i = 0; i < THREADCOUNT; i++)
            {
                pthread_cancel(tid_[i]);
            }
        }

        void ThreadPoolClear()
        {
            //标志位
            pthread_mutex_lock(&lock_);
            IsExit = true;
            pthread_mutex_unlock(&lock_);

            if(cur_thread_count_ > 0)
            {
                pthread_cond_broadcast(&cond_);
            }
        }

    private:
        static void* ThreadStart(void* arg)
        {
            ThreadPool* tp = (ThreadPool*)arg;
            while(1)
            {
                //从队列当中获取数据，进行消费 对于不同的线程而言，在获取数据的时候，是互斥的
                pthread_mutex_lock(&tp->lock_);
                ThreadTask* tt;
                while(tp->que_.empty())
                {
                    //可以进行结束
                    if(tp->IsExit)
                    {
                        //退出
                        tp->cur_thread_count_--;
                        pthread_mutex_unlock(&tp->lock_);
                        pthread_exit(NULL);
                    }
                    //调用条件变量 等待接口
                    pthread_cond_wait(&tp->cond_, &tp->lock_);
                }
                tp->Pop(&tt);
                pthread_mutex_unlock(&tp->lock_);

                //调用队列当中元素提供的函数，去处理数据 对于线程走到该位置的时候，就可以并行的处理了
                tt->Run();
                //防止内存泄漏
                delete tt;
            }
        }

        bool ThreadCreate()
        {
            for(int i = 0; i < THREADCOUNT; i++)
            {
                int ret = pthread_create(&tid_[i], NULL, ThreadStart, (void*)this);
                if(ret != 0)
                {
                    perror("pthread_create");
                    return false;
                }
            }
            return true;
        }
    private:
        std::queue<ThreadTask*> que_;
        size_t capacity_;

        //互斥
        pthread_mutex_t lock_;
        //同步 消费线程的条件变量，但是并没有生产线程的条件变量
        //由于我们所说的，客户端的请求行为我们是无法控制的。所以就不需要通知生产者来进行生产，当生产线程有了数据，就直接往线程池当中抛入就可以了，在通知消费线程来进行消费
        pthread_cond_t cond_;

        //线程池当中的初始化的时候线程数量
        size_t thread_capacity_;

        //标识具体还有多少线程数量
        size_t cur_thread_count_;

        //保存线程池当中的线程的线程标识符
        pthread_t tid_[THREADCOUNT];

        //标志是否可以退出
        bool IsExit;
};

void* DealData(int data)
{
    printf("consume data is %d\n", data);
    return NULL;
}

int main()
{
    ThreadPool* tp = new ThreadPool();
    //在这个代码当中main函数的线程，就充当生产线程，往线程池的线程安全队列当中push数据
    for(int i = 1; i <= 50; i++)
    {
        ThreadTask* tt = new ThreadTask(i, DealData);
        tp->Push(tt);
    }

    //等待线程池当中线程退出
    sleep(15);
    tp->ThreadPoolClear();
    tp->ThreadJoin();
    delete tp;
    return 0;
}
