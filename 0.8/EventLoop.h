 //作者：shao chuang
//QQ群：1020789851
//时间：2020.03.19


#pragma once
#include "Epoll.h"
#include "Channel.h"
#include <stdio.h>
#include <memory>
#include <functional>
#include "MutexLock.h"
#include "CurrentThread.h"
//using namespace lidan;

class Channel;
class Epoll;
typedef std::shared_ptr<Channel> SP_Channel;

//namespace lidan{


class EventLoop{

public:
	typedef std::function<void()> Functor;
	EventLoop();
	~EventLoop();
	bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }//判断当前线程是否为eventloop类的所属线程
	void assertInLoopThread() { assert(isInLoopThread()); }
	void updatachannel(Channel* newchannel);//更新channel在epoll中的状态
	void removechannel(Channel* newchannel);//将channel从epoll中删除
	//void AddtoEpool(Channel* newchannel);//将一个新的channel加入到epoll中
	//void UptoEpool(Channel* oldchannel);//当epool中的事件相应过之后，需要重新将此事件添加到epoll中
	//void RemovetoEpool(Channel* thechannel);//移除epoll中的channel
	void doPendingFunctors();//
	void loop();//eventloop的循环函数
	int GetEpoolFd();//得到epoolfd
	void wakeup();//epoll从epoll_wait中唤醒
	void handleRead();//pwakeupChannel_的可读事件执行函数
	void quit(); //执行此函数后eventloop将会从loop中退出

	//所有的读写任务都需要在eventloop所在的线程中被执行
	void runInLoop(Functor&& cb);//这是一个可跨线程执行的函数。Functor函数可能被直接执行，也可能通过queueInLoop函数被添加进queueInLoop中，在适当时执行
	void queueInLoop(Functor&& cb);//这是一个可跨线程执行的函数。

	friend class Epoll;
 
private:
	
	std::unique_ptr<Epoll> epoll_;//
	int wakeupFd_;//这是一个eventfd，通过eventfd来唤醒epoll_wait。因为epoll_wait的机制时当没有事件来临的话，
	              //将会阻塞一段时间，我们可以使用eventfd来唤醒阻塞的epoll_wait
	              //wakeupFd_的顺序要在，pwakeupChannel_之前 
	
	SP_Channel pwakeupChannel_;//wakeupFd_所属的channel
	std::vector<Functor> pendingFunctors_; //对于I/O线程来说，除了处理I/O事件以外，可能还会处理其他计算任务。其他计算任务就在此处执行。
	mutable MutexLock mutex_;//mutex_是为了保护pendingFunctors_
	const pid_t threadId_;//EventLoop类的所属线程线程号，EventLoop类被构造时初始化
	bool quit_;//为0时，loop将会一直循环。为1时，loop将会退出循环
	bool eventHandling_;//为0时，I/O线程不在执行计算任务，为1时，I/O线程正在执行计算任务
	bool looping_;//为0时，I/O线程不在执行loop，为1时，I/O线程正在执行loop
	std::vector<Channel*> ChannelList_;//epool中返回的需要处理的I/O事件的channel。都存储在这里
	bool callingPendingFunctors_;





};











//}
