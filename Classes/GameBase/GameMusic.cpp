//
//  GameMusic.cpp
//  PopStarEx
//
//  Created by JustinYang on 12/2/14.
//
//

#include "GameMusic.h"
#include "SimpleAudioEngine.h"
#include "PublicDefine.h"

void GameMusic::playEffect(const char *sound,bool loop/*= false*/)
{
    if (userDefault->getBoolForKey(MUSIC_ONOFF)){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(sound,loop);
    }
}

void GameMusic::playMusic(const char *music)
{
    if (userDefault->getBoolForKey(MUSIC_ONOFF)){
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(music);
    }
}

void GameMusic::stopEffect()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}