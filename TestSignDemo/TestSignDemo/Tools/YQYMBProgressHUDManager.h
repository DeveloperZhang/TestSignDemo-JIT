//
//  YQYMBProgressHUDManager.h
//  TestSignDemo
//
//  Created by ZhangYu on 2020/10/15.
//  Copyright © 2020 JIT. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface YQYMBProgressHUDManager : NSObject

+ (instancetype)sharedSingleton;

/*
*  @brief hud显示文案后自动隐藏
*
*  @param showText 文案内容
*  @param callBack 回调block
*  @param seconds 多少秒后自动隐藏
*
*/
- (void)showHudWithText:(NSString *)showText callBack:(void(^)(void))callBack delayHideSecond:(CGFloat)seconds;

/*
 *  @brief hud显示文案,不自动隐藏
 *
 *  @param showText 文案内容
 *
 */
- (void)showHudWithText:(NSString *)showText;

/*
 *  @brief 隐藏hud
 *
 *  @param animated 是否有动画效果
 */
- (void)hideHudAnimated:(BOOL)animated;

/*
*  @brief 等待loading
*
*/
- (void)showWaiting;

@end

NS_ASSUME_NONNULL_END
