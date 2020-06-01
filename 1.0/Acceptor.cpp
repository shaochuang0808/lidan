#include "Acceptor.h"



Acceptor::Acceptor(EventLoop* loop, int port)
:loop_(loop),
port_(port),
listenfd_(sockets::SocketBind(port_)),
listenchannel_(new Channel(loop_, listenfd_)){
	
	listenchannel_->SetReadHandl(std::bind(&Acceptor::handleRead, this));
}


Acceptor::~Acceptor(){}


void Acceptor::handleRead(){

		struct sockaddr_in client;
		socklen_t client_addrlength = sizeof(client);
		int connfd = accept(listenfd_, (struct sockaddr*)&client, &client_addrlength);
		if(connfd > 0){
			if (newConnectionCallback_){
		      	newConnectionCallback_(connfd);
		    }
		}

}

void Acceptor::start(){
	listenchannel_->enableReading();
}