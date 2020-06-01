#include <memory>
#include <tr1/memory>


#include "TcpConnection.h"


//TcpConnection::TcpConnection(EventLoop* loop, std::string name, int fd, int event)
//: loop_(loop),
//name_(name),
//channel_(new Channel(loop, fd, event))
//{}


TcpConnection::TcpConnection(EventLoop* loop,      const std::string& nameArg, int sockfd )
  : loop_(loop),
    name_(nameArg),
    state_(kConnecting),
    //reading_(false),
    channel_(new Channel(loop, sockfd))
{
  channel_->SetReadHandl(
      std::bind(&TcpConnection::handleRead, this));
  channel_->SetWriteHandl(
      std::bind(&TcpConnection::handleWrite, this));
  channel_->SetCloseHandl(
      std::bind(&TcpConnection::handleClose, this));
  channel_->SetErrorHandl(
      std::bind(&TcpConnection::handleError, this));
}


TcpConnection::~TcpConnection(){}

void TcpConnection::connectEstablished()//建立连接
{
  loop_->assertInLoopThread();
  assert(state_ == kConnecting);
  setState(kConnected);
  //channel_->tie(shared_from_this());
  channel_->enableReading();

  connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed()//断开连接
{
  loop_->assertInLoopThread();
  if (state_ == kConnected)
  {
    setState(kDisconnected);
    channel_->disableAll();

    connectionCallback_(shared_from_this());
  }
  channel_->remove();
}

//void TcpConnection::startRead()
//{
//  loop_->runInLoop(std::bind(&TcpConnection::startReadInLoop, this));
//}

//void TcpConnection::startReadInLoop()
//{
//  loop_->assertInLoopThread();
//  if (!reading_ || !channel_->isReading())
// {
//    channel_->enableReading();
//    reading_ = true;
//  }
//}

//void TcpConnection::stopRead()
//{
//  loop_->runInLoop(std::bind(&TcpConnection::stopReadInLoop, this));
//}

//void TcpConnection::stopReadInLoop()
//{
//  loop_->assertInLoopThread();
//  if (reading_ || channel_->isReading())
//  {
//    channel_->disableReading();
//    reading_ = false;
//  }
//}

void TcpConnection::send(const void* data, int len)
{
  send(std::string(static_cast<const char*>(data), len));
}

void TcpConnection::send(const std::string& message)
{
  if (state_ == kConnected)//判断tcpconnection的状态
  {
    if (loop_->isInLoopThread())//如果处于I/O线程中，直接发送
    {
      sendInLoop(message);
    }
    else
    {
      void (TcpConnection::*fp)(const std::string& message) = &TcpConnection::sendInLoop;
      loop_->runInLoop(
          std::bind(fp,
                    this,     // FIXME
                    message));
                    //std::forward<string>(message)));
    }
  }
}

// FIXME efficiency!!!
void TcpConnection::send(Buffer* buf)
{
  if (state_ == kConnected)
  {
    if (loop_->isInLoopThread())
    {
	  printf("***************************************************");
	  sendInLoop(buf->peek(), buf->readableBytes());
      buf->retrieveAll();//buf中的数据已经被发送，或者被写入outputbuffer_中
    }
    else
    {
      void (TcpConnection::*fp)(const std::string& message) = &TcpConnection::sendInLoop;
      loop_->runInLoop(
          std::bind(fp,
                    this,     // FIXME
                    buf->retrieveAllAsString()));
                    //std::forward<string>(message)));
    }
  }
}

void TcpConnection::sendInLoop(const std::string& message)
{
  sendInLoop(message.data(), message.size());
}

void TcpConnection::sendInLoop(const void* data, size_t len)
{
  loop_->assertInLoopThread();//保证一定在I/O线程中
  ssize_t nwrote = 0;
  size_t remaining = len;
  //bool faultError = false;
  if (state_ == kDisconnected)//如果线程已经断开，则放弃发送
  {
    //LOG_WARN << "disconnected, give up writing";
    return;
  }
  // if no thing in output queue, try writing directly
  if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0)//如果channel_->isWriting()为真，则说明此时outputbuffer中
  {																	//还有数据没有发送完，此时不能直接发送，而是往outputbuffer中写
	printf("6666666666666666666666666666666666666666666666666666666");
	nwrote = sockets::write(channel_->getfd(), data, len);//向channel中写数据
    if (nwrote >= 0)
    {
      remaining = len - nwrote;
      //if (remaining == 0 && writeCompleteCallback_)//发送完成后，执行发送完成回掉函数
      //{
      //  loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
      //}
    }
    else // nwrote < 0出错
    {
      //nwrote = 0;
      //if (errno != EWOULDBLOCK)
      //{
      // LOG_SYSERR << "TcpConnection::sendInLoop";
      //  if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
      //  {
      //    faultError = true;
      //  }
      //}
    }
  }

  assert(remaining <= len);
  if (remaining > 0)//(!faultError && remaining > 0)//如果没有出错，且数据没有发送完毕
  {
    size_t oldLen = outputBuffer_.readableBytes();
    //if (oldLen + remaining >= highWaterMark_
    //    && oldLen < highWaterMark_
    //    && highWaterMarkCallback_)
    //{
    // loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
    //}
    outputBuffer_.append(static_cast<const char*>(data)+nwrote, remaining);//将没有发送完的数据，写入outputBuffer_
    if (!channel_->isWriting())
    {
      channel_->enableWriting();//channel关注可写事件
    }
  }
}



void TcpConnection::handleRead(){
	loop_->assertInLoopThread();//读写操作都要求必须在I/O线程中
	ssize_t n = inputBuffer_.readFd(channel_->getfd());//将数据读取到inputBuffer_中，由于epoll采用的LT模式，所以这里不担心一次读不完
	if (n > 0)
	{
		messageCallback_(shared_from_this(), &inputBuffer_);//此函数由用户给出
	}
	else if (n == 0)//客户端已经关闭
	{
		handleClose();
	}
	else
	{
		//errno = savedErrno;
		//LOG_SYSERR << "TcpConnection::handleRead";
		handleError();//连接出错
	}
}
void TcpConnection::handleWrite()
{
  loop_->assertInLoopThread();
  if (channel_->isWriting())//判断是否可写
  {
  	//向channel中写数据，如果写不完，则关注channel的可写事件。等下一轮触发，继续写
    ssize_t n = sockets::write(channel_->getfd(),
                               outputBuffer_.peek(),
                               outputBuffer_.readableBytes());
    if (n > 0)
    {
      outputBuffer_.retrieve(n);
      if (outputBuffer_.readableBytes() == 0)
      {
        channel_->disableWriting();//如果数据全部发送完毕，则不再关注channel的可写事件
        //if (writeCompleteCallback_)
        //{ //发送完毕后执行的回掉函数，此函数由用户给出
        //  loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
        //}
        if (state_ == kDisconnecting)
        {
          //shutdownInLoop();//服务器主动释放客户端
        }
      }
    }
    else
    {
      //LOG_SYSERR << "TcpConnection::handleWrite";
      // if (state_ == kDisconnecting)
      // {
      //   shutdownInLoop();
      // }
    }
  }
  else
  {
    //LOG_TRACE << "Connection fd = " << channel_->fd()
    //          << " is down, no more writing";
  }
}
void TcpConnection::handleClose(){}
void TcpConnection::handleError(){}
