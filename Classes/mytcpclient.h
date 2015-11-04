#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/TcpClient.h>

#include <iostream>
#include <stdio.h>

class MyTcpClient : muduo::noncopyable
{
 public:
  MyTcpClient(muduo::net::EventLoop* loop, const muduo::net::InetAddress& serverAddr);

  void connect();
  void disconnect();

  void write(const muduo::StringPiece& message);

 private:
  void onConnection(const muduo::net::TcpConnectionPtr& conn);
  void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp receiveTime);

  muduo::net::TcpClient client_;
  muduo::MutexLock mutex_;
  muduo::net::TcpConnectionPtr connection_;
};

#endif // MYTCPCLIENT_H
