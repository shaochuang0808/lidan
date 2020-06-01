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


namespace sockets{

int SocketBind(int port);

ssize_t read(int sockfd, void *buf, size_t count);


ssize_t write(int sockfd, const void *buf, size_t count);


uint64_t hostToNetwork64(uint64_t host64);


uint32_t hostToNetwork32(uint32_t host32);


uint16_t hostToNetwork16(uint16_t host16);


uint64_t networkToHost64(uint64_t net64);


uint32_t networkToHost32(uint32_t net32);


uint16_t networkToHost16(uint16_t net16);


ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);





}


