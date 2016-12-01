#ifndef _PUBLIC_DEFINE_HEAD_FILE_
#define _PUBLIC_DEFINE_HEAD_FILE_

/************************************************************************/
/*∂®“Â¡À”Œœ∑÷–µƒ≥£¡ø°¢≈‰÷√Œƒº˛∫Õª˘¥°Õ∑Œƒº˛“˝”√                          */
/************************************************************************/
#include "cocos2d.h"
USING_NS_CC;

#include <vector>
#include "APGeometry.h"
#include "GameMusic.h"
#include <iostream>
#include "ZYTools.h"

extern int ipadAdaptation;

#define kKindBlankIndex	-1
// 横 竖
#define kXDirCellNum	10
#define kYDirCellNum	10
#define ADSHight        100
//#define kYDirCellNum2   11
//貌似间距（为确定）
#define kXDirOffset		5
#define kYDirOffset		5
//精灵宽
#define kCellWidth		64
//种类
#define kKindMaxNumber	5


#define SETPX(__x__)     __x__*kCellWidth+kCellWidth/2
#define GETPX(__X__)     (__X__-kCellWidth/2)/kCellWidth
#define SETPY(__y__)     __y__*kCellWidth+kCellWidth/2+ADSHight
#define GETPY(__Y__)     (__Y__-ADSHight-kCellWidth/2)/kCellWidth

#define winSize      CCDirector::getInstance()->getWinSize()

#define kGameTop    1000000000
#define ancpZero    Vec2(0,0)
#define ancpHalf    Vec2(0.5,0.5)
#define ancpOne     Vec2(1,1)

#define userDefault             UserDefault::getInstance()
#define CONTI_BUTTON            "iscontEnable"  //判断继续按钮是不是可用
#define MAGIC_STAR_NUM          "magicnum"     //魔法星星数目
#define REARRANGE_NUM           "replaynum"    //重拍数目
#define STAGE_CURRENT           "stage"      //当前管
#define SCORE_HIGH              "Scorehigh" //最高分
#define SCORE_CURRENT           "Scorecurrent"     //当前分
#define SCORE_LAST              "Scorelast" //重玩之前的分数
#define MUSIC_ONOFF             "musicswitch"   //音乐开关
#define RANK_PERCENT            "rankpercent"//排行百分比
#define LOGIC_DATE              "datetime"  //登陆的时间
#define LOGIC_TIMES             "numoftimes"//登陆的次数
#define MONEY_LABEL             "moneyhas"
#define DOUBLE_SWITCH           "isTwo"
#define STAGE_COMPLE            "stagecomple"
#define FRIST_SHARE             "fristshare"


#define TransitionTime          0.5

#define SOUND_APPLAUSE          "sounds/applause.mp3"
#define SOUND_STARTGAME         "sounds/button_start.mp3"
#define SOUND_CHEERS            "sounds/cheers.mp3"
#define SOUND_GAMELOGO          "sounds/logo.mp3"
#define SOUND_STARCLEAR         "sounds/select.mp3"
#define SOUND_STARSELECT        "sounds/select.mp3"
#define SOUND_BUTTONCLIK        "sounds/button.mp3"
#define SOUND_FALLDOWN          "sounds/fall.mp3"
#define SOUND_GOLDSIN           "sounds/coinsin.wav"
#define SOUND_COMPLET           "sounds/clear.wav"
#define SOUND_COIN              "sounds/coin.mp3"


#define BMfontName              ZYTools::GetLanguage()==1? "fonts/FZPTYJW.ttf":"fonts/CREABBB.ttf"
#define NO_AD                   "noads"


//GameCenter
#define GC_STORE                "starrank"


#endif