//作者：shao chuang
//QQ群：1020789851
//时间：2020.03.19
#include "Epoll.h"

using namespace std;
using namespace lidan;




namespace lidan{



Epoll::Epoll():epollfd_(epoll_create1(EPOLL_CLOEXEC)), events_(FDSIZE){

}

Epoll::~Epoll(){

}


void Epoll::epoll_add(int fd){
	 struct epoll_event ev;
	 ev.data.fd = fd;
	 ev.events = EPOLLIN | EPOLLET;
	 epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev);
	 fcntl(fd, F_SETFL, fcntl(fd, F_GETFD,0) | O_NONBLOCK);
	 printf("fd added to epoll!\n");
}



int Epoll::poll(){
	int epoll_events_count = epoll_wait(epollfd_, &*events_.begin(), FDSIZE, EPOLLWAIT_TIME);
	assert(epoll_events_count >= 0);
	return epoll_events_count;
}















}
