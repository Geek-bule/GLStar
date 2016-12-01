//
//  AdVideoAdapterApplovin.m
//  sdkIOSDemo
//
//  Created by JustinYang on 16/9/22.
//
//

#import "ZYVideoAdapterApplovin.h"
#include "ALInterstitialAd.h"

@implementation ZYVideoAdapterApplovin


+ (void)load
{
    if (nil != NSClassFromString (@"ALSdk")) {
        [[ZYVideoAdRegistry sharedRegistry] registerClass:self and:ZYVideoApplovin];
    }
}

- (void)initAd
{
    //Initialize InMobi SDK with your property-ID
    [ALSdk initializeSdk];
    //load ad
    [self getAd];
}

- (void)getAd
{
    [[ALSdk shared].adService loadNextAd: [ALAdSize sizeInterstitial] andNotify: self];
}


- (void)showVideo:(UIViewController *)viewController
{
        // Optional: Assign delegates
    [ALInterstitialAd shared].adDisplayDelegate = self;
    [ALInterstitialAd shared].adVideoPlaybackDelegate = self;
        
    [[ALInterstitialAd shared] showOver:[UIApplication sharedApplication].keyWindow andRender: self.ad];
}



#pragma mark - Ad Load Delegate

- (void)adService:(nonnull ALAdService *)adService didLoadAd:(nonnull ALAd *)ad
{
    if(self.isShowLog)NSLog(@"视频Applovin:视频加载成功");
    self.ad = ad;
    [self.delegate success:self withType:ZYVideoApplovin];
    
}

- (void) adService:(nonnull ALAdService *)adService didFailToLoadAdWithError:(int)code
{
    // Look at ALErrorCodes.h for list of error codes
    if(self.isShowLog)NSLog(@"视频Applovin:视频加载失败");
    [self.delegate failure:self withType:ZYVideoApplovin];
}

#pragma mark - Ad Display Delegate

- (void)ad:(nonnull ALAd *)ad wasDisplayedIn:(nonnull UIView *)view
{
    if(self.isShowLog)NSLog(@"视频Applovin:视频展示完成");
    
}

- (void)ad:(nonnull ALAd *)ad wasHiddenIn:(nonnull UIView *)view
{
    if(self.isShowLog)NSLog(@"视频Applovin:视频关闭");
    [self.delegate finish:self withType:ZYVideoApplovin];
}

- (void)ad:(nonnull ALAd *)ad wasClickedIn:(nonnull UIView *)view
{
    if(self.isShowLog)NSLog(@"视频Applovin:视频点击");
}

#pragma mark - Ad Video Playback Delegate

- (void)videoPlaybackBeganInAd:(nonnull ALAd *)ad
{
    if(self.isShowLog)NSLog(@"视频Applovin:视频开始播放");
    [self.delegate play:self withType:ZYVideoApplovin];
}

- (void)videoPlaybackEndedInAd:(nonnull ALAd *)ad atPlaybackPercent:(nonnull NSNumber *)percentPlayed fullyWatched:(BOOL)wasFullyWatched
{
    if (wasFullyWatched) {
        if(self.isShowLog)NSLog(@"视频Applovin:视频播放完成");
    }else{
        if(self.isShowLog)NSLog(@"视频Applovin:视频中途退出");
        [self.delegate pause:self withType:ZYVideoApplovin];
    }
}


@end
