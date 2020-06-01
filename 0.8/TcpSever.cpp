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
 
 }

TcpSever::~TcpSever(){}


void TcpSever::Start(){
	
	acceptor_->start();
	return;
}

void TcpSever::ReadClient(TcpConnectionPtr newtcpconnection, Buffer* thebuffer ){
	
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
			//newtcpconnection->setMessageCallback(std::bind(&TcpSever::ReadClient, this, newtcpconnection, newtcpconnection->getintputbuffer()));
			//newtcpconnection->setConnectionCallback(std::bind(&TcpSever::NewConnectCallback, this, newtcpconnection));
			newtcpconnection->setMessageCallback(messageCallback_);
			newtcpconnection->connectEstablished(connectionCallback_);
			TcpConnectionMap[nameArg] = newtcpconnection;
			newtcpconnection->startRead();
			
		//}



}





 //}
