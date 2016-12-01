//
//  GameOver.cpp
//  泡你妹改
//
//  Created by justin yang on 13-9-30.
//
//

#include "GameOver.h"
#include "GamePlayScene.h"
#include "GameBse.h"
#include "SimpleAudioEngine.h"
#include "GameStartScene.h"

static GameOver *instance = nullptr;

GameOver *GameOver::getInstance()
{
    return instance;
}


bool GameOver::init()
{
    if(!Layer::create()){
        return false;
    }
    instance = this;
    LayerColor *layer = LayerColor::create(Color4B(0, 0, 0, 180));
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
    MenuItem* pCountinue = MenuItemImage::create("res/selectbtn.png", "res/selectbtn.png", "res/selectbtn.png", CC_CALLBACK_1(GameOver::_contiHander, this));
    pCountinue->setAnchorPoint(ancpHalf);
    menu->addChild(pCountinue,1,1);
    pCountinue->setPosition(Vec2(winSize.width*0.25, winSize.height*0.5-120));
    
    Label*pTipLabel = Label::createWithSystemFont("金币复活", "", 30);
    pCountinue->addChild(pTipLabel);
    pTipLabel->setPosition(pCountinue->getContentSize()/2);
    
    
    MenuItem* pVideoCont = MenuItemImage::create("res/selectbtn.png", "res/selectbtn.png", "res/selectbtn.png", CC_CALLBACK_1(GameOver::_videoHandler, this));
    pVideoCont->setAnchorPoint(ancpHalf);
    menu->addChild(pVideoCont,1,2);
    pVideoCont->setPosition(Vec2(winSize.width*0.75, winSize.height*0.5-120));
    pVideoCont->setVisible(false);
    Label*pTipVideo = Label::createWithSystemFont("免费复活", "", 30);
    pVideoCont->addChild(pTipVideo);
    pTipVideo->setPosition(pVideoCont->getContentSize()/2);
    
    Sprite*pVideoImage = Sprite::create("com_tvFrame.png");
    pVideoImage->setPosition(Vec2(0, 0));
    pVideoCont->addChild(pVideoImage);
    
    //重玩选择 否
    MenuItem* pMenuBut = MenuItemImage::create("res/option_no.png", "res/option_no.png", "res/option_no.png", CC_CALLBACK_1(GameOver::_exitHaner, this));
    pMenuBut->setAnchorPoint(ancpHalf);
    menu->addChild(pMenuBut);
    pMenuBut->setPosition(Vec2(winSize.width*0.85, winSize.height*0.5+100));
    
    //视频墙，列表墙，商城按钮
    MenuItem* pMoneyBut = MenuItemImage::create("res/money_plus.png", "res/money_plus.png", "res/money_plus.png", CC_CALLBACK_1(GameOver::_shopHandle, this));
    pMoneyBut->setAnchorPoint(ancpHalf);
    menu->addChild(pMoneyBut,1,TaButton);
    pMoneyBut->setPosition(Vec2(winSize.width*0.08, winSize.height*0.87));
    
    ParticleSystemQuad *par = ParticleSystemQuad::create("particle/goldtip.plist");
    par->setPosition(pMoneyBut->getPosition());
    addChild(par);
    
    
    std::string string1 = GameBase::getCString("replay");
    Label *show = Label::createWithTTF(string1, BMfontName, 35);
    show->setPosition(Vec2(winSize.width*0.5, winSize.height*0.5));
    addChild(show);
    
    //check video
    isHasVideo(ZYTools::isVideoVisible);
    
    return true;
}

GameOver::~GameOver()
{
    instance = nullptr;
}

void GameOver::isHasVideo(bool isHas)
{
    if (isHas) {
        MenuItemImage* item1 = (MenuItemImage*)getChildByTag(TagMenu)->getChildByTag(1);
        item1->setPosition(Vec2(winSize.width*0.25, winSize.height*0.5-120));
        MenuItemImage* item2 = (MenuItemImage*)getChildByTag(TagMenu)->getChildByTag(2);
        item2->setVisible(true);
    }else{
        MenuItemImage* item1 = (MenuItemImage*)getChildByTag(TagMenu)->getChildByTag(1);
        item1->setPosition(Vec2(winSize.width*0.5, winSize.height*0.5-120));
        MenuItemImage* item2 = (MenuItemImage*)getChildByTag(TagMenu)->getChildByTag(2);
        item2->setVisible(false);
    }
}

void GameOver::_contiHander(Ref *pSender)
{
    GameMusic::playEffect(SOUND_BUTTONCLIK);
    if (GamePlayScene::getInstance()->getMoney() >= 10){
        GamePlayScene::getInstance()->setMoney(GamePlayScene::getInstance()->getMoney()-10);
        
        GamePlayScene::getInstance()->replayHander();
        removeFromParentAndCleanup(true);
        ZYTools::uMengUseProps("2", 1, 10);
    }else{
        MenuItemImage* item = (MenuItemImage*)getChildByTag(TagMenu)->getChildByTag(TaButton);
        item->runAction(Blink::create(0.3, 5));
    }
}

void GameOver::_videoHandler(cocos2d::Ref *pSender)
{
    ZYTools::showVideo();
}

void GameOver::playFinish()
{
    GamePlayScene::getInstance()->replayHander();
    removeFromParentAndCleanup(true);
}

void GameOver::_exitHaner(Ref *pSender)
{
    GameMusic::playEffect(SOUND_BUTTONCLIK);
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, GameStartScene::createScene()));
}

void GameOver::_videoHandle(cocos2d::Ref *pSender)
{
    GameMusic::playEffect(SOUND_BUTTONCLIK);
}

void GameOver::_shopHandle(cocos2d::Ref *pSender)
{
    GameMusic::playEffect(SOUND_BUTTONCLIK);
    //buy some money
    ZYTools::buyProductIdentifier();
}

void GameOver::_wallHandle(Ref* pSender)
{
   GameMusic::playEffect(SOUND_BUTTONCLIK);
}

void GameOver::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchEnded = CC_CALLBACK_2(GameOver::onTouchEnded, this);
    listener->onTouchBegan = CC_CALLBACK_2(GameOver::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameOver::onTouchMoved, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameOver::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void GameOver::onExit()
{
    Layer::onExit();
}

bool GameOver::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void GameOver::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}

void GameOver::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}


















