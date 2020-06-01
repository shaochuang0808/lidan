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
#include <map>
#include <memory>

#include "Channel.h"

#define FDSIZE 1024
#define EPOLLWAIT_TIME 10

//using namespace lidan;

class Channel;
typedef std::shared_ptr<Channel> SP_Channel;


//namespace lidan{



class Epoll{
	public:
		Epoll();
		~Epoll();
		void epoll_add(SP_Channel new_channel);
		void epoll_mod(SP_Channel old_channel);
		void epoll_del(SP_Channel the_channel);
		int GetFd();
		void poll(std::vector<SP_Channel>& ChannelList);
	public:
		int epollfd_;
		std::vector<epoll_event> events_;//供epoll_wait()使用的epoll_event数组
		std::map<int, SP_Channel> ChannelMap;//所有在epoll中注册的channel集合
};




//}
