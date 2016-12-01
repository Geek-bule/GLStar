//
//  GameData.cpp
//  泡你妹改
//
//  Created by justin yang on 13-10-8.
//
//

#include "GameData.h"
#include "GamePlayScene.h"
#include "GameBse.h"

typedef std::vector<GamePuyoPos> gamePuyoPosArray;
static gamePuyoPosArray m_need_RemoveArray; //|
static gamePuyoPosArray m_to_RemoveArray;   //| 为了点一个不变色 多余一个才变色
static gamePuyoPosArray m_to_Rearrange;

#define kRemovingAnimateTime		0.13
#define kFallingAnimateTime			0.05

static GameData *share = NULL;

GameData *GameData::getInstance()
{
    return share;
}

bool GameData::init()
{
    if (!Layer::create()) {
        return false;
    }
    share = this;
    fallArray = Array::create();
    fallArray->retain();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/block_old.plist", "res/block_old.png");
    
    aParent = SpriteBatchNode::create("res/block_old.png");
    addChild(aParent, 0);
    
    m_node = Node::create();
    addChild(m_node);
    
    return true;
}

GameData::~GameData()
{
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("res/block_old.plist");
    fallArray->release();
    share = NULL;
}

//新游戏
void GameData::newGame()
{
    m_overLeft = 0;
//    aParent->setPosition(Vec2(winSize.width, 0));
    _gameBeginPart(TYPE_NEWGAME);
}

// 继续游戏
void GameData::resGame()
{
    m_overLeft = 0;
//    aParent->setPosition(Vec2(winSize.width, 0));
    //读取并显示当前分
    int nScore = userDefault->getIntegerForKey(SCORE_CURRENT);
    GamePlayScene::getInstance()->setScoreCurr(nScore);
    _gameBeginPart(TYPE_RESUME);
}

void GameData::lockTouch()
{
    log("棋盘加锁");
    GamePlayScene::getInstance()->lock();
    isTouchEnable = false;
}

void GameData::unlockTouch()
{
    if (!_winOrlost()) {
        //提前进行计算数据输赢来判断接下来的步骤
        log("本关结束");
        _endLabel();
    }else{
        log("棋盘解锁");
        GamePlayScene::getInstance()->unlock();
        isTouchEnable = true;
    }
}
//通过value获取星星sprite
Sprite *GameData::getSprite(int value, bool isSel)
{
    std::string name;
    switch (value) {
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
            log("Error::value%d不是元素id",value);
            break;
    }
    Sprite* sprite = Sprite::createWithSpriteFrameName(name.c_str());
    aParent->addChild(sprite);
    
    if (isSel) {
        Sprite *sprite_sel = Sprite::createWithSpriteFrameName("block_select-hd.png");
        sprite_sel->setPosition(sprite->getContentSize().width/2,sprite->getContentSize().height/2);
        sprite->addChild(sprite_sel);
    }
    
    return sprite;
}

bool GameData::isStarExsit(int potX, int potY)
{
    if (puyo_[potX][potY].typePuyo !=kKindBlankIndex) {
        return true;
    }
    return false;
}

//添加
void GameData::_addNewPuyo(int idx, int jdy, int kind, bool isLoad)
{
    int pkind = kind;
    GamePuyo* puyo = &puyo_[idx][jdy];
    //不为空创建节点
    if (pkind !=kKindBlankIndex) {
        Sprite* sprite = getSprite(pkind);
        sprite->setAnchorPoint(ancpHalf);
        sprite->setPosition(Vec2(SETPX(idx), SETPY(jdy)+winSize.height));
        puyo->m_sprite = sprite;
    }
    puyo->typePuyo = pkind;
}

void GameData::removePuyo(Node *pSprite)
{
    aParent->removeChild(pSprite, true);
}

void GameData::removePuyoWithParticle(Point point)
{
    int idx = point.x;
    int jdy = point.y;
    GamePuyo* puyo = &puyo_[idx][jdy];
    if (isBlank_puyo(puyo)) {
        log("Waring::空点不能进行删除");
    }
    //星星特效
    _partCreate(Vec2(SETPX(idx), SETPY(jdy)), puyo->typePuyo);
    //初始化节点数据
    aParent->removeChild(puyo->m_sprite, true);
    Puyo_Res(puyo);
    GameMusic::playEffect(SOUND_STARCLEAR);
}

void GameData::removePuyoWithParticleSpecial(Point point)
{
    int idx = point.x;
    int jdy = point.y;
    GamePuyo* puyo = &puyo_[idx][jdy];
    if (isBlank_puyo(puyo)) {
        log("Waring::空点不能进行删除");
    }
    //星星特效
    _partCreate(Vec2(SETPX(idx), SETPY(jdy)), puyo->typePuyo);
    //初始化节点数据
    aParent->removeChild(puyo->m_sprite, true);
    GameMusic::playEffect(SOUND_STARCLEAR);
}

//保存数据
void GameData::saveData()
{
    char str[50] = {};
    for (int i = 0; i < 100; i++) {
        sprintf(str, "puyo%d",i);
        GamePuyo puyo =  puyo_[i%10][i/10];
        userDefault->setIntegerForKey(str, puyo.typePuyo);//保存棋盘数据
    }
}

//读取单个数据
int GameData::_loadData(int i, int j)
{
    int puyoType;
    char str[50] = {0};
    sprintf(str, "puyo%d",i+j*10);
    puyoType = userDefault->getIntegerForKey(str);//读取单个棋子
    if (puyoType == 0) {
        log("Error::读取存档出错，进行修正");
        puyoType = rand()%5+1;
    }
    return puyoType;
}

void GameData::removePuyoSwapAnother()
{
//    Sprite* sprite = getSprite(puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].typePuyo,true);
//    sprite->setAnchorPoint(Point(0.5, 0.5));
//    sprite->setPosition(Vec2(pos.m_puyoX, pos.m_puyoY));
//    aParent->removeChild(puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite, true);
//    puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite = sprite;
//    puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite->runAction(Sequence::create(MoveBy::create(0.1, Point(0, 6)), MoveBy::create(0.1, Point(0, -6)),NULL));
}

