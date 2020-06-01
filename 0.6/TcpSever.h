 //作者：shao chuang
 //QQ群：1020789851
 //时间：2020.03.19

#pragma once

#include "EventLoop.h"
#include "Channel.h"
#include "Acceptor.h"

#define BUF_SIZE 1024
 //namespace lidan{

class TcpSever{
public:
	TcpSever(EventLoop * loop, int port);
	~TcpSever();
	void Start();
	void NewConnect(int connfd);
	void ReadClient(SP_Channel newchannel);

private:
	EventLoop* loop_; 
	std::unique_ptr<Acceptor> acceptor_;//用于监听的acceptor
	std::map<int, SP_Channel> ChannelMap;//tcpsever总维护的所有连接








};




 //}