//
//  GameProps.hpp
//  PopingStar
//
//  Created by JustinYang on 16/10/13.
//
//

#ifndef GameProps_hpp
#define GameProps_hpp

#include <stdio.h>
#include "PublicDefine.h"


class GameProps : public Layer
{
public:
    CREATE_FUNC(GameProps);
    virtual bool init();
    ~GameProps();
    
    void magicHandler(Ref* pSender);
    void closeHandler(Ref* pSender);
private:
    void onEnter();
    void onExit();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    
    void _closeLayer();
    
    bool isSelected;
    Vec2 selectPos;
};

#endif /* GameProps_hpp */
