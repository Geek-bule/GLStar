//
//  GameProps.cpp
//  PopingStar
//
//  Created by JustinYang on 16/10/13.
//
//

#include "GameProps.hpp"
#include "cocos-ext.h"
#include "GameData.h"
#include "GamePlayScene.h"

bool GameProps::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    LayerColor* color1 = LayerColor::create(Color4B(0, 0, 0, 150), winSize.width, winSize.height);
    color1->setAnchorPoint(Vec2(0, 0));
    color1->setPosition(Vec2(0, kXDirCellNum*kCellWidth+ADSHight));
    addChild(color1);
    
    LayerColor* color2 = LayerColor::create(Color4B(0, 0, 0, 150), winSize.width, ADSHight);
    color2->setAnchorPoint(Vec2(0, 0));
    color2->setPosition(Vec2(0, 0));
    addChild(color2);
    
    //箭头
    Sprite *arrow = Sprite::create("res/paint_arrow.png");
    arrow->setPosition(Vec2(0, 0));
    arrow->setAnchorPoint(Vec2(0.5, 0));
    arrow->setVisible(false);
    addChild(arrow,0,2);
    
    //创建横条，但是隐藏
    Sprite* back = Sprite::create("res/paint_bg.png");
    back->setAnchorPoint(Vec2(0.5, 1));
    back->setPosition(apccp(0.5, 0.5));
    addChild(back,0,1);
    back->setVisible(false);
    
    for (int i=0; i < 5; i++) {
        std::string name;
        switch (i+1) {
            case 1:
                name = "block_purple-hd.png";
                break;
            case 2:
                name = "block_blue-hd.png";
                break;
            case 3:
                name = "block_red-hd.png";
                break;
            case 4:
                name = "block_green-hd.png";
                break;
            case 5:
                name = "block_yellow-hd.png";
                break;
            default:
                log("Error::value%d不是元素id",i);
                break;
        }
        ui::Button* pStar = ui::Button::create(name.c_str(),name.c_str(),name.c_str(),ui::Button::TextureResType::PLIST);
        pStar->addClickEventListener(CC_CALLBACK_1(GameProps::magicHandler, this));
        pStar->setPosition(Vec2(back->getContentSize().width/2+winSize.width/7*(-2.5+i), back->getContentSize().height/2));
        back->addChild(pStar,1,i+1);
    }
    
    ui::Button* pBack = ui::Button::create("res/paint_back.png","res/paint_back.png");
    pBack->addClickEventListener(CC_CALLBACK_1(GameProps::closeHandler, this));
    pBack->setPosition(Vec2(back->getContentSize().width/2+winSize.width/7*2.5, back->getContentSize().height/2));
    back->addChild(pBack);
    
    Sprite* pArrow = Sprite::create("com_arrow.png");
    pArrow->setPosition(Vec2(winSize.width/2, winSize.height*0.7));
    addChild(pArrow);
    
    auto action1 = MoveBy::create(0.3, Vec2(0, 10));
    auto action2 = MoveBy::create(0.3, Vec2(0, -10));
    auto action3 = Sequence::create(action1,action2, NULL);
    auto action = RepeatForever::create(action3);
    pArrow->runAction(action);
    
    Label *tipLabel = Label::createWithSystemFont("点击星星来替换星星颜色，消耗3金币", "", 32);
    tipLabel->setPosition(Vec2(pArrow->getContentSize().width/2, pArrow->getContentSize().height*1.3));
    pArrow->addChild(tipLabel);
    
    
    
    isSelected = false;
    
    return true;
}

GameProps::~GameProps()
{
    
}


void GameProps::magicHandler(cocos2d::Ref *pSender)
{
    Node*item =(Node*)pSender;
    if (GamePlayScene::getInstance()->getMoney() >= 3){
        if (GameData::getInstance()->boyChange(selectPos, item->getTag())){
            GamePlayScene::getInstance()->setMoney(GamePlayScene::getInstance()->getMoney()-3);
            ZYTools::uMengUseProps("1", 1, 3);
        }
        _closeLayer();
    }else{
        MessageBox("您的金币不足3金币，无法使用道具，奖励您点击屏幕下方观看视频领取免费金币，数量有限的哦～", "金币不足！");
    }
}

void GameProps::closeHandler(cocos2d::Ref *pSender)
{
    isSelected =false;
    Sprite* bg = (Sprite*)getChildByTag(1);
    bg->setVisible(false);
    Sprite* arrow = (Sprite*)getChildByTag(2);
    arrow->setVisible(false);
}

void GameProps::_closeLayer()
{
    GamePlayScene::getInstance()->stopMaigcAction();
    removeFromParentAndCleanup(true);
}

void GameProps::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchEnded = CC_CALLBACK_2(GameProps::onTouchEnded, this);
    listener->onTouchBegan = CC_CALLBACK_2(GameProps::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameProps::onTouchMoved, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameProps::onExit()
{
    Layer::onExit();
}

bool GameProps::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    Point location = CCDirector::getInstance()->convertToGL(pTouch->getLocationInView());
    location = location+Vec2(-ipadAdaptation, 0);
    //如果触摸到星星块，就显示替换星星界面，如果没有就去掉这个界面
    if (isSelected) {
        
    }else{
        Rect rect = Rect(0, ADSHight, kXDirCellNum*kCellWidth, kXDirCellNum*kCellWidth);
        if (rect.containsPoint(location)) {
            int potX = GETPX(location.x+32);
            int potY = GETPY(location.y+32);
            if (GameData::getInstance()->isStarExsit(potX, potY)) {
                isSelected = true;
                Sprite* bg = (Sprite*)getChildByTag(1);
                bg->setVisible(true);
                selectPos.x = SETPX( potX );
                selectPos.y = SETPY( potY );
                log("potX:(%d,%d)--pos:(%f,%f)",potX,potY,selectPos.x,selectPos.y);
                bg->setPosition(Vec2(winSize.width/2, (selectPos.y > winSize.height*0.3)?selectPos.y-32:selectPos.y+32+bg->getContentSize().height));
                Sprite*arrow = (Sprite*)getChildByTag(2);
                if (selectPos.y > winSize.height*0.3) {
                    arrow->setFlippedY(true);
                    arrow->setVisible(true);
                    arrow->setPosition(Vec2(selectPos.x+ipadAdaptation,selectPos.y-arrow->getContentSize().height));
                }else{
                    arrow->setVisible(true);
                    arrow->setPosition(Vec2(selectPos.x+ipadAdaptation,selectPos.y));
                }
            }
        }else {
            _closeLayer();
        }
    }
    
    return true;
}

void GameProps::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}

void GameProps::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}