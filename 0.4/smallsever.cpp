#include "Epoll.h"


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



#include <vector>

using namespace std;



#define BUF_SIZE 1024
#define EPOLL_SIZE 19

 void addfd(int epollfd, int fd){
	 struct epoll_event ev;
	 ev.data.fd = fd;
	 ev.events = EPOLLIN | EPOLLET;
	 epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
	 fcntl(fd, F_SETFL, fcntl(fd, F_GETFD,0) | O_NONBLOCK);
	 printf("fd added to epoll!\n");
 }


 int main(){
	/*关于socket*/
 	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);//IPV4, TCPint optval = 1;//设置为此套接字结束后，其对应的端口，可以立即被新的套接字绑定
	assert(listen_fd >= 0);
	int optval = 1;//设置为此套接字结束后，其对应的端口，可以立即被新的套接字绑定
	setsockopt(listen_fd, SOL_SOCKET,  SO_REUSEADDR, &optval, sizeof(optval));
	// 设置服务器IP和Port，和监听描述副绑定
    struct sockaddr_in server_addr;
	int port = 8888;
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
	int ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	assert(ret != -1);
	ret = listen(listen_fd, 19);
	assert(ret != -1);


	lidan::Epoll myepoll;
	std::vector<int> clients_list;
	
	myepoll.epoll_add(listen_fd);


	while(1){
		int epoll_events_count = myepoll.poll();
		
		for(int i = 0; i < epoll_events_count; i++){
			int sockfd = myepoll.events_[i].data.fd;
			if(sockfd == listen_fd){
				struct sockaddr_in client;
				socklen_t client_addrlength = sizeof(client);
				int connfd = accept(listen_fd, (struct sockaddr*)&client, &client_addrlength);
				assert(connfd > 0);

				myepoll.epoll_add(connfd);
				clients_list.push_back(connfd);
				printf("new client connecte\n");
			}
			else{
				char buffer_recv[BUF_SIZE] = {0};
				memset(buffer_recv, 0, BUF_SIZE);
				ret = recv(sockfd, buffer_recv, BUF_SIZE-1, 0);
				assert(ret >= 0);

				for(int i = 0; i < clients_list.size(); i++){
					send(clients_list[i], buffer_recv, strlen(buffer_recv), 0);
				}
			}
		}



	}

	for(int i = 0; i < clients_list.size(); i++){
		close(clients_list[i]);
	}
	close(listen_fd);
	return 0;

 	
 }
