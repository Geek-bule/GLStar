//
//  GCHelper.h
//  CatRace
//
//  Created by Ray Wenderlich on 4/23/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>


struct LocalPlayer{
    const char* alias;      //昵称
	int authenticated;      //是否已经验证
	int isFriend;
	const char* playerID;   //是否已经成年
	int underage;
};//登陆玩家信息


@protocol GCHelperDelegate //
- (void)matchStarted;
- (void)matchEnded;
- (void)match:(GKMatch *)match didReceiveData:(NSData *)data fromPlayer:(NSString *)playerID;
@end

@interface GCHelper : NSObject <GKMatchmakerViewControllerDelegate, GKMatchDelegate, GKLeaderboardViewControllerDelegate> {
    
    BOOL gameCenterAvailable;//是否支持是支持GameCenter的设备
    BOOL userAuthenticated;//是否原本用户（貌似）
    LocalPlayer user;
    
    //GameCenter 游戏使用
    UIViewController *presentingViewController;
    GKMatch *match;
    BOOL matchStarted;
    id <GCHelperDelegate> delegate;
    
}

@property (assign, readonly) BOOL gameCenterAvailable;
@property (retain) UIViewController *presentingViewController;
@property (retain) GKMatch *match;
@property (assign) id <GCHelperDelegate> delegate;

+ (GCHelper *)sharedInstance;
- (void)authenticateLocalUser;
- (void)reportScore: (int64_t) score forCategory: (NSString*) category;
- (void)updatePlayerScores:(NSString*) category upScore:(int64_t)score tagChoose:(int)Tag;

//GameCenter 游戏使用
- (void)showGameCenter:(UIViewController *)viewController;

- (void)findMatchWithMinPlayers:(int)minPlayers maxPlayers:(int)maxPlayers viewController:(UIViewController *)viewController delegate:(int) theDelegate;

@end
