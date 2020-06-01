//作者：shao chuang
//QQ群：1020789851
//时间：2020.03.19

#include "Channel.h"


//namespace lidan{


Channel::Channel(EventLoop * loop, int fd, int event):loop_(loop), fd_(fd), event_(event){

}
Channel::~Channel(){

	close(fd_);

}


int Channel::getfd(){
	return fd_;
}

int Channel::getEvents(){
	return event_;
}

void Channel::ReadHandl(){
	if(ReadHandler_){
		ReadHandler_();
	}
}
void Channel::WriteHandl(){
	if(WriteHandler_){
		WriteHandler_();
	}

}
void Channel::CloseHandl(){
	if(CloseHandler_){
		CloseHandler_();
	}

}

void Channel::ErrorHandl(){
	if(ErroeHandler_){
		ErroeHandler_();
	}

}

void Channel::MessageCallback(){
	if(MessageCallback_){
		MessageCallback_();
	}
}

void Channel::EventHandl(std::shared_ptr<Channel> thischannel){
	//event_ = 0;
    if ((revent_ & EPOLLHUP) && !(revent_ & EPOLLIN)) {//对端异常断开
      //event_ = 0;
      return;
    }
	
    if (revent_ & EPOLLERR) {//服务端本地报错
      ErrorHandl();
      //event_ = 0;
      return;
    }

    if (revent_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
		printf("eventhandl-read\n");
      ReadHandl();//这三个都是可读事件，EPOLLIN(可读、已连接)，EPOLLPRI(外带数据),EPOLLRDHUP(对端正常断开)
    }
	
    if (revent_ & EPOLLOUT) {//可写事件
      WriteHandl();
	  printf("eventhandl-writ\n");
    }
	loop_->UptoEpool(thischannel);
	printf("eventhandl-1thischannlefd:%d\n", thischannel->getfd());

}




//}