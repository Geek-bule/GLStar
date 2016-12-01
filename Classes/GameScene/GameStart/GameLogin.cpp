//
//  GameLogin.cpp
//  ExtremeTest
//
//  Created by JustinYang on 14/12/28.
//
//

#include "GameLogin.h"



bool GameLogin::init()
{
    if (!Layer::create()) {
        return false;
    }
    
    LayerColor *layer = LayerColor::create(Color4B(0,0,0,150));
    addChild(layer,0,1);
    
    int getnum = userDefault->getIntegerForKey(LOGIC_TIMES);
    std::string day = __String::createWithFormat("第%d天登录奖励",getnum)->getCString();
    if (ZYTools::GetLanguage() != 1) {
        day = __String::createWithFormat("DAY: %d",getnum)->getCString();
    }
    Label *pLabel = Label::createWithTTF(day, BMfontName, 50);
    pLabel->setPosition(Vec2(winSize.width*0.5, winSize.height*0.65));
    layer->addChild(pLabel);
    
    return true;
}

void GameLogin::createCoins()
{
    int getnum = userDefault->getIntegerForKey(LOGIC_TIMES);
    for (int index = 0; index < getnum; index++) {
        Sprite* pCoins = Sprite::create("res/coin.png");
        pCoins->setPosition(winSize.width*0.2+index*winSize.width*0.1,winSize.height*0.5);
        addChild(pCoins,1);
        pCoins->setOpacity(0);
        
        auto action1 = DelayTime::create(0.1+index*0.05);
        auto action2 = FadeIn::create(0.2);
        auto action3 = DelayTime::create(0.8+index*0.1);
        auto action4 = MoveTo::create(0.8, Vec2(winSize.width*0.93, winSize.height*0.87));
        auto action5 = EaseBackIn::create(action4);
        auto action6 = CallFunc::create(CC_CALLBACK_0(GameLogin::animteOver, this, index+1));
        auto action7 = CallFunc::create(CC_CALLBACK_0(GameLogin::removeLayer, this));
        auto action8 = CallFunc::create(CC_CALLBACK_0(GameLogin::removeSelf, this));
        
        Vector<FiniteTimeAction*> pAction;
        pAction.pushBack(action1);
        pAction.pushBack(action2);
        pAction.pushBack(action3);
        if (index == getnum-1) {
            pAction.pushBack(action7);
        }
        pAction.pushBack(action5);
        pAction.pushBack(action6);
        if (index == getnum-1) {
            pAction.pushBack(action8);
        }
        
        auto action = Sequence::create(pAction);
        pCoins->runAction(action);
    }
}

void GameLogin::removeLayer()
{
    removeChildByTag(1);
}

void GameLogin::animteOver(int day)
{
    GameMusic::playEffect(SOUND_GOLDSIN);
    userDefault->setIntegerForKey(MONEY_LABEL, day*2+userDefault->getIntegerForKey(MONEY_LABEL));
    ZYTools::uMengBonusCoin(day*2, 5);
}

void GameLogin::removeSelf()
{
    removeFromParentAndCleanup(true);
}


void GameLogin::setDate(double date, int daynum)
{
    userDefault->setDoubleForKey(LOGIC_DATE, date);
    userDefault->setIntegerForKey(LOGIC_TIMES, daynum);
}


void GameLogin::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchEnded = CC_CALLBACK_2(GameLogin::onTouchEnded, this);
    listener->onTouchBegan = CC_CALLBACK_2(GameLogin::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameLogin::onTouchMoved, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    createCoins();
}

void GameLogin::onExit()
{
    Layer::onExit();
}

bool GameLogin::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void GameLogin::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}

void GameLogin::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}


