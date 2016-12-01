//
//  GameOver.h
//  泡你妹改
//
//  Created by justin yang on 13-9-30.
//
//

#ifndef ________GameOver__
#define ________GameOver__

#include <iostream>
#include "PublicDefine.h"

class GameOver : public Layer
{
public:
    static GameOver *getInstance();
    CREATE_FUNC(GameOver);
    virtual bool init();
    ~GameOver();
    
    void isHasVideo(bool isHas);
    void playFinish();
private:
    void onEnter();
    void onExit();
    void onEnterTransitionDidFinish();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    void _contiHander(Ref* pSender);
    void _videoHandler(Ref* pSender);
    void _exitHaner(Ref* pSender);
    void _videoHandle(Ref* pSender);
    void _shopHandle(Ref* pSender);
    void _wallHandle(Ref* pSender);
    
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
        TagSelect,
        TagLabel2,
        TagLayer,
        TagTip,
        TaButton,
    };
};

#endif /* defined(________GameOver__) */
