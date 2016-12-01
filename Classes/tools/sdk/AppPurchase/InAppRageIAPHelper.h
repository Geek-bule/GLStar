//
//  InAppRageIAPHelper.h
//  InAppRage
//
//  Created by Ray Wenderlich on 2/28/11.
//  Copyright 2011 Ray Wenderlich. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IAPHelper.h"
#import "MBProgressHUD.h"
#include "cocos2d.h"
USING_NS_CC;

@interface InAppRageIAPHelper : IAPHelper {
    MBProgressHUD* _hud;
}

@property(retain) MBProgressHUD *hud;

+ (InAppRageIAPHelper *) sharedHelper;
- (void)createHUD:(NSString*)string;
- (void)dismissHUD:(id)arg;

@end
