//
//  GameTip.h
//  泡你妹改
//
//  Created by justin yang on 13-9-30.
//
//

#ifndef ________GameTip__
#define ________GameTip__

#include <iostream>
#include "PublicDefine.h"

class GameTip : public Layer
{
public:
    CREATE_FUNC(GameTip);
    virtual bool init();
    ~GameTip();
private:
    void onEnter();
    void onExit();
    void onEnterTransitionDidFinish();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    void _continHandle(Ref* pSender);
    void _cancleHanele(Ref* pSender);
    
    enum{
        TagBg = 1,
        TagMenu,
        TagMusic,
        TagSelect,
        TagLabel2,
        TagLayer,
        TagTip,
    };
};

#endif /* defined(________GameTip__) */
