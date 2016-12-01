#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__



/*
 1.星星换文件－ok
 2.暂停加排汗说明-ok
 3.动画调整时间-OK
 4.结束界面金币购买-OK
 5.添加声音-OK
 6.结算分数转移-OK
 7.good动画调整包括完成目标OK
 8.星星出场动画-ok
 9.特效资源
 
 2.0版本
 1.添加sharesdk
 2.gamecenter 成就
 3.想象是不是要把分数改成float
 4.多国账号
 5.重新申请
 6.推送奖励
 7.名字，关键字
 8.itunes 地址设成自动获取
 9.分享地址。url
 */