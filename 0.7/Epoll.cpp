//作者：shao chuang
//QQ群：1020789851
//时间：2020.03.19
#include "Epoll.h"

using namespace std;
//using namespace lidan;




//namespace lidan{



Epoll::Epoll():epollfd_(epoll_create1(EPOLL_CLOEXEC)){//
	events_.resize(FDSIZE);
	printf("\nFD:%d\n", epollfd_);

}

Epoll::~Epoll(){

}


void Epoll::epoll_updata(Channel* new_channel){
	if(new_channel->getflag()){
		epoll_mod(new_channel);
	}
	else{
		epoll_add(new_channel);
		new_channel->setflag(true);
	}
}




void Epoll::epoll_add(Channel* new_channel){
	 
	 struct epoll_event ev;
	 ev.data.fd = new_channel->getfd();
	 ev.events = new_channel->getEvents(); //这里为了减少代码的复杂度，这里读事件和写事件都关注
	 printf("\nnew_channel->getfd():%d\n", new_channel->getfd());
	 epoll_ctl(epollfd_, EPOLL_CTL_ADD, new_channel->getfd(), &ev);
	 fcntl(new_channel->getfd(), F_SETFL, fcntl(new_channel->getfd(), F_GETFD,0) | O_NONBLOCK);
	 ChannelMap[new_channel->getfd()] = new_channel;
	 printf("fd added to epoll!!!!!!!!!");
	 printf("?????");
	 return;
}

void Epoll::epoll_mod(Channel* old_channel){
	struct epoll_event event;
	event.data.fd = old_channel->getfd();
	event.events = old_channel->getEvents();
	epoll_ctl(epollfd_, EPOLL_CTL_MOD, old_channel->getfd(), &event);
	return;
}

void Epoll::epoll_del(Channel* the_channel){
	struct epoll_event event;
	event.data.fd = the_channel->getfd();
	event.events = the_channel->getEvents();
	epoll_ctl(epollfd_, EPOLL_CTL_DEL, the_channel->getfd(), &event);
	ChannelMap.erase(the_channel->getfd());
	the_channel->setflag(false);  //将flag设为0，说明此channel没有在epoll中注册
	return;
}

int Epoll::GetFd(){
	return epollfd_;
}


void Epoll::poll(std::vector<Channel*>& ChannelList){
	
	events_.clear();
	events_.resize(FDSIZE);
	//printf("lalalal");
	int epoll_events_count = epoll_wait(epollfd_, &*events_.begin(), FDSIZE, EPOLLWAIT_TIME);
	if(epoll_events_count == 0){
		//printf("no\n");
	}
	else{
		printf("yes%d\n", epoll_events_count);	
	}
	//printf("222222222");
	//printf("%d",epoll_events_count);
	assert(epoll_events_count >= 0);
	for(int i = 0; i < epoll_events_count; i++){
		if(ChannelMap.find(events_[i].data.fd) != ChannelMap.end()){
			ChannelMap[events_[i].data.fd]->SetRevent(events_[i].events);
			//ChannelMap[events_[i].data.fd]->SetEvent(0);
			ChannelList.push_back(ChannelMap[events_[i].data.fd]);
		}
	}

}















//}
