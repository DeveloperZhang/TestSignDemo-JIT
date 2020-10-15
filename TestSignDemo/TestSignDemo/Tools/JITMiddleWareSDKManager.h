//
//  JITMiddleWareSDKManager.h
//  TestSignDemo
//
//  Created by ZhangYu on 2020/10/15.
//  Copyright © 2020 JIT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <JITMiddleWareSDK/JITMiddleWareSDK.h>

NS_ASSUME_NONNULL_BEGIN

@interface JITMiddleWareSDKManager : NSObject

@property (strong, nonatomic) JITMiddleWareSDK *jitMCTK;

+ (instancetype)sharedSingleton;

/**
 * 产生P10数据
 * @author  Zhang Yu
 * @param   strCN              证书主题【形如：CN=CompanyName/PersonName/host,C=CN】
 * @param   strP10Base64  【出参】P10数据的Base64编码值
 * @remarks 备注 无
 * @return  YES:成功
 */
- (BOOL)createP10WithCertCN:(NSString *)strCN
              andP10Base64:(NSString *_Nullable*_Nullable)strP10Base64;

/*
 *  @brief 申请公钥证书和证书序列号
 *  @author  Zhang Yu
 *  @param p10Str P10数据的Base64编码值
 *  @param successBlock 成功回调
 *  @remarks 备注 无
 */
- (void)applyCertInfo:(NSString *)p10Str successBlock:(void(^)(id  _Nullable responseObject))successBlock;


/*
*  @brief 生成p7b数据
*  @author  Zhang Yu
*  @param cerString 公钥证书数据的Base64编码值
*  @param successBlock 成功回调
*  @remarks 备注 无
*/
- (void)generateP7bInfo:(NSString *)cerString successBlock:(void(^)(id  _Nullable responseObject))successBlock;

@end

NS_ASSUME_NONNULL_END
