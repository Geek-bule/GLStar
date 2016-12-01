//
//  GameLogin.h
//  ExtremeTest
//
//  Created by JustinYang on 14/12/28.
//
//

#ifndef __ExtremeTest__GameLogin__
#define __ExtremeTest__GameLogin__

#include <stdio.h>
#include "PublicDefine.h"

class GameLogin : public Layer
{
public:
    CREATE_FUNC(GameLogin);
    virtual bool init();
    void setDate(double date, int daynum);
private:
    //layer基本函数
    void onEnter();
    void onExit();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    
    void createCoins();
    void removeLayer();
    void animteOver(int day);
    void removeSelf();
    
};

#endif /* defined(__ExtremeTest__GameLogin__) */
