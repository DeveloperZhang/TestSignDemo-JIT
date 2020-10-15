//
//  AFHttpManager.h
//  TestSignDemo
//
//  Created by ZhangYu on 2020/10/15.
//  Copyright © 2020 JIT. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface YQYAFHttpManager : NSObject

+ (instancetype)sharedSingleton;

/*
 *  @brief post请求
 *  @author  Zhang Yu
 *  @param params 请求参数
 *  @param uriString 请求路径的相对地址
 *  @param successBlock 成功回调
 *  @param failureBlock 失败回调
 *  @remarks 备注 无
 */
- (void)postHttpRequestWithParams:(NSDictionary *)params
                        uriString:(NSString *)uriString
                     successBlock:(void(^)(id  _Nullable responseObject))successBlock
                     failureBlock:(void(^)(NSError * _Nonnull error))failureBlock;

@end

NS_ASSUME_NONNULL_END
