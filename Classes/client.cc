#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/TcpClient.h>

#include <iostream>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

class ChatClient : muduo::noncopyable
{
 public:
  ChatClient(EventLoop* loop, const InetAddress& serverAddr)
    : client_(loop, serverAddr, "ChatClient")
  {
    client_.setConnectionCallback(std::bind(&ChatClient::onConnection, this, std::placeholders::_1));
    client_.setMessageCallback(std::bind(&ChatClient::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    client_.enableRetry();
  }

  void connect()
  {
    client_.connect();
  }

  void disconnect()
  {
    client_.disconnect();
  }

  void write(const StringPiece& message)
  {
    MutexLockGuard lock(mutex_);
    if (connection_)
    {
      connection_->send(message);
    }
  }

 private:
  void onConnection(const TcpConnectionPtr& conn)
  {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

    MutexLockGuard lock(mutex_);
    if (conn->connected())
    {
      connection_ = conn;
    }
    else
    {
      connection_.reset();
    }
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
  {
      if (buf->readableBytes() >= sizeof(int32_t))
      {
          /*
          const void* data = buf->peek();
          int32_t be32 = *static_cast<const int32_t*>(data);
          buf->retrieve(sizeof(int32_t));
          time_t time = sockets::networkToHost32(be32);
          Timestamp ts(implicit_cast<uint64_t>(time) * Timestamp::kMicroSecondsPerSecond);
          LOG_INFO << "Server time = " << time << ", " << ts.toFormattedString();
          */
          std::string str = static_cast<std::string>(buf->retrieveAllAsString());
          std::cout << "Server time = " << str;
      }
      else
      {
          std::cout << conn->name() << " no enough data " << buf->readableBytes()
                   << " at " << receiveTime.toFormattedString();
      }
  }


  TcpClient client_;
  MutexLock mutex_;
  TcpConnectionPtr connection_;
};

int main(int argc, char* argv[])
{
    EventLoopThread loopThread;
    uint16_t port = static_cast<uint16_t>(40000);
    InetAddress serverAddr("127.0.0.1", port);

    ChatClient client(loopThread.startLoop(), serverAddr);
    client.connect();
    std::string line;
    while (std::getline(std::cin, line))
    {
      client.write(line);
    }
    client.disconnect();
    CurrentThread::sleepUsec(1000*1000);  // wait for disconnect, see ace/logging/client.cc
}
