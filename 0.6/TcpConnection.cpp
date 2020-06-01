
#include "TcpConnection.h"

TcpConnection::TcpConnection(EventLoop* loop, std::string name, int fd, int event)
: loop_(loop),
name_(name),
channel_(new Channel(loop, fd, event))
{}


TcpConnection::~TcpConnection(){}