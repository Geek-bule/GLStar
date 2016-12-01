//
//  GameTip.cpp
//  泡你妹改
//
//  Created by justin yang on 13-9-30.
//
//

#include "GameTip.h"
#include "GamePlayScene.h"
#include "GameBse.h"
#include "SimpleAudioEngine.h"



bool GameTip::init()
{
    if(!Layer::create()){
        return false;
    }
    LayerColor *layer = LayerColor::create(Color4B(0, 0, 0, 100));
    addChild(layer);

    Sprite* back = GameBase::createSprite("res/paint_bg.png");
    back->setAnchorPoint(ancpHalf);
    back->setPosition(apccp(0.5, 0.5));
    this->addChild(back,0);
    
    Menu *menu = Menu::create();
    addChild(menu,1,TagMenu);
    menu->setAnchorPoint(ancpZero);
    menu->setPosition(Vec2(0, 0));
    
    //重玩选择 是
    MenuItem* pCountinue = MenuItemImage::create("res/option_yes.png", "res/option_yes.png", "res/option_yes.png", CC_CALLBACK_1(GameTip::_continHandle, this));
    pCountinue->setAnchorPoint(ancpHalf);
    menu->addChild(pCountinue);
    pCountinue->setPosition(Vec2(winSize.width*0.3, winSize.height*0.5-960*0.1));
    
    //重玩选择 否
    MenuItem* pMenuBut = MenuItemImage::create("res/option_no.png", "res/option_no.png", "res/option_no.png", CC_CALLBACK_1(GameTip::_cancleHanele, this));
    pMenuBut->setAnchorPoint(ancpHalf);
    menu->addChild(pMenuBut);
    pMenuBut->setPosition(Vec2(winSize.width*0.7, winSize.height*0.5-960*0.1));
    
    int stage = userDefault->getIntegerForKey(STAGE_CURRENT);
    std::string string1 = GameBase::getCString("newtip");
    __String* tip = __String::createWithFormat(string1.c_str(),stage);
    Label *show = Label::createWithTTF(tip->getCString(), BMfontName, 40);
    show->setPosition(winSize.width*0.5, winSize.height*0.5);
    addChild(show);
    
    return true;
}

GameTip::~GameTip()
{
    
}

void GameTip::_continHandle(cocos2d::Ref *pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, GamePlayScene::newScene()));
}

void GameTip::_cancleHanele(cocos2d::Ref *pSender)
{
    removeFromParentAndCleanup(true);
}

void GameTip::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchEnded = CC_CALLBACK_2(GameTip::onTouchEnded, this);
    listener->onTouchBegan = CC_CALLBACK_2(GameTip::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameTip::onTouchMoved, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameTip::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void GameTip::onExit()
{
    Layer::onExit();
}

bool GameTip::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void GameTip::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}

void GameTip::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}


















