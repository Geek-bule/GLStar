//
//  GamePlayScene.h
//  泡你妹改
//
//  Created by justin yang on 13-10-8.
//
//

#ifndef ________GamePlayScene__
#define ________GamePlayScene__

#include <iostream>
#include "PublicDefine.h"
#include "GameScore.h"
#include "GameData.h"

class GamePlayScene : public Layer
{
public:
    CREATE_FUNC(GamePlayScene);
    virtual bool init();
    ~GamePlayScene();
    static Scene *newScene();
    static Scene *resScene();
    static GamePlayScene *getInstance();
    
    void genScoreHigh();//更新最高分数
    int  needScore(int  guan);
    void saveExit();//保存退出
    void needScoreBlink();//目标分数闪烁
    void overHander();//结束判断
    void replayHander();
    
    void lock();
    void unlock();
    
    void saveData();
    void loadData();
    
    int  m_stage;
    int  m_isWin;//1:游戏中 2:获胜 3:失败
    bool m_CanSave;
    
    void setScoreCurr(int score);//跟新当前分数
    int  getScoreCurr();        //获取当前分数
    void setStage(int stage);
    int  getStage();
    void setMoney(int var);
    int  getMoney();
    
    void updateMoney(float dt);
    void isHasVideo(bool isHas);
    void stopMaigcAction();
private:
    //layer基本函数
    void onEnter();
    void onExit();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    //游戏开始现实的label
    void _labelOne();
    void _labelTwo();
    void _labelThr();
    void _labelFou();
    
    void _initialze();//初始化
    void _newplay();//new
    void _resplay();//continue
    void _pauseHander(Ref* pSender);
    void _moneyHandle(Ref* pSender);
    void _magicHandle(Ref* pSender);
    void _videoHandler(Ref* pSender);
    void _lostGame();//游戏结束
    
    void _removeSelf(Node *pNode);//移除保留
private:
    GameData *m_data;
    GameScore *m_score;
    int  m_scoreCurr;
    bool isRes;
    int  m_Money;
    bool isSelect;
    enum{
        zorderBG,
        zorderPic,
        zorderMenu,
        zorderLayer,
        zorderTip,
    };
    
    enum{
        TagBg = 1,
        TagMenu,
        TagPause,
        TagLabel1,
        TagLabel2,
        TagMoney,
        TagLayer,
        TagTip,
    };
    
};

#endif /* defined(________GamePlayScene__) */
