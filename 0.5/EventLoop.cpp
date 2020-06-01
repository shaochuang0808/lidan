//作者：shao chuang
//QQ群：1020789851
//时间：2020.03.19

#include "EventLoop.h"
#include "Epoll.h"

//namespace lidan{


EventLoop::EventLoop():epoll_(new Epoll()){}
EventLoop::~EventLoop(){}

void EventLoop::AddtoEpool(SP_Channel newchannel){
	
	epoll_->epoll_add(newchannel);
	printf("AddtoEpool\n");
	return;
}


void EventLoop::UptoEpool(SP_Channel oldchannel){
	epoll_->epoll_mod(oldchannel);
	return;
}

void EventLoop::RemovetoEpool(SP_Channel thechannel){
	epoll_->epoll_del(thechannel);
	return;
}


int EventLoop::GetEpoolFd(){
	return epoll_->GetFd();

}


void EventLoop::loop(){
	while(1){
		std::vector<SP_Channel> ChannelList = epoll_->poll();
		for(int i = 0; i < ChannelList.size(); i++){
			ChannelList[i]->EventHandl(ChannelList[i]);
			printf("in loop\n");

		}
		
	}

}

void EventLoop::doPendingFunctors(){
	for(int i = 0; i < pendingFunctors_.size(); i++){
		pendingFunctors_[i]();
	}
	pendingFunctors_.clear();
}






//}
