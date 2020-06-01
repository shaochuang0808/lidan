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

typedef std::shared_ptr<Channel> SP_Channel;

class Channel;

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
//	listenchannel_->SetReadHandl(std::bind(&TcpSever::NewConnect, this));
	
	loop_->AddtoEpool(acceptor_->getlistenchannel());
	
	return;
}

void TcpSever::ReadClient(SP_Channel newchannel){
	std::shared_ptr<std::vector<char>> buffer_recv(new std::vector<char>(1024, 0));
	//char buffer_recv[1024] = {0};
	//memset(buffer_recv, 0, 1024);
	int ret = recv(newchannel->getfd(), &*(buffer_recv->begin()), BUF_SIZE-1, 0);
	assert(ret >= 0);
	if(ret == 0){//此时客户端连接断开
		loop_->RemovetoEpool(newchannel);
		ChannelMap.erase(newchannel->getfd());
		return;
	}
	printf("%s\n", &*(buffer_recv->begin())); //
	for(auto it = ChannelMap.begin(); it != ChannelMap.end(); it++){
		if(it->first != newchannel->getfd())
		send(it->first, &*(buffer_recv->begin()), strlen(&*(buffer_recv->begin())), 0);
	}

}


 
void TcpSever::NewConnect(int connfd){//监听事件被触发时，将会执行此函数
		//struct sockaddr_in client;
		//socklen_t client_addrlength = sizeof(client);
		//int connfd = accept(listenfd_, (struct sockaddr*)&client, &client_addrlength);
		//if(connfd > 0){
			printf("connfd:%d\n", connfd);
			SP_Channel newchannel(new Channel(loop_, connfd, EPOLLIN | EPOLLET));
			newchannel->SetReadHandl(std::bind(&TcpSever::ReadClient, this, newchannel));
			loop_->AddtoEpool(newchannel);
			ChannelMap[connfd] = newchannel;
		//}



}





 //}
