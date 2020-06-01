//作者：shao chuang
//QQ群：1020789851
//时间：2020.03.19

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



#include <vector>


#define FDSIZE 1024
#define EPOLLWAIT_TIME 1000


using namespace std;
//using namespace lidan;

namespace lidan{



class Epoll{
	public:
		Epoll();
		~Epoll();
		void epoll_add(int fd);
		int poll();
	public:
		int epollfd_;
		std::vector<epoll_event> events_;
};




}
