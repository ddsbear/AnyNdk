## 【NDK系列】c++线程

线程，有时被称为轻量进程，是程序执行的最小单元。 

## 1. C++11线程

```c
#include <thread>

void task(int i) {
	cout << "task:" << i << endl;
}

thread t1(task,100);
//等待线程结束再继续执行
t1.join();
```

## 2. POSIX线程

POSIX 可移植操作系统接口，标准定义了操作系统应该为应用程序提供的接口标准 

```c
#include <pthread.h>
void *pthreadTask(void* args) {
	int* i = static_cast<int*>(args);
	cout << "posix线程:" << *i << endl;
	return 0;
}
pthread_t pid;
int pi = 100;
pthread_create(&pid, 0, pthreadTask, &pi);
//等待线程的结束
pthread_join(pid,0);
```

### 线程属性

线程具有属性，用 `pthread_attr_t` 表示 

```c
pthread_attr_t attr;
//初始化 attr中为操作系统实现支持的线程所有属性的默认值
pthread_attr_init(&attr);
pthread_attr_destroy(&attr);
```

#### 分离线程

线程创建默认是非分离的，当`pthread_join()`函数返回时，创建的线程终止，释放自己占用的系统资源

分离线程不能被其他线程等待,`pthread_join()`无效，线程自己玩自己的。

```c
//设置是否为分离线程
//PTHREAD_CREATE_DETACHED 分离
//PTHREAD_CREATE_JOINABLE 非分离
pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
```

#### 调度策略与优先级

```c
//设置调度策略 
//返回0 设置成功
pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
// SCHED_FIFO 
//	实时调度策略，先到先服务 一旦占用cpu则一直运行。一直运行直到有更高优先级任务到达或自己放弃。
// SCHED_RR
//	实时调度策略，时间轮转 系统分配一个时间段，在时间段内执行本线程


//设置优先级
//获得对应策略的最小、最大优先级
int max = sched_get_priority_max(SCHED_FIFO);
int min = sched_get_priority_min(SCHED_FIFO);
sched_param param;
param.sched_priority = max;
pthread_attr_setschedparam(&attr, &param);
```

### 线程同步

多线程同时读写同一份共享资源的时候，可能会引起冲突。需要引入线程“同步”机制，即各位线程之间有序地对共享资源进行操作。

以下是会出现问题的写法

```c
#include <pthread.h>
using namespace std;

queue<int> q;
void *pop(void* args) {
    // 线程未同步导致的多线程安全问题
    // 会有重复的数据取出并出现异常
	if (!q.empty())
	{
		printf("取出数据:%d\n", q.front());
		q.pop();
	}
	else {
		printf("无数据\n");
	}
	return 0;
}

int main()
{
	for (size_t i = 0; i < 5; i++)
	{
		q.push(i);
	}
	pthread_t pid[10];
	for (size_t i = 0; i < 10; i++)
	{
		pthread_create(&pid[i], 0, pop, &q);
	}
	system("pause");
	return 0;
}
```

加入互斥锁

```c
queue<int> q;
pthread_mutex_t mutex;
void *pop(void* args) {
	// 锁
	pthread_mutex_lock(&mutex);
	if (!q.empty())
	{
		printf("取出数据:%d\n", q.front());
		q.pop();
	}
	else {
		printf("无数据\n");
	}
    // 放
	pthread_mutex_unlock(&mutex);
	return 0;
}

int main()
{
    //初始化互斥锁
	pthread_mutex_init(&mutex, 0);
	for (size_t i = 0; i < 5; i++)
	{
		q.push(i);
	}
	pthread_t pid[10];
	for (size_t i = 0; i < 10; i++)
	{
		pthread_create(&pid[i], 0, pop, &q);
	}
    //需要释放
 	for (size_t i = 0; i < 10; i++)
	{
		pthread_join(pid[i], 0);
	}
	pthread_mutex_destroy(&mutex);
	system("pause");
	return 0;
}
```

### 条件变量

条件变量是线程间进行同步的一种机制，主要包括两个动作：一个线程等待"条件变量的条件成立"而挂起；另一个线程使"条件成立",从而唤醒挂起线程



```c
template <class T>
class SafeQueue {
public:
	SafeQueue() {
		pthread_mutex_init(&mutex,0);
	}
	~SafeQueue() {
		pthread_mutex_destory(&mutex);
	}
	void enqueue(T t) {
		pthread_mutex_lock(&mutex);
		q.push(t);
		pthread_mutex_unlock(&mutex);
	}
	int dequeue(T& t) {
		pthread_mutex_lock(&mutex);
		if (!q.empty())
		{
			t = q.front();
			q.pop();
			pthread_mutex_unlock(&mutex);
			return 1;
		}
		pthread_mutex_unlock(&mutex);
		return 0;
	}

private:
	queue<T> q;
	pthread_mutex_t mutex;
};
```

上面的模板类存放数据T，并使用互斥锁保证对queue的操作是线程安全的。这就是一个生产/消费模式。

如果在取出数据的时候，queue为空，则一直等待，直到下一次enqueue加入数据。

这就是一个典型的生产/消费模式, 加入条件变量使 “dequeue”  挂起,直到由其他地方唤醒

**下面是使用条件变量实现**

通过使用cond条件来 唤醒等待中的线程

```c
#pragma once
#include <queue>
using namespace std;

template <class T>
class SafeQueue {
public:
	SafeQueue() {
		pthread_mutex_init(&mutex,0);
		pthread_cond_init(&cond, 0);
	}
	~SafeQueue() {
		pthread_mutex_destory(&mutex);
		pthread_cond_destory(&cond);
	}
	void enqueue(T t) {
		pthread_mutex_lock(&mutex);
		q.push(t);
		//发出信号 通知挂起线程
		//由系统唤醒一个线程
		//pthread_cond_signal(&cond);
		// 广播 对应多个消费者的时候 多个线程等待唤醒所有
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex);
	}
	int dequeue(T& t) {
		pthread_mutex_lock(&mutex);
		//可能被意外唤醒 所以while循环
		while (q.empty())
		{
			pthread_cond_wait(&cond, &mutex);
		}
		t = q.front();
		q.pop();
		pthread_mutex_unlock(&mutex);
		return 1;
	}

private:
	queue<T> q;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
};
```







