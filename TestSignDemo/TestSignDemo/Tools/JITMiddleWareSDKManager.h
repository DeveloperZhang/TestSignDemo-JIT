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


// 证书类型
typedef enum {
    CERT_TYPE_YQY_RSA,     // RSA证书
    CERT_TYPE_YQY_SM2     // 国密证书
} CERT_TYPE_YQY;

@interface JITMiddleWareSDKManager : NSObject

@property (strong, nonatomic) JITMiddleWareSDK *jitMCTK;
@property (nonatomic, assign) CERT_TYPE_YQY certType;

+ (instancetype)sharedSingleton;

/**
* 配置证书类型
* @author  Zhang Yu
* @param   certType 证书类型
* @remarks 备注 无
*/
- (void)configCerType:(CERT_TYPE_YQY)certType;

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

/**
 * 保存证书（p7b）
 * @author  Zhang Yu
 * @param   strCertSN          单证书序列号
 * @param   strP7b             单证书P7b
 * @remarks 备注
 * @return  返回值 YES:成功 NO:失败
 */
- (BOOL)saveCertWithCertSN:(NSString *)strCertSN andP7b:(NSString *)strP7b;

@end

NS_ASSUME_NONNULL_END
