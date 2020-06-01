//作者：shao chuang
//QQ群：1020789851
//时间：2020.05.05

#pragma once
#include "Channel.h"
#include "Socket.h"

typedef std::shared_ptr<Channel> SP_Channel;
//用于监听的channel
class Acceptor{
	public:
		typedef std::function<void (int sockfd)> NewConnectionCallback;
		Acceptor(EventLoop* loop, int port);
		~Acceptor();
		int getport() const {return port_;}
		int getlistenfd() const {return listenfd_;}
		SP_Channel getlistenchannel() const {return listenchannel_;}
		void setNewConnectionCallback(const NewConnectionCallback& cb)  { newConnectionCallback_ = cb; }

	private:
		void handleRead();//listenchannel_的可读函数句柄
		EventLoop* loop_; //这里的变量的顺序不能变，因为他们的初始化有先后顺序
		int port_;//tcpsever监听的端口号
		int listenfd_;//tcpsever的监听描述符
		SP_Channel listenchannel_;//TCPsever用来监听的channel
		NewConnectionCallback newConnectionCallback_;//有新连接到来时的回调函数，此函数由用户指定



};