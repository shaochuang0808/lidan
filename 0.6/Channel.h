//作者：shao chuang
//QQ群：1020789851
//时间：2020.03.19


#pragma once
#include "EventLoop.h"

#include <stdio.h>
#include <memory>
#include <functional>


class EventLoop;


//namespace lidan{


class Channel{
public:
	typedef std::function<void()> CallBack;

public:
	Channel(EventLoop* loop, int fd, int event);
	~Channel();
	int getfd();
	int getEvents();
	void SetReadHandl(CallBack &&      ReadHandler){ReadHandler_ = ReadHandler;};
	void SetWriteHandl(CallBack && WriteHandler){WriteHandler_ = WriteHandler;};
	void SetCloseHandl(CallBack && CloseHandler){CloseHandler_ = CloseHandler;};
	void SetErrorHandl(CallBack && ErroeHandler){ErroeHandler_ = ErroeHandler;};
	void SetMessageCallback(CallBack && MessageCallback){MessageCallback_ = MessageCallback;};

	void ReadHandl();
	void WriteHandl();
	void CloseHandl();
	void ErrorHandl();
	void MessageCallback();

	void EventHandl(std::shared_ptr<Channel> thischannel);//处理channel回调函数的接口函数

	void SetEvent(int event){event_ = event;};
	void SetRevent(int revent){revent_ = revent;};
	

	


private:
	int fd_;
	int event_;//channel关注的事件
	int revent_;//epoll返回的事件类型
	EventLoop* loop_; //此指针指向该channel所属的eventloop
	CallBack ReadHandler_;//回调函数句柄//特别注释，epoll监听的可读事件就是连接事件
	CallBack WriteHandler_;
	CallBack CloseHandler_;
	CallBack ErroeHandler_;
	CallBack MessageCallback_;//TCP连接的处理函数







};


typedef std::shared_ptr<Channel> SP_Channel;
//}