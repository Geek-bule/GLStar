//
//  GamePause.cpp
//  泡你妹改
//
//  Created by justin yang on 13-9-30.
//
//

#include "GamePause.h"
#include "GamePlayScene.h"
#include "GameBse.h"
#include "SimpleAudioEngine.h"

Scene* GamePause::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GamePause::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


bool GamePause::init()
{
    if(!Layer::create()){
        return false;
    }
    LayerColor *layer = LayerColor::create(Color4B(0, 0, 0, 100));
    addChild(layer);

    Sprite* back = GameBase::createSprite("res/up_bg.png");
    back->setAnchorPoint(Vec2(0.5, 1));
    back->setPosition(apccp(0.5, 1));
    this->addChild(back,0);
    
    Menu *menu = Menu::create();
    addChild(menu,1,TagMenu);
    menu->setAnchorPoint(ancpZero);
    menu->setPosition(Vec2(0, 0));
    
    MenuItem* pCountinue = MenuItemImage::create("res/continue.png", "res/continue.png", "res/continue.png", CC_CALLBACK_1(GamePause::_contiHander, this));
    pCountinue->setAnchorPoint(ancpHalf);
    menu->addChild(pCountinue);
    pCountinue->setPosition(Vec2(winSize.width*0.2, winSize.height*0.2));
    
    MenuItem* pMenuBut = MenuItemImage::create("res/option_resume.png", "res/option_resume.png", "res/option_resume.png", CC_CALLBACK_1(GamePause::_exitHaner, this));
    pMenuBut->setAnchorPoint(ancpHalf);
    menu->addChild(pMenuBut);
    pMenuBut->setPosition(Vec2(winSize.width*0.4, winSize.height*0.2));
    
    
    MenuItemImage* image1 = MenuItemImage::create("res/tap_double.png", "res/tap_single.png");
    MenuItemImage* image2 = MenuItemImage::create("res/tap_single.png", "res/tap_double.png");
    MenuItemToggle* touchSel = MenuItemToggle::createWithCallback(CC_CALLBACK_1(GamePause::_centerHandle,this), image1,image2,NULL);
    if (userDefault->getBoolForKey(DOUBLE_SWITCH)) {
        touchSel->setSelectedIndex(0);
    }else{
        touchSel->setSelectedIndex(1);
    }
    touchSel->setPosition(Vec2(winSize.width*0.6, winSize.height*0.2));
    menu->addChild(touchSel,1,TagSelect);
    
    MenuItemImage* image3 = MenuItemImage::create("res/no_music.png", "res/music.png");
    MenuItemImage* image4 = MenuItemImage::create("res/music.png", "res/no_music.png");
    MenuItemToggle* pMusic = MenuItemToggle::createWithCallback(CC_CALLBACK_1(GamePause::_musicHandle,this), image3,image4,NULL);
    if (userDefault->getBoolForKey(MUSIC_ONOFF)) {
        pMusic->setSelectedIndex(1);
    }else{
        pMusic->setSelectedIndex(0);
    }
    pMusic->setPosition(Vec2(winSize.width*0.8, winSize.height*0.2));
    menu->addChild(pMusic,1,TagMusic);
    
    LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();
    std::string string = "玩法说明\n1.选中并消除选中的方块\n2.消除相连的方块越多，分数越高\n3.剩余方块少于10，有额外奖励\n4.12连以上有额外2金币奖励";
    if (currentLanguageType != LanguageType::CHINESE) {
        string = "BASIC RULES:\n\n1.SELECT AND CLEAR CONNECTED BLOCKS\n\n2.MORE CONNECTED,HIGHER SCORE\n\n3.GET BONUS WHEN LESS THAN 10 REWARDED";
    }
    Label *show = Label::createWithTTF(string, BMfontName, 40);
    show->setPosition(apccp(0.5, 0.77));
    addChild(show);
    
    int high = userDefault->getIntegerForKey(SCORE_HIGH);
    std::string string1 = GameBase::getCString("lasthigh");
    __String* tip = __String::createWithFormat(string1.c_str(),high);
    Label *highScore = Label::createWithTTF(tip->getCString(), BMfontName, 40);
    highScore->setAnchorPoint(Vec2(0,0.5));
    highScore->setPosition(apccp(0.1, 0.52));
    addChild(highScore,1,TagLabel2);
    
    MenuItem* pShare = MenuItemImage::create("res/com_weixin.png", "res/com_weixin.png", "res/share.png", CC_CALLBACK_1(GamePause::_shareHandle, this));
    pShare->setAnchorPoint(ancpHalf);
    menu->addChild(pShare);
    pShare->setPosition(Vec2(winSize.width*0.8, winSize.height*0.52));
    
    int nRank = userDefault->getIntegerForKey(RANK_PERCENT);
    std::string string2 = GameBase::getCString("worldrank");
    __String* ranktip = __String::createWithFormat(string2.c_str(),nRank);
    Label *rank = Label::createWithTTF(ranktip->getCString(), BMfontName, 40);
    rank->setAnchorPoint(Vec2(0,0.5));
    rank->setPosition(apccp(0.1, 0.4));
    addChild(rank,1,TagLabel1);
    
    MenuItem* pGameCenter = MenuItemImage::create("res/ranking.png", "res/ranking.png", "res/ranking.png", CC_CALLBACK_1(GamePause::_rankHandle, this));
    pGameCenter->setAnchorPoint(ancpHalf);
    menu->addChild(pGameCenter);
    pGameCenter->setPosition(Vec2(winSize.width*0.8, winSize.height*0.4));
    
    
    std::string string3 = GameBase::getCString("logintip");
    Label *pTip = Label::createWithTTF(string3, BMfontName, 25);
    pTip->setPosition(apccp(0.5, 0.35));
    addChild(pTip);
    
    std::string string4 = GameBase::getCString("sharetip");
    Label *pTip2 = Label::createWithTTF(string4, BMfontName, 25);
    pTip2->setPosition(apccp(0.5, 0.47));
    addChild(pTip2);
    
//    MenuItem* pFeedback = MenuItemImage::create("res/feedback.png", "res/feedback.png", "res/feedback.png", CC_CALLBACK_1(GamePause::_feedHandle, this));
//    pFeedback->setAnchorPoint(ancpHalf);
//    menu->addChild(pFeedback);
//    pFeedback->setPosition(Vec2(winSize.width*0.8, winSize.height*0.6));
    
//    std::string string4 = GameBase::getCString("title");
//    Label *pTitle = Label::createWithTTF(string4, BMfontName, 45);
//    pTitle->setPosition(apccp(0.5, 0.95));
//    addChild(pTitle);
    
    if (currentLanguageType != LanguageType::CHINESE) {
        show->setScale(0.6);
        rank->setScale(0.8);
        highScore->setScale(0.8);
        pTip->setScale(0.65);
    }
    
    
    schedule(schedule_selector(GamePause::_updateLabel), 0.1);
    
    return true;
}

