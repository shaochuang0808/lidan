cmake_minimum_required(VERSION 2.6) #限制最低版本
project(pas CXX) #cmake的工程名叫做pas，限定语言为c++
ADD_DEFINITIONS(-std=c++11) #采用c++11的标准
SET(CMAKE_CXX_FLAGS -pthread) #设置c++编译器支持多线程
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++0x")
#INCLUDE_DIRECTORIES(/home/shaochuang/build/release-install-cpp11/include)#头文件所在路径
#LINK_DIRECTORIES(/home/shaochuang/build/release-install-cpp11/lib)//库所在路径
#LINK_LIBRARIES(libmuduo_base.a libmuduo_net.a)//连接的静态库
add_executable(smallsever smallsever.cpp Channel.cpp Epoll.cpp EventLoop.cpp TcpSever.cpp Acceptor.cpp 
CurrentThread.cpp ThreadData.cpp Socket.cpp CountDownLatch.cpp)#最终生成的可执行文件，源文件
add_executable(smallclient smallclient.cpp)