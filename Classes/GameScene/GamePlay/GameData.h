//
//  GameData.h
//  泡你妹改
//
//  Created by justin yang on 13-10-8.
//
//

#ifndef ________GameData__
#define ________GameData__

#include <iostream>
#include "PublicDefine.h"

typedef enum
{
    State_Begining =1,
    State_Continuing,
    State_Nothing,
    State_Checking,
    State_Removing,
    State_falling,
    State_Lefting,
    State_Overing,
    State_Trying,
    
}GameState;

typedef struct
{
    int typePuyo;
    bool isChecked;
    bool isDel;
    bool isFall;
    Sprite* m_sprite;
    
}GamePuyo;//qi zi shu xing

typedef struct
{
    int m_puyoX;
    int m_puyoY;
    
}GamePuyoPos;//wei zhi

//给pos赋值
inline static GamePuyoPos pos_Make(int idX,int idY)
{
    GamePuyoPos pos ={idX , idY};
    return pos;
}

//初始化
inline static void Puyo_Res(GamePuyo* puyo){
    puyo->m_sprite = NULL;
    puyo->isDel = false;
    puyo->isChecked =false;
    puyo->typePuyo = kKindBlankIndex;
    puyo->isFall = false;
}

inline static bool isBlank_puyo(GamePuyo* puyo)//判断 时否为空
{
    if (puyo->typePuyo == kKindBlankIndex) {
        return true;
    }
    return false;
}

#define TYPE_NEWGAME        1
#define TYPE_RESUME         2

class GameData : public Layer
{
public:
    CREATE_FUNC(GameData);
    virtual bool init();
    ~GameData();
    static GameData *getInstance();
    //魔法星星使用
    bool boyChange(Vec2 point ,int type);
    //推出时所有星星迸发特效
    void exitPart();
    //重玩游戏
    void playAgain();
    //重新排列道具
    void reArrange();
    //使所有星星回归自己的位置＊＊暂时不使用
    void puyoBackStyle();
    //获取星星
    bool isStarExsit(int potX,int potY);
    
    void swapStarFrame();
    //
    void saveData();
    void newGame();//new game
    void resGame();//resume game
    void lockTouch();
    void unlockTouch();
    
    Sprite *getSprite(int  value, bool isSel = false);
    void _addNewPuyo(int idx,int jdy,int kind,bool isLoad = false);//棋盘摆放星星
    void removePuyo(Node *pSprite);//只是移除
    void removePuyoWithParticle(Vec2 point);//移除外带特效
    void removePuyoWithParticleSpecial(Vec2 point);//移除外带特效,为bug特别设计的
    //
    void removePuyoSwapAnother();
    CC_SYNTHESIZE(bool, m_status, Status);
private:
    void onEnter();
    void onExit();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    void playcoin();
    
    //触发触摸处理逻辑
    void _touchBegin(int x,int y);
    //消灭星星提示label
    void _tipLabelAnimate();
    void _partCreate(Vec2 point,int kind);//星星消除特效
    void _gameBeginPart(int type);//开盘效果
    void _gameBeginFinsh();//开盘结束
    
    void _checkSelect(int idx,int jdy);//根据点击的点寻找相连的点
    void _checkTo(int idx, int jdy);//将选中的点添加到消除数组
    void _dataHander();//计算可下落点
    
    void _clearSelcet();//
    void _cancalSelect();//
    
    void _fallData(int idx,int jdy);//
    void _fallAnimate();//
    
    void _leftDate(int x);//
    void _leftAnmiate();//
    
    void _removeAnmiate();//
    void _removeWithScore(int pid, GamePuyoPos pos);//
    void _removeHander(Node* pSenderm, int score);//
    void _scoreAnimate(int size,GamePuyoPos pos);//
    
    bool _winOrlost();//是否还有棋子可选进行判断
    bool _overCheck(int x, int y);//判断执行部分 水平太差 垃圾计算
    
    void _endLabel();//
    void _endAnimate();//
    
    void _changeAnmiate(Node* pSender,int type);//星星道具
    int  _loadData(int x,int y);//读取存档
    
    void _checkAward(int partid);//
    void _aWardPart();//
    
    float  waitTimeMax;//动画效果的等待时间，用来计算什么时候解锁
    GamePuyo puyo_[kXDirCellNum][kYDirCellNum];
    SpriteBatchNode *aParent;
    Node *m_node;//用来清理页面
    __Array *fallArray;
    
    int down_num[kXDirCellNum];//为了效果而增加得下落数组
    int move_left[kXDirCellNum];//左移效果
    int fallDown[kXDirCellNum][kYDirCellNum];//有一个子得上下都消除时会出bug 修改bug而设 很烂
    
    int m_scoreAward;//结束时候每多剩余一个子就减少分数 直到0
    int m_scoreCount;//结算时候的分数统计
    
    int m_overLeft;//还有多少个星星未消除
    std::vector<int> m_particle;
    bool m_isParticle;
private:
    void _magicAward(int size);
    void _boyAdd(Node* node);
    void _scoreAdd(int score);
    void _overTo();
    void _teacherHander(Ref *pSender);
    void _removeSelf(Node *pNode);
    
    
    bool isTouchEnable;
    
private:
    Label *getLabel;
};

#endif /* defined(________GameData__) */
