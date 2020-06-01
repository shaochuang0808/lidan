//作者：shao chuang
//QQ群：1020789851
//时间：2020.05.02

#pragma once
#include <pthread.h>
#include <functional>
#include <string>
#include "CountDownLatch.h"
#include "noncopyable.h"


//Thread类是在父线程中建立的一个类，而此类维护者这一个子线程，
//因此类中的各个属性其实是子线程的属性，比如tid_指的是此类维护的子线程的线程号
class Thread:noncopyable{
	public:
		typedef std::function<void()> ThreadFunc;
		explicit Thread(const ThreadFunc&, const std::string& name = std::string());
		~Thread();
		void start();
		int join();
		bool started() const { return started_; }
		pid_t tid() const { return tid_; }
		const std::string& name() const { return name_; }

	private:
		void setDefaultName();
		bool started_;//子线程是否开始
		bool joined_;//子线程是否归并
		pthread_t pthreadId_;//子线程的线程号，进程内唯一
		pid_t tid_;//子线程的线程号，全局唯一
		ThreadFunc func_;//子线程的线程入口
		std::string name_;//子线程名字
		CountDownLatch latch_;//用来告知父线程子线程已经开始运行了




};