void GameData::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchEnded = CC_CALLBACK_2(GameData::onTouchEnded, this);
    listener->onTouchBegan = CC_CALLBACK_2(GameData::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameData::onTouchMoved, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameData::onExit()
{
    Layer::onExit();
}

bool GameData::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    Point location = CCDirector::getInstance()->convertToGL(pTouch->getLocationInView());
    location = location+Vec2(-ipadAdaptation, 0);
    if (!isTouchEnable) {
        return true;
    }
    if (location.y > ADSHight && location.y < ADSHight+kCellWidth*kYDirCellNum) {
        int puyo_X = GETPX(location.x+32);
        int puyo_Y = GETPY(location.y+32);
        _touchBegin(puyo_X, puyo_Y);
    }
    return true;
}

void GameData::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}

void GameData::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    
}

void GameData::_tipLabelAnimate()
{
    getLabel->setPosition(Vec2(0.5*winSize.width-ipadAdaptation, 0.8*winSize.height));
    
    if (!m_need_RemoveArray.empty()) {
        getLabel->stopAllActions();
        int size = (int)m_need_RemoveArray.size();
        int score = 5*size*size;
        std::string string;
        string = GameBase::getCString("scoretip");
        __String *txt = __String::createWithFormat(string.c_str(),size,score);
        getLabel->setString(txt->getCString());
        
        auto action2 = ScaleTo::create(0.06, 1.2,0.8);
        auto action3 = ScaleTo::create(0.09, 0.5);
        auto action4 = ScaleTo::create(0.04, 0.6);
        
        Vector<FiniteTimeAction*> pAction;
        pAction.pushBack(action2);
        pAction.pushBack(action3);
        pAction.pushBack(action4);
        
        getLabel->runAction(Sequence::create(pAction));
    }
}

void GameData::_touchBegin(int puyo_X, int puyo_Y)
{
    
    if (userDefault->getBoolForKey(DOUBLE_SWITCH) && puyo_[puyo_X][puyo_Y].isChecked){//点击块被选中时候
            if (m_need_RemoveArray.size() >= 2) {
                log("开始消除加锁");
                lockTouch();
                _removeAnmiate();
            }else{
                _cancalSelect();
            }
    }else{                                  //点击块没被选中的时候
            if (m_need_RemoveArray.empty()){
                _checkTo(puyo_X, puyo_Y);
                _tipLabelAnimate();
            }else{
                _cancalSelect();
                _checkTo(puyo_X, puyo_Y);
                _tipLabelAnimate();
            }
        
    }
    
    if (!userDefault->getBoolForKey(DOUBLE_SWITCH)) {
        if (puyo_[puyo_X][puyo_Y].isChecked) {
            if (m_need_RemoveArray.size() >= 2) {
                log("updata(dt) lock touch");
                lockTouch();
                _removeAnmiate();
            }
            else
            {
                _cancalSelect();
            }
        }
    }
}

void GameData::_partCreate(cocos2d::Point point,int kind)
{
//    return;
    Color4F color;
    switch (kind) {
        case 1://huang
            color = Color4F(100, 0, 252, 255);
            break;
        case 2://lan
            color = Color4F(0, 50, 255, 255);
            break;
        case 3://purple
            
            color = Color4F(1, 0, 0, 255);
            break;
        case 4://green
            color = Color4F(0, 150, 0, 255);
            break;
        case 5://red
            
            color = Color4F(250, 255, 0, 255);
            break;
        default:
            log("Waring::没有符合的特效出现,确定kind值");
            break;
    }
    ParticleSystemQuad *par = ParticleSystemQuad::create("particle/explode.plist");
    par->setStartColor(color);
    par->setPosition(point.x, point.y);
    m_node->addChild(par);
    par->setAutoRemoveOnFinish(true);
}

//游戏开始部分
void GameData::_gameBeginPart(int type)
{
    for (int j = 0; j < kYDirCellNum; j++) {
        for (int i = 0; i < kXDirCellNum; i++) {
            //根据是不是新游戏选择是不是读取数据
            if (type == TYPE_NEWGAME) {
                _addNewPuyo(i, j, rand()%5+1);
            }else{
                if (userDefault->getBoolForKey(STAGE_COMPLE)) {
                    userDefault->setBoolForKey(STAGE_COMPLE, false);
                    _addNewPuyo(i, j, rand()%5+1);
                }else{
                    _addNewPuyo(i, j, _loadData(i,j));
                }
            }
            //星星的出现特效
            if (puyo_[i][j].typePuyo != kKindBlankIndex) {
                Vector<FiniteTimeAction*> pAction;
                float time = 0.1*j+0.02*i;
                auto action1 = DelayTime::create(time);
                auto action2 = MoveTo::create(0.5,Vec2(SETPX(i), SETPY(j)));
                auto action3 = EaseSineInOut::create(action2);
                
                pAction.pushBack(action1);
                pAction.pushBack(action3);
                
                if (i==9&&j==9) {
                    auto action6 = CallFunc::create(CC_CALLBACK_0(GameData::_gameBeginFinsh, this));
                    pAction.pushBack(action6);
                    
                    puyo_[i][j].m_sprite->runAction(Sequence::create(pAction));
                }else{
                    puyo_[i][j].m_sprite->runAction(Sequence::create(pAction));
                }
            }
        }
    }
    _gameBeginFinsh();
}
//星星出现结束后
void GameData::_gameBeginFinsh()
{
    getLabel = Label::createWithTTF("", BMfontName, 40);
    getLabel->setAnchorPoint(ancpHalf);
    getLabel->setPosition(Vec2(-0.5*winSize.width, 0.8*winSize.height));
    m_node->addChild(getLabel);

    log("_gameBeginFinish() unlock touch");
    unlockTouch();
    GamePlayScene::getInstance()->needScoreBlink();
}
//教程的处理
void GameData::_teacherHander(Ref *pSender)
{
}

//点击后统计链接的星星
void GameData::_checkSelect(int i, int j)
{
    if (i<0||i>=kXDirCellNum||j<0||j>=kYDirCellNum||puyo_[i][j].typePuyo == kKindBlankIndex) {
        return;
    }
    m_to_RemoveArray.push_back(pos_Make(SETPX(i), SETPY(j)));
    puyo_[i][j].isChecked = true;
    log("pos (%d ,%d) value %d",i,j,puyo_[i][j].typePuyo);
    if(j+1<kYDirCellNum){
        if (puyo_[i][j+1].typePuyo != kKindBlankIndex) {
            if (puyo_[i][j].typePuyo == puyo_[i][j+1].typePuyo && !puyo_[i][j+1].isChecked) {
                
                _checkSelect(i, j+1);
                
            }
        }
    }
    if(j-1 >= 0){
        if (puyo_[i][j-1].typePuyo != kKindBlankIndex) {
            if (puyo_[i][j].typePuyo == puyo_[i][j-1].typePuyo && !puyo_[i][j-1].isChecked ) {
                
                _checkSelect(i, j-1);
                
            }
        }
    }
    if(i+1 < kXDirCellNum){
        if (puyo_[i+1][j].typePuyo != kKindBlankIndex) {
            if (puyo_[i][j].typePuyo == puyo_[i+1][j].typePuyo && !puyo_[i+1][j].isChecked) {
                
                _checkSelect(i+1, j);
                
            }
        }
    }
    if(i-1 >=0){
        if (puyo_[i-1][j].typePuyo != kKindBlankIndex) {
            if (puyo_[i][j].typePuyo == puyo_[i-1][j].typePuyo && !puyo_[i-1][j].isChecked) {
                
                _checkSelect(i-1, j);
                
            }
        }
    }
}

//将选中的星星改变形状
void GameData::_checkTo(int x, int y)
{
    
    _checkSelect(x, y);
    if (m_to_RemoveArray.size() >= 2) {
        //GameMusic::playEffect(SOUND_STARSELECT);
        for (int i = 0; i < m_to_RemoveArray.size(); i++) {
            GamePuyoPos pos = m_to_RemoveArray[i];
            m_need_RemoveArray.push_back(pos);
            //交换
            Sprite* sprite = getSprite(puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].typePuyo,true);
            sprite->setAnchorPoint(Point(0.5, 0.5));
            sprite->setPosition(Vec2(pos.m_puyoX, pos.m_puyoY));
            aParent->removeChild(puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite, true);
            puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite = sprite;
            auto action1 = MoveBy::create(0.1, Point(0, 6));
            auto action2 = MoveBy::create(0.1, Point(0, -6));
            auto action = Sequence::create(action1, action2,NULL);
            puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite->runAction(action);
            
        }
    }else if(m_to_RemoveArray.size() > 0){
        GamePuyoPos pos = m_to_RemoveArray[0];
        puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].isChecked = false;
    }
    m_to_RemoveArray.clear();
}

//===============================数据部分=================================

//消除之后进行下落的数据处理
void GameData::_dataHander()
{
    //初始化处理数组
    for (int i = 0; i< kXDirCellNum; i++) {
        down_num[i] = 0;
        move_left[i] = 0;
    }
    //根据消除的数组处理下落数组
    for (int i = 0; i< (int)m_need_RemoveArray.size(); i++) {
        GamePuyoPos pos = m_need_RemoveArray[i];
        for (int j = 0; j < kXDirCellNum; j++) {
            if (j == GETPX(pos.m_puyoX)) {
                down_num[j]++;
            }
        }
    }
    _clearSelcet();
    //左移数组部分的处理
    for (int i = 0; i < m_need_RemoveArray.size(); i++) {
        bool isBlankY = true;
        GamePuyoPos pos = m_need_RemoveArray[i];
        for (int j = 0 ; j < kYDirCellNum; j++) {
            if(puyo_[GETPX(pos.m_puyoX)][j].typePuyo != kKindBlankIndex){
                isBlankY = false;
            }
        }
        if (isBlankY) {
            move_left[GETPX(pos.m_puyoX)] = 1;
        }
    }
    
    //根据数组数据处理响应的函数
    for (int i = kXDirCellNum-1 ; i >= 0 ; i --) {
        if (down_num[i] != 0) {
            _fallData(i, down_num[i]);
        }
        if (move_left[i] == 1) {
            _leftDate(i);
        }
    }
    
    m_need_RemoveArray.clear();//...................................
}

//数据下落部分处理
void GameData::_fallData(int x, int n)
{
    int holeCounter = 0;
    
    for (int i = 0; i < kYDirCellNum; i++)
    {
        if (isBlank_puyo(&puyo_[x][i]))
        {
            holeCounter++;
        }
        else if (holeCounter != 0)
        {
            puyo_[x][i - holeCounter] = puyo_[x][i];
            Sprite* sprite = puyo_[x][i -  holeCounter].m_sprite;
            
            fallDown[x][i] = holeCounter;
            fallArray->addObject(sprite);
            Puyo_Res(&puyo_[x][i]);
        }
    }
}

//左移数据处理星星数据
void GameData::_leftDate(int x)
{
    for (int i = x+1; i < kXDirCellNum; i++) {
        for (int j = 0; j < kYDirCellNum; j++) {
            puyo_[i-1][j] = puyo_[i][j];
            Puyo_Res(&puyo_[i][j]);
        }
    }
}

//将之前选中星星清除。并初始化
void GameData::_clearSelcet()
{
    for (int i = 0; i< (int)m_need_RemoveArray.size(); i++)
    {
        GamePuyoPos pos = m_need_RemoveArray[i];
        Puyo_Res(&puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)]);
    }
}
//取消选中状态，星星变形
void GameData::_cancalSelect()
{
    for (int i = 0; i< (int)m_need_RemoveArray.size(); i++)
    {
        GamePuyoPos pos = m_need_RemoveArray[i];
        puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].isChecked = false;
        //交换
        Sprite* sprite = getSprite(puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].typePuyo);
        sprite->setAnchorPoint(Point(0.5, 0.5));
        sprite->setPosition(Point(pos.m_puyoX, pos.m_puyoY));
        aParent->removeChild(puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite, true);
        puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite = sprite;
        
    }
    m_need_RemoveArray.clear();
}

//===============================图形部分=================================

//下落星星的图形处理
void GameData::_fallAnimate()
{
    //将数组中数据重置,为bug而做
    for (int i = 0; i< (int)m_need_RemoveArray.size(); i++)
    {
        GamePuyoPos pos = m_need_RemoveArray[i];
        Puyo_Res(&puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)]);
    }
    //计算下落与横移
    _dataHander();
    
    int noMove = 10;
    for (int i = 0 ; i < kXDirCellNum ; i ++) {
        if (move_left[i] == 1) {
            noMove = i;
            break;
        }
    }
    
    
    int timeMax = 0;
    for (int i = 0 ; i < fallArray->count(); i++) {
        
        Sprite* ob = (Sprite*)fallArray->getObjectAtIndex(i);
        int obx = GETPX(ob->getPositionX());
        int oby = GETPY(ob->getPositionY());
        int fall = fallDown[obx][oby];
        if (fall > waitTimeMax) {
            timeMax = fall;
        }
        if (obx < noMove) {
            Vector<FiniteTimeAction*> pAction;
            auto action1 = EaseSineOut::create(MoveBy::create(0.1, Point(0,18)));
            auto action2 = EaseSineIn::create(MoveBy::create(0.14+0.02*fall, Point(0, -kCellWidth*fall-24)));
            auto action3 = EaseOut::create(MoveBy::create(0.05, Point(0,6)),1);
            pAction.pushBack(action1);
            pAction.pushBack(action2);
            pAction.pushBack(action3);
            ob->runAction(Sequence::create(pAction));
            
        }
        
        fallDown[obx][oby] = 0;
    }
    
    waitTimeMax = 0.14+0.02*timeMax +0.15;
    
    if (fallArray->count() > 0) {
        auto action1 = DelayTime::create(waitTimeMax);
        auto action2 = CallFunc::create([&]{
            GameMusic::playEffect(SOUND_FALLDOWN);
        });
        auto action3 = Sequence::create(action1,action2, NULL);
        runAction(action3);
    }
    fallArray->removeAllObjects();
    _leftAnmiate();
}


//左移星星的图形处理
void GameData::_leftAnmiate()
{
    int leftNum = 10;//如果是10就不用左移了
    for (int i =0; i <kXDirCellNum; i++) {
        if (move_left[i] == 1) {
            move_left[i] = 0;
            leftNum = i;
            break;
        }
    }
    
    for (int j = 0; j < kYDirCellNum; j++) {
        for (int k = leftNum; k < kXDirCellNum; k++) {
            if(puyo_[k][j].typePuyo != kKindBlankIndex){
                Sprite* sprite = puyo_[k][j].m_sprite;
                int leftX = GETPX( sprite->getPositionX())-k;
                if ((0.06*leftX+(k-leftNum)*0.02) > waitTimeMax) {
                    waitTimeMax = 0.06*leftX+(k-leftNum)*0.02+0.03;
                }
                //左移动作
                Vector<FiniteTimeAction*> pAction;
                auto action1 = MoveTo::create(0.06*leftX+(k-leftNum)*0.02, Point(SETPX( k)-5, SETPY( j)));
                auto action2 = MoveBy::create(0.03, Point(5,0));
                pAction.pushBack(action1);
                pAction.pushBack(action2);
                
                puyo_[k][j].m_sprite->runAction(Sequence::create(pAction));
                
            }
        }
    }
    
    auto action3 = DelayTime::create(waitTimeMax);
    auto action4 = CallFunc::create(CC_CALLBACK_0(GameData::unlockTouch, this));
    auto action = Sequence::create(action3,action4, NULL);
    runAction(action);
    
    waitTimeMax = 0;
}

//消除星星的消除动画处理
void GameData::_removeAnmiate()
{
    int nSize = m_need_RemoveArray.size();
    //根据size执行不同的动画
    _checkAward(nSize);
    int scoreCurr = GamePlayScene::getInstance()->getScoreCurr();
    userDefault->setIntegerForKey(SCORE_CURRENT, scoreCurr);
//在消除星星特效时候推出游戏，则游戏数据会混乱，所以通过这个来判断是不是特效时退出的
    //蹦出数字的位置
    GamePuyoPos pPos;
    //消除动作
    for (int k = 0; k < nSize; k++) {
        GamePuyoPos pos = m_need_RemoveArray[k];
        Vector<FiniteTimeAction*> pAction;
        float time;
        if (k > 30) {
            time = 30*kRemovingAnimateTime-30*30*0.001 + (k-30)*0.04;
        }else{
            time = k*kRemovingAnimateTime-k*k*0.001;
        }
        auto action1 = DelayTime::create(time);
        auto action2 = CallFunc::create(CC_CALLBACK_0(GameData::removePuyoWithParticleSpecial, this, Point(GETPX(pos.m_puyoX), GETPY(pos.m_puyoY))));
        auto action3 = CallFunc::create(CC_CALLBACK_0(GameData::_fallAnimate, this));
        auto action4 = CallFunc::create(CC_CALLBACK_0(GameData::_removeWithScore, this, k+1, pos));
        
        pAction.pushBack(action1);
        pAction.pushBack(action4);
        pAction.pushBack(action2);
        if (k == 0){
            pPos = pos;
        }
        if (k == nSize-1) {
            pAction.pushBack(action3);
        }
        
        auto action = Sequence::create(pAction);
        runAction(action);
    }
}
//消除时产生的分数动画
void GameData::_removeWithScore(int pid, GamePuyoPos pos)
{
    int nScore = (pid*2-1)*5;
    __String *cScore =__String::createWithFormat("%d",nScore);
    Label *sc_lable = Label::createWithTTF(cScore->getCString(), BMfontName, 40);
    sc_lable->setPosition(Vec2(pos.m_puyoX, pos.m_puyoY));
    m_node->addChild(sc_lable);
    int sc_n = GETPY((740-pos.m_puyoY));
    auto action6 = CCEaseBackIn::create(MoveTo::create(0.8+0.05*sc_n, Vec2(winSize.width/2, winSize.height-140)));
    auto action7 = ScaleTo::create(0.4+0.05*sc_n, 1.0);
    auto action8 = Spawn::create(action6, action7,NULL);
    sc_lable->runAction(Sequence::create(action8,FadeTo::create(0.01, 0),CallFuncN::create(CC_CALLBACK_1(GameData::_removeHander, this, (pid*2-1)*5)),NULL));
}
//消除分数处理之后的判断
void GameData::_removeHander(cocos2d::Node *pSender, int score)
{
    Label* font = (Label*)pSender;
    m_node->removeChild(font, true);
    //当前分数增加
    int nScore = score+GamePlayScene::getInstance()->getScoreCurr();
    GamePlayScene::getInstance()->setScoreCurr(nScore);
    int nNeedScore = GamePlayScene::getInstance()->needScore(GamePlayScene::getInstance()->m_stage);
    if (GamePlayScene::getInstance()->m_isWin == 1) {
        if (nScore >= nNeedScore) {
            //胜利
            GamePlayScene::getInstance()->m_isWin = 2;
            //胜利特效
            _checkAward(-1);
        }
    }
}

