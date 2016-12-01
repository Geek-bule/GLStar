//
//  GamePlayScene.cpp
//  泡你妹改
//
//  Created by justin yang on 13-10-8.
//
//

#include "GamePlayScene.h"
#include "SimpleAudioEngine.h"
#include "GameBse.h"
#include "GameStartScene.h"
#include "GameOver.h"
#include "GamePause.h"
#include "ZYTools.h"
#include "GameProps.hpp"



Scene* GamePlayScene::newScene()
{
    Scene *scene = Scene::create();
    GamePlayScene* play = GamePlayScene::create();
    scene->addChild(play);
    play->_newplay();
    return scene;
}

Scene* GamePlayScene::resScene()
{
    Scene *scene = Scene::create();
    GamePlayScene* play = GamePlayScene::create();
    scene->addChild(play);
    play->_resplay();
    return scene;
}

static GamePlayScene* sharePlay = NULL;

GamePlayScene *GamePlayScene::getInstance()
{
    if (sharePlay != NULL) {
        return sharePlay;
    }
    return NULL;
}

bool GamePlayScene::init()
{
    if (!Layer::create()) {
        return false;
    }
    sharePlay = this;
    m_CanSave = false;
    _initialze();
    return true;
}

GamePlayScene::~GamePlayScene()
{
    sharePlay = NULL;
}

void GamePlayScene::_initialze()
{
    m_stage = 1;
    m_isWin = 1;//1:游戏中 2:获胜 3:失败
    m_CanSave = false;
    m_scoreCurr = 0;
    isRes = false;
    
    Sprite *backgroud = GameBase::createSprite("res/backgroud.png");
    backgroud->setAnchorPoint(Vec2(0.5, 1));
    backgroud->setPosition(apccp(0.5, 1));
    this->addChild(backgroud,-1);
    
    Menu *menu = Menu::create();
    addChild(menu,zorderMenu,TagMenu);
    menu->setAnchorPoint(ancpZero);
    menu->setPosition(Vec2(0, 0));
    menu->setEnabled(false);//游戏开始暂停需要锁定
    
    MenuItem* pauseBut = MenuItemImage::create("res/option_button.png", "res/option_button.png", "res/option_button.png", CC_CALLBACK_1(GamePlayScene::_pauseHander, this));
    pauseBut->setAnchorPoint(ancpHalf);
    menu->addChild(pauseBut);
    pauseBut->setPosition(Vec2(winSize.width*0.93, winSize.height*0.94));
    
    MenuItem* pMoneyBut = MenuItemImage::create("res/money_plus.png", "res/money_plus.png", "res/money_plus.png", CC_CALLBACK_1(GamePlayScene::_moneyHandle, this));
    pMoneyBut->setAnchorPoint(ancpHalf);
    menu->addChild(pMoneyBut);
    pMoneyBut->setPosition(Vec2(winSize.width*0.08, winSize.height*0.89));
    
    //视频按钮
    MenuItem* pVideoBut = MenuItemImage::create("res/com_tvFrame.png", "res/com_tvFrame_.png", "res/com_tvFrame_.png", CC_CALLBACK_1(GamePlayScene::_videoHandler, this));
    pVideoBut->setAnchorPoint(Vec2(0.5, 0));
    menu->addChild(pVideoBut,1,101);
    pVideoBut->setPosition(Vec2(winSize.width*0.5, 8));
    if (!ZYTools::isVideoVisible) {
        pVideoBut->setVisible(false);
    }
    
    Label* pTipVideo = Label::createWithSystemFont("免费金币", "Aril", 30);
    pTipVideo->setPosition(Vec2(pVideoBut->getContentSize().width/2, 5));
    pVideoBut->addChild(pTipVideo);
    
    
    Sprite* pArrow = Sprite::create("com_arrow.png");
    pArrow->setPosition(Vec2(110, pVideoBut->getContentSize().height/2));
    pVideoBut->addChild(pArrow);
    pArrow->setRotation(90);
    
    auto action1 = MoveBy::create(0.3, Vec2(10, 0));
    auto action2 = MoveBy::create(0.3, Vec2(-10, 0));
    auto action3 = Sequence::create(action1,action2, NULL);
    auto action = RepeatForever::create(action3);
    pArrow->runAction(action);
    
    
    //道具按钮
    MenuItem* pMagicStar = MenuItemImage::create("res/props_paint.png", "res/props_paint.png", "res/props_paint.png", CC_CALLBACK_1(GamePlayScene::_magicHandle, this));
    pMagicStar->setAnchorPoint(Vec2(0.5, 0.5));
    menu->addChild(pMagicStar,1,102);
    pMagicStar->setPosition(Vec2(winSize.width*0.8, winSize.height*0.85));
    
    
    m_Money = userDefault->getIntegerForKey(MONEY_LABEL);
    __String *pMoneyStr = __String::createWithFormat("%d",m_Money);
    Label *pMoneyLabel = Label::createWithTTF(pMoneyStr->getCString(), BMfontName, 40);
    pMoneyLabel->setPosition(winSize.width*0.18, winSize.height*0.87);
    addChild(pMoneyLabel,zorderTip,TagMoney);

    m_data = GameData::create();
    m_data->setPosition(Vec2(ipadAdaptation+ 0, 0));
    addChild(m_data,zorderBG);
    
    m_score = GameScore::create();
    m_score->setPosition(Vec2(0, 0));
    addChild(m_score,1);
    
    bool isMusic = userDefault->getBoolForKey(MUSIC_ONOFF);
    if (!isMusic) {
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
    }else{
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
    }
    
    schedule(schedule_selector(GamePlayScene::updateMoney), 0.1);
}


