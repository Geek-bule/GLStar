//
//  ZYTools-apple.cpp
//  sdkIOSDemo
//
//  Created by JustinYang on 16/9/30.
//
//

#include "ZYTools.h"
#include "ZYParamOnline.h"
#include "platform/ios/CCEAGLView-ios.h"
#include "ZYVideoManager.h"
#include "ZYIosRateApp.h"
#include "GCHelper.h"
#include "InAppRageIAPHelper.h"
#include "AppController.h"
#include "MobClickGameAnalytics.h"
#include "PublicDefine.h"
#include "ShareHelper.h"
#include "appleWatch.h"
#include "GameOver.h"
#include "GamePlayScene.h"
#include "GameMusic.h"


#define isPad (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)

void ZYTools::init()
{
    //开启log
    ZYTools::showLog();
    
    //初始化在线参数
    [[ZYParamOnline shareParam] initParamBack:^(NSDictionary *dict) {
     
     }];
    //初始化版本提醒
    [[ZYParamOnline shareParam] checkNewVersion];
    //初始化互推游戏
//    [[ZYGameServer shareServer] loadGameServer];
    
#ifdef ZYTOOLS_ADVIEW
    //初始化adviewConfig
//    ZYTools::initConfig();
#endif
    [[GCHelper sharedInstance] authenticateLocalUser];
    [[SKPaymentQueue defaultQueue] addTransactionObserver:[InAppRageIAPHelper sharedHelper]];
    appleWatchCpp::init();
}


int getDaysBetween(long day1, long day2)
{
    if (day1 < day2) {
        log("Error::日期错误");
        return  -1;
    }
    struct tm *tmday1;
    tmday1 = localtime(&day1);
    int nYear1 = tmday1->tm_year;
    int nYday1 = tmday1->tm_yday;

    struct tm *tmday2;
    tmday2 = localtime(&day2);
    int nYear2 = tmday2->tm_year;
    int nYday2 = tmday2->tm_yday;

    if (nYear1-nYear2 > 1) {
        return 30+1;
    }else if (nYear1-nYear2 > 0) {
        return nYday1+366-nYday2;
    }else {
        int days = nYday1-nYday2;
        return days;
    }
}

void shareSuccess(int award)
{
    struct timeval now;
    gettimeofday(&now,NULL);
    long nowtime = now.tv_sec;
    long dayLast = userDefault->getDoubleForKey("shareDate",0);
    if (getDaysBetween(nowtime, dayLast) > 0) {
        GameMusic::playEffect(SOUND_GOLDSIN);
        userDefault->setIntegerForKey(MONEY_LABEL, userDefault->getIntegerForKey(MONEY_LABEL)+award);
        UIAlertView *av = [[UIAlertView alloc] initWithTitle:@"分享成功"
                                                     message:@"您获得了每天首次分享奖励5金币！"
                                                    delegate:nil       //委托给Self，才会执行上面的调用
                                           cancelButtonTitle:@"好的"
                                           otherButtonTitles:nil];
        [av show];
        [av release];
        userDefault->setDoubleForKey("shareDate",nowtime);
        ZYTools::uMengBonusCoin(award, 3);
    }
}

void ZYTools::initAdSdk()
{
    //添加互推
//    auto view = cocos2d::Director::getInstance()->getOpenGLView();
//    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
//    [[ZYAdGameShow shareShow] showAdGame:eaglview];
#ifdef ZYTOOLS_ADVIEW
    //初始化横幅
    ZYTools::initBannerView();
    //初始化插屏
    ZYTools::initInterstitial();
#endif
#ifdef ZYTOOLS_VIDEO
    //初始化视频sdk
    ZYTools::initVideoSdk();
#endif
    ShareHelper::shareHelper()->init();
    ShareHelper::shareHelper()->initFunction(shareSuccess);
    appleWatchCpp::startWatch();
}

bool ZYTools::IsIPad()
{
    return isPad;
}

void ZYTools::showLog()
{
    //需要显示的日志打开即可
    [[ZYParamOnline shareParam] showLog];
//    [[ZYGameServer shareServer] showLog];
    [[ZYIosRateApp shareRate] showLog];
//    [[ZYAdview shareAdview] showBannerLog];
//    [[ZYAdview shareAdview] showInterlLog];
    [[ZYVideoManager sharedManager] showLog];
}


