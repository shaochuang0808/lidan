
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
#include "CurrentThread.h"

//此头文件中的内容，每一个线程中都将会有独立的一份

class ThreadData;



namespace CurrentThread{
	__thread int t_cachedTid = 0; //线程标示符号
	__thread char t_tidString[32];//线程标识符的字符串表示方法
	__thread int t_tidStringLength = 6;//线程标识符的长度
	__thread const char* t_threadName = "default";

}



pid_t CurrentThread::gettid() { return static_cast<pid_t>(::syscall(SYS_gettid)); }

void CurrentThread::cacheTid(){
	if (t_cachedTid == 0) {
		t_cachedTid = gettid();
		t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
	}
}


int CurrentThread::tid() {
  if (__builtin_expect(t_cachedTid == 0, 0)) {
	cacheTid();
  }
  return t_cachedTid;
}

void* CurrentThread::startThread(void* obj) {//子线程的公共入口
	  ThreadData* data = static_cast<ThreadData*>(obj);
	  data->runInThread();
	  delete data;
	  return NULL;
}




