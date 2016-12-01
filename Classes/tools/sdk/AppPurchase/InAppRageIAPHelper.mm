//
//  InAppRageIAPHelper.m
//  InAppRage
//
//  Created by Ray Wenderlich on 2/28/11.
//  Copyright 2011 Ray Wenderlich. All rights reserved.
//

#import "InAppRageIAPHelper.h"
#import "AppController.h"
#import "RootViewController.h"
#import "PublicDefine.h"
#import "GamePlayScene.h"
@implementation InAppRageIAPHelper
@synthesize hud = _hud;

static InAppRageIAPHelper * _sharedHelper;

+ (InAppRageIAPHelper *) sharedHelper {
    
    if (_sharedHelper != nil) {
        return _sharedHelper;
    }
    _sharedHelper = [[InAppRageIAPHelper alloc] init];
    return _sharedHelper;
    
}

- (void) dealloc
{
    [super dealloc];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (id)init {
    
    NSSet *productIdentifiers = [NSSet setWithObjects:SHOU,
                                 /*BOY1,BOY2,BOY3,
                                 REP1,REP2,REP3,*/
        nil];
    
    if ((self = [super initWithProductIdentifiers:productIdentifiers])) {                
        
    }
    
    //
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(productsLoaded:) name:
        kProductsLoadedNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(productPurchased:) name:
        kProductPurchasedNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector: @selector(productPurchaseFailed:) name:
        kProductPurchaseFailedNotification object: nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector: @selector(productPurchaseRestore:) name:
        kProductPurchaseRestoreNotification object: nil];
    return self;
    
}

- (void)createHUD:(NSString*)string{
    
    AppController* app = (AppController*)[UIApplication sharedApplication].delegate;
    self.hud = [MBProgressHUD showHUDAddedTo:app.viewController.view animated:YES];
    _hud.labelText = string;
    NSLog(@"show message %@ ",string);
    [self performSelector:@selector(timeout:) withObject:nil afterDelay:30.0];
}

- (void)dismissHUD:(id)arg {
    
    AppController* app = (AppController*)[UIApplication sharedApplication].delegate;
    [MBProgressHUD hideHUDForView:app.viewController.view animated:YES];
    self.hud = nil;
    
}

- (void)timeout:(id)arg {
    
    _hud.labelText = @"Timeout!";
    _hud.detailsLabelText = @"Please try again later.";
    _hud.customView = [[[UIImageView alloc] initWithImage:[UIImage imageNamed:@"37x-Checkmark.png"]] autorelease];
	_hud.mode = MBProgressHUDModeCustomView;
    [self performSelector:@selector(dismissHUD:) withObject:nil afterDelay:3.0];
    
}

- (void)productsLoaded:(NSNotification *)notification {
    
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    AppController* app = (AppController*)[UIApplication sharedApplication].delegate;
    [MBProgressHUD hideHUDForView:app.viewController.view animated:YES];
}

- (void)productPurchased:(NSNotification *)notification {
    
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    AppController* app = (AppController*)[UIApplication sharedApplication].delegate;
    [MBProgressHUD hideHUDForView:app.viewController.view animated:YES];
    
    NSString *productIdentifier = (NSString *) notification.object;
    NSLog(@"Purchased success: %@", productIdentifier);
    
    //GameShopBoy::buySu();
    if (GamePlayScene::getInstance() != NULL){
        GamePlayScene::getInstance()->setMoney(50+GamePlayScene::getInstance()->getMoney());
    }else{
        userDefault->setIntegerForKey(MONEY_LABEL, userDefault->getIntegerForKey(MONEY_LABEL)+50);
    }
    ZYTools::uMengPayCoin(6, 1, 50);
}

- (void)productPurchaseFailed:(NSNotification *)notification {
    
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    AppController* app = (AppController*)[UIApplication sharedApplication].delegate;
    [MBProgressHUD hideHUDForView:app.viewController.view animated:YES];
    
    SKPaymentTransaction * transaction = (SKPaymentTransaction *) notification.object;
    if (transaction.error.code != SKErrorPaymentCancelled) {
        UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:@"Error!"
                                                         message:transaction.error.localizedDescription
                                                        delegate:nil
                                               cancelButtonTitle:nil
                                               otherButtonTitles:@"OK", nil] autorelease];
        
        [alert show];
    }else{
        NSLog(@" cancel button is press");
    }
    
}

-(void)productPurchaseRestore:(NSNotification*)notification{
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    AppController* app = (AppController*)[UIApplication sharedApplication].delegate;
    [MBProgressHUD hideHUDForView:app.viewController.view animated:YES];
    
    NSString *productIdentifier = (NSString *) notification.object;
    NSLog(@"Purchased Restore success: %@", productIdentifier);
}

@end