void GamePlayScene::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchEnded = CC_CALLBACK_2(GamePlayScene::onTouchEnded, this);
    listener->onTouchBegan = CC_CALLBACK_2(GamePlayScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GamePlayScene::onTouchMoved, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GamePlayScene::onExit()
{
    Layer::onExit();
}

bool GamePlayScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void GamePlayScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}

void GamePlayScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}

//更新当前分数
void GamePlayScene::setScoreCurr(int score)
{
    m_scoreCurr = score;
    m_score->setShowScore(m_scoreCurr);
}
//获取当前分数
int GamePlayScene::getScoreCurr()
{
    return m_scoreCurr;
}

void GamePlayScene::setMoney(int var)
{
    GameMusic::playEffect(SOUND_GOLDSIN);
    userDefault->setIntegerForKey(MONEY_LABEL, var);
}

int  GamePlayScene::getMoney()
{
    return m_Money;
}

void GamePlayScene::updateMoney(float dt)
{
    m_Money = userDefault->getIntegerForKey(MONEY_LABEL);
    __String *pMoney = __String::createWithFormat("%d",m_Money);
    Label* pMoneyLabel = (Label*)getChildByTag(TagMoney);
    pMoneyLabel->setString(pMoney->getCString());

}

//目标分数
int GamePlayScene::needScore(int m_stage)
{
//    int high[30] = {
//        1000,
//        2500,//为了测试修改的数值，正式游戏记得改回去
//        4000,
//        5000,
//        6500,
//        8500,
//        10000,
//        12500,
//        14500,
//        16500,
//        18000,
//        20000,
//        22000,
//        24500,
//        25500,
//        28000,
//        30000,
//        32000,
//        34000,
//        36500,
//        39000,
//        40000,
//        41500,
//        44500,
//        47000,
//        49000,
//        52000,
//        55500,
//        59000,
//        62000,};
//    if (m_stage <= 30) {
//        return high[m_stage-1];
//    }else if (m_stage > 50){
//        return high[29]+20*3500 +4*500+(m_stage-50)*4000+((m_stage-50) /5)*500;
//    }
//    else{
//        return high[29]+(m_stage - 30)*3500 +((m_stage-30) /5)*500;
//    }
    int high[30] = {
        1000,
        2500,
        4000,
        5000,
        6500,
        8500,
        10000,
        12500,
        14500,
        16500,
        18000,
        20000,
        22000,
        24500,
        25500,
        28000,
        30000,
        32000,
        34000,
        36500,//20
        39000,
        40000,
        41500,
        43500,
        46000,
        47500,
        50000,
        51500,
        53500,
        56500,};
    if (m_stage <= 30) {
        return high[m_stage-1];
    }else if (m_stage > 70 && m_stage <=110){
        return high[29]+40*2000 +8*500+(m_stage-70)*2500+((m_stage-70) /5)*500;
    }else if (m_stage > 110) {
        return high[29]+40*2000 +8*500+40*2500 +8*500+(m_stage-110)*3500+((m_stage-110) /5)*500;
    }else{
        return high[29]+(m_stage - 30)*2000 +((m_stage-30) /5)*500;
    }
}
//目标分数闪烁效果
void GamePlayScene::needScoreBlink()
{
    m_score->needScoreShake();
}

