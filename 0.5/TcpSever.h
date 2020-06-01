 //作者：shao chuang
 //QQ群：1020789851
 //时间：2020.03.19

#pragma once

#include "EventLoop.h"
#include "Channel.h"

#define BUF_SIZE 1024
 //namespace lidan{

class TcpSever{
public:
	TcpSever(EventLoop * loop, int port);
	~TcpSever();
	void Start();
	void NewConnect();
	void ReadClient(SP_Channel newchannel);

private:
	EventLoop* loop_; //这里的变量的顺序不能变，因为他们的初始化有先后顺序
	int port_;//tcpsever监听的端口号
	int listenfd_;//tcpsever的监听描述符
	SP_Channel listenchannel_;//TCPsever用来监听的channel
	std::map<int, SP_Channel> ChannelMap;//tcpsever总维护的所有连接








};




 //}