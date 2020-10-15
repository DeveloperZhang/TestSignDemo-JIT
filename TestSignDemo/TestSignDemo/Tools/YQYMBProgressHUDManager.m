//
//  YQYMBProgressHUDManager.m
//  TestSignDemo
//
//  Created by ZhangYu on 2020/10/15.
//  Copyright © 2020 JIT. All rights reserved.
//

#import "YQYMBProgressHUDManager.h"
#import "YQYTools.h"

@interface YQYMBProgressHUDManager ()

@property (nonatomic, strong) MBProgressHUD *hud;

@end


@implementation YQYMBProgressHUDManager

+ (instancetype)sharedSingleton {
    static YQYMBProgressHUDManager *_sharedSingleton = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        //不能再使用alloc方法
        //因为已经重写了allocWithZone方法，所以这里要调用父类的分配空间的方法
        _sharedSingleton = [[super allocWithZone:NULL] init];
        [_sharedSingleton configHUD];
    });
    return _sharedSingleton;
}

- (void)showHudWithText:(NSString *)showText callBack:(void(^)(void))callBack delayHideSecond:(CGFloat)seconds {
    self.hud.hidden = NO;
    self.hud.label.text = showText;
    self.hud.mode = MBProgressHUDModeText;
    [self.hud showAnimated:YES];
    dispatch_async(dispatch_get_global_queue(QOS_CLASS_USER_INITIATED, 0), ^{
        if (callBack) {
            callBack();
        }
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(seconds * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            dispatch_async(dispatch_get_main_queue(), ^{
                [self hideHudAnimated:YES];
            });
        });

    });
}

- (void)showWaiting {
    self.hud.mode = MBProgressHUDModeIndeterminate;
    self.hud.label.text = @"";
    self.hud.detailsLabel.text = @"";
    [self showHudAnimated:YES];
}

- (void)showHudWithText:(NSString *)showText {
    self.hud.label.text = showText;
    self.hud.mode = MBProgressHUDModeText;
    [self showHudAnimated:YES];
}

- (void)showHudAnimated:(BOOL)animated {
    if (self.hud.hidden == NO) {
        [self hideHudAnimated:NO];
    }
    self.hud.hidden = NO;
    [self.hud showAnimated:animated];
}

- (void)hideHudAnimated:(BOOL)animated {
    self.hud.hidden = YES;
    [self.hud hideAnimated:animated];
}


- (void)configHUD {
    self.hud = [[MBProgressHUD alloc] initWithView:[YQYTools currentWindow]];
    [[YQYTools currentWindow] addSubview:self.hud];
    self.hud.bezelView.color = [UIColor blackColor];
    self.hud.backgroundView.color = [UIColor colorWithWhite:0.f alpha:.2f];
    self.hud.hidden = YES;
    
}

@end
