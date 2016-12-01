
//  AdVideoAdapterVungle.m
//  sdkIOSDemo
//
//  Created by JustinYang on 16/8/26.
//
//

#import "ZYVideoAdapterVungle.h"

@implementation ZYVideoAdapterVungle



+ (void)load
{
    if (nil != NSClassFromString (@"VungleSDK")) {
        [[ZYVideoAdRegistry sharedRegistry] registerClass:self and:ZYVideoVungle];
    }
}


- (void)initAd
{
    Class VungleSDKClass = NSClassFromString (@"VungleSDK");
    if (VungleSDKClass == nil) {
        //get vungle sdk fail
        return;
    }
    
    VungleSDK *vungle = [VungleSDK sharedSDK];
    [vungle setDelegate:self];
    [vungle startWithAppId:self.appid];
}

- (void)getAd
{
    
}


- (void)showVideo:(UIViewController *)viewController
{
    //vungle
    VungleSDK* sdk = [VungleSDK sharedSDK];
    NSError *error;
    [sdk playAd:viewController error:&error];
    if (error) {
        NSLog(@"视频Vungle:Error: %@", error);
    }
}


#pragma mark -
#pragma mark vungleSDK delegate
/**
 * 实施之后,当广告被缓存之后调用. 现在可以播放广告了！
 */

- (void)vungleSDKAdPlayableChanged:(BOOL)isAdPlayable {
    if (isAdPlayable) {
        if(self.isShowLog)NSLog(@"视频Vungle:视频缓存成功");
        [self.delegate success:self withType:ZYVideoVungle];
    }
    //vungle属于自动加载不需要失败判断
//    else {
//        if(self.isShowLog)NSLog(@"视频Vungle:视频加载失败");
//        [self.delegate failure:self withType:AdVideoVungle];
//    }
}


/**
 * 实施之后, 当SDK要播广告时被调用. 可以在此设定游戏暂停还有禁声。
 */
- (void)vungleSDKwillShowAd
{
    if(self.isShowLog)NSLog(@"视频Vungle:视频播放开始");
    [self.delegate play:self withType:ZYVideoVungle];
}

/**
 * 实施之后, 当SDK 关广告时被调用, 但这并不表示广告体验已经完成，可能会跳转到产品介绍（Product Sheet）。
 * 如果广告之后不会跳转到产品介绍（Product Sheet)，可以在这里回到你的app。
 * 如果有产品介绍(Product Sheet)，我们建议你在等它关闭之后再回到app。
 * 向用户确认奖励信息，比如：在viewInfo辞典中会包含以下值：
 * - "completedView": NSNumber 一个布林值，代表video有没有完整播放。
 * - "playTime": NSNumber 记录用户收看的秒数。
 * - "didDownlaod": NSNumber 一个布林值，表示用户有没有按”下载” 键。
 */
- (void)vungleSDKwillCloseAdWithViewInfo:(NSDictionary*)viewInfo willPresentProductSheet:(BOOL)willPresentProductSheet
{
    NSNumber *complete = [viewInfo objectForKey:0];
    if ([complete intValue] == 0) {
        if(self.isShowLog)NSLog(@"视频Vungle:SDK播放完成");
        [self.delegate finish:self withType:ZYVideoVungle];
    }else{
        if(self.isShowLog)NSLog(@"视频Vungle:SDK播放wei完成");
        [self.delegate pause:self withType:ZYVideoVungle];
    }
}

/**
 * 实施之后, 在产品说明（Product Sheet） 被关闭时调用
 * 只有在产品说明（Product Sheet） 显示的情况下可能被调用
 */
- (void)vungleSDKwillCloseProductSheet:(id)productSheet
{
    if(self.isShowLog)NSLog(@"视频Vungle:手动关闭广告");
}




@end
