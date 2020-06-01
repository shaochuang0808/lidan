 //作者：shao chuang
 //QQ群：1020789851
 //时间：2020.05.10

#pragma once
#include "Channel.h"
#include "EventLoop.h"
#include "CallBack.h"
#include <string>
#include "Buffer.h"

class EventLoop;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>{
public:
	TcpConnection(EventLoop* loop,      const std::string& nameArg, int sockfd );
	~TcpConnection();

	enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };//TCPconnection所处的四种状态

	void connectEstablished();//建立连接
	void connectDestroyed();//断开连接
	//void startRead();//开始读
	//void startReadInLoop();
	//void stopRead();//停止读
	//void stopReadInLoop();
	
	void setState(StateE s) { state_ = s; }//设置TcpConnection的状态
	void setConnectionCallback(const ConnectionCallback& cb){ connectionCallback_ = cb; }
    void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }
 
	void send(const void* data, int len);
	void send(const std::string& message);
	void send(Buffer* buf);
	void sendInLoop(const std::string& message);
	void sendInLoop(const void* data, size_t len);
	//这几个函数是最终channel中的函数
	void handleRead();
	void handleWrite();
	void handleClose();
	void handleError();

	Buffer* getintputbuffer(){return &inputBuffer_;};
	Buffer* getoutputbuffer(){return &outputBuffer_;};
	std::string getname(){return name_;};

private:
	
	EventLoop* loop_;//
	const std::string name_;//TcpConnection的名字
	
	std::unique_ptr<Channel> channel_;
	Buffer inputBuffer_; //输入缓冲区
	Buffer outputBuffer_;//输出缓冲区

	StateE state_;//用来表示 TCPconnection的连接状态
	//bool reading_;//表示TCPconnection是否可以读数据，当reading_为真是，此时TCPconnection连接建立成功，且可以被读
	
	ConnectionCallback connectionCallback_;//建立和断开连接时的回调函数
	MessageCallback messageCallback_; //消息到来时的回调函数


};