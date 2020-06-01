 //作者：shao chuang
 //QQ群：1020789851
 //时间：2020.05.10

#pragma once
#include <functional>
#include <memory>



//此文件记录了所有的回调函数的定义

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;//一个新的tcpconnection建立时的回调函数
typedef std::function<void (const TcpConnectionPtr&)> MessageCallback;//tcpconnection收到消息时的回调函数



