#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "mytcpclient.h"
#include "client.cc"

class HelloWorld : public cocos2d::Layer
{
public:
    HelloWorld(muduo::net::EventLoop* loop, const muduo::net::InetAddress& serverAddr);
    virtual ~HelloWorld();

    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    static HelloWorld* create(muduo::net::EventLoop* loop, const muduo::net::InetAddress& serverAddr);

private:
    ChatClient _client;
};

#endif // __HELLOWORLD_SCENE_H__
