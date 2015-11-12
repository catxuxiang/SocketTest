#include "mytcpclient.h"
#include "constants.h"
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;
using namespace cocos2d;

std::shared_ptr<MyTcpClient> MyTcpClient::gInstance_ = nullptr;

MyTcpClient::MyTcpClient(EventLoop* loop, const InetAddress& serverAddr)
  : client_(loop, serverAddr, "MyTcpClient")
{
    client_.setConnectionCallback(std::bind(&MyTcpClient::onConnection, this, std::placeholders::_1));
    client_.setMessageCallback(std::bind(&MyTcpClient::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    client_.enableRetry();
}
MyTcpClient::~MyTcpClient()
{
}

void MyTcpClient::connect()
{
    client_.connect();
}

void MyTcpClient::disconnect()
{
    client_.disconnect();
}

void MyTcpClient::write(const char* pMsg)
{
    MutexLockGuard lock(mutex_);
    if (connection_)
    {
        connection_->send(std::string(pMsg));
    }
}

void MyTcpClient::onConnection(const TcpConnectionPtr& conn)
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

void MyTcpClient::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
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
        LOG_INFO << "Server time = " << str;
    }
    else
    {
        LOG_INFO << conn->name() << " no enough data " << buf->readableBytes()
                 << " at " << receiveTime.toFormattedString();
    }
}
