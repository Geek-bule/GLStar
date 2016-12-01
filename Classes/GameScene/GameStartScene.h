#ifndef _START_SCENE_HEAD_FILE_
#define _START_SCENE_HEAD_FILE_
#include "PublicDefine.h"

class GameStartScene : public Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
	CREATE_FUNC(GameStartScene);
protected:
    void _paritcle();
    void _startHandle(Ref* pSender);
    void _contiHandle(Ref* pSender);
    void _rankHandle(Ref* pSender);
    void _moneyHandle(Ref* pSender);
    void _logicShow();
    
    void updateMoney(float dt);
    
    int  m_Money;
    
    Label *pListText;
    
    enum{
        zorderBG,
        zorderPic,
        zorderMenu,
        zorderLayer,
        zorderTip,
        zOrderlogic
    };
    
    enum{
        TagMoney,
    };
};


#endif