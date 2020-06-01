#include "Epoll.h"
#include "Channel.h"
#include "EventLoop.h"
//#include "Socket.h"
#include "TcpSever.h"





#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/epoll.h>
#include <fcntl.h>



void ReadClient(TcpConnectionPtr newtcpconnection, Buffer* thebuffer ){
	
	std::string data = thebuffer->retrieveAllAsString();
	for(auto it = TcpConnectionMap.begin(); it != TcpConnectionMap.end(); it++){
		it->second->send(data);
	}
	printf("ReadClient\n");

}

void NewConnectCallback(TcpConnectionPtr newtcpconnection){
	printf("\n%s\n", newtcpconnection->getname().c_str());
}


class EventLoop;
class TcpServer;

//sing namespace lidan;

 int main(){
 	//printf("0");
	
 	EventLoop myloop;
	//printf("1");
	TcpSever mysever(&myloop,8888);
	//printf("2");
	
	mysever.Start();
	//printf("3");
	
	//struct epoll_event events[9];
	//int epoll_events_count = epoll_wait(myloop.GetEpoolFd(), events, 9, -1);
	//printf("\nfd:%d\n",myloop.GetEpoolFd());
	//printf("hahahah");
	
	myloop.loop();
	printf("4");
	

	return 0;

 	
 }
