 #include "Thread.h"

Thread::Thread(const ThreadFunc& func, const string& n)
    : started_(false),
      joined_(false),
      pthreadId_(0),
      tid_(0),
      func_(func),
      name_(n),
      latch_(1) {
  setDefaultName();
}


Thread::~Thread() {
	//如果thread类在被销毁的时候，thread类维护的子线程还没有归并
	//那么就将子线程设置为detach状态，在此状态下，如果子线程运行结束将会自行清理内存
	if (started_ && !joined_) pthread_detach(pthreadId_);
}

void Thread::setDefaultName() {
  if (name_.empty()) {
    char buf[32];
    snprintf(buf, sizeof buf, "Thread");
    name_ = buf;
  }
}

void Thread::start() {
  assert(!started_);
  started_ = true;
  ThreadData* data = new ThreadData(func_, name_, &tid_, &latch_);
  if (pthread_create(&pthreadId_, NULL, &CurrentThread::startThread, data)) {//创建子线程
    started_ = false;
    delete data;
  } else {
    latch_.wait();//父线程会在此处阻塞，直到子线程开始运行
    assert(tid_ > 0);
  }
}

int Thread::join() {
  assert(started_);
  assert(!joined_);
  joined_ = true;
  return pthread_join(pthreadId_, NULL);//此处会阻塞，直到子线程运行结束
}





