// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//

#include "Buffer.h"




#include <errno.h>
#include <sys/uio.h>




const char Buffer::kCRLF[] = "\r\n"; //停止符

const size_t Buffer::kCheapPrepend;//预备字节的大小
const size_t Buffer::kInitialSize;//缓冲区的大小




//这个函数是将fd中的内容读写到buffer中。
//使用readv函数来读取，读取过程分为三步：
//1.将fd中的内容直接读入buffer缓冲区中
//2.如果缓冲区可写的空间不足，将剩余的内容读取到额外空间中extrabuf
//3.将额外空间中extrabuf中的内容，重写写入buffer缓冲区中（此时buffer缓冲区大小已经别调整到合适大小）
ssize_t Buffer::readFd(int fd)
{
  // saved an ioctl()/FIONREAD call to tell how much to read
  char extrabuf[65536];
  struct iovec vec[2];
  const size_t writable = writableBytes();
  vec[0].iov_base = begin()+writerIndex_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof extrabuf;
  // when there is enough space in this buffer, don't read into extrabuf.
  // when extrabuf is used, we read 128k-1 bytes at most.
  const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
  const ssize_t n = sockets::readv(fd, vec, iovcnt);
  if (n < 0)
  {
    //avedErrno = errno;
  }
  else if (static_cast<size_t>(n) <= writable)
  {
    writerIndex_ += n;
  } 
  else
  {
    writerIndex_ = buffer_.size();
    append(extrabuf, n - writable);
  }
  // if (n == writable + sizeof extrabuf)
  // {
  //   goto line_30;
  // }
  return n;
}