//分数弹出特效
void GameData::_scoreAnimate(int size, GamePuyoPos pos)
{
    //弹出分数特效
    __String *nScore = __String::createWithFormat("%d",size*size*5);
    Label *tip = Label::createWithTTF(nScore->getCString(), BMfontName, 40);
    tip->setPosition(Vec2(pos.m_puyoX, pos.m_puyoY + 70));
    m_node->addChild(tip,3);
    //调整位置
    if(pos.m_puyoX<40)
    {
        tip->setPosition(Vec2(64, pos.m_puyoY+ 70));
    }
    else if(pos.m_puyoX>winSize.width-40)
    {
        tip->setPosition(Vec2(winSize.width-64, pos.m_puyoY+ 70));
    }
    
    tip->setScale(0.1);
    tip->setOpacity(0);
    
    auto action1 = FadeIn::create(0.05);
    auto action2 = MoveBy::create(0.12, Vec2(0, 100));
    auto action2_= EaseSineIn::create(action2);
    auto action3 = ScaleTo::create(0.07, 0.65);
    auto action3_= EaseBackIn::create(action3);
    auto action4 = Spawn::create(action2_,action3, NULL);
    auto action5 = MoveBy::create(1.2, Vec2(0, 40));
    auto action5_= EaseSineInOut::create(action5);
    auto action6 = FadeOut::create(0.4);
    auto action7 = ScaleTo::create(0.4, 0.4);
    auto action8 = Spawn::create(action6,action7, NULL);
    auto action9 = CallFuncN::create(CC_CALLBACK_1(GameData::_removeSelf, this));
    tip->runAction(Sequence::create(action1,action4,action5,action8,action9, NULL));
}

//===============================其他部分=================================

//判断输赢
bool GameData::_winOrlost()
{
    m_overLeft = 0;
    bool isOver = false;
    for (int i = 0; i < kXDirCellNum; i++) {
        for (int j = 0; j < kYDirCellNum; j++) {
            if (puyo_[i][j].typePuyo != kKindBlankIndex) {
                m_overLeft += 1;
            }
            if (i%2 == 0 && j%2 == 0) {
                if (puyo_[i][j].typePuyo != kKindBlankIndex) {
                    if (_overCheck(i, j)) {
                        isOver = true;
                        break;
                    }
                }
            }
            if (i%2 != 0 && j%2 != 0) {
                if (puyo_[i][j].typePuyo != kKindBlankIndex) {
                    if (_overCheck(i, j)) {
                        isOver = true;
                        break;
                    }
                }
            }
        }
    }
    return isOver;
}

bool GameData::_overCheck(int i, int j)
{
    bool isOver = false;
    if(j+1<kYDirCellNum){
        if (puyo_[i][j].typePuyo == puyo_[i][j+1].typePuyo) {
            isOver = true;
        }
    }
    if(j-1 >= 0){
        if (puyo_[i][j].typePuyo == puyo_[i][j-1].typePuyo ) {
            isOver = true;
        }
    }
    if(i+1 < kXDirCellNum){
        if (puyo_[i][j].typePuyo == puyo_[i+1][j].typePuyo) {
            isOver = true;
        }
    }
    if(i-1 >=0){
        if (puyo_[i][j].typePuyo == puyo_[i-1][j].typePuyo) {
            isOver = true;
        }
    }
    return isOver;
}

