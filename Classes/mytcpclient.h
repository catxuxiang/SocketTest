#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/TcpClient.h>

#include <iostream>
#include <stdio.h>
#include "cocos2d.h"

class MyTcpClient : public muduo::noncopyable, public cocos2d::Ref
{
 public:
  virtual ~MyTcpClient();

  static MyTcpClient* getInstance();

  void connect();
  void disconnect();

  void write(const char* pMsg);

 private:
  MyTcpClient(muduo::net::EventLoop* loop, const muduo::net::InetAddress& serverAddr);

  static MyTcpClient* create();
  bool init();


  void onConnection(const muduo::net::TcpConnectionPtr& conn);
  void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp receiveTime);

  muduo::net::TcpClient client_;
  muduo::MutexLock mutex_;
  muduo::net::TcpConnectionPtr connection_;

  static MyTcpClient* gInstance_;
};

#endif // MYTCPCLIENT_H
