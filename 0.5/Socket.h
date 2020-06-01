//作者：shao chuang
//QQ群：1020789851
//时间：2020.03.19
//说明：此头文件封装与socket相关的函数


#pragma once
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



int SocketBind(int port){
	/*关于socket*/
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);//IPV4, TCPint optval = 1;//设置为此套接字结束后，其对应的端口，可以立即被新的套接字绑定
	assert(listen_fd >= 0);
	int optval = 1;//设置为此套接字结束后，其对应的端口，可以立即被新的套接字绑定
	setsockopt(listen_fd, SOL_SOCKET,  SO_REUSEADDR, &optval, sizeof(optval));
	// 设置服务器IP和Port，和监听描述副绑定
	struct sockaddr_in server_addr;
	bzero((char*)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons((unsigned short)port);
	int ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	assert(ret != -1);
	ret = listen(listen_fd, 19);
	assert(ret != -1);
	printf("\ncreatfd:%d\n", listen_fd);
	return listen_fd;
}