GamePause::~GamePause()
{
    
}

void GamePause::_contiHander(Ref *pSender)
{
    GameMusic::playEffect(SOUND_BUTTONCLIK);
    Director::getInstance()->popScene();
}

void GamePause::_exitHaner(Ref *pSender)
{
    GameMusic::playEffect(SOUND_BUTTONCLIK);
    GamePlayScene::getInstance()->saveExit();
}

void GamePause::_musicHandle(cocos2d::Ref *pSender)
{
    GameMusic::playEffect(SOUND_BUTTONCLIK);
    bool isMusic = userDefault->getBoolForKey(MUSIC_ONOFF);
    isMusic = !isMusic;
    userDefault->setBoolForKey(MUSIC_ONOFF, isMusic);
}

void GamePause::_rankHandle(cocos2d::Ref *pSender)
{
    ZYTools::reviewPort();
    GameMusic::playEffect(SOUND_BUTTONCLIK);
    ZYTools::OpenGameCenter();
}

void GamePause::_shareHandle(cocos2d::Ref *pSender)
{
    GameMusic::playEffect(SOUND_BUTTONCLIK);
//    toolsOS::UmengUpdateNum("share", "暂停的分享");
    LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();
    std::string string = __String::createWithFormat("我的世界排名：%d,敢来比比吗？",userDefault->getIntegerForKey(RANK_PERCENT))->getCString();
    if (currentLanguageType != LanguageType::CHINESE) {
        string = __String::createWithFormat("My world ranking：%d,show me yours.",userDefault->getIntegerForKey(RANK_PERCENT))->getCString();
    }
    if (!userDefault->getBoolForKey(FRIST_SHARE)){
        string = "我发现了一款不错的游戏，消灭星星：世纪版，和我一起来玩吧";
        if (currentLanguageType != LanguageType::CHINESE) {
            string = __String::createWithFormat("I found a good game, come and play with me")->getCString();
        }
    }
    ZYTools::shareWith(string.c_str(), 10);
}

void GamePause::_centerHandle(cocos2d::Ref *pSender)
{
    GameMusic::playEffect(SOUND_BUTTONCLIK);
    userDefault->setBoolForKey(DOUBLE_SWITCH, !userDefault->getBoolForKey(DOUBLE_SWITCH));
}

void GamePause::_feedHandle(cocos2d::Ref *pSender)
{
//    toolsOS::UmengUpdateNum("feed", "反馈按钮");
//    toolsOS::uMengFeedBack();
}

void GamePause::_updateLabel(float dt)
{
    int nRank = userDefault->getIntegerForKey(RANK_PERCENT);
    std::string string2 = GameBase::getCString("worldrank");
    __String* ranktip = __String::createWithFormat(string2.c_str(),nRank);
    Label *rank = (Label*)getChildByTag(TagLabel1);
    rank->setString(ranktip->getCString());
    
    int high = userDefault->getIntegerForKey(SCORE_HIGH);
    std::string string1 = GameBase::getCString("lasthigh");
    __String* tip = __String::createWithFormat(string1.c_str(),high);
    Label *highScore = (Label*)getChildByTag(TagLabel2);
    highScore->setString(tip->getCString());
}

void GamePause::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchEnded = CC_CALLBACK_2(GamePause::onTouchEnded, this);
    listener->onTouchBegan = CC_CALLBACK_2(GamePause::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GamePause::onTouchMoved, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GamePause::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    //每次打开暂停页面都进行一次数据存储，以免读写数据造成的程序延迟
    GamePlayScene::getInstance()->saveData();
}

void GamePause::onExit()
{
    Layer::onExit();
}

bool GamePause::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void GamePause::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}

void GamePause::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}


