void ZYTools::rateWithTip()
{
    //判断是不是可以弹出评论
    if ([[ZYIosRateApp shareRate] isCanRateApp]) {
        //可以弹出评论就弹出评论
        [[ZYIosRateApp shareRate] RateWithTipAndBlock:^{
            //评论成功的回调 TODO：
            userDefault->setIntegerForKey(MONEY_LABEL, userDefault->getIntegerForKey(MONEY_LABEL)+100);
            UIAlertView *av = [[UIAlertView alloc] initWithTitle:@"评论成功"
                                                         message:@"感谢你对我们的支持，赠送给您100金币！谢谢！"
                                                        delegate:nil       //委托给Self，才会执行上面的调用
                                               cancelButtonTitle:@"好的"
                                               otherButtonTitles:nil];
            [av show];
            [av release];
            ZYTools::uMengBonusCoin(100, 2);
         }];
    }else{
        //不可以弹出评论的时候进行处理
    }
}

void ZYTools::rateWithUrl()
{
    //判断是不是可以弹出评论
    if ([[ZYIosRateApp shareRate] isCanRateApp]) {
        //可以弹出评论就弹出评论
        [[ZYIosRateApp shareRate] RateWithUrlAndBlock:^{
            //评论成功的回调 TODO：
         }];
    }else{
        //不可以弹出评论的时候进行处理
    }
}

void ZYTools::setAdGame(bool isShow)
{
    //设置互推功能显示与否
//    [[ZYAdGameShow shareShow] setAdHide:!isShow Page:0];
}

void ZYTools::setAdGamePos()
{
    //设置互推图片的位置和大小
    //这个设置根据自己的需求来修改
//    [[ZYAdGameShow shareShow] setAdPot:CGPointMake(0.5, 0.2) Scale:1.0];
}

void ZYTools::reviewPort()
{
    //审核接口（每次根据策划指定的位置调用函数，同以前的umengbug函数一样）
    [[ZYParamOnline shareParam] reviewPort];
}


bool ZYTools::isReviewStatus()
{
    //判断审核状态，可用于审核时隐藏部分功能（将在线参数中的 ZYVersion 填写成与当前审核版本号相同即进入审核状态）
    return [[ZYParamOnline shareParam] isReviewStatus];
}



void ZYTools::shareWith(const char* str, int award)
{
    ShareHelper::shareHelper()->shareWithMsg("", str, 5, Vec2(0, 0));
}

void ZYTools::OpenGameCenter()
{
    AppController *delegate = (AppController* ) [UIApplication sharedApplication].delegate;
    [[GCHelper sharedInstance] showGameCenter:(UIViewController*)delegate.viewController];
}

void ZYTools::updateGcScore(int score, std::string category)
{
    NSString * strPath = [NSString stringWithUTF8String:category.c_str()];
    [[GCHelper sharedInstance] updatePlayerScores:strPath upScore:score tagChoose:0];/*tagChoose 0高分覆盖 1低分覆盖*/
}

void ZYTools::buyProductIdentifier()
{
    [[InAppRageIAPHelper sharedHelper] createHUD:@""];
    [[InAppRageIAPHelper sharedHelper] buyProductIdentifier:SHOU];
}

int ZYTools::GetLanguage()
{
    LanguageType currentLanguageType = CCApplication::sharedApplication()->getCurrentLanguage();
    int nLanguageType = 2;
    if (currentLanguageType == kLanguageChinese) {
        nLanguageType = 1;
    }
    return nLanguageType;
}

void ZYTools::uMengStartLevel(int level)
{
    NSString *_level = [NSString stringWithFormat:@"%d",level];
    [MobClickGameAnalytics startLevel:_level];
}

void ZYTools::uMengFinishLevel(int level)
{
    NSString *_level = [NSString stringWithFormat:@"%d",level];
    [MobClickGameAnalytics finishLevel:_level];
}

void ZYTools::uMengFailLevel(int level)
{
    NSString *_level = [NSString stringWithFormat:@"%d",level];
    [MobClickGameAnalytics failLevel:_level];
}


void ZYTools::uMengBonusCoin(double coin, int source)
{
    [MobClickGameAnalytics bonus:coin source:source];
}

