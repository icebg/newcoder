#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include "locker.h"

// 线程池类，将它定义为模板类是为了代码复用，模板参数T是任务类
template<typename T>
class threadpool {
public:
    /*thread_number是线程池中线程的数量，max_requests_num是请求队列中最多允许的、等待处理的请求的数量*/
    threadpool(int thread_number = 8, int max_requests_num = 10000);
    ~threadpool();
    //request 指向 型参T类（任务类）。
    bool append(T* request);

private:
    /*工作线程运行的函数，它不断从工作队列中取出任务并执行之*/
    static void* worker(void* arg);
    void run();

private:
    // 线程的数量
    int m_thread_number;  
    
    // 描述线程池的数组，大小为m_thread_number    
    pthread_t * m_threads;

    // 请求队列中最多允许的、等待处理的请求的数量  
    int m_max_requests_num; 
    
    // 请求队列 (装的是 任务)
    std::list< T* > m_workqueue;  

    // 保护请求队列的互斥锁
    locker m_queuelocker;   

    // 是否有任务需要处理(任务队列的信号量)
    sem m_queuestat;

    // 是否结束线程          
    bool m_stop;                    
};

template< typename T >
threadpool< T >::threadpool(int thread_number, int max_requests_num) : 
        m_thread_number(thread_number), m_max_requests_num(max_requests_num), 
        m_stop(false), m_threads(NULL) {

    if((thread_number <= 0) || (max_requests_num <= 0) ) {
        throw std::exception();
    }

    m_threads = new pthread_t[m_thread_number];
    if(!m_threads) {
        throw std::exception();
    }

    // 创建thread_number 个线程，并将他们设置为脱离线程。
    for ( int i = 0; i < thread_number; ++i ) {
        printf( "create the %dth thread\n", i);
        //创建出来以后，各个线程的线程号们存入了堆区的数组。
        if(pthread_create(m_threads + i, NULL, worker, this ) != 0) {
            delete [] m_threads;
            throw std::exception();
        }
        
        if( pthread_detach( m_threads[i] ) ) {
            delete [] m_threads;
            throw std::exception();
        }
    }
}

template< typename T >
threadpool< T >::~threadpool() {
    delete [] m_threads;
    m_stop = true;
}

template< typename T >
bool threadpool< T >::append( T* request )
{
    // 操作工作队列时一定要加锁，因为它被所有线程共享。
    m_queuelocker.lock();
    if ( m_workqueue.size() > m_max_requests_num ) {
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post(); //任务队列的信号量,发布操作(意味着生产一个产品)
    return true;
}

template< typename T >
void* threadpool< T >::worker( void* arg ) //因为此例程需要修改对象的非静态成员，所以传入this指针，
{
    threadpool* pool = ( threadpool* )arg;
    pool->run();//这样的话，在run里可以直接访问成员，而不是在当前函数里写很多pool->成员。
    return pool;
}

template< typename T >
void threadpool< T >::run() {

    while (!m_stop) { //只要没喊停,喊停了的话，run函数就停下来啥也不干
        m_queuestat.wait(); //信号量 wait一个,取走一个产品(处理一个任务)。
        m_queuelocker.lock(); //锁 获得一个
        if ( m_workqueue.empty() ) {
            m_queuelocker.unlock();
            continue;
        }
        T* request = m_workqueue.front();
        m_workqueue.pop_front(); //从队列取出最前头的任务
        m_queuelocker.unlock(); //释放锁
        if ( !request ) {
            continue;
        }
        request->process(); //这个process应该是处理业务,process还未实现。(因为它是 型参T类的public成员)
    }

}

#endif
