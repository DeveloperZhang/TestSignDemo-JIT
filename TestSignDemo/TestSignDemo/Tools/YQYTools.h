//
//  YQYTools.h
//  TestSignDemo
//
//  Created by ZhangYu on 2020/10/15.
//  Copyright © 2020 JIT. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface YQYTools : NSObject

SINGLETON_H

/*
 *  @brief 获取当前窗口
 *  @author  Zhang Yu
 *  @remarks 备注 无
 *  @return 返回结果描述
 */
+ (UIWindow *)currentWindow;

/*
 *  @brief 32位小写的MD5值
 *  @author  Zhang Yu
 *  @param str 原数据
 *  @remarks 备注 无
 *  @return 返回MD5值
 */
+ (NSString *)stringToMD5:(NSString *)str;


/*
 *  @brief  HmacSHA1加密
 *  @author  Zhang Yu
 *  @param key 加密key
 *  @param data 加密原数据
 *  @remarks 备注 无
 *  @return 返回加密后字符串
 */
+ (NSString *)hmacSha1Encrypt:(NSString *)key data:(NSString *)data;


/*
 *  @brief 根据date格式化日期 格式为:yyyy-MM-dd HH:mm:ss
 *  @author  Zhang Yu
 *  @param date 日期类
 *  @remarks 格式化后的日期字符串
 *  @return 返回结果描述
 */
- (NSString *)formateDateWithDate:(NSDate *)date;


@end

NS_ASSUME_NONNULL_END
