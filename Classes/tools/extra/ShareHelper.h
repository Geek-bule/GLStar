//
//  ShareHelper.h
//  SpriteSheet
//
//  Created by JustinYang on 15/8/17.
//
//

#ifndef __SpriteSheet__ShareHelper__
#define __SpriteSheet__ShareHelper__

#import "cocos2d.h"

//分享
#define SHARE_registerApp           "477764954e80" //4ff8051a5fc
#define SHARE_WXappid               "wxff7de2a9ff90e605"
#define SHARE_WXscrid               "e2a4cb80df21cded605df06af72195bc"
#define SHARE_FBappid               "107704292745179"
#define SHARE_FBscrid               "38053202e1a5fe26c80c753071f0b573"


typedef std::function<void(int)> ccShareCallBack;

class ShareHelper
{
public:
    static ShareHelper *shareHelper();
    void init();
    void initFunction(const ccShareCallBack call);
    void shareWithMsg(const char* title, const char* message,int award, cocos2d::Vec2 pt);
    void shareToWechat(const char* msg,int award);
    
    ccShareCallBack callback;
};

#endif /* defined(__SpriteSheet__ShareHelper__) */
