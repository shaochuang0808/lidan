//作者：shao chuang
//QQ群：1020789851
//时间：2020.03.19


#pragma once
#include "Epoll.h"
#include "Channel.h"
#include <stdio.h>
#include <memory>
#include <functional>
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
	void AddtoEpool(SP_Channel newchannel);
	void UptoEpool(SP_Channel oldchannel);
	void RemovetoEpool(SP_Channel thechannel);
	void doPendingFunctors();
	void loop();
	int GetEpoolFd();

private:
	
	std::unique_ptr<Epoll> epoll_;
	std::vector<Functor> pendingFunctors_; //除了读事件，所有的事件处理函数都在这里





};











//}