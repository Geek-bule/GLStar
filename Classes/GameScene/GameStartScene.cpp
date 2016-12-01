#include "GameStartScene.h"				//◊‘…ÌÕ∑Œƒº˛
#include "GamePlayScene.h"
#include "GameBse.h"
#include "GameTip.h"
#include "GameLogin.h"
#include "ZYTools.h"

cocos2d::Scene* GameStartScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameStartScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GameStartScene::init()
{
	do 
	{
		CC_BREAK_IF(Layer::init()==false);
        
        //data initialze
        if (!userDefault->getBoolForKey("exsit")) {
            userDefault->setBoolForKey("exsit", true);
            userDefault->setBoolForKey(CONTI_BUTTON, false);
            userDefault->setIntegerForKey(MAGIC_STAR_NUM, 10);
            userDefault->setIntegerForKey(REARRANGE_NUM, 10);
            userDefault->setIntegerForKey(STAGE_CURRENT, 1);
            userDefault->setIntegerForKey(SCORE_HIGH, 0);
            userDefault->setIntegerForKey(SCORE_CURRENT, 0);
            userDefault->setIntegerForKey(SCORE_LAST, 0);
            userDefault->setBoolForKey(MUSIC_ONOFF, true);
            userDefault->setIntegerForKey(RANK_PERCENT, 0);
            userDefault->setBoolForKey(DOUBLE_SWITCH, false);
        }
        if (!userDefault->getBoolForKey("exsit2")) {
            userDefault->setBoolForKey("exsit2", true);
            userDefault->setIntegerForKey(MONEY_LABEL,20+userDefault->getIntegerForKey(MONEY_LABEL));
            userDefault->setBoolForKey(FRIST_SHARE, false);
            userDefault->setIntegerForKey(LOGIC_TIMES, 1);
        }
        
        
        //bg
        Sprite *pBg = Sprite::create("res/backgroud.png");
        pBg->setPosition(apccp(0.5, 0.5));
        addChild(pBg);

		//title
        Sprite *pTitle = Sprite::create("res/title.png");
        pTitle->setPosition(apccp(0.5, 0.67));
        addChild(pTitle,zorderPic);
        
        //high score
        std::string string2 = GameBase::getCString("hscore");
        Label *pHigh = Label::createWithTTF(string2, BMfontName, 40);
        pHigh->setPosition(apccp(0.18, 0.95));
        addChild(pHigh,zorderPic);
        
        LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();
        if (currentLanguageType != LanguageType::CHINESE)
        {
            pHigh->setScale(0.7);
        }
        
        Sprite *pScoreBg = Sprite::create("res/nowscore.png");
        pScoreBg->setPosition(apccp(0.55, 0.95));
        addChild(pScoreBg);
        
        int hScore = userDefault->getIntegerForKey(SCORE_HIGH);
        __String *ScoreStr = __String::createWithFormat("%d",hScore);
        Label *pScore = Label::createWithTTF(ScoreStr->getCString(), BMfontName, 40);
        pScore->setPosition(apccp(0.55, 0.95));
        addChild(pScore,zorderPic);
        
        //menu
        Menu *menu = Menu::create();
        menu->setPosition(0,0);
        addChild(menu,zorderMenu);
        
		//new game
        std::string sStartName = GameBase::getPicName("res/menu_start");
        MenuItemImage *pStart = MenuItemImage::create(sStartName, sStartName, CC_CALLBACK_1(GameStartScene::_startHandle, this));
        pStart->setPosition(apccp(0.5, 0.42));
        menu->addChild(pStart);
        
        //continue game
        std::string sResumeName = GameBase::getPicName("res/menu_resume");
        std::string sResumeName_ = GameBase::getPicName("res/menu_resume_disabled");
        MenuItemImage *pResume = MenuItemImage::create(sResumeName, sResumeName, sResumeName_, CC_CALLBACK_1(GameStartScene::_contiHandle, this));
        pResume->setPosition(apccp(0.5, 0.31));
        menu->addChild(pResume);
        
        if (!userDefault->getBoolForKey(CONTI_BUTTON)) {
            pResume->setEnabled(false);
        }
        
        //game rank
        std::string sRankName = GameBase::getPicName("res/menu_ranking");
        MenuItemImage *pRank = MenuItemImage::create(sRankName, sRankName, CC_CALLBACK_1(GameStartScene::_rankHandle, this));
        pRank->setPosition(apccp(0.5, 0.2));
        menu->addChild(pRank);
        
        
        MenuItem* pMoneyBut = MenuItemImage::create("res/money_plus.png", "res/money_plus.png", "res/money_plus.png", CC_CALLBACK_1(GameStartScene::_moneyHandle, this));
        pMoneyBut->setAnchorPoint(ancpHalf);
        menu->addChild(pMoneyBut);
        pMoneyBut->setPosition(Vec2(winSize.width*0.93, winSize.height*0.9));
        
        m_Money = userDefault->getIntegerForKey(MONEY_LABEL);
        __String *pMoneyStr = __String::createWithFormat("%d",m_Money);
        Label *pMoneyLabel = Label::createWithTTF(pMoneyStr->getCString(), BMfontName, 40);
        pMoneyLabel->setPosition(winSize.width*0.8, winSize.height*0.9);
        addChild(pMoneyLabel,zorderTip,TagMoney);
        
        
        LayerColor *layer = LayerColor::create(Color4B(0, 0, 0, 150), winSize.width, 35);
        layer->setPosition(0, winSize.height*0.1-17);
        addChild(layer,1);
        
        pListText = Label::createWithSystemFont("提示:一次连消大于12颗星星会有2个金币的奖励哦～", "", 25);
        pListText->setPosition(Vec2(winSize.width/2, winSize.height*0.1));
        addChild(pListText,2);
        
        
        schedule(schedule_selector(GameStartScene::updateMoney), 0.03);
        
		return true;
	} while (false);
	return false;
}

void GameStartScene::_paritcle()
{
    Sprite *particle1 = Sprite::create("res/light_1.png");
    particle1->setPosition(apccp(0.2, 0.55));
    addChild(particle1);
    
    auto action1 = DelayTime::create(0.2);
    auto action2 = FadeIn::create(1.3);
    auto action3 = FadeOut::create(1.3);
    auto action4 = Sequence::create(action1,action2,action3, NULL);
    particle1->runAction(RepeatForever::create(action4));
    
    Sprite *particle2 = Sprite::create("res/light_2.png");
    particle2->setPosition(apccp(0.15, 0.75));
    addChild(particle2);
    
    auto action11 = DelayTime::create(0.3);
    auto action21 = FadeOut::create(1.2);
    auto action31 = FadeIn::create(1.2);
    auto action41 = Sequence::create(action11,action21,action31, NULL);
    particle2->runAction(RepeatForever::create(action41));
    
    Sprite *particle3 = Sprite::create("res/light_3.png");
    particle3->setPosition(apccp(0.55, 0.45));
    addChild(particle3);
    
    auto action12 = DelayTime::create(0.4);
    auto action22 = FadeOut::create(1.5);
    auto action32 = FadeIn::create(1.5);
    auto action42 = Sequence::create(action12,action22,action32, NULL);
    particle3->runAction(RepeatForever::create(action42));
    
    Sprite *particle4 = Sprite::create("res/light_4.png");
    particle4->setPosition(apccp(0.85, 0.65));
    addChild(particle4);
    auto action13 = DelayTime::create(0.5);
    auto action23 = FadeOut::create(1.4);
    auto action33 = FadeIn::create(1.4);
    auto action43 = Sequence::create(action13,action23,action33, NULL);
    particle4->runAction(RepeatForever::create(action43));
    
    Sprite *particle5 = Sprite::create("res/light_5.png");
    particle5->setPosition(apccp(0.5, 0.8));
    addChild(particle5);
    
    auto action14 = DelayTime::create(0.2);
    auto action24 = FadeOut::create(1.2);
    auto action34 = FadeIn::create(1.2);
    auto action44 = Sequence::create(action14,action24,action34, NULL);
    particle5->runAction(RepeatForever::create(action44));
}

void GameStartScene::updateMoney(float dt)
{
    m_Money = userDefault->getIntegerForKey(MONEY_LABEL);
    __String *pMoney = __String::createWithFormat("%d",m_Money);
    Label* pMoneyLabel = (Label*)getChildByTag(TagMoney);
    pMoneyLabel->setString(pMoney->getCString());
    
    
    CCSize visibleSize=CCDirector::sharedDirector()->getVisibleSize();
    
    int newX=pListText->getPositionX()-4;
    if (newX<=-pListText->getContentSize().width/2)//如果滚动到这个位置，重置
    {
        newX=visibleSize.width+pListText->getContentSize().width/2;
    }
    pListText->setPositionX(newX);
}

void GameStartScene::_startHandle(cocos2d::Ref *pSender)
{
    //new game button blink
    GameMusic::playEffect(SOUND_STARTGAME);
    MenuItem* item = (MenuItem*)pSender;
    item->runAction(Blink::create(0.5, 7));
    Menu *menu = (Menu*)item->getParent();
    menu->setEnabled(false);
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, GamePlayScene::newScene()));

}

void GameStartScene::_contiHandle(cocos2d::Ref *pSender)
{
    //resume game button blink
    GameMusic::playEffect(SOUND_STARTGAME);
    MenuItem* item = (MenuItem*)pSender;
    item->runAction(Blink::create(0.5, 7));
    Menu *menu = (Menu*)item->getParent();
    menu->setEnabled(false);
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, GamePlayScene::resScene()));
}

void GameStartScene::_rankHandle(cocos2d::Ref *pSender)
{
    //open game center
//    toolsOS::UmengUpdateNum("center2", "主菜单的排行榜");
    GameMusic::playEffect(SOUND_BUTTONCLIK);
    ZYTools::OpenGameCenter();
}

void GameStartScene::_moneyHandle(cocos2d::Ref *pSender)
{
    //购买金币
    GameMusic::playEffect(SOUND_BUTTONCLIK);
    ZYTools::buyProductIdentifier();
}

void GameStartScene::_logicShow()
{
    int  numoftimes = userDefault->getIntegerForKey(LOGIC_TIMES);
    double lasttime = userDefault->getDoubleForKey(LOGIC_DATE);
    struct timeval now;
    gettimeofday(&now, NULL);
    double nowtime =  now.tv_sec;
    if (nowtime - lasttime >  24*60*60) {
        if (nowtime - lasttime < 2*24*60*60) {
            numoftimes++;
            if (numoftimes > 7) {
                numoftimes = 7;
            }
        }
        if (nowtime - lasttime > 2*24*60*60) {
            numoftimes = 1;
            
        }
        struct tm * tm;
        tm = localtime(&now.tv_sec);
        int hour = tm->tm_hour;
        nowtime -= hour*60*60;
        
        GameLogin* log = GameLogin::create();
        addChild(log,zOrderlogic);
        log->setDate(nowtime, numoftimes);
    }
}


void GameStartScene::onEnter()
{
    Layer::onEnter();
    
}

void GameStartScene::onEnterTransitionDidFinish()
{
    //same particle
    _paritcle();
    GameMusic::playEffect(SOUND_GAMELOGO);
    _logicShow();
}