void ZYTools::uMengPayCoin(double cash, int source, double coin)
{
    [MobClickGameAnalytics pay:cash source:source coin:coin];
}

void ZYTools::uMengBuyProps(std::string item, int amount, double price)
{
    NSString *_item = [NSString stringWithUTF8String:item.c_str()];
    [MobClickGameAnalytics buy:_item amount:amount price:price];
}

void ZYTools::uMengUseProps(std::string item, int amount, double price)
{
    NSString *_item = [NSString stringWithUTF8String:item.c_str()];
    [MobClickGameAnalytics use:_item amount:amount price:price];
}

    //==========ZYAdview sdk接入==========
#ifdef ZYTOOLS_ADVIEW
void ZYTools::initConfig()
{
    [[ZYAdview shareAdview] initAdView];
}

//init banner
void ZYTools::initBannerView()
{
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
    [[ZYAdview shareAdview] createBanner:eaglview];
    [[ZYAdview shareAdview] setAdPosition:CGPointMake(-1, -2)];//banner位置 0:上 -1:中 -2:下
}

//showBanner
void ZYTools::showBannerView()
{
    [[ZYAdview shareAdview] showBanner];
}

//hideBanner
void ZYTools::hideBannerView()
{
    [[ZYAdview shareAdview] hideBanner];
}

//init interstitial
void ZYTools::initInterstitial()
{
    [[ZYAdview shareAdview] createInstl];
}
//show interstitial
void ZYTools::showInterstitial()
{
    [[ZYAdview shareAdview] showInstl];
}

//show splash
void ZYTools::showSplash()
{
    [[ZYAdview shareAdview] createSplash];
}
#endif






    //==========ZYVideo sdk接入==========
#ifdef ZYTOOLS_VIDEO
bool ZYTools::isVideoVisible = false;
//init video
void ZYTools::initVideoSdk()
{
    ZYVideoManager*manager = [ZYVideoManager sharedManager];
    [manager loadVideoConfig];
    [manager isHasVideo:^(BOOL isHas) {
        //根据视频是否存在来设定视频按钮显示与隐藏
        //TODO:
        ZYTools::isVideoVisible = isHas;
        
        if (GameOver::getInstance()!=nullptr) {
            GameOver::getInstance()->isHasVideo(isHas);
        }
        if (GamePlayScene::getInstance()!=nullptr) {
            GamePlayScene::getInstance()->isHasVideo(isHas);
        }
     }];
}
//show video
void ZYTools::showVideo()
{
    UIViewController *result = nil;
    
    UIWindow *topWindow = [[UIApplication sharedApplication] keyWindow];
    
    if (topWindow.windowLevel != UIWindowLevelNormal){
        NSArray *windows = [[UIApplication sharedApplication] windows];
        for(topWindow in windows){
            if (topWindow.windowLevel == UIWindowLevelNormal){
                break;
            }
        }
    }
    
    UIView *rootView = [[topWindow subviews] objectAtIndex:0];
    id nextResponder = [rootView nextResponder];
    if ([nextResponder isKindOfClass:[UIViewController class]]){
        
        result = nextResponder;
        
    }else if ([topWindow respondsToSelector:@selector(rootViewController)] && topWindow.rootViewController != nil){
        
        result = topWindow.rootViewController;
        
    }
    
    [[ZYVideoManager sharedManager] showVideo:result begin:^{
        //暂停音乐 TODO:
     } pause:^{
         //开启音乐 TODO:
     } finish:^{
         //开启音乐 TODO:
         if (GameOver::getInstance()!=nullptr) {
             GameOver::getInstance()->playFinish();
             ZYTools::uMengUseProps("3", 1, 0);
         }else{
             GameMusic::playEffect(SOUND_GOLDSIN);
             userDefault->setIntegerForKey(MONEY_LABEL, userDefault->getIntegerForKey(MONEY_LABEL)+10);
             UIAlertView *av = [[UIAlertView alloc] initWithTitle:@"成功"
                                                          message:@"恭喜获得视频奖励10金币"
                                                         delegate:nil       //委托给Self，才会执行上面的调用
                                                cancelButtonTitle:@"好的"
                                                otherButtonTitles:nil];
             [av show];
             [av release];
             ZYTools::uMengBonusCoin(10, 6);
         }
     }];
}
#endif
