//
//  GameScore.h
//  泡你妹改
//
//  Created by justin yang on 13-10-8.
//
//

#ifndef ________GameScore__
#define ________GameScore__

#include <iostream>
#include "PublicDefine.h"

class GameScore : public Layer 
{
public:
    CREATE_FUNC(GameScore);
    virtual bool init();
    ~GameScore();
    
    void setShowScore(int num);
    void setNeedScore(int num);
    void needScoreShake();
    void setTarget(int num);
    
private:
    void _initialze();
    enum{
        TagStage,
        TagNeedScore,
        TagNowScore,
    };
};

#endif /* defined(________GameScore__) */