//结算部分的图形处理
void GameData::_endLabel()
{
    m_scoreAward = 2000;
    m_scoreCount = 0;
    
    std::string string = GameBase::getCString("awardtip");
    __String *txt = __String::createWithFormat(string.c_str(), m_overLeft);
    Label* weixiaochu = Label::createWithTTF(txt->getCString(), BMfontName, 40);
    weixiaochu->setAnchorPoint(ancpHalf);
    weixiaochu->setPosition(Vec2(0.5*winSize.width-ipadAdaptation, 0.4*winSize.height));
    m_node->addChild(weixiaochu,3,12);
    
    float tipScale = 0;
    LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();
    if (currentLanguageType == LanguageType::ENGLISH)
    {
        tipScale = 30;
    }
    if (currentLanguageType == LanguageType::JAPANESE)
    {
        tipScale = 60;
    }
    
    std::string string2 = GameBase::getCString("bouns");
    __String *txt2 = __String::createWithFormat(string2.c_str(), 2000);
    Label* numsc = Label::createWithTTF(txt2->getCString(), BMfontName, 40);
    numsc->setAnchorPoint(ancpHalf);
    numsc->setPosition(Vec2(0.53*winSize.width-ipadAdaptation +tipScale, 0.55*winSize.height));
    m_node->addChild(numsc,3,14);
    
    //将剩余的星星全部加入数组
    for (int j = kYDirCellNum-1; j >=0 ; j--) {
        for (int i = 0; i < kXDirCellNum; i++) {
            if (puyo_[i][j].typePuyo != kKindBlankIndex) {
                m_need_RemoveArray.push_back(pos_Make(SETPX( i),SETPY( j)));
            }
        }
    }
    
    //在结算特效之前，将数组处理并存储
    int nScore = GamePlayScene::getInstance()->getScoreCurr();
    if (m_overLeft >= 10) {
        
    }else{
        nScore += 2000-20*m_overLeft-20*m_overLeft*(m_overLeft-1);
    }
    int stage = GamePlayScene::getInstance()->m_stage;
    int needScore = GamePlayScene::getInstance()->needScore(stage);
    if (nScore >= needScore) {
        //过关
        userDefault->setBoolForKey(CONTI_BUTTON,true);
        userDefault->setIntegerForKey(SCORE_CURRENT, nScore);
        userDefault->setIntegerForKey(STAGE_CURRENT, stage+1);
        userDefault->setIntegerForKey(SCORE_LAST, nScore);//保存上局的分数＊＊＊＊＊＊
        //将下关的随机数取消改为一个判断
        userDefault->setBoolForKey(STAGE_COMPLE, true);
        /*char str[50] = {};
        for (int i = 0; i < 100; i++) {
            sprintf(str, "puyo%d",i);
            userDefault->setIntegerForKey(str, rand()%5+1);
        }*/
    }else{
        //失败
        userDefault->setBoolForKey(CONTI_BUTTON,false);
    }
    GamePlayScene::getInstance()->m_CanSave = false;
    
    //结算动画之前星星闪烁
    if (m_need_RemoveArray.size() == 0){
        _endAnimate();
    }
    for (int i =0; i < m_need_RemoveArray.size(); i++) {
        GamePuyoPos pos = m_need_RemoveArray[i];
        auto action1 = Blink::create(0.5, 5);
        auto action2 = CallFunc::create(CC_CALLBACK_0(GameData::_endAnimate, this));//结算动画
        auto action3 = Sequence::create(action1,action2, NULL);
        if (i == m_need_RemoveArray.size()-1) {
            puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite->runAction(action3);
        }else{
            puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite->runAction(action1);
        }
        
    }
}
//结算部分的动画
void GameData::_endAnimate()
{
    Label *numsc = (Label*)m_node->getChildByTag(14);
    if (m_scoreAward > 0){
        std::string string2 = GameBase::getCString("bouns");
        __String *strnum = __String::createWithFormat(string2.c_str(), m_scoreAward);
        numsc->setString(strnum->getCString());
        numsc->runAction(Blink::create(0.1, 1));
        if (m_need_RemoveArray.size() > 0) {
            //计算奖励分数
            m_scoreAward -= 20+40*m_scoreCount;
            m_scoreCount ++;
            
            GamePuyoPos pos = *m_need_RemoveArray.begin();
            m_need_RemoveArray.erase(m_need_RemoveArray.begin());
            removePuyoWithParticle(Point(GETPX(pos.m_puyoX), GETPY(pos.m_puyoY)));
            auto action1 = DelayTime::create(kRemovingAnimateTime*2);
            auto action2 = CallFunc::create(CC_CALLBACK_0(GameData::_endAnimate, this));
            runAction(Sequence::create(action1,action2, NULL));
        }else{
            //添加分数转移部分
            auto action1 = DelayTime::create(0.2);
            auto action1_ = MoveTo::create(0.4, apccp(0.5, 0.75));
            auto action2 = CallFunc::create(CC_CALLBACK_0(GameData::playAgain, this));
            auto action2_ = CallFunc::create(CC_CALLBACK_0(GameData::_scoreAdd, this, m_scoreAward));
            auto action3_  = Sequence::create(action1,action1_,action2_, NULL);
            numsc->runAction(action3_);
        }
    }else{
        m_scoreAward = 0;
        __String *strnum = __String::createWithFormat("%d",m_scoreAward);
        numsc->setString(strnum->getCString());
        for (int i = 0; i < m_need_RemoveArray.size(); i++) {
            GamePuyoPos pos = m_need_RemoveArray[i];
            removePuyoWithParticle(Point(GETPX(pos.m_puyoX), GETPY(pos.m_puyoY)));
        }
        auto action1 = DelayTime::create(1);
        auto action2 = CallFunc::create(CC_CALLBACK_0(GameData::_overTo, this));
        auto action3 = Sequence::create(action1,action2, NULL);
        runAction(action3);
    }
}

void GameData::playcoin()
{
    GameMusic::playEffect(SOUND_COIN,true);
}

void GameData::_scoreAdd(int score)
{
    int add = 0;
    if (score > 1000){
        score -= 30;
        add = 30;
    }else if (score > 500){
        score -= 20;
        add = 20;
    }else if (score > 40){
        score -= 10;
        add = 10;
    }else if (score > 0){
        score -= 1;
        add = 1;
    }else{
        score = 0;
        add = 0;
    }
    Label *numsc = (Label*)m_node->getChildByTag(14);
    std::string string2 = GameBase::getCString("bouns");
    __String *strnum = __String::createWithFormat(string2.c_str(), score);
    numsc->setString(strnum->getCString());
    
    GamePlayScene::getInstance()->setScoreCurr(GamePlayScene::getInstance()->getScoreCurr()+add);
    
    auto action1 = DelayTime::create(0.02);
    auto action2 = CallFunc::create(CC_CALLBACK_0(GameData::_scoreAdd, this, score));
    auto action3 = CallFunc::create(CC_CALLBACK_0(GameData::_overTo, this));
    Sequence* action4;
    if (score == 0) {
        action4 = Sequence::create(action1,action3, NULL);
    }else{
        action4 = Sequence::create(action1,action2, NULL);
    }
    runAction(action4);
}

//结算部分结束后的判断
void GameData::_overTo()
{
    GameMusic::stopEffect();
    int nScore = GamePlayScene::getInstance()->getScoreCurr();
    GamePlayScene::getInstance()->setScoreCurr(nScore);
    
    int nNeedScore = GamePlayScene::getInstance()->needScore(GamePlayScene::getInstance()->m_stage);
    if (GamePlayScene::getInstance()->m_isWin == 1) {
        if (nScore >= nNeedScore) {
            //胜利
            GamePlayScene::getInstance()->m_isWin = 2;
            //胜利特效
            _checkAward(-1);
        }else{
            //失败
            GamePlayScene::getInstance()->m_isWin = 3;
        }
    }
    
    //为重玩本关道具保存分数
    if (GamePlayScene::getInstance()->m_isWin == 2)
    {
        userDefault->setIntegerForKey(SCORE_LAST, nScore);//保存上局的分数＊＊＊＊＊＊
    }
    
    //清理页面，保持界面清洁
    aParent->removeAllChildrenWithCleanup(true);
    m_node->removeAllChildrenWithCleanup(true);
    
    GamePlayScene::getInstance()->overHander();
}

//连消时的特效数据处理
void GameData::_checkAward(int size)
{
    _magicAward(size);
    int partid = 0;
    if (size > 15) {
        partid = 4;
    }else if (size > 12) {
        partid = 3;
    }else if (size > 8){
        partid = 2;
    }else if (size > 6){
        partid = 1;
    }else if (size == -1){
        partid = 10;
    }else{
        return;
    }
    m_particle.push_back(partid);
    if (!m_isParticle) {
        m_isParticle = true;
        _aWardPart();
    }
}

//效果集，将效果一个接着一个的显示出来
void GameData::_aWardPart()
{
    if (m_particle.size() > 0) {
        int partId = *m_particle.begin();
        m_particle.erase(m_particle.begin());
        
        GameMusic::playEffect(SOUND_APPLAUSE);
        GameMusic::playEffect(SOUND_CHEERS);
        
        __String *partName = __String::createWithFormat("res/combo_%d",partId);
        std::string comboName = GameBase::getPicName(partName->getCString());
        Sprite* partSpr = GameBase::createSprite(comboName.c_str());
        partSpr->setPosition(Vec2(0.5*winSize.width-ipadAdaptation, 0.6*winSize.height));
        partSpr->setOpacity(255);
        partSpr->setScale(4.0);
        m_node->addChild(partSpr,4);
        
        auto action1 = ScaleTo::create(0.15, 1);
        auto action1_ = DelayTime::create(0.3);
        auto action2 = Blink::create(0.5, 3);
        auto action3 = DelayTime::create(0.5);
        auto action4 = FadeOut::create(0.5);
        auto action5 = CallFunc::create(CC_CALLBACK_0(GameData::_aWardPart, this));
        auto action9 = CallFuncN::create(CC_CALLBACK_1(GameData::_removeSelf, this));
        auto action6 = CCMoveTo::create(0.2, Vec2(0.1*winSize.width-ipadAdaptation, 0.82*winSize.height));
        auto action7 = CCScaleBy::create(0.2, 0.2);
        auto action8 = Spawn::create(action6, action7, NULL);

        Vector<FiniteTimeAction*> pAction;
        pAction.pushBack(action1);
        pAction.pushBack(action1_);
        pAction.pushBack(action2);
        if (partId == 10) {
            GameMusic::playEffect(SOUND_COMPLET);
            pAction.pushBack(action8);
            pAction.pushBack(action5);
        }else{
            pAction.pushBack(action3);
            pAction.pushBack(action4);
            pAction.pushBack(action5);
            pAction.pushBack(action9);
        }
        partSpr->runAction(Sequence::create(pAction));
    }else{
        m_isParticle = false;
    }
}

//12连销时的奖励处理
void GameData::_magicAward(int size)
{
    //大于12个 给个童子（第一次显示卡片）
    if (size > 12) {
        
        Sprite* spr_add = GameBase::createSprite("res/coin.png");
        spr_add->setPosition(Vec2(winSize.width/2-15, winSize.height/3-15));
        m_node->addChild(spr_add,13);
        
        auto action1 = JumpTo::create(0.4, Vec2(winSize.width/2, winSize.height/3), 25, 2);
        auto action2 = DelayTime::create(1.0);
        auto action3 = MoveTo::create(0.8, Vec2(winSize.width*0.08, winSize.height*0.87));
        auto action4 = EaseBackIn::create(action3);
        auto action5 = CallFuncN::create(CC_CALLBACK_1(GameData::_boyAdd, this));
        Vector<FiniteTimeAction*> pAction;
        pAction.pushBack(action1);
        pAction.pushBack(action2);
        pAction.pushBack(action4);
        pAction.pushBack(action5);
        
        spr_add->runAction(Sequence::create(pAction));
    }
    
}
//12连销奖励的星星动画
void GameData::_boyAdd(cocos2d::Node *node)
{
    ZYTools::uMengBonusCoin(2, 4);
    GamePlayScene::getInstance()->setMoney(GamePlayScene::getInstance()->getMoney()+2);
    m_node->removeChild(node);
}
//通用的移除逻辑
void GameData::_removeSelf(cocos2d::Node *pNode)
{
    m_node->removeChild(pNode);
}

//暂不适用，只是将可能串位置的图片放回原来的位置
void GameData::puyoBackStyle()
{
    for (int i = 0 ; i < kXDirCellNum; i ++) {
        for (int j = 0; j < kYDirCellNum; j ++) {
            if (puyo_[i][j].typePuyo != kKindBlankIndex) {
                puyo_[i][j].m_sprite->setPosition(Vec2(SETPX(i), SETPY(j)));
            }
        }
    }
}

//====================道具的部分===========================

//
void GameData::_changeAnmiate(cocos2d::Node *pSender, int type_)
{
    Sprite *spr =  (Sprite*)pSender;
    int x = spr->getPositionX();
    int y = spr->getPositionY();
    puyo_[GETPX(x)][GETPY(y)].typePuyo = type_;
    //交换
    Sprite* sprite = getSprite(type_);
    sprite->setAnchorPoint(Point(0.5, 0.5));
    sprite->setPosition(Point(SETPX(GETPX(x)), SETPY(GETPY(y))));
    aParent->removeChild(puyo_[GETPX(x)][GETPY(y)].m_sprite, true);
    puyo_[GETPX(x)][GETPY(y)].m_sprite = sprite;
    ParticleSystemQuad *par = ParticleSystemQuad::create("particle/change_.plist");
    par->setAnchorPoint(Vec2(0.5, 0.5));
    par->setPosition(Vec2(x, y));
    m_node->addChild(par,2);
    par->setAutoRemoveOnFinish(true);
    
    //更新道具数量
    
    //检查输赢
    if (!_winOrlost()) {
        //提前进行计算数据输赢来判断接下来的步骤
        log("本关结束");
        _endLabel();
    }
    
}

bool GameData::boyChange(cocos2d::Point point, int type)
{
    _cancalSelect();
    if (point.y < ADSHight || point.y > ADSHight+kCellWidth*kYDirCellNum) {
        return false;
    }
    GamePuyo puyo = puyo_[(int)GETPX(point.x+32)][(int)GETPY(point.y+32)];
    int ptype = puyo.typePuyo;
    if (ptype == kKindBlankIndex || ptype == type) {
        return false;
    }
     puyo.m_sprite->runAction(Sequence::create(ScaleBy::create(0.2, 0.5), CallFuncN::create(CC_CALLBACK_1(GameData::_changeAnmiate, this, type)),NULL));
    return true;
}

void GameData::exitPart()
{
    log("exit() lock touch");
    lockTouch();
    m_need_RemoveArray.clear();
    int count = 0;
    for (int j = 0; j < kYDirCellNum; j ++) {
        for (int i = 0 ; i < kXDirCellNum; i ++) {
            if (puyo_[i][j].typePuyo != kKindBlankIndex) {
                count ++;
                aParent->removeChild(puyo_[i][j].m_sprite, true);
                if (count > 30) {
                    if (count %2 == 0) {
                        _partCreate(Vec2(SETPX(i), SETPY(j)),puyo_[i][j].typePuyo);
                    }
                }else{
                    _partCreate(Vec2(SETPX(i), SETPY(j)),puyo_[i][j].typePuyo);
                }
            }
        }
    }
}

void GameData::playAgain()
{
    log("playAgain() lock touch");
    lockTouch();
    exitPart();
    
    int nScore = userDefault->getIntegerForKey(SCORE_LAST);
    GamePlayScene::getInstance()->setScoreCurr(nScore);
}

void GameData::reArrange()
{
    log("reArrange() lock touch");
    lockTouch();
    _cancalSelect();
    
    for (int idx = 0; idx < kXDirCellNum; idx ++) {
        for (int jdy = 0; jdy < kXDirCellNum; jdy ++) {
            if (puyo_[idx][jdy].typePuyo != kKindBlankIndex) {
                GamePuyoPos pos = pos_Make( SETPX(idx), SETPY(jdy));
                m_to_Rearrange.push_back(pos);
            }
        }
    }
    int size = (int)m_to_Rearrange.size();
    for (int i = 0; i < size/2; i++) {
        GamePuyoPos pos_1 = m_to_Rearrange[0];
        int index = rand()%(m_to_Rearrange.size()-1)+1;
        GamePuyoPos pos_2 = m_to_Rearrange[index];
        GamePuyo puyo = puyo_[GETPX(pos_1.m_puyoX)][GETPY(pos_1.m_puyoY)];
        puyo_[GETPX(pos_1.m_puyoX)][GETPY(pos_1.m_puyoY)] = puyo_[GETPX(pos_2.m_puyoX)][GETPY(pos_2.m_puyoY)];
        puyo_[GETPX(pos_2.m_puyoX)][GETPY(pos_2.m_puyoY)] = puyo;
        m_to_Rearrange.erase(m_to_Rearrange.begin());
        m_to_Rearrange.erase(m_to_Rearrange.begin() + index - 1);
    }
    m_to_Rearrange.clear();
    if (!_winOrlost()) {
        log("死局出现");
        reArrange();
        return;
    }
    for (int idx = 0; idx < kXDirCellNum; idx ++) {
        for (int jdy = 0; jdy < kXDirCellNum; jdy ++) {
            if (puyo_[idx][jdy].typePuyo != kKindBlankIndex) {
                GamePuyoPos pos = pos_Make(SETPX(idx), SETPY(jdy));
                m_to_Rearrange.push_back(pos);
            }
        }
    }
    /*
    for (int i = 0; i < m_to_Rearrange.size(); i++) {
        GamePuyoPos pos = m_to_Rearrange[i];
        if (puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].typePuyo != kKindBlankIndex) {
            MoveTo* action = MoveTo::create(1, Vec2(pos.m_puyoX, pos.m_puyoY));
            if (i == m_to_Rearrange.size()-1) {
                log("reArrange() unlock touch");
                CallFunc* action1 = CallFunc::create(CC_CALLBACK_0(GameData::unlockTouch, this));
                Sequence* action2 = Sequence::create(action,action1,NULL);
                puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite->runAction(action2);
            }else{
                puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite->runAction(action);
            }
        }
    }*/
    
    int nNum = 0;		//为了使重置时顺序进行计算时间变量
    for (int i = 0; i < m_to_Rearrange.size(); i++) {
        GamePuyoPos pos = m_to_Rearrange[i];
        if (puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].typePuyo != kKindBlankIndex) {
            nNum++;
            MoveTo* action = CCMoveTo::create(1, Vec2(pos.m_puyoX, pos.m_puyoY));
            ScaleBy* pScaleBy = CCScaleBy::create(0.95,1.4);  //放大的动画
            ScaleBy* pScaleBy2 = CCScaleBy::create(0.05,1/1.4);  //缩小的动画
            Sequence* pScale = CCSequence::create(CCDelayTime::create(0.01*nNum),pScaleBy,pScaleBy2,NULL);  //先放大后缩小
            if (i == m_to_Rearrange.size()-1) {
                log("reArrange() unlock touch");
                CallFunc* action1 = CCCallFunc::create(CC_CALLBACK_0(GameData::unlockTouch, this));
                Sequence* action2 = CCSequence::create(CCDelayTime::create(0.01*nNum),action,action1,NULL);
                puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite->runAction(action2);
                puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite->runAction(pScale);
            }else{
                Sequence* action2 = CCSequence::create(CCDelayTime::create(0.01*nNum),action,NULL);
                puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite->runAction(action2);
                puyo_[GETPX(pos.m_puyoX)][GETPY(pos.m_puyoY)].m_sprite->runAction(pScale);
            }
        }
    }
    
    m_to_Rearrange.clear();
}
