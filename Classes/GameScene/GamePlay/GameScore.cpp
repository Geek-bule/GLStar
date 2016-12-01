//
//  GameScore.cpp
//  泡你妹改
//
//  Created by justin yang on 13-10-8.
//
//

#include "GameScore.h"
#include "GameBse.h"

bool GameScore::init()
{
    if (!Layer::create()) {
        return false;
    }
    _initialze();
    return true;
}

GameScore::~GameScore()
{
    
}

void GameScore::_initialze()
{
    Label* showScore = Label::createWithTTF("0", BMfontName, 42);
    showScore->setAnchorPoint(Vec2(0.5, 0.5));
    showScore->setPosition(Vec2(winSize.width*0.5,winSize.height*0.85));
    addChild(showScore,1,TagNowScore);
    
    //target score label
    std::string string1 = GameBase::getCString("gscore");
    Label* needLabel = Label::createWithTTF(string1, BMfontName, 35);
    needLabel->setAnchorPoint(Vec2(0, 0.4));
    needLabel->setPosition(Vec2(winSize.width*0.35,winSize.height*0.94));
    addChild(needLabel,1);
    
    Label* needScore = Label::createWithTTF("0", BMfontName, 38);
    needScore->setAnchorPoint(Vec2(0, 0.4));
    needScore->setPosition(Vec2(winSize.width*0.64,winSize.height*0.94));
    addChild(needScore,1,TagNeedScore);
    
    std::string string2 = GameBase::getCString("stage");
    Label* stagetLabel = Label::createWithTTF(string2, BMfontName, 35);
    stagetLabel->setAnchorPoint(Vec2(0, 0.4));
    stagetLabel->setPosition(Vec2(winSize.width*0.02,winSize.height*0.94));
    addChild(stagetLabel,1);
    
    Label* target = Label::createWithTTF("0", BMfontName, 38);
    target->setAnchorPoint(Vec2(0, 0.4));
    target->setPosition(Vec2(winSize.width*0.25,winSize.height*0.94));
    addChild(target,1,TagStage);
    
    Sprite *scorebg = Sprite::create("res/nowscore.png");
    scorebg->setPosition(Vec2(winSize.width*0.5,winSize.height*0.85));
    addChild(scorebg);
}


void GameScore::setShowScore(int num)
{
    Label*showScore = (Label*)this->getChildByTag(TagNowScore);
    char str[50] = {};
    sprintf(str, "%d" , num);
    showScore->setString(str);
}

void GameScore::setNeedScore(int num)
{
    Label*needScore = (Label*)this->getChildByTag(TagNeedScore);
    __String *txt2 = __String::createWithFormat("%d", num);
    needScore->setString(txt2->getCString());
}

void GameScore::needScoreShake()
{
    Label*needScore = (Label*)this->getChildByTag(TagNeedScore);
    ActionInterval *action = Blink::create(1.5, 5);
    needScore->stopAllActions();
    needScore->runAction(action);
}

void GameScore::setTarget(int num)
{
    Label*target = (Label*)this->getChildByTag(TagStage);
    __String *txt2 = __String::createWithFormat("%d", num);
    target->setString(txt2->getCString());
}