//更新最高分数
void GamePlayScene::genScoreHigh()
{
    int high = userDefault->getIntegerForKey(SCORE_HIGH);
    if (m_scoreCurr > high) {
        userDefault->setIntegerForKey(SCORE_HIGH, m_scoreCurr);
        ZYTools::updateGcScore(m_scoreCurr, GC_STORE);
    }
}

void GamePlayScene::overHander()
{
    DelayTime* action = DelayTime::create(1);
    
    if (m_stage > 4) {
        ZYTools::rateWithTip();
    }
    
    if (m_isWin == 2) {
        ZYTools::uMengFinishLevel(m_stage);
        m_stage ++;
        this->runAction(Sequence::create(action,CallFunc::create(CC_CALLBACK_0(GamePlayScene::_labelOne, this)),NULL));
        genScoreHigh();
    }else if (m_isWin == 3){
        ZYTools::uMengFailLevel(m_stage);
        genScoreHigh();
        this->runAction(Sequence::create(action,CallFunc::create(CC_CALLBACK_0(GamePlayScene::_lostGame, this)),NULL));
    }else{
        log("Error::不可能出现的状况");
    }
}

void GamePlayScene::replayHander()
{
    //话费金币重玩本关
    m_data->playAgain();
    _labelOne();
}

void GamePlayScene::_lostGame()
{
    //game over picture do action
    GameOver *over = GameOver::create();
    addChild(over,zorderLayer);
}

void GamePlayScene::saveExit()
{
    //回主菜单
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, GameStartScene::createScene()));
}

void GamePlayScene::lock()
{
    log("界面按钮加锁");
    Menu* menu = (Menu*)getChildByTag(TagMenu);
    menu->setEnabled(false);
}

void GamePlayScene::unlock()
{
    log("界面按钮解锁");
    Menu* menu = (Menu*)getChildByTag(TagMenu);
    menu->setEnabled(true);
}

void GamePlayScene::saveData()
{
    if (!m_CanSave) {
        return;
    }
    userDefault->setBoolForKey(CONTI_BUTTON,true);
    userDefault->setIntegerForKey(STAGE_CURRENT, m_stage);
    userDefault->setIntegerForKey(SCORE_CURRENT, m_scoreCurr);
    m_data->saveData();
}

void GamePlayScene::loadData()
{
    
}

void GamePlayScene::_removeSelf(Node *pNode)
{
    pNode->removeFromParentAndCleanup(true);
}

void GamePlayScene::_newplay()
{
    m_stage = 1;
    userDefault->setBoolForKey(CONTI_BUTTON, false);
    userDefault->setIntegerForKey(SCORE_CURRENT, 0);
    userDefault->setIntegerForKey(SCORE_LAST, 0);
    userDefault->setIntegerForKey(STAGE_CURRENT, m_stage);
    _labelOne();
}

void GamePlayScene::_resplay()
{
    m_stage = userDefault->getIntegerForKey(STAGE_CURRENT);
    isRes = true;
    _labelOne();
}

void GamePlayScene::_pauseHander(Ref *pSender)
{
    GameMusic::playEffect(SOUND_BUTTONCLIK);
    Director::getInstance()->pushScene(TransitionMoveInT::create(0.5, GamePause::createScene()));
}

void GamePlayScene::_moneyHandle(cocos2d::Ref *pSender)
{
    //购买金币
    GameMusic::playEffect(SOUND_BUTTONCLIK);
    ZYTools::buyProductIdentifier();
}

