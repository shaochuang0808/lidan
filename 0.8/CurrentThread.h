

//作者：shao chuang
//QQ群：1020789851
//时间：2020.04.30

#pragma once
#include <functional>
#include <pthread.h>
#include <sys/prctl.h>
#include <assert.h>
#include <errno.h>
#include <linux/unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory>
#include <string>

#include "CountDownLatch.h"
#include "ThreadData.h"

//此头文件中的内容，每一个线程中都将会有独立的一份

class ThreadData;

namespace CurrentThread{
	extern __thread int t_cachedTid ; //线程标示符号
	extern __thread char t_tidString[32];//线程标识符的字符串表示方法
	extern __thread int t_tidStringLength ;//线程标识符的长度
	extern __thread const char* t_threadName ;

	pid_t gettid();

	void cacheTid();

	
	int tid() ;

	void* startThread(void* obj) ;
}


