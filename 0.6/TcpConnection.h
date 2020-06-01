 //作者：shao chuang
 //QQ群：1020789851
 //时间：2020.05.10

#pragma once
#include "Channel.h"
#include "EventLoop.h"
#include "CallBack.h"
#include <string>


class TcpConnection{
public:
	TcpConnection(EventLoop* loop, std::string name, int fd, int event);
	~TcpConnection();

private:
	EventLoop* loop_;//
	const std::string name_;//TcpConnection的名字
	
	std::unique_ptr<Channel> channel_;
	
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;


};