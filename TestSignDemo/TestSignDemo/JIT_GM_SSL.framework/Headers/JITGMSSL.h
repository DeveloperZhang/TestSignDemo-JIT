//
//  JITGMSSL.h
//  JIT-GM-SSL
//
//  Created by JITPlatform001 on 2019/3/7.
//  Copyright © 2019年 JIT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface JITGMSSL : NSObject

@property (nonatomic,copy) void(^encEnvlopHandler)(BOOL bSuccess, NSString *message);
@property (nonatomic,copy) void(^decEnvlopHandler)(BOOL bSuccess, NSString *message);

@property (nonatomic,copy) void(^encSymmEnvHandler)(BOOL bSuccess, NSString *message, NSData *symmData);
@property (nonatomic,copy) void(^decSymmEnvHandler)(BOOL bSuccess, NSString *message, NSData *symmData);

+ (instancetype)shareGMSSL;

/**
 * 去Server端获取随机数
 * @author  han harvey
 * @param   strIP       服务器IP
 * @param   port        服务器端口
 * @param   strFilePath 随机数文件保存的路径
 * @remarks 备注 无
 * @return  返回值 0：随机数获取成功 其他值：失败
 */
- (int)getRandomFromServer:(NSString *)strIP
                   andPort:(int)port
               andFilePath:(NSString *)strFilePath;


/**
 * 打信封，支持SM2和RSA p7信封 - (流式信封)
 * @author  han harvey
 * @param   alg             对称加密算法，SM4:"SM4-ECB"  DES-EDE3:"DES-EDE3-ECB"
 * @param   certData        证书NSData
 * @param   ivData          IV值
 * @param   srcDataLen      数据源的总长度
 * @param   inputStream     输入流
 * @param   outputStream    输出流
 * @remarks 备注         国密只支持SM4:"SM4-ECB"
                        handler内的bSuccess代表打信封是否成功
 */
- (void)EncryptEnvelopStream:(NSString *)alg
                     andCert:(NSData *)certData
                   andIVData:(NSData *)ivData
               andSrcDataLen:(long long)srcDataLen
                 inputStream:(NSInputStream *)inputStream
                outputStream:(NSOutputStream *)outputStream
                     handler:(void(^)(BOOL bSuccess, NSString *message))handler;


/**
 * 解信封，支持SM2和RSA p7信封 - (流式信封)
 * @author  han harvey
 * @param   alg             对称加密算法，SM4:"SM4-ECB"  DES-EDE3:"DES-EDE3-ECB"
 * @param   certData        证书NSData
 * @param   ivData          IV值
 * @param   srcDataLen      数据源的总长度
 * @param   inputStream     输入流
 * @param   outputStream    输出流
 * @remarks 备注         国密只支持SM4:"SM4-ECB"
 handler内的bSuccess代表打信封是否成功
 */
- (void)DecryptEnvelopStream:(NSString *)alg
                     andCert:(NSData *)certData
                   andIVData:(NSData *)ivData
               andSrcDataLen:(long long)srcDataLen
                 inputStream:(NSInputStream *)inputStream
                outputStream:(NSOutputStream *)outputStream
                     handler:(void(^)(BOOL bSuccess, NSString *message))handler;


/**
 * 做信封-（支持多人信封）
 * @author  han harvey
 * @param   multiPubCertB64 公钥证书集合（单个证书就是单人信封，多个证书就是多人信封）
 * @param   strSymmetricAlg 对称算法(支持SM4-ECB)
 * @param   outputStream  输出流1(对称密钥信封)
 * @remarks 备注 无
 */
- (void)EncryptEnvelopMulti:(NSArray *)multiPubCertB64
               symmetricAlg:(NSString *)strSymmetricAlg
               outputStream:(NSOutputStream *)outputStream
                    handler:(void(^)(BOOL bSuccess, NSString *message, NSData *symmData))handler;

/**
 * 解信封-（支持多人信封）
 * @author  han harvey
 * @param   inputStream     输入流
 * @param   outputStream    输出流
 * @remarks 备注 无
 */
- (void)DecryptEnvelopMulti:(NSInputStream *)inputStream
               outputStream:(NSOutputStream *)outputStream
                    handler:(void(^)(BOOL bSuccess, NSString *message))handler;


//解析流式信封中的非对称加密结果
- (int)ParaseEnvelopInfo:(NSData *)EnvelopData
              publicCert:(NSData *)publicCertData
                 symmAlg:(NSString *_Nonnull*_Nonnull)symmAlgName
            encryptData1:(NSData *_Nonnull*_Nonnull)EncryptData1
            encryptData2:(NSData *_Nonnull*_Nonnull)EncryptData2
            encrpytData3:(NSData *_Nonnull*_Nonnull)EncryptData3
            encryptData4:(NSData *_Nonnull*_Nonnull)EncryptData4
         symmEncryptDataPos:(int *)pSymmEncryptDataPos
symmEncryptDataTotalLen:(long long *)pSymmEncryptDataTotalLen
                  ivData:(NSData *_Nonnull*_Nonnull)pIVData;


@end

NS_ASSUME_NONNULL_END
