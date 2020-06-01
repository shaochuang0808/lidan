 //作者：shao chuang
 //QQ群：1020789851
 //时间：2020.03.19


#include "TcpSever.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Acceptor.h"
#include <stdio.h>
#include <functional>
#include <memory>
#include <vector>
#include "TcpConnection.h"
#include <string>

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;


// namespace lidan{


 TcpSever::TcpSever(EventLoop * loop, int port)
 	:loop_(loop),
 	acceptor_(new Acceptor(loop, port))
 	
 {
	acceptor_->setNewConnectionCallback(std::bind(&TcpSever::NewConnect, this, std::placeholders::_1));
 //printf("\nlistenfd_:%d\n", listenfd_);
	 //printf("\nlistenchannel_->getfd():%d\n", listenchannel_->getfd());

 }

TcpSever::~TcpSever(){}


void TcpSever::Start(){
	
	acceptor_->start();
	return;
}

void TcpSever::ReadClient(TcpConnectionPtr newtcpconnection, Buffer* thebuffer ){
	//std::shared_ptr<std::vector<char>> buffer_recv(new std::vector<char>(1024, 0));
	
	//int ret = recv(newchannel->getfd(), &*(buffer_recv->begin()), BUF_SIZE-1, 0);
	//assert(ret >= 0);
	//if(ret == 0){//此时客户端连接断开
	//	loop_->removechannel(newchannel);
	//	ChannelMap.erase(newchannel->getfd());
	//	return;
	//}
	//printf("%s\n", &*(buffer_recv->begin())); //
	//for(auto it = ChannelMap.begin(); it != ChannelMap.end(); it++){
	//	if(it->first != newchannel->getfd())
	//	send(it->first, &*(buffer_recv->begin()), strlen(&*(buffer_recv->begin())), 0);
	//}
	std::string data = thebuffer->retrieveAllAsString();
	for(auto it = TcpConnectionMap.begin(); it != TcpConnectionMap.end(); it++){
		it->second->send(data);
	}
	printf("ReadClient\n");

}

void TcpSever::NewConnectCallback(TcpConnectionPtr newtcpconnection){
	printf("\n%s\n", newtcpconnection->getname().c_str());
}

 
void TcpSever::NewConnect(int connfd){//监听事件被触发时，将会执行此函数
		//struct sockaddr_in client;
		//socklen_t client_addrlength = sizeof(client);
		//int connfd = accept(listenfd_, (struct sockaddr*)&client, &client_addrlength);
		//if(connfd > 0){
			printf("connfd:%d\n", connfd);
			
			//SP_Channel newchannel(new Channel(loop_, connfd, EPOLLIN | EPOLLET));
			//newchannel->SetReadHandl(std::bind(&TcpSever::ReadClient, this, newchannel.get()));
			//loop_->updatachannel(newchannel.get());
			//ChannelMap[connfd] = newchannel;
			std::string nameArg = "tcpconnection" + std::to_string(connfd);
			TcpConnectionPtr newtcpconnection(new TcpConnection(loop_, nameArg, connfd));
			newtcpconnection->setMessageCallback(std::bind(&TcpSever::ReadClient, this, newtcpconnection, newtcpconnection->getintputbuffer()));
			newtcpconnection->setConnectionCallback(std::bind(&TcpSever::NewConnectCallback, this, newtcpconnection));
			newtcpconnection->connectEstablished();
			TcpConnectionMap[nameArg] = newtcpconnection;
			newtcpconnection->startRead();
			
		//}



}





 //}