void GamePlayScene::_magicHandle(cocos2d::Ref *pSender)
{
    if (isSelect) {
        stopMaigcAction();
    }else{
        if (getMoney() >= 3){
            MenuItem*item = (MenuItem*)pSender;
            auto action1 = ScaleTo::create(0.3, 1.2);
            auto action2 = ScaleTo::create(0.3, 1.0);
            auto action3 = Sequence::create(action1,action2, NULL);
            auto action = RepeatForever::create(action3);
            item->runAction(action);
            isSelect = true;
            
            GameProps *props = GameProps::create();
            addChild(props,10);
        }else{
            MessageBox("您的金币不足3金币，无法使用道具，点击屏幕下方观看视频领取免费金币，数量有限的哦～", "金币不足！");
        }
    }
}

void GamePlayScene::stopMaigcAction()
{
    isSelect = false;
    MenuItem*item = (MenuItem*)getChildByTag(TagMenu)->getChildByTag(102);
    item->stopAllActions();
}


void GamePlayScene::isHasVideo(bool isHas)
{
    Node *item = getChildByTag(TagMenu)->getChildByTag(101);
    if (isHas) {
        item->setVisible(true);
    }else{
        item->setVisible(false);
    }
}

void GamePlayScene::_videoHandler(cocos2d::Ref *pSender)
{
    ZYTools::showVideo();
}

void GamePlayScene::_labelOne()
{
    m_score->setTarget(m_stage);
    m_score->setNeedScore(needScore(m_stage));
    
    std::string string = GameBase::getCString("label1");
    __String *txt = __String::createWithFormat(string.c_str(), m_stage);
    Label *label1 = Label::createWithTTF(txt->getCString(), BMfontName, 40);
    label1->setAnchorPoint(ancpHalf);
    label1->setPosition(apccp(1, 0.6));
    addChild(label1,zorderPic,TagLabel1);
    
    MoveTo* action_ = MoveTo::create(0.5, apccp(0.5, 0.6));
    auto action = EaseSineOut::create(action_);
    CallFunc* action1 = CallFunc::create(CC_CALLBACK_0(GamePlayScene::_labelTwo, this));
    Sequence* action2 = Sequence::create(action,action1,NULL);
    label1->runAction(action2);
    
    //umeng
    ZYTools::uMengStartLevel(m_stage);
}

void GamePlayScene::_labelTwo()
{
    m_isWin = 1;
    std::string string = GameBase::getCString("label2");
    __String *txt = __String::createWithFormat(string.c_str(), needScore(m_stage));
    Label *label2 = Label::createWithTTF(txt->getCString(), BMfontName, 40);
    label2->setAnchorPoint(ancpHalf);
    label2->setPosition(apccp(1, 0.4));
    addChild(label2,zorderPic,TagLabel2);
    
    MoveTo* action_ = MoveTo::create(0.5, apccp(0.5, 0.4));
    auto action = EaseSineOut::create(action_);
    CallFunc* action1 = CallFunc::create(CC_CALLBACK_0(GamePlayScene::_labelThr, this));
    Sequence* action2 = Sequence::create(action,action1,NULL);
    label2->runAction(action2);
}

void GamePlayScene::_labelThr()
{
    Label* label1 = (Label*)getChildByTag(TagLabel1);
    Label* label2 = (Label*)getChildByTag(TagLabel2);
    DelayTime* action = DelayTime::create(1);
    DelayTime* action_ = DelayTime::create(1);
    MoveBy*action1 = MoveBy::create(0.4, Vec2(-winSize.width, 0));
    MoveBy*action2 = MoveBy::create(0.4, Vec2(-winSize.width, 0));
    CallFunc* action3 = CallFunc::create(CC_CALLBACK_0(GamePlayScene::_labelFou, this));
    label1->runAction(Sequence::create(action,action1,NULL));
    label2->runAction(Sequence::create(action_,action2,action3,NULL));
}

void GamePlayScene::_labelFou()
{
    removeChildByTag(TagLabel1);
    removeChildByTag(TagLabel2);
    if (isRes) {
        m_data->resGame();
    }else{
        m_data->newGame();
    }
    isRes = false;
    m_CanSave = true;
}











