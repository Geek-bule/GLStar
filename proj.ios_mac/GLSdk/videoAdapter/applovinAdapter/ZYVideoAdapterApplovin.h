//
//  AdVideoAdapterApplovin.h
//  sdkIOSDemo
//
//  Created by JustinYang on 16/9/22.
//
//

#import <Foundation/Foundation.h>
#import "ZYVideoAdapter.h"
#import "ALSdk.h"
#import "ALAdDisplayDelegate.h"
#import "ALAdLoadDelegate.h"
#import "ALAdRewardDelegate.h"

@interface ZYVideoAdapterApplovin : ZYVideoAdapter<ALAdLoadDelegate, ALAdDisplayDelegate, ALAdVideoPlaybackDelegate>

@property (nonatomic, strong) ALAd *ad;

@end
