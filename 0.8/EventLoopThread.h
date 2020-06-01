//作者：shao chuang
//QQ群：1020789851
//时间：2020.04.30

#pragma once
#include "EventLoop.h"
#include "Condition.h"
#include "MutexLock.h"
#include "Thread.h"
#include "noncopyable.h"



//EventLoopThread类是一个EventLoop和Thread类结合的一个类
//需要注意的是，EventLoop的实例是在Thread维护的子线程中建立的，父线程中只有EventLoop的指针
class EventLoopThread : noncopyable{
	public:
		EventLoopThread();
		  ~EventLoopThread();
		  EventLoop* startLoop();

	private:
		void threadFunc();//子线程的入口函数
		EventLoop* loop_;//EventLoop的实例是在Thread维护的子线程中建立的，父线程中只有EventLoop的指针
		//bool exiting_;
		Thread thread_;
		MutexLock mutex_;
		Condition cond_;
};


