//
//  GamePause.h
//  泡你妹改
//
//  Created by justin yang on 13-9-30.
//
//

#ifndef ________GamePause__
#define ________GamePause__

#include <iostream>
#include "PublicDefine.h"

class GamePause : public Layer
{
public:
    static Scene* createScene();
    CREATE_FUNC(GamePause);
    virtual bool init();
    ~GamePause();
private:
    void onEnter();
    void onExit();
    void onEnterTransitionDidFinish();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    void _contiHander(Ref* pSender);
    void _exitHaner(Ref* pSender);
    void _musicHandle(Ref *pSender);
    void _rankHandle(Ref* pSender);
    void _shareHandle(Ref* pSender);
    void _centerHandle(Ref* pSender);
    void _feedHandle(Ref* pSender);
    
    void _updateLabel(float dt);
    
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
        TagMusic,
        TagSelect,
        TagLabel2,
        TagLabel1,
        TagTip,
    };
};

#endif /* defined(________GamePause__) */
