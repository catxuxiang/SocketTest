#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/TcpClient.h>

#include <iostream>
#include <stdio.h>
#include "cocos2d.h"

class MyTcpClient : public muduo::noncopyable
{
 public:
  MyTcpClient(muduo::net::EventLoop* loop, const muduo::net::InetAddress& serverAddr);
  virtual ~MyTcpClient();

  void connect();
  void disconnect();

  void write(const char* pMsg);

 private:


  void onConnection(const muduo::net::TcpConnectionPtr& conn);
  void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp receiveTime);

  muduo::net::TcpClient client_;
  muduo::MutexLock mutex_;
  muduo::net::TcpConnectionPtr connection_;

  static std::shared_ptr<MyTcpClient> gInstance_;
};

#endif // MYTCPCLIENT_H
