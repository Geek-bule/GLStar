//
//  GameMusic.h
//  PopStarEx
//
//  Created by JustinYang on 12/2/14.
//
//

#ifndef __PopStarEx__GameMusic__
#define __PopStarEx__GameMusic__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class GameMusic
{
public:
    static void playEffect(const char *sound,bool loop = false);
    static void playMusic(const char *music);
    static void stopEffect();
};

#endif /* defined(__PopStarEx__GameMusic__) */
