//作者：shao chuang
//QQ群：1020789851
//时间：2020.03.19

#include "EventLoop.h"
#include "Epoll.h"
#include "Socket.h"
#include <sys/eventfd.h>


//namespace lidan{


int createEventfd() {
  int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);//生成一个非阻塞的可读事件，这个事件是一个计数器
  if (evtfd < 0) {
    //LOG << "Failed in eventfd";
    abort();
  }
  return evtfd;
}




EventLoop::EventLoop()
	:epoll_(new Epoll()), 
	threadId_(CurrentThread::tid()),
	wakeupFd_(createEventfd()),
	quit_(0),
	eventHandling_(0),
	mutex_(),
	looping_(0),
	pwakeupChannel_(new Channel(this, wakeupFd_, EPOLLIN | EPOLLET)){

		pwakeupChannel_->SetReadHandl(std::bind(&EventLoop::handleRead, this));
	}
	
EventLoop::~EventLoop(){}

void EventLoop::updatachannel(Channel * newchannel){
	epoll_->epoll_updata(newchannel);
}

void EventLoop::removechannel(Channel* thechannel){
	epoll_->epoll_del(thechannel);
	return;
}


int EventLoop::GetEpoolFd(){
	return epoll_->GetFd();

}


void EventLoop::loop(){

	assert(!looping_);
	assert(isInLoopThread());//loop函数是不可跨线程调用的
	
	looping_ = true;
	quit_ = false;
	while(!quit_){
		ChannelList_.clear();
		epoll_->poll(ChannelList_);
		eventHandling_ = true;
		for(int i = 0; i < ChannelList_.size(); i++){//处理I/O事件
			ChannelList_[i]->EventHandl();
			printf("in loop\n");

		}
		eventHandling_ = false;
		doPendingFunctors();//处理计算任务
		//printf("IM OK");
	}
	looping_ = false;
}

void EventLoop::doPendingFunctors() {
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;

  {
    MutexLockGuard lock(mutex_);
    functors.swap(pendingFunctors_);//这个地方处理的比较细。pendingFunctors_中的内容被复制到functors中，并将pendingFunctors_中的内容清空
    								//这样避免了锁的频繁使用，也防止程序在doPendingFunctors中无法退出。
  }

  for (size_t i = 0; i < functors.size(); ++i) functors[i]();
  callingPendingFunctors_ = false;
}


void EventLoop::wakeup()
{
  uint64_t one = 1;
  ssize_t n = sockets::write(wakeupFd_, &one, sizeof one);
  if (n != sizeof one)
  {
    //LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
  }
}


void EventLoop::handleRead()
{
  uint64_t one = 1;
  ssize_t n = sockets::read(wakeupFd_, &one, sizeof one);
  if (n != sizeof one)
  {
    //LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
  }
}


void EventLoop::quit() {//这是一个可跨线程执行的函数。
  quit_ = true;
  if (!isInLoopThread()) {
    wakeup(); //如果跨线程操作，需要唤醒epoll。这样可以使得loop更加快速的退出
  }
}


void EventLoop::runInLoop(Functor&& cb) {//这是一个可跨线程执行的函数。
  if (isInLoopThread())
    cb();
  else{
		printf("IM OK");
		queueInLoop(std::move(cb));
  }
    
}

void EventLoop::queueInLoop(Functor&& cb) {//这是一个可跨线程执行的函数。
  {
    MutexLockGuard lock(mutex_);
    pendingFunctors_.emplace_back(std::move(cb));
  }
  //如果跨线程操作，或者正在处理计算任务，需要唤醒epoll。这样可以使得Functor&& cb更加快速的被执行到
  if (!isInLoopThread() || callingPendingFunctors_) wakeup();
}



//}
