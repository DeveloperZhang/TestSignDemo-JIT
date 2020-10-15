//
//  JITMiddleWareSDK.h
//  JITMiddleWareSDK

//  Created by JITPlatform001 on 2018/7/2.
//  Copyright © 2018年 JIT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class JITReqCertAttribute;
@class JITOAuthAttribute;


/**************** 枚举 ****************/
typedef enum {
    Index_AppFlag,     // 应用标识，该标识由网关管理员设定
    Index_AppName,     // 应用名称，该名称由网关管理员设定
    Index_AppURI,      // 应用的资源地址，该地址由网关管理员设定
    Index_AccessType,  // 应用的代理类型，1.正向  2.反向   其他暂不支持
    Index_AppRDPFlag   // 此项已废弃
} APPInfo_Indexs;

// 证书类型
typedef enum {
    CERT_TYPE_MCTK_RSA,     // RSA证书
    CERT_TYPE_MCTK_SM2,     // 国密证书
    CERT_TYPE_MCTK_ED25519  // ED25519证书
} CERT_TYPE_MCTK;

// 更新证书状态
typedef enum {
    UPDATE_CERT_STATUS_HOLD,        // 冻结
    UPDATE_CERT_STATUS_UNHOLD,      // 解冻
    UPDATE_CERT_STATUS_REVOKE       // 注销
} UPDATE_CERT_STATUS;

// 证书注销原因
typedef enum {
    CERT_REVOKE_REASON_UNSPECIFIED,         // 未指明原因
    CERT_REVOKE_REASON_KEY_COMPROMISE,      // 秘钥泄密
    CERT_REVOKE_REASON_CA_COMPROMISE,       // CA泄密
    CERT_REVOKE_REASON_AFFILIATION_CHANGED, // 从属关系改变
    CERT_REVOKE_REASON_SUPERSEDED,          // 证书被取代
    CERT_REVOKE_REASON_CESSATION_OF_OPERATION,// 操作终止
    CERT_REVOKE_REASON_REMOVE_FROM_CRL,     // 从CRL删除
    CERT_REVOKE_REASON_NONE,                // 非注销状态（不注销的时候传入此参数）
} CERT_REVOKE_REASON;

// 编辑证书类型
typedef enum {
    EDIT_CERT_TYPE_DELAY,               // 延期
    EDIT_CERT_TYPE_UPDATE               // 更新
} EDIT_CERT_TYPE;

// 证书存储位置
typedef enum {
    CERT_STORAGE_LOCATION_SAFEMODEL,        // 密码模块
    CERT_STORAGE_LOCATION_SANDBOX,          // 沙盒
    CERT_STORAGE_LOCATION_KEYCHAIN_IOS_API, // iOS keyChain，用苹果的API做业务
    CERT_STORAGE_LOCATION_KEYCHAIN_JIT_API, // JIT keyChain，不用苹果的API做业务
    CERT_STORAGE_LOCATION_SAFEMODEL_BLUETOOTH // 蓝牙模式
} CERT_STORAGE_LOCATION;

// 手机类型
typedef enum {
    MCTK_PKITool_PhoneType_NONE,     // 手机类型无设置
    MCTK_PKITool_PhoneType_IPHONE,   // iPhone
    MCTK_PKITool_PhoneType_IPAD      // iPad
} MCTK_PKITool_PhoneType;

// HmacHash算法
typedef enum {
    MCTKHmacHash_SHA1,
    MCTKHmacHash_SHA224,
    MCTKHmacHash_SHA256,
    MCTKHmacHash_SHA384,
    MCTKHmacHash_SHA512,
    MCTKHmacHash_MD5,
    MCTKHmacHash_SM3,
} MCTK_HmacHash;

@interface JITMiddleWareSDK : NSObject 

// 证书存储方式(密码模块，沙盒，iOS KeyChain，JIT KeyChain，蓝牙Key)
@property (assign, nonatomic) CERT_STORAGE_LOCATION storageLocation;


/**
 * MCTK 实例化方式【标准版本初始化方法】
 * @author  han harvey
 * @param   location                    证书存储方式（必填）
 * @param   strIP                           获取License信息服务器IP地址（必填）
 * @param   strPort                       获取License信息服务器端口（必填）
 * @param   strSafeModelPath   密码模块自定义相对路径 不需要传nil 即可
 * @remarks 备注
 
 1.0.5版本之后新增加实例化方式
 1.0.10版本之后（不包括1.0.10）新增加密码模块实例化方式
 CERT_STORAGE_LOCATION_SAFEMODEL,               // 密码模块
 CERT_STORAGE_LOCATION_SANDBOX,                   // 沙盒
 CERT_STORAGE_LOCATION_KEYCHAIN_IOS_API,   // iOS keyChain，用苹果的API做业务
 CERT_STORAGE_LOCATION_KEYCHAIN_JIT_API,    // JIT keyChain，不用苹果的API做业务
 CERT_STORAGE_LOCATION_SAFEMODEL_BLUETOOTH  // 蓝牙模式
 
 * @return  返回值 MCTK实例化对象
 */

- (instancetype)initWithCertStorageLocation:(CERT_STORAGE_LOCATION)location
                                      andIP:(NSString *)strIP
                                    andPort:(NSString *)strPort
                           andSafeModelPath:(NSString *)strSafeModelPath;

/**
 MCTK 实例化方式 【MCTK使用蓝牙Key需要调用该接口进行初始化】
 */
- (instancetype)initWithCertStorageLocation:(CERT_STORAGE_LOCATION)location
                                      andIP:(NSString *)strIP
                                    andPort:(NSString *)strPort
                                 andDevName:(NSString *)strDevName
                           andSafeModelPath:(NSString *)strSafeModelPath;


/**
 * 预设置协同服务器地址接口【密码模块模式下，MCTK初始化之前调用】
 * 协同版本的密码模块才会需要调用该接口
 * @param   strIP         协同服务器IP
 * @param   strPort     协同服务器端口
*/
+ (void)SetSafeModelCoServerIP:(NSString *)strIP
               andCoServerPort:(NSString *)strPort;


/**
 MCTK 实例化方式 【协同版本密码模块存储模式初始化方法】
 调用该接口之前先调用 + (void)SetSafeModelCoServerIP: andCoServerPort: 接口，预设置服务器地址
 * @author  han harvey
 * @param   location                    证书存储方式（必填）
 * @param   strIP                           获取License信息服务器IP地址（必填）
 * @param   strPort                       获取License信息服务器端口（必填）
 * @param   strSafeModelPath   密码模块自定义相对路径 不需要传nil 即可
 * @remarks 备注
 
 1.0.5版本之后新增加实例化方式
 1.0.10版本之后（不包括1.0.10）新增加密码模块实例化方式
 CERT_STORAGE_LOCATION_SAFEMODEL,               // 密码模块
 CERT_STORAGE_LOCATION_SANDBOX,                   // 沙盒
 CERT_STORAGE_LOCATION_KEYCHAIN_IOS_API,   // iOS keyChain，用苹果的API做业务
 CERT_STORAGE_LOCATION_KEYCHAIN_JIT_API,    // JIT keyChain，不用苹果的API做业务
 CERT_STORAGE_LOCATION_SAFEMODEL_BLUETOOTH  // 蓝牙模式

 */
- (instancetype)initWithCertStorageLocation:(CERT_STORAGE_LOCATION)location
                                      andIP:(NSString *)strIP
                                    andPort:(NSString *)strPort
                           andSafeModelPath:(NSString *)strSafeModelPath
                                andUserInfo:(NSString *)strUserInfo;


/**
 * 获取中间件版本号
 * @remarks 备注 2016-1-20 by chunxu_yuan
 * 1.0.5是ios第一版带版本号的中间件
 * 此前发布的中间件均不存在版本号接口
 * @return  MCTK最新版本
 */
+ (NSString *)GetVersion;

/**
 * 获取应用中的library全路径
 * @return  沙盒library全路径
 */
+ (NSString *)GetUserLibraryPath;

/**
 * 获取应用中的documents全路径
 * @return  沙盒documents全路径
 */
+ (NSString *)GetUserDocumentPath;

/**
 * 获取错误码
 * @author  han harvey
 * @return  返回值 错误码
 */
- (int)GetErrorCode;

/**
 * 根据错误码获取错误描述
 * @author  han harvey
 * @param   uiErrorCode 错误码
 * @return  返回值 错误描述
 */
- (NSString *)GetErrorMessage:(int)uiErrorCode;


/******************* 证书管理 Start ***********************/

// 申请证书、删除证书、修改证书密码、获取证书列表、更新证书、注销证书、冻结解冻证书

/**
 * 申请RSA证书(端口默认443，秘钥长度默认1024)
 * @author  han harvey
 * @param   strServerIP             证书服务器IP
 * @param   strAuthorizationCode    证书授权码（从CA系统或RA系统的管理员处获得）
 * @param   strPassword             证书预设的密码【密码模块模式默认Aa111111】
 * @return  返回值 0:成功 其他为错误码
 */
- (int)RequestCert:(NSString *)strServerIP
 authorizationCode:(NSString *)strAuthorizationCode
          password:(NSString *)strPassword;

/**
 * 申请RSA证书(外传IP和端口，外传秘钥长度)
 * @author  han harvey
 * @param   strServerIP             证书服务器IP
 * @param   strPort                      证书服务器端口【默认为：50625】
 * @param   strAuthorizationCode    申请证书需要用到的授权码
 * @param   strPassword             证书密码【密码模块模式默认Aa111111】
 * @param   keyLength                  密钥长度，1024或2048
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)RequestCertEx:(NSString *)strServerIP
                Port:(NSString *)strPort
   authorizationCode:(NSString *)strAuthorizationCode
            password:(NSString *)strPassword
        andKeyLength:(NSString *)keyLength;

/**
 * 申请RSA证书(外传IP和端口，外传秘钥长度；证书PIN码受指纹认证或Face ID保护）
 * @author  han harvey
 * @param   strServerIP             证书服务器IP
 * @param   strPort                      证书服务器端口【默认为：50625】
 * @param   strAuthorizationCode    申请证书需要用到的授权码
 * @param   strPassword             证书密码【密码模块模式默认Aa111111】
 * @remarks 备注 此方式申请的证书PIN码受指纹认证保护
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)RequestCertWithFingerprintAuth:(NSString *)strServerIP
                                 Port:(NSString *)strPort
                    authorizationCode:(NSString *)strAuthorizationCode
                             password:(NSString *)strPassword
                         andKeyLength:(NSString *)keyLength;


/**
 * 申请RSA证书 (可以增加附加属性)
 * @author  han harvey
 * @param   strServerIP             证书服务器IP
 * @param   strPort                      证书服务器端口【默认为：50625】
 * @param   strAuthorizationCode    申请证书需要用到的授权码
 * @param   strPassword             证书密码【密码模块模式默认Aa111111】
 * @param   keyLength                 密钥长度，1024或2048（字符串）
 * @param   attribute                 附加参数
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)RequestCertEx:(NSString *)strServerIP
                Port:(NSString *)strPort
   authorizationCode:(NSString *)strAuthorizationCode
            password:(NSString *)strPassword
        andKeyLength:(NSString *)keyLength
       andAttributes:(JITReqCertAttribute *)attribute;

/**
 * 申请RSA证书 (可以增加附加属性；证书PIN码受指纹认证保护）
 * @author  han harvey
 * @param   strServerIP             证书服务器IP
 * @param   strPort                      证书服务器端口【默认为：50625】
 * @param   strAuthorizationCode    申请证书需要用到的授权码
 * @param   strPassword             证书密码【密码模块模式默认Aa111111】
 * @param   keyLength               密钥长度，1024或2048
 * @param   attribute               附加参数
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)RequestCertWithFingerprintAuthEx:(NSString *)strServerIP
                                   Port:(NSString *)strPort
                      authorizationCode:(NSString *)strAuthorizationCode
                               password:(NSString *)strPassword
                           andKeyLength:(NSString *)keyLength
                          andAttributes:(JITReqCertAttribute *)attribute;

/**
 * 申请ED25519证书(外传IP和端口,授权码)
 * @author  yanlong lv
 * @param   strServerIP             证书服务器IP
 * @param   strPort                      证书服务器端口【默认为：50625】
 * @param   strAuthorizationCode    申请证书需要用到的授权码
 * @param   strPassword             证书密码【密码模块模式默认Aa111111】
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)RequestED25519Cert:(NSString *)strServerIP
                     Port:(NSString *)strPort
        authorizationCode:(NSString *)strAuthorizationCode
                 password:(NSString *)strPassword;


/**
 * 申请国密证书IP外传，端口默认443
 * @author  han harvey
 * @param   strServerIP             证书服务器IP
 * @param   strAuthorizationCode    授权码
 * @param   strPassword             证书密码【密码模块模式默认Aa111111】
 * @remarks 备注 当存储模式为iOS KeyChain时，此方法不可用
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)RequestGMCert:(NSString *)strServerIP
   authorizationCode:(NSString *)strAuthorizationCode
            password:(NSString *)strPassword;

/**
 * 申请国密证书IP和端口外传
 * @author  han harvey
 * @param   strServerIP             证书服务器IP
 * @param   strPort                      证书服务器端口【默认为：50625】
 * @param   strAuthorizationCode    授权码
 * @param   strPassword             证书密码【密码模块模式默认Aa111111】
 * @remarks 备注 当存储模式为iOS KeyChain时，此方法不可用
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)RequestGMCertEx:(NSString *)strServerIP
                  Port:(NSString *)strPort
     authorizationCode:(NSString *)strAuthorizationCode
              password:(NSString *)strPassword;

/*
 * 申请证书并下载
 * @param    strServerIP     服务器IP
 * @param    port            服务器端口
 * @param    strCertType

    certApplyAndDown_sm2 sm2单证
    certApplyAndDown_sm2_double sm2双证
    certApplyAndDown_rsa_1024 rsa单证
    certApplyAndDown_rsa_2048
    certApplyAndDown_rsa_double_1024 rsa双证
    certApplyAndDown_rsa_double_2048
 
 * @param    strPassword     证书密码【密码模块模式默认Aa111111】
 * @param    strUniqueSign   唯一标识
 * @param    strDevID        设备ID
 * @param    strPhoneNum     手机号
 * @param    attri           用户属性信息 可以传空
 * @return   返回值0成功 其他值：错误
 */
- (int)requestCertByIdentificationEx:(NSString *)strServerIP
                             andPort:(NSString *)strPort
                         andCertType:(NSString *)strCertType
                         andPassword:(NSString *)strPassword
                       andUniqueSign:(NSString *)strUniqueSign
                            andDevID:(NSString *)strDevID
                           andTelNum:(NSString *)strPhoneNum
                            andAttri:(NSDictionary *)attri
                           andCertSN:(NSString **)strCertSN;

/*
 * 4证申请并下载
 * @param    strServerIP 服务器IP
 * @param    port    服务器端口
 * @param    strPassword 证书密码
 * @param    strUniqueSign   唯一标识
 * @param    strDevID    设备ID
 * @param    strPhoneNum 手机号
 * @param    strKeyLength（1024/2048）密钥长度
 * @param    attri   用户属性信息 可以传空
 * @return   返回值0成功 其他值：错误
 */
- (int)requestCertByIdentification:(NSString *)strServerIP
                           andPort:(NSString *)strPort
                       andPassword:(NSString *)strPassword
                     andUniqueSign:(NSString *)strUniqueSign
                          andDevID:(NSString *)strDevID
                         andTelNum:(NSString *)strPhoneNum
                      andKeyLength:(NSString *)strKeyLength
                          andAttri:(NSDictionary *)attri;

/**
 * 申请国密证书IP和端口外传（证书PIN码受指纹认证保护）
 * @author  han harvey
 * @param   strServerIP             证书服务器IP
 * @param   strPort                      证书服务器端口【默认为：50625】
 * @param   strAuthorizationCode    授权码
 * @param   strPassword             证书密码【密码模块模式默认Aa111111】
 * @remarks 备注 当存储模式为iOS KeyChain时，此方法不可用
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)RequestGMCertWithFingerprintAuth:(NSString *)strServerIP
                                   Port:(NSString *)strPort
                      authorizationCode:(NSString *)strAuthorizationCode
                               password:(NSString *)strPassword;


/**
 * 申请国密证书（可以增加附加属性）
 * @author  han harvey
 * @param   strServerIP             证书服务器IP
 * @param   strPort                      证书服务器端口【默认为：50625】
 * @param   strAuthorizationCode    授权码
 * @param   strPassword             证书密码【密码模块模式默认Aa111111】
 * @param   attribute                  附加参数
 * @remarks 备注 当存储模式为iOS KeyChain时，此方法不可用
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)RequestGMCertEx:(NSString *)strServerIP
                  Port:(NSString *)strPort
     authorizationCode:(NSString *)strAuthorizationCode
              password:(NSString *)strPassword
         andAttributes:(JITReqCertAttribute *)attribute;

/**
 * 申请国密证书（可以增加附加属性；证书PIN码受指纹认证保护）
 * @author  han harvey
 * @param   strServerIP             证书服务器IP
 * @param   strPort                      证书服务器端口【默认为：50625】
 * @param   strAuthorizationCode    授权码
 * @param   strPassword             证书密码
 * @param   attribute                  附加参数
 * @remarks 备注 当存储模式为iOS KeyChain时，此方法不可用
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)RequestGMCertWithFingerprintAuth:(NSString *)strServerIP
                                   Port:(NSString *)strPort
                      authorizationCode:(NSString *)strAuthorizationCode
                               password:(NSString *)strPassword
                          andAttributes:(JITReqCertAttribute *)attribute;

/**
 * 两步申请证书 -> 产生P10数据
 * @author  han harvey
 * @param   strCN                   证书主题【形如：CN=xxx,C=CN】
 * @param   keyLength          密钥长度，1024或2048
 * @param   strPassword      预设PIN码（保护证书私钥的密码）
 * @param   strP10Base64  【出参】P10数据
 * @remarks 备注 无
 * @return  0:成功 其他值:错误码
 */
- (int)createP10WithCertCN:(NSString *)strCN
               andCertType:(CERT_TYPE_MCTK)certType
              andKeyLength:(NSString *)keyLength
               andPassword:(NSString *)strPassword
              andP10Base64:(NSString **)strP10Base64;


/**
 * 两步申请证书 -> 保存证书（p7b）
 * @author  han harvey
 * @param   certType                 证书类型 （RSA或者SM2,不区分大小写）
 * @param   strPassword           证书密码
 * @param   strCertSN               单证书序列号
 * @param   strP7b                      单证书P7b
 * @param   strDoubleCertSN  双证书序列号（可不传）
 * @param   strDoubleP7b         双证书P7b（可不传）
 * @param   strDleEncSessionKey SessionKey（可不传）
 * @param   strSessionKeyAlg    SessionKeyAlg（可不传）
 * @param   strDleEncPriKey     EncPrivateKey（可不传）
 * @remarks 备注
 strDoubleCertSN strDoubleP7b strDleEncSessionKey
 strSessionKeyAlg strDleEncPriKey
 等5个参数可以同时不传，但如果传那就必须5个参数同时传否则无效。
 * @return  返回值 0:成功 其他:错误码
 */
- (int)saveCertWithCertType:(NSString *)certType
            andCertPassword:(NSString *)strPassword
                  andCertSN:(NSString *)strCertSN
                     andP7b:(NSString *)strP7b
            andDoubleCertSN:(NSString *)strDoubleCertSN
               andDoubleP7b:(NSString *)strDoubleP7b
     andDoubleEncSessionKey:(NSString *)strDleEncSessionKey
           andSessionKeyAlg:(NSString *)strSessionKeyAlg
        andDleEncPrivateKey:(NSString *)strDleEncPriKey;


/**
 * 根据X509证书 保存证书
 * @author  han harvey
 * @param   certType            证书类型 （RSA或者SM2,不区分大小写）
 * @param   strPassword         证书密码
 * @param   strCertSN           单证书序列号
 * @param   strSignCertB64      单证书Base64
 * @param   strDoubleCertSN     双证书序列号（可不传）
 * @param   strEncCertB64       双证书Base64（可不传）
 * @param   strDleEncSessionKey SessionKey（可不传）
 * @param   strSessionKeyAlg    SessionKeyAlg（可不传）
 * @param   strDleEncPriKey     EncPrivateKey（可不传）
 * @remarks 备注
 strDoubleCertSN
 strEncCertB64
 strDleEncSessionKey
 strSessionKeyAlg
 strDleEncPriKey
 等5个参数可以同时不传，但如果传那就必须5个参数同时传否则无效。
 如果有一个没有值则默认当作单证书处理。
 * @return  返回值 0:成功 其他:错误码
 */
- (int)saveCertWithCertTypeEx:(NSString *)certType
              andCertPassword:(NSString *)strPassword
                    andCertSN:(NSString *)strCertSN
               andSignCertB64:(NSString *)strSignCertB64
              andDoubleCertSN:(NSString *)strDoubleCertSN
                andEncCertB64:(NSString *)strEncCertB64
       andDoubleEncSessionKey:(NSString *)strDleEncSessionKey
             andSessionKeyAlg:(NSString *)strSessionKeyAlg
          andDleEncPrivateKey:(NSString *)strDleEncPriKey;

/**
 * 获取证书列表，不做任何过滤
 * @remarks 备注
 * @return  证书列表
 */
- (NSMutableArray *)GetCertList;


/**
 * 删除证书
 * @author  han harvey
 * @param   arrayIndex  证书列表中的索引，证书列表从GetCertList获取
 * @param   strPassword 证书密码（证书存储在iOS KeyChain中时此参数无用）
 * @remarks 备注 证书存储在iOS KeyChain中时密码随意传值，此参数不使用，建议传空串即可。
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)DeleteCert:(int)arrayIndex
         password:(NSString *)strPassword;

/**
 * 删除证书-证书PIN码受指纹认证保护
 * @author  han harvey
 * @param   arrayIndex  证书列表中的索引，证书列表从GetCertList获取
 * @remarks 备注 证书存储在iOS KeyChain中时密码随意传值，此参数不使用，建议传空串即可。
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)DeleteCertWithFingerprint:(int)arrayIndex;

/**
 * 根据证书唯一标识删除证书
 * @author  han harvey
 * @param   pfxNameOrCertSN     唯一标识
 * @param   authCodeStr             验证码 -> 调用GetAuthCode方法获取
 * @remarks 备注
 authCodeStr：验证码从- (NSString *)GetAuthCodeWithLength:(int)length;获得
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)DeleteCert:(NSString *)pfxNameOrCertSN
     withAuthCode:(NSString *)authCodeStr;

/**
 * 生成随机验证码
 * @author  han harvey
 * @remarks 备注 字母数字组成，使用时不需要区分大小写
 */
- (NSString *)GetAuthCodeWithLength:(int)length;

/**
 * 修改证书密码 -> 根据证书索引值
 * @author  han harvey
 * @param   arrayIndex      证书列表中的索引，证书列表从GetCertList获取（如果证书存储模式使用的是密码模块，则此参数传0即可）
 * @param   strOldPassword  旧密码
 * @param   strNewPassword  新密码
 * @param   retryCount          重试次数
 * @remarks 备注 只有证书存储在沙盒中时，此方法可用
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)ModifyCertPassword:(int)arrayIndex
              oldPassword:(NSString *)strOldPassword
              newPassword:(NSString *)strNewPassword
            andRetryCount:(int *)retryCount;

/**
 * 修改证书密码 -> 根据证书唯一标识
 * @author  han harvey
 * @param   pfxNameOrCertSN     证书唯一标识【如果存储模式使用密码模块，则此参数不作校验，建议传空串】
 * @param   strPassword     证书现用密码
 * @param   strNewPassword  证书新密码
 * @param   retryCount          重试次数
 * @remarks 备注
 如果存储模式使用密码模块，则pfxNameOrCertSN不作校验，建议传空串
 当storageLocation == CERT_STORAGE_LOCATION_SANDBOX 时，pfxNameOrCertSN为证书路径
 当storageLocation == CERT_STORAGE_LOCATION_KEYCHAIN_JIT_API 时，pfxNameOrCertSN为证书序列号
 当storageLocation == CERT_STORAGE_LOCATION_KEYCHAIN_IOS_API 时，此方法不可用
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)ModifyPFXPassword:(NSString *)pfxNameOrCertSN
                password:(NSString *)strPassword
             newPassword:(NSString *)strNewPassword
           andRetryCount:(int *)retryCount;

/**
 * 证书更新
 * @author  han harvey
 * @param   arrayIndex  证书列表中的索引，证书列表从GetCertList获取
 * @param   strIP           证书服务器IP
 * @param   strPort       证书服务器端口
 * @param   passStr       证书密码
 * @param   editType     更新方式
 * @remarks 备注
 // 证书更新类型
 EDIT_CERT_TYPE_DELAY,               // 延期
 EDIT_CERT_TYPE_UPDATE               // 更新
 * @return  返回值 0:成功 其他:错误码
 */
- (int)updateCertWithIndex:(int)arrayIndex
           andCertPassword:(NSString *)passStr
             andUpdateType:(EDIT_CERT_TYPE)editType
                   andGWIP:(NSString *)strIP
                 andGWPort:(NSString *)strPort;

/**
 * 证书更新-证书密码受指纹认证保护
 * @author  han harvey
 * @param   arrayIndex  证书列表中的索引，证书列表从GetCertList获取
 * @param   strIP     网关IP
 * @param   strPort   网关端口
 * @param   editType    更新方式
 * @remarks 备注 无
 * @return  返回值 0:成功 其他:错误码
 */
- (int)updateCertFingerprintAuthWithIndex:(int)arrayIndex
                            andUpdateType:(EDIT_CERT_TYPE)editType
                                  andGWIP:(NSString *)strIP
                                andGWPort:(NSString *)strPort;


/**
 * 更新证书状态（冻结、解冻、注销）
 * @author  han harvey
 * @param   arrayIndex  证书索引
 * @param   passStr     证书PIN码
 * @param   certStatus  更新证书状态
 * @param   reason      注销原因（当更新证书状态为注销状态时，此参数必须传入）
 * @param   strDesc     更新状态描述
 * @param   ipStr       IP地址
 * @param   portStr     端口
 * @remarks 备注         iOS KeyChain 存储模式无需输入证书PIN码
 * @return  返回值 0:成功 其他:错误码
 */
- (int)updateCertStatusWithIndex:(int)arrayIndex
                     andPassword:(NSString *)passStr
                   andCertStatus:(UPDATE_CERT_STATUS)certStatus
                 andRevokeReason:(CERT_REVOKE_REASON)reason
                         andDesc:(NSString *)strDesc
                           andIP:(NSString *)ipStr
                         andPort:(NSString *)portStr;

/**
 * 更新证书状态（冻结、解冻、注销）- 密码受指纹/Face ID保护
 * @author  han harvey
 * @param   arrayIndex  证书索引
 * @param   certStatus  更新证书状态
 * @param   reason      注销原因（当更新证书状态为注销状态时，此参数必须传入）
 * @param   strDesc     更新状态描述
 * @param   ipStr       IP地址
 * @param   portStr     端口
 * @remarks 备注 无
 * @return  返回值 0:成功 其他:错误码
 */
- (int)updateCertStatusFingerprintAuthWithIndex:(int)arrayIndex
                                  andCertStatus:(UPDATE_CERT_STATUS)certStatus
                                andRevokeReason:(CERT_REVOKE_REASON)reason
                                        andDesc:(NSString *)strDesc
                                          andIP:(NSString *)ipStr
                                        andPort:(NSString *)portStr;

/**
 * 获取签名或加密证书
 * @author  yanlong lv
 * @param   isSign True代表签名，False代表加密
 * @param   index  证书列表索引
 * @return  返回值     公钥证书部分
 */
- (NSString *)getCert:(BOOL)isSign
            certIndex:(int)index;

/**
 * 获取证书DN
 * @author  han harvey
 * @param   arrayIndex 证书列表中的索引，请先获取证书列表
 * @return  返回值 证书DN
 */
- (NSString *)GetCertDN:(int)arrayIndex;

/**
 * 获取证书SN
 * @author  han harvey
 * @param   arrayIndex 证书列表中的索引，请先获取证书列表
 * @return  返回值 证书SN
 */
- (NSString *)GetCertSN:(int)arrayIndex;

/**
 * 获取颁发者DN
 * @author  han harvey
 * @param   arrayIndex 证书列表中的索引，请先获取证书列表
 * @return  返回值 颁发者DN
 */
- (NSString *)GetIssuerDN:(int)arrayIndex;

/**
 * 获取证书起始生效时间
 * @author  han harvey
 * @param   arrayIndex 证书列表中的索引，请先获取证书列表
 * @return  返回值 证书起始生效时间
 */
- (NSString *)GetCertStartTime:(int)arrayIndex;

/**
 * 获取证书失效时间
 * @author  han harvey
 * @param   arrayIndex 证书列表中的索引，请先获取证书列表
 * @return  返回值 证书失效时间
 */
- (NSString *)GetCertEndTime:(int)arrayIndex;

/**
 * 获取证书CN
 * @author  han harvey
 * @param   arrayIndex 证书列表中的索引，请先获取证书列表
 * @return  返回值 证书CN
 */
- (NSString *)GetCertCN:(int)arrayIndex;

/**
 * 获取颁发者CN
 * @author  han harvey
 * @param   arrayIndex 证书列表中的索引，请先获取证书列表
 * @return  返回值 颁发者CN
 */
- (NSString *)GetIssuerCN:(int)arrayIndex;

/**
 * 获取证书文件路径（或者容器名或者证书的序列号）
 * @author  han harvey
 * @param   arrayIndex 证书列表中的索引，请先获取证书列表
 * @remarks 备注 只有证书存储在沙盒中时，此方法可用
 * @return  返回值 证书路径字符串或者容器名或者证书的序列号
 */
- (NSString *)GetCertFile:(int)arrayIndex;

/**
 * 根据证书路径或者证书SN获取证书主题
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书唯一标识（路径/序列号/容器名）
 * @remarks 备注
 * @return  返回值 证书主题字符串
 */
- (NSString *)GetCertSubject:(NSString *)pfxNameOrCertSN;

/**
 * 根据证书路径或者证书SN获取证书序列号
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书唯一标识（路径/序列号/容器名）
 * @remarks 备注
 * @return  返回值 证书序列号
 */
- (NSString *)GetCertSerialNumber:(NSString *)pfxNameOrCertSN;

/**
 * 根据证书路径或者证书SN获取颁发者主题
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书唯一标识（路径/序列号/容器名）
 * @remarks 备注
 * @return  返回值 颁发者主题
 */
- (NSString *)GetIssuerSubject:(NSString *)pfxNameOrCertSN;

/**
 * 根据证书路径或者证书SN获取证书主题CN
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书唯一标识（路径/序列号/容器名）
 * @remarks 备注
 * @return  返回值 证书主题CN
 */
- (NSString *)GetCertSubjectCN:(NSString *)pfxNameOrCertSN;

/**
 * 根据证书路径或者证书SN获取颁发者主题CN
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书唯一标识（路径/序列号/容器名）
 * @remarks 备注
 * @return  返回值 颁发者主题CN
 */
- (NSString *)GetIssuerSubjectCN:(NSString *)pfxNameOrCertSN;

/**
 * 根据证书路径或者证书SN获取证书生效开始时间
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书唯一标识（路径/序列号/容器名）
 * @remarks 备注
 * @return  返回值 证书生效开始时间
 */
- (NSString *)GetCertEffectiveTime:(NSString *)pfxNameOrCertSN;

/**
 * 根据证书路径或者证书SN获取证书失效时间
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书唯一标识（路径/序列号/容器名）
 * @remarks 备注
 * @return  返回值 证书失效时间
 */
- (NSString *)GetCertExpiryTime:(NSString *)pfxNameOrCertSN;

/**
 * 根据证书路径或者证书SN获取证书类型
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书唯一标识（路径/序列号/容器名）
 * @remarks 备注
 * @return  返回值 证书类型 @"RSA证书" 或者 @"国密证书"
 */
- (NSString *)GetCertType:(NSString *)pfxNameOrCertSN;

/**
 * 检查证书是否逾期
 * @author  han harvey
 * @param   arrayIndex  证书列表中的索引，请先获取证书列表
 * @param   isShow      是否弹出逾期提示
 * @remarks 备注 如果isShow为YES，则证书是否逾期都是弹出提示框
 * @return  返回值 YES:逾期 NO:未逾期
 */
- (BOOL)checkCertIsOverdueWithIndex:(int)arrayIndex
                          andIsShow:(BOOL)isShow;

/**
 * 下载PFX证书和Cert证书，申请的证书文件将保存在lib路径下（可以通过接口GetUserLibraryPath获得该路径）或者KeyChain中
 * @author  han harvey
 * @param   strServerIP MCS地址
 * @param   strPort     MCS的端口号
 * @param   strIdentifier xxx？？？
 * @remarks 备注 通过此函数下载证书必须匹配2016年2月后的MCS服务器，否则不支持该接口
 当storageLocation == CERT_STORAGE_LOCATION_SANDBOX 时，此方法可用
 当storageLocation == CERT_STORAGE_LOCATION_KEYCHAIN_JIT_API 时，此方法不可用
 当storageLocation == CERT_STORAGE_LOCATION_KEYCHAIN_IOS_API 时，并且是RSA证书时，此方法可用
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)DownloadCert:(NSString *)strServerIP
               Port:(NSString *)strPort
         Identifier:(NSString *)strIdentifier;


/** 国家信息中心移动证书项目新增加接口 ***/

/**
 * 自助申请获取请求报文
 * @author  han harvey
 * @param   phoneStr            手机号码
 * @param   uniqueSignStr       唯一标识
 * @param   certType            证书模版类型
 * @param   devID               设备ID
 * @param   strRequestXML      【出参】生成的请求报文
 * @param   strURI             【出参】请求URI
 * @param   reqHeader          【出参】请求头信息
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)getSelfApplyXML:(NSString *)phoneStr
         andUniqueSign:(NSString *)uniqueSignStr
               andType:(int)certType
           andDeviceID:(NSString *)devID
          andRequesXML:(NSString **)strRequestXML
                andURI:(NSString **)strURI
          andReqHeader:(NSDictionary **)reqHeader;

// 模拟Server自助申请
- (int)RequestSelfApplyWithXML:(NSString *)strXML
                         andIP:(NSString *)strIP
                       andPort:(NSString *)strPort
                   andResponse:(NSString **)strResp
                      andError:(NSString **)strErrMessage;

/**
 * 解析自助申请响应报文
 * @author  han harvey
 * @param   strResponseXML         响应报文
 * @return  返回值 0:自助申请成功 其他：错误码
 */
- (int)praseSelfApplyResponseXML:(NSString *)strResponseXML;

/**
 * 短信获取授权码获取请求报文
 * @author  lvyanlong
 * @param   phoneStr        手机号码
 * @param   uniqueSignStr   唯一标识
 * @param   certType        证书模版类型
 * @param   devID           设备ID
 * @param   requestXML     【出参】生成的请求报文
 * @param   strURI         【出参】请求URI
 * @param   reqHeader      【出参】请求头信息
 * @remarks 备注 无
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)getAuthCodeXMLWithPhoneNumber:(NSString *)phoneStr
                           andUnique:(NSString *)uniqueSignStr
                             andType:(int)certType
                         andDeviceID:(NSString *)devID
                       andRequestXML:(NSString **)requestXML
                              andURI:(NSString **)strURI
                        andReqHeader:(NSDictionary **)reqHeader;

// 模拟Server短信获取授权码
- (int)RequestAuthCodeWithXML:(NSString *)strXML
                         andIP:(NSString *)strIP
                       andPort:(NSString *)strPort
                   andResponse:(NSString **)strResp
                      andError:(NSString **)strErrMessage;

/**
 * 解析短信获取授权码响应报文
 * @author  lvyanlong
 * @param   strResponseXML         响应报文
 * @return  返回值 0:自助申请成功 其他：错误码
 */
- (int)praseAuthCodeResponseXML:(NSString *)strResponseXML
                andErrorMessage:(NSString **)strErrorMessage;

/**
 * 生成申请证书的XML报文
 * @author  lvyanlong
 * @param   strAuthorizationCode        授权码
 * @param   strPassword                 证书密码
 * @param   strKeyLength                密钥长度
 * @param   isGM                        是否国密
 * @param   isFingerAuth                是否指纹模式
 * @param   attribute                   附加属性
 * @param   strRequestXML              【出参】生成的XML报文
 * @param   strURI                     【出参】请求URI
 * @param   reqHeader                  【出参】请求头信息
 * @return  返回值 0:成功 其他：错误码
 */
- (int)getRequestCertXML:(NSString *)strAuthorizationCode
                password:(NSString *)strPassword
            andKeyLength:(NSString *)strKeyLength
                    isGM:(BOOL)isGM
            isFingerAuth:(BOOL)isFingerAuth
           andAttributes:(JITReqCertAttribute *)attribute
              requestXML:(NSString **)strRequestXML
                  andURI:(NSString **)strURI
            andReqHeader:(NSDictionary **)reqHeader;

// 模拟Server端申请证书
- (int)RequestCertWithXML:(NSString *)strXML
                    andIP:(NSString *)strIP
                  andPort:(NSString *)strPort
              andResponse:(NSString **)strResp
                 andError:(NSString **)strErrMessage;
/**
 * 解析申请证书的响应XML报文
 * @author  lvyanlong
 * @param   strResponseXML          响应XML报文
 * @param   strPassword             证书密码
 * @param   certType                证书类型
 * @param   errorMessage           【出参】错误信息
 * @return  返回值 0:成功  其他:错误码
 */
- (int)praseCertResponseXML:(NSString *)strResponseXML
                andPassword:(NSString *)strPassword
                andCertType:(CERT_TYPE_MCTK)certType
               errorMessage:(NSString **)errorMessage;


/**
 * 更新/延期证书，申请P10数据
 * @author  han harvey
 * @param   certIndex           证书索引
 * @param   strPassword       证书密码
 * @param   strP10Type          P10类型【UPDATE / DELAY】更新或延期
 * @param   strP10B64          【出参】P10数据
 * @remarks 备注
 * @return  返回值 0:成功  其他:错误码
 */
- (int)GetP10WithCertIndex:(int)certIndex
               andPassword:(NSString *)strPassword
                andP10Type:(NSString *)strP10Type
                 andP10B64:(NSString **)strP10B64;


/**
 * 更新/延期证书，导入证书【根据证书P7B数据】
 * @author  han harvey
 * @param   certIndex                           证书索引
 * @param   strPassword                       证书密码
 * @param   strP7b                                  证书P7b数据
 * @param   strDleP7b                            证书Double P7b数据
 * @param   strDleEncPriKey               被加密的私钥数据
 * @param   strSessionKeyAlg             对称加密算法
 * @param   strDleEncSessionKey       SessionKey
 * @remarks 备注
 * 通过公钥判断是直接替换证书还是保存新证书
 * @return  返回值 0:成功  其他:错误码
 */
- (int)ImportCertWithCertIndex:(int)certIndex
                   andPassword:(NSString *)strPassword
                        andP7b:(NSString *)strP7b
                     andDleP7b:(NSString *)strDleP7b
               andDleEncPriKey:(NSString *)strDleEncPriKey
              andSessionKeyAlg:(NSString *)strSessionKeyAlg
           andDleEncSessionKey:(NSString *)strDleEncSessionKey;

/**
 * 从移动安全管理端获取解锁码
 * @author  han harvey
 * @param   strIP                   移动安全管理系统IP
 * @param   strPort               移动安全管理系统端口
 * @param   strName               解锁人姓名
 * @param   strEmail             邮件
 * @param   strPhone             手机号
 * @param   strApplyDesc    解锁原因
 * @param   bePhone               是否用手机号接收解锁码
 * @remarks 备注
 * @return  0:成功  其他:错误码
 */
- (int)SendRequestForUnlockWithIP:(NSString *)strIP
                          andPort:(NSString *)strPort
                          andName:(NSString *)strName
                         andEmail:(NSString *)strEmail
                         andPhone:(NSString *)strPhone
                     andApplyDesc:(NSString *)strApplyDesc
                       andBePhone:(BOOL)bePhone;

/**
 * 解锁密码模块PIN码
 * @author  han harvey
 * @param   strUnlockCode           解锁码
 * @param   strPIN                          新PIN码
 * @param   retryCount                 重试次数
 * @remarks 备注
 * @return  YES：解锁成功，新PIN码为输入PIN码 NO：解锁失败
 */
- (BOOL)UnLockWithCode:(NSString *)strUnlockCode
             andNewPIN:(NSString *)strPIN
         andRetryCount:(int *)retryCount;


/******************** 证书管理 End ************************/




/******************* 认证管理 Start ***********************/

/**
 * 登录网关，身份认证-有出参
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书唯一标识（路径/序列号/容器名）
 * @param   strPassword     证书密码
 * @param   strGWAddress    网关IP
 * @param   nGWPort         网关端口
 * @param   pAppInfoArray  【出参】从服务端返回的应用信息列表
 每一个元素用一个NSArray接收，每个元素包含5个对象，分别是：
 0）. 对应服务端的应用别名
 1）. 对应服务端的应用名称
 2）. 映射的应用的URL地址
 3）. 应用类型，其对应关系如下：
 1. 正向（逻辑主路）
 2. 反向（逻辑主路）
 3. AGENT（逻辑旁路）
 4. 报文（逻辑旁路）
 4) .已废弃，始终为0
 * @remarks 备注
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)Login:(NSString *)pfxNameOrCertSN
    password:(NSString *)strPassword
   gwAddress:(NSString *)strGWAddress
      gwPort:(UInt16)nGWPort
appInfoArray:(NSMutableArray **)pAppInfoArray;

/**
 * 登录网关-有出参，身份认证，证书PIN码受指纹认证保护
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书唯一标识
 * @param   strGWAddress    网关IP
 * @param   nGWPort         网关端口
 * @param   pAppInfoArray  【出参】从服务端返回的应用信息列表
 每一个元素用一个NSArray接收，每个元素包含5个对象，分别是：
 0）. 对应服务端的应用别名
 1）. 对应服务端的应用名称
 2）. 映射的应用的URL地址
 3）. 应用类型，其对应关系如下：
 1. 正向（逻辑主路）
 2. 反向（逻辑主路）
 3. AGENT（逻辑旁路）
 4. 报文（逻辑旁路）
 4) .已废弃，始终为0
 * @remarks 备注
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)LoginWithFingerprintAuth:(NSString *)pfxNameOrCertSN
                      gwAddress:(NSString *)strGWAddress
                         gwPort:(UInt16)nGWPort
                   appInfoArray:(NSMutableArray **)pAppInfoArray;

/**
 * 登录网关-无出参
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书文件全路径＋名称
 * @param   strGWAddress    网关IP
 * @param   nGWPort         网关端口
 * @remarks 备注
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)LoginEx:(NSString *)pfxNameOrCertSN
      password:(NSString *)strPassword
     gwAddress:(NSString *)strGWAddress
        gwPort:(UInt16)nGWPort;

/**
 * 登录网关-无出参（证书PIN码受指纹认证保护）
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书文件全路径＋名称
 * @param   strGWAddress    网关IP
 * @param   nGWPort         网关端口
 * @remarks 备注
 当storageLocation == CERT_STORAGE_LOCATION_SANDBOX 时，pfxNameOrCertSN为证书路径
 当storageLocation == CERT_STORAGE_LOCATION_KEYCHAIN_JIT_API 时，pfxNameOrCertSN为证书序列号
 当storageLocation == CERT_STORAGE_LOCATION_KEYCHAIN_IOS_API 时，pfxNameOrCertSN为证书序列号
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)LoginWithFingerprintAuthEx:(NSString *)pfxNameOrCertSN
                        gwAddress:(NSString *)strGWAddress
                           gwPort:(UInt16)nGWPort;

/**
 * 创建本地服务，用于正向代理
 * @author  han harvey
 * @param   strServerAddress    应用的IP地址
 * @param   strServerPort       应用的端口号
 * @return  返回值 主机IP:端口号，主机ip固定为127.0.0.1，端口号为被代理的应用的端口号
 */
- (NSString *)CreateLocalServer:(NSString *)strServerAddress
                     serverPort:(NSString *)strServerPort;

/**
 * 关闭本地服务
 * @author  han harvey
 * @param   strLocalServer 从CreateLocalServer返回的字符串，选择要关闭的正向代理服务
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)CloseLocalServer:(NSString *)strLocalServer;


/**
 * 获取授权报文
 * @author  han harvey
 * @param   arrayIndex      证书列表中的索引，请先获取证书列表
 * @param   strAppFlag      应用标示（在服务端配置网关代理应用时，需要填写三项与应用名称相关的项，1.应用名称；2.应用标示；3.应用别名，其中与此参数对应的是第二项：应用标示）
 * @param   strPassword     证书密码
 * @param   isBase64Encode  是否做Base64编码
 * @param   strAuthXML     【出参】，从服务器端接收的授权报文
 * @return  返回值            0:成功 其他值：错误编号
 */
- (int)GetAuthXML:(NSUInteger)arrayIndex
          appFlag:(NSString *)strAppFlag
         password:(NSString *)strPassword
     base64encode:(BOOL)isBase64Encode
          authXML:(NSString **)strAuthXML;

/**
 * 获取授权报文-PIN码受指纹保护
 * @author  han harvey
 * @param   arrayIndex      证书列表中的索引，请先获取证书列表
 * @param   strAppFlag      应用标示（在服务端配置网关代理应用时，需要填写三项与应用名称相关的项，1.应用名称；2.应用标示；3.应用别名，其中与此参数对应的是第二项：应用标示）
 * @param   isBase64Encode  是否做Base64编码
 * @param   strAuthXML     【出参】，从服务器端接收的授权报文
 * @return  返回值            0:成功 其他值：错误编号
 */
- (int)GetAuthXMLWithFingerprintAuth:(NSUInteger)arrayIndex
                             appFlag:(NSString *)strAppFlag
                        base64encode:(BOOL)isBase64Encode
                             authXML:(NSString **)strAuthXML;

/**
 * 获取授权报文-令牌外传
 * @author  han harvey
 * @param   arrayIndex      证书列表中的索引，请先获取证书列表
 * @param   strAppFlag      应用标示（在服务端配置网关代理应用时，需要填写三项与应用名称相关的项，1.应用名称；2.应用标示；3.应用别名，其中与此参数对应的是第二项：应用标示）
 * @param   strPassword     证书密码
 * @param   randomStr       令牌
 * @param   isBase64Encode  是否做Base64编码
 * @param   strAuthXML     【出参】，从服务器端接收的授权报文
 * @return  返回值            0:成功 其他值：错误编号
 */
- (int)GetAuthXML:(NSUInteger)arrayIndex
          appFlag:(NSString *)strAppFlag
         password:(NSString *)strPassword
     andRandomStr:(NSString *)randomStr
     base64encode:(BOOL)isBase64Encode
          authXML:(NSString **)strAuthXML;

/**
 * 获取授权报文-PIN码受指纹保护 -令牌外传
 * @author  han harvey
 * @param   arrayIndex      证书列表中的索引，请先获取证书列表
 * @param   strAppFlag      应用标示（在服务端配置网关代理应用时，需要填写三项与应用名称相关的项，1.应用名称；2.应用标示；3.应用别名，其中与此参数对应的是第二项：应用标示）
 * @param   randomStr       令牌
 * @param   isBase64Encode  是否做Base64编码
 * @param   strAuthXML     【出参】，从服务器端接收的授权报文
 * @return  返回值           0:成功 其他值：错误编号
 */
- (int)GetAuthXMLWithFingerprintAuth:(NSUInteger)arrayIndex
                             appFlag:(NSString *)strAppFlag
                        andRandomStr:(NSString *)randomStr
                        base64encode:(BOOL)isBase64Encode
                             authXML:(NSString **)strAuthXML;

/**
 * 从二维码扫描结果中获取网关IP
 * @param   QRStr 二维码中的原始信息
 * @return  返回值 网关的IP地址，以字符串形式体现
 */
+ (NSString *)GetGwIpFromQRStr:(NSString *)QRStr;

/**
 * 从二维码扫描结果中获取随机数
 * @param   QRStr 二维码中的原始信息
 * @return  返回值 随机数，以字符串形式体现。
 */
+ (NSString *)GetRandomStrFromQRStr:(NSString *)QRStr;

/**
 * 从二维码扫描结果中获取应用名称
 * @author  han harvey
 * @param   QRStr 二维码中的原始信息
 * @return  返回值 应用名称，以字符串形式体现。
 */
+ (NSString *)GetAppNameFromQRStr:(NSString *)QRStr;

/**
 * 从二维码扫描结果中获取二维码生成时间
 * @param   QRStr 二维码中的原始信息
 * @return  返回值 二维码生成时间，以字符串形式体现为距1970年的毫秒值。
 */
+ (NSString *)GetQRTimeFromQRStr:(NSString *)QRStr;

/**
 * 从二维码扫描结果中获取二维码生成时间
 * @param   QRStr   二维码中的原始信息
 * @param   format  时间格式
 * @return  返回值 二维码生成时间，以字符串形式体现为距1970年的毫秒值。
 */
+ (NSString *)GetQRTimeFromQRStrEx:(NSString *)QRStr
                                  :(NSString *)format;

/**
 * 向网关服务端发送二维码信息，相当于告知服务端此二维码已经顺利扫描
 * @param   QRStr 二维码原始信息
 * @remarks 备注 异常描述：
 1. 二维码格式错误：JIT_QR_BAD_STYLE。
 2. 二维码已经过期：使用服务器返回的错误号及错误信息。
 3. 连接服务器超时：JIT_CONNECT_SERVER_TIME_OUT
 * @return  返回值 0:成功 其他：错误码
 */
+ (int)ScanedQRCode:(NSString *)QRStr;

/**
 * 向网关服务端发送二维码信息，相当于告知服务端此二维码已经顺利扫描-> 可外传IP 端口
 * @param   QRStr 二维码原始信息
 * @param   strGWAddress 外传IP地址
 * @param   strPort 外传端口
 * @remarks 备注 异常描述：
 1. 二维码格式错误：JIT_QR_BAD_STYLE。
 2. 二维码已经过期：使用服务器返回的错误号及错误信息。
 3. 连接服务器超时：JIT_CONNECT_SERVER_TIME_OUT
 * @return  返回值 0:成功 其他：错误码
 */
+ (int)ScanedQRCodeEx:(NSString *)QRStr
              andGWIP:(NSString *)strGWAddress
            andGWPort:(NSString *)strPort;

/**
 * 二维码签名授权，改版前
 * @param   QRStr               二维码原始信息
 * @param   CertPath            证书唯一标识
 * @param   PinCode             证书PIN码
 * @param   CustomAttributes    自定义属性值
 * @remarks 备注
 * @return  返回值 0:成功 其他：错误码
 */
- (int)QRAuth:(NSString *)QRStr
             :(NSString *)CertPath
             :(NSString *)PinCode
             :(NSString *)CustomAttributes;

/**
 * 二维码签名授权，改版前-证书PIN码受指纹认证保护
 * @param   QRStr               二维码原始信息
 * @param   CertPath            证书唯一标识
 * @param   CustomAttributes    自定义属性值
 * @remarks 备注
 * @return  返回值 0:成功 其他：错误码
 */
- (int)QRAuthWithFingerprintAuth:(NSString *)QRStr
                                :(NSString *)CertPath
                                :(NSString *)CustomAttributes;

/**
 * 二维码签名授权 -> IP 端口可外传
 * @param   QRStr               二维码原始信息
 * @param   CertPath            证书唯一标识
 * @param   PinCode             证书PIN码
 * @param   CustomAttributes    自定义属性值
 * @remarks 备注
 * @return  返回值 0:成功 其他：错误码
 */
- (int)QRAuthEx:(NSString *)QRStr
               :(NSString *)CertPath
               :(NSString *)PinCode
               :(NSString *)strGWAddress
               :(NSString *)strPort
               :(NSString *)CustomAttributes;

/**
 * 二维码签名授权，PIN码受指纹认证保护 -> IP 端口可外传
 * @param   QRStr               二维码原始信息
 * @param   CertPath            证书唯一标识
 * @param   CustomAttributes    自定义属性值
 * @remarks 备注
 * @return  返回值 0:成功 其他：错误码
 */
- (int)QRAuthWithFingerprintAuthEx:(NSString *)QRStr
                                  :(NSString *)CertPath
                                  :(NSString *)strGWAddress
                                  :(NSString *)strPort
                                  :(NSString *)CustomAttributes;

/**
 * 二维码扫描授权，改版后
 * @param   qrCode              二维码扫描信息
 * @param   CustomAttributes    自定义属性值
 * @remarks 备注 端口号默认443
 * @return  返回值 0:成功 其他：错误码
 */
- (int)QRAuthorize:(NSString *)qrCode
                  :(NSString *)CustomAttributes;

/**
 * 二维码扫描授权，改版后-可以使用外部传进来的网关地址及端口
 * @author  han harvey
 * @param   qrCode              二维码扫描信息
 * @param   CustomAttributes    自定义属性值
 * @param   strGWAddress        网关地址
 * @param   strPort             端口号
 * @remarks 备注 无
 * @return  返回值 0:成功 其他：错误码
 */
- (int)QRAuthorizeEx:(NSString *)qrCode
                    :(NSString *)CustomAttributes
                    :(NSString *)strGWAddress
                    :(NSString *)strPort;

/**
 * 二维码扫描授权 （和报文认证同时使用）
 * @param   qrCode              二维码扫描信息
 * @param   tokenStr            Token
 * @param   pubKeyStr           公钥证书（可选参数，可以传空串）
 * @param   CustomAttributes    自定义属性值
 * @param   xmlStr             【出参】xml报文信息（如果传入公钥证书出参为xml报文的信封结果，如果传入空串出参为xml报文的base64str形式）
 * @remarks 备注 端口号默认443
 * @return  返回值 0:成功 其他：错误码
 */
- (int)QRAuthorize:(NSString *)qrCode
          andToken:(NSString *)tokenStr
      andPubKeyB64:(NSString *)pubKeyStr
     andCustomAttr:(NSString *)CustomAttributes
         andXMLStr:(NSString **)xmlStr;

/**
 * 检测网关是否可以通讯，仅检测作用
 * @author  han harvey
 * @param   strGWAddress    网关地址
 * @param   nGWPort         网关端口号，默认端口号443
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)CheckGateWayConfig:(NSString*)strGWAddress
                   gwPort:(UInt16)nGWPort;

/**
 * 注销登录
 * @author  han harvey
 * @remarks 备注 此方法配合登录网关使用
 * @return  返回值 0:成功 其他:错误码
 */
- (int)QRAuthorizeLogout;

/**
 * 根据应用属性键值获取应用属性
 * @author  han harvey
 * @param   strServerAddress    应用的IP地址
 * @param   strServerPort       应用的端口
 * @param   strAttributeKey     应用属性键值，当网关服务端为应用配置模板时才会用到，该值在应用集成时可以联系网关管理员获得
 * @param   pstrAttributeValue 【出参】属性值字符串
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)GetAppAttribute:(NSString *)strServerAddress
            serverPort:(NSString *)strServerPort
          attributeKey:(NSString *)strAttributeKey
        attributeValue:(NSString **)pstrAttributeValue;

/*
 关于MCS的说明：
 MCS：mobile cert service 移动证书服务，是为移动端申请证书设计的服务
 移动端可以通过该服务申请证书，一般步骤为：MCS管理员通过后台自助证书
 申请功能菜单向系统提交证书申请，系统返回证书申请授权码，将此授权码输入
 到移动端并配置MCS地址及端口，即可从MCS申请到P12的证书，授权码只能
 使用一次，MCS的证书申请默认端口号为50625。
 */

/**
 * 检测MCS服务器是否可以正常服务（不是简单的网络连接检测，已经包含功能检测）
 * @author  han harvey
 * @param   mcsAddress  MCS服务器IP地址
 * @param   mcsPort     MCS服务器端口地址
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)CheckMCSConfig:(NSString*)mcsAddress
                 Port:(NSString *)mcsPort;


/**
 * 报文认证
 * @author  han harvey
 * @param   strServerIP     服务器IP，可以是应用服务器IP，也可以是网关IP
 * @param   strPort         服务器端口号，网关默认6180也支持443
 * @param   strURI          接口URI
 * @param   bIsSSL          是否使用SSL加密通讯
 * @param   strCertPath     证书唯一标识
 * @param   strPassword     证书密码
 * @param   strAppid        应用标识
 * @param   isAccessControl xxx
 * @param   responseStr   xxx
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)AuthCredential:(NSString *)strServerIP
                 Port:(NSString *)strPort
                  URI:(NSString *)strURI
                IsSSL:(BOOL)bIsSSL
                 Cert:(NSString *)strCertPath
             Password:(NSString *)strPassword
                Appid:(NSString *)strAppid
        AccessControl:(BOOL)isAccessControl
             Response:(NSString **)responseStr;

/**
 * 报文认证，证书PIN码受指纹认证保护
 * @author  han harvey
 * @param   strServerIP     服务器IP，可以是应用服务器IP，也可以是网关IP
 * @param   strPort         服务器端口号，网关默认6180也支持443
 * @param   strURI          接口URI
 * @param   bIsSSL          是否使用SSL加密通讯
 * @param   strCertPath     证书唯一标识
 * @param   strAppid        应用标识
 * @param   isAccessControl xxx
 * @param   responseStr   xxx
 * @return  返回值 0:成功 其他值：错误编号
 */
- (int)AuthCredentialWithFingerprintAuth:(NSString *)strServerIP
                                    Port:(NSString *)strPort
                                     URI:(NSString *)strURI
                                   IsSSL:(BOOL)bIsSSL
                                    Cert:(NSString *)strCertPath
                                   Appid:(NSString *)strAppid
                           AccessControl:(BOOL)isAccessControl
                                Response:(NSString **)responseStr;

/**
 * 设置做解信封业务的证书的PIN码
 * @author  han harvey
 * @param   strPin 证书的PIN码
 * @remarks 备注 证书只有存储在沙盒中时，才方法可用
 * @return  返回值   0:成功 其他：失败
 */
-(int)SetLoginPin:(NSString *)strPin;

/**
 * 创建设备ID
 * @author  han harvey
 * @return  返回值 设备ID
 */
+ (NSString *)CreateDeviceID;



/**
* 通过手机短信获取申请证书时使用的授权码 （广州）
* @param   phoneStr        手机号码
* @param   ipStr           IP地址
* @param   portStr         端口

* @return  返回值 0:成功 其他:错误码
*/
- (int)getAuthCodeWithPhoneNumber_GZ:(NSString *)phoneStr andIP:(NSString *)ipStr andPort:(NSString *)portStr;
/**
 * 通过手机短信获取申请证书时使用的授权码（无设备ID）
 * @author  han harvey
 * @param   phoneStr        手机号码
 * @param   uniqueSignStr   唯一标识
 * @param   certType        证书类型
 * @param   ipStr           IP地址
 * @param   portStr         端口
 * @remarks 备注             唯一标识从IMS处获得
 * @return  返回值 0:成功 其他:错误码
 */
- (int)getAuthCodeWithPhoneNumber:(NSString *)phoneStr
                    andUniqueSign:(NSString *)uniqueSignStr
                          andType:(NSString *)certType
                            andIP:(NSString *)ipStr
                          andPort:(NSString *)portStr;

/**
 * 通过手机短信获取申请证书时使用的授权码(有设备ID)
 * @author  han harvey
 * @param   phoneStr        手机号码
 * @param   uniqueSignStr   唯一标识
 * @param   certType        证书类型
 * @param   devID           设备ID
 * @param   ipStr           IP地址
 * @param   portStr         端口
 * @remarks 备注             唯一标识从IMS处获得
 * @return  返回值 0:成功 其他:错误码
 */
- (int)getAuthCodeWithPhoneNumber:(NSString *)phoneStr
                    andUniqueSign:(NSString *)uniqueSignStr
                          andType:(NSString *)certType
                      andDeviceID:(NSString *)devID
                            andIP:(NSString *)ipStr
                          andPort:(NSString *)portStr;

/**
 * 申请证书-自助申请 (可传入设备ID，自助申请)
 * @author  han harvey
 * @param   phoneStr        手机号码
 * @param   uniqueSignStr   唯一标识
 * @param   devID           设备ID
 * @param   certType        证书类型
 * @param   ipStr           IP地址
 * @param   portStr         端口
 * @remarks 备注 唯一标识从IMS处获得
 * @return  返回值 0:成功 其他:错误码
 */
- (int)getAuthCodeWithPhoneNumberEx:(NSString *)phoneStr
                      andUniqueSign:(NSString *)uniqueSignStr
                        andDeviceID:(NSString *)devID
                            andType:(NSString *)certType
                              andIP:(NSString *)ipStr
                            andPort:(NSString *)portStr;

/**
 * 申请证书-自助申请 (可传入设备ID及附加属性，自助申请)
 * @author  han harvey
 * @param   phoneStr        手机号码
 * @param   uniqueSignStr   唯一标识
 * @param   devID           设备ID
 * @param   certType        证书类型
 * @param   ipStr           IP地址
 * @param   portStr         端口
 * @param   attribute       自定义属性
 * @remarks 备注
 唯一标识从IMS处获得
 * @return  返回值 0:成功 其他:错误码
 */
- (int)getAuthCodeWithPhoneNumberEx:(NSString *)phoneStr
                      andUniqueSign:(NSString *)uniqueSignStr
                        andDeviceID:(NSString *)devID
                            andType:(NSString *)certType
                              andIP:(NSString *)ipStr
                            andPort:(NSString *)portStr
                            andAttr:(NSDictionary *)attribute;


/**
 * 将PKCS12Data导入keychain
 * @author  han harvey
 * @param   p12Data         p12证书的NSData格式
 * @param   password        p12证书的密码
 * @param   errorMessage   【出参】错误信息
 * @remarks 备注
 * @return  返回值 0:成功 其他:错误编码
 */
- (int)importP12DataToKeyChainWithData:(NSData *)p12Data
                           andPassword:(NSString *)password
                       andErrorMessage:(NSString **)errorMessage;


/**
 * 设置证书PIN码受指纹认证保护
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书唯一标识
 * @param   passStr         证书密码
 * @remarks 备注 当证书存储模式时，此方法不可用
 * @return  返回值 0表示成功，非0表示失败
 */
- (int)ProtectCertPINCode:(NSString *)pfxNameOrCertSN
              andPassword:(NSString *)passStr;

/**
 * 解绑PIN码受指纹认证保护
 * @author  han harvey
 * @param   pfxNameOrCertSN 证书唯一标识
 * @param   passStr         证书PIN码
 * @remarks 备注 当证书存储模式时，此方法不可用
 * @return  返回值 0:解绑成功 其他：解绑失败，错误码
 */
- (int)unbindCertPINCodeProtected:(NSString *)pfxNameOrCertSN
                      andPassword:(NSString *)passStr;

/**
 * OAuth认证
 * @author  han harvey
 * @param   gwIPStr         网关IP
 * @param   gwPortStr       网关端口
 * @param   pfxNameOrCertSN 证书唯一标识
 * @param   passStr         证书密码
 * @param   clientAPPID     客户端应用APP ID
 * @param   redirectURI     客户端应用重定向地址
 * @param   oauthAttribute  OAuth认证-附加参数(可选，不填请传nil）
 * @param   authCodeStr    【出参】授权码
 * @remarks 备注 无
 * @return  返回值 0成功 其他值均为错误码
 */
- (int)oAuthSingleStepWithGWIP:(NSString *)gwIPStr
                     andGWPort:(NSString *)gwPortStr
                   andCertPath:(NSString *)pfxNameOrCertSN
                   andPassword:(NSString *)passStr
                andClientAPPID:(NSString *)clientAPPID
                andRedirectURI:(NSString *)redirectURI
                 andCustomAttr:(JITOAuthAttribute *)oauthAttribute
                   andAuthCode:(NSString **)authCodeStr;


/**
 * OAuth认证（指纹模式下使用）
 * @author  han harvey
 * @param   gwIPStr         网关IP
 * @param   gwPortStr       网关端口
 * @param   pfxNameOrCertSN 证书唯一标识
 * @param   clientAPPID     客户端应用APP ID
 * @param   redirectURI     客户端应用重定向地址
 * @param   oauthAttribute  OAuth认证-附加参数
 * @param   authCodeStr    【出参】授权码
 * @remarks 备注 无
 * @return  返回值 0成功 其他值均为错误码
 */
- (int)oAuthSingleStepWithGWIPWithFingerprint:(NSString *)gwIPStr
                                    andGWPort:(NSString *)gwPortStr
                                  andCertPath:(NSString *)pfxNameOrCertSN
                               andClientAPPID:(NSString *)clientAPPID
                               andRedirectURI:(NSString *)redirectURI
                                andCustomAttr:(JITOAuthAttribute *)oauthAttribute
                                  andAuthCode:(NSString **)authCodeStr;


/**
 * FIDO注册
 * @author  han harvey
 * @param   jsonReqStr          FIDO注册参数
 * @param   certIndex           注册使用证书的索引值
 * @param   passStr             注册使用证书的密码
 * @param   responseURLStr     【出参】FIDO注册响应URL,字符串
 * @remarks 备注
 注册参数：（例）
 {"message":{"uafProtocolMessage":"[{\"username\":\"\",\"header\":{\"serverData\":\"ZXhwaXJlRGF0ZTo6OjIwMjEtMTItMTMgMDI6NDc6MDk7VmVyc2lvbjo6OjEuMDo6OjZhYTc0YTI3LTAxMTQtNGEwNi1iNjU0LWFhYTIzMTEzNjZlYg\",\"appID\":\"\",\"op\":\"Reg\",\"upv\":{\"major\":\"1\",\"minor\":\"0\"}},\"challenge\":\"MC4zMDk1MzI1OTkwNjE0NDU1\",\"policy\":{\"accepted\":[{\"aaid\":\"jits#0001\"}]}}]"},"channelBindings":{}}
 * @return  返回值 0：成功 其他值：错误码
 */
- (int)JITFIDORegistWithJSONReq:(NSString *)jsonReqStr
                   andCertIndex:(int)certIndex
                andCertPassword:(NSString *)passStr
                    andResponse:(NSString **)responseURLStr;

/**
 * FIDO注册 - 密码受指纹保护
 * @author  han harvey
 * @param   jsonReqStr          FIDO注册参数
 * @param   certIndex           注册使用证书的索引值
 * @param   responseURLStr     【出参】FIDO注册响应URL,字符串
 * @remarks 备注
 注册参数：（例）
 {"message":{"uafProtocolMessage":"[{\"username\":\"\",\"header\":{\"serverData\":\"ZXhwaXJlRGF0ZTo6OjIwMjEtMTItMTMgMDI6NDc6MDk7VmVyc2lvbjo6OjEuMDo6OjZhYTc0YTI3LTAxMTQtNGEwNi1iNjU0LWFhYTIzMTEzNjZlYg\",\"appID\":\"\",\"op\":\"Reg\",\"upv\":{\"major\":\"1\",\"minor\":\"0\"}},\"challenge\":\"MC4zMDk1MzI1OTkwNjE0NDU1\",\"policy\":{\"accepted\":[{\"aaid\":\"jits#0001\"}]}}]"},"channelBindings":{}}
 * @return  返回值 0：成功 其他值：错误码
 */
- (int)JITFIDORegistWithFingerprintAuthAndJSONReq:(NSString *)jsonReqStr
                                     andCertIndex:(int)certIndex
                                      andResponse:(NSString **)responseURLStr;

/**
 * FIDO认证
 * @author  han harvey
 * @param   jsonReqStr          FIDO认证参数
 * @param   certIndex           认证使用证书的索引值
 * @param   passStr             认证使用证书的密码
 * @param   responseURLStr     【出参】FIDO注册响应URL,字符串
 * @remarks 备注 无
 认证参数：（例）
 {"message":{"uafProtocolMessage":"[{\"challenge\":\"MC45MzcwMzAzNDM3OTQzMDg2\",\"policy\":{\"accepted\":[{\"aaid\":\"jits#0001\"}]},\"header\":{\"serverData\":\"ZXhwaXJlRGF0ZTo6OjIwMjEtMTItMTMgMDI6NTE6NTA7VmVyc2lvbjo6OjEuMDo6OjZkMDY3MjkyLTExNjEtNDkzMC05YjhiLWI1OTk5Yzk2N2I3Ng\",\"appID\":\"\",\"op\":\"Auth\",\"upv\":{\"major\":\"1\",\"minor\":\"0\"}}}]"},"channelBindings":{}}
 * @return  返回值 0：成功 其他值：错误码
 */
- (int)JITFIDOAuthWithJSONReq:(NSString *)jsonReqStr
                 andCertIndex:(int)certIndex
              andCertPassword:(NSString *)passStr
                  andResponse:(NSString **)responseURLStr;

/**
 * FIDO认证 - 密码受指纹保护
 * @author  han harvey
 * @param   jsonReqStr          FIDO认证参数
 * @param   certIndex           认证使用证书的索引值
 * @param   responseURLStr     【出参】FIDO注册响应URL,字符串
 * @remarks 备注 无
 认证参数：（例）
 {"message":{"uafProtocolMessage":"[{\"challenge\":\"MC45MzcwMzAzNDM3OTQzMDg2\",\"policy\":{\"accepted\":[{\"aaid\":\"jits#0001\"}]},\"header\":{\"serverData\":\"ZXhwaXJlRGF0ZTo6OjIwMjEtMTItMTMgMDI6NTE6NTA7VmVyc2lvbjo6OjEuMDo6OjZkMDY3MjkyLTExNjEtNDkzMC05YjhiLWI1OTk5Yzk2N2I3Ng\",\"appID\":\"\",\"op\":\"Auth\",\"upv\":{\"major\":\"1\",\"minor\":\"0\"}}}]"},"channelBindings":{}}
 * @return  返回值 0：成功 其他值：错误码
 */
- (int)JITFIDOAuthWithFingerprintAuthAndJSONReq:(NSString *)jsonReqStr
                                   andCertIndex:(int)certIndex
                                    andResponse:(NSString **)responseURLStr;


/**
 * 获取多因子需要的令牌
 * @author  han harvey
 * @param   srcData 原文
 * @remarks 备注 无
 * @return  令牌
 */
- (NSString *)getUniqueID:(NSData *)srcData;

/**
 * 将用户标识和设备标识注册到多因子服务器
 * @author  han harvey
 * @param   userID      用户标识
 * @param   devID       设备标识
 * @param   serverIP    服务器IP
 * @param   serverPort  服务器端口
 * @remarks 备注 无
 * @return  返回值 0：成功 其他值：错误码
 */
- (int)regInfo:(NSString *)userID
      andDevID:(NSString *)devID
   andServerIP:(NSString *)serverIP
 andServerPort:(int)serverPort;

/**
 * 获取离线令牌
 * @author  han harvey
 * @param   userID  用户标识
 * @param   devID   设备标识
 * @param   times   获取令牌时间，格式：yyyyMMddHHmm，时区等信息需预先转换好，长度必须12个字符
 * @param   token  【出参】设备标识
 * @remarks 备注
 此接口需在regInfo接口注册成功之后再调用，否则无法和服务器令牌同步
 * @return  返回值 0：成功 其他值：错误码
 */
- (int)getToKenWithUserID:(NSString *)userID
                 andDevID:(NSString *)devID
                  andTime:(NSString *)times
                 andToken:(NSString **)token;


/**
 * 将用户标识和设备标识从多因子服务器移除
 * @author  han harvey
 * @param   userID      用户标识
 * @param   devID       设备标识
 * @param   serverIP    服务器IP
 * @param   serverPort  服务器端口
 * @remarks 备注 无
 * @return  返回值 0：成功 其他值：错误码
 */
- (int)unRegInfo:(NSString *)userID
        andDevID:(NSString *)devID
     andServerIP:(NSString *)serverIP
   andServerPort:(int)serverPort;


/**
 * 获取认证Token
 */
- (NSString *)GetToken;

/**
 * 重置认证Token
 */
- (void)ResetToken;


/******************** 认证管理 End ************************/




/******************* 密码服务 Start ***********************/

/**
 * 验证密码模块的PIN码【此接口只有当存储模式是密码模块的时候才能使用】
 * @author  han harvey
 * @param   strPWD               密码模块的PIN码
 * @param   retryCount      剩余重试次数
 * @remarks 备注
 * @return  返回值 0:成功 其他值:错误编号
 */
- (int)VerifyPWD:(NSString *)strPWD andRetryCount:(int *)retryCount;

/**
 * 设置做签名业务的证书
 * @author  han harvey
 * @param   strPFXNameOrCertSN  UniqueID
 * @param   strPassword         证书的PIN码
 * @remarks 备注
 * @return  返回值 0:成功 其他值:错误编号
 */
-(int)SetCert:(NSString *)strPFXNameOrCertSN
     password:(NSString *)strPassword;

/**
 * 设置做签名业务的证书，证书PIN受指纹认证保护
 * @author  han harvey
 * @param   strPFXNameOrCertSN  证书的完整路径或者证书的SN
 * @remarks 备注 1.0.6新加功能
 当storageLocation == CERT_STORAGE_LOCATION_SANDBOX 时，strPFXNameOrCertSN为证书的全路径
 当storageLocation == CERT_STORAGE_LOCATION_KEYCHAIN_JIT_API 时，strPFXNameOrCertSN为证书的SN（即序列号）
 当storageLocation == CERT_STORAGE_LOCATION_KEYCHAIN_IOS_API 时，strPFXNameOrCertSN为证书的SN（即序列号）PIN码可以任意输入，不使用
 * @return  返回值 0:成功 其他值:错误编号
 */
-(int)SetCertFingerprintAuth:(NSString *)strPFXNameOrCertSN;

/**
 * P1签名【先做摘要再做签名】
 * @author  han harvey
 * @param   pSrcData p1签名原文
 * @return  返回值    p1签名结果Base64
 */
- (NSString *)P1Sign:(NSData *)pSrcData;


/**
 * P1验签名【先做摘要再做签名】
 * @author  han harvey
 * @param   pSrcData                p1签名原文
 * @param   strPublicKeyCertBase64  p1验签名的公钥证书Base64
 * @param   strSignDataBase64       p1签名结果
 * @return  返回值 0:验签成功 其他:失败
 */
- (int)VerifyP1Sign:(NSData *)pSrcData
publicKeyCertBase64:(NSString *)strPublicKeyCertBase64
     SignDataBase64:(NSString *)strSignDataBase64;


/**
 * P1签名【直接做签名】
 * @author  han harvey
 * @param   pSrcData p1签名原文
 * @return  返回值    p1签名结果Base64
 */
- (NSString *)P1SignWithSrcData:(NSData *)pSrcData;

/**
 * P1验签名【直接做签名】
 * @author  han harvey
 * @param   pSrcData                p1签名原文
 * @param   strPublicKeyCertBase64  p1验签名的公钥证书Base64
 * @param   strSignDataBase64       p1签名结果
 * @return  返回值 0:验签成功 其他:失败
 */
- (int)VerifyP1SignWithSrcData:(NSData *)pSrcData
           publicKeyCertBase64:(NSString *)strPublicKeyCertBase64
                SignDataBase64:(NSString *)strSignDataBase64;


/**
 * Attach签名
 * @author  han harvey
 * @param   pSrcData 做签名的原数据
 * @return  返回值    签名结果Base64
 */
- (NSString *)AttachSign:(NSData *)pSrcData;

/**
 * Attach验签名
 * @author  han harvey
 * @param   strSignDataBase64 签名结果Base64
 * @return  返回值             做签名的原数据
 */
- (NSData *)VerifyAttachSign:(NSString *)strSignDataBase64;


/**
 *MOF签名
 *@parama   inData  签名原文
 *@parama   tsaData     tsa数据（可以传空）
 *@parama   serverInfoData  服务器所需内容
 *@parama   nFlag   标识
 *@return 签名结果
 */
- (NSString *)MOFSign:(NSData *)inData
           andTsaData:(NSData *)tsaData
        andServerInfo:(NSData *)serverInfoData
              andFlag:(int)nFlag;

/**
 *MOF验签
 *@parama   inData  签名结果
 *@parama   srcData     签名原文
 *@parama   nFlag       标识
 *@parama   outData     出参
 *@parama   tsaData     出参
 *@parama   serverData  出参
 *@parama   UTCTimeData 出参
 *@parama   outPubCertData  出参
 *@parama   hashName        出参
 */

- (int)VerifyMOFSign:(NSData *)inData
          andSrcData:(NSData *)srcData
             andFlag:(int)nFlag
          andOutData:(NSData **)outData
          andTsaData:(NSData **)tsaData
       andServerData:(NSData **)serverData
          andUTCTime:(NSData **)UTCTimeData
          andPubCert:(NSData **)outPubCertData
         andHashName:(NSData **)hashName;

/**
 * 初始化做业务时可能用到的资源
 * @author  han harvey
 * @param   strReserve 储备参数，以后可能用到
 * @return  返回值 0:成功 其他:失败
 */
- (int)DoInitalize:(NSString *)strReserve;

/**
 * Detach签名
 * @author  han harvey
 * @param   pSrcData 做签名的原数据
 * @return  返回值    签名结果Base64
 */
- (NSString *)DetachSign:(NSData *)pSrcData;

/**
 * Detach验签名
 * @author  han harvey
 * @param   pSrcData            做签名的原数据
 * @param   strSignDataBase64   detach签名结果Base64
 * @return  返回值
 */
- (int)VerifyDetachSign:(NSData *)pSrcData SignData:(NSString *)strSignDataBase64;


/**
 * 从P7数据中获取公钥证书
 * @author  han harvey
 * @param   strP7SignData   签名结果
 * @remarks 备注 无
 * @return  返回值 证书Base64
 */
- (NSString *)GetPubCertFromP7SignData:(NSString *)strP7SignData;


/**
 * 检验一个证书是否是另一个证书的根证书
 * @author  han harvey
 * @param   strRootCertB64      根证书
 * @param   strPubCertB64       待验证公钥证书
 * @remarks 备注 无
 * @return  返回值 YES:strRootCertB64是strPubCertB64的根证书，NO:strRootCertB64不是strPubCertB64的根证书
 */
- (BOOL)CheckIsRootCert:(NSString *)strRootCertB64
             andPubCert:(NSString *)strPubCertB64;

/**
 * 国密证书获取公钥数据
 */
- (NSData *)getPubKeyFromGMCert:(NSString *)strCertB64;
/**
 * 打信封，支持SM2和RSA p7信封
 * @author  han harvey
 * @param   alg         对称加密算法，SM4:"SM4-ECB"  DES-EDE3:"DES-EDE3-ECB"
 * @param   certB64     证书Base64
 * @param   srcData     原始数据
 * @param   valB64      输出的信封Base64
 * @return  返回值       错误标号，0:成功，1:失败
 */
- (int)EncryptEnvelop:(NSString *)alg
                     :(NSString *)certB64
                     :(NSData *)srcData
                     :(NSString **)valB64;


/**
 * 打信封，支持SM2和RSA p7信封
 * @author  han harvey
 * @param   alg         对称加密算法，SM4:"SM4-ECB"  DES-EDE3:"DES-EDE3-ECB"
 * @param   certB64     证书Base64
 * @param   srcData     原始数据
 * @param   valB64      输出的信封Base64
 * @remarks 备注         国密只支持SM4:"SM4-ECB"
 * @return  返回值       错误标号，0:成功，1:失败
 */
- (int)EncryptEnvelopEx:(NSString *)alg
    publicKeyCertBase64:(NSString *)certB64
                srcData:(NSString *)srcData
        envResultBase64:(NSString **)valB64;

/**
 * 打信封（RSA）
 * @author  han harvey
 * @param   pSrcData                原始数据
 * @param   strPublicKeyCertBase64  证书Base64
 * @remarks 备注 目前只支持3DES
 * @return  返回值                   打信封结果Base64
 */
- (NSString *)EncryptEnvelop:(NSData *)pSrcData
         publicKeyCertBase64:(NSString *)strPublicKeyCertBase64;

/**
 * 做信封-（支持多人信封）
 * @author  han harvey
 * @param   multiPubCertB64 公钥证书集合（单个证书就是单人信封，多个证书就是多人信封）
 * @param   strSymmetricAlg 对称算法(支持SM4-ECB)
 * @param   inputStream         输入流（加密原文）
 * @param   srcDatalen          输入流数据长度
 * @param   outputStreamForKey  输出流1(对称密钥)
 * @param   outputStreamForVal  输出流2(对称加密结果)
 * @remarks 备注 无
 只支持SM4-ECB算法
 如果bSuccess=YES -> 做多人信封成功
 如果bSuccess=NO  -> 做对人信封失败 message为错误原因
 */
- (void)EncryptEnvelopMulti:(NSArray *)multiPubCertB64
               symmetricAlg:(NSString *)strSymmetricAlg
                inputStream:(NSInputStream *)inputStream
                 srcDataLen:(int)srcDatalen
         outputStreamForKey:(NSOutputStream *)outputStreamForKey
         outputStreamForVal:(NSOutputStream *)outputStreamForVal
                    handler:(void(^)(BOOL bSuccess, NSString *message))handler;

/**
 * 解信封-（支持多人信封）
 * @author  han harvey
 * @param   strSymmetricAlg         对称算法
 * @param   encEnvelopDataLen       信封总长度
 * @param   inputStreamForKey       输入流（对称密钥信封结果）
 * @param   inputStreamForVal       输入流（对称加密结果）
 * @param   outputStream            输出流（加密原文）
 * @remarks 备注 无
 只支持SM4-ECB算法
 如果bSuccess=YES -> 解多人信封成功
 如果bSuccess=NO  -> 解对人信封失败 message为错误原因
 */
- (void)DecryptEnvelopMulti:(NSString *)strSymmetricAlg
          encEnvelopDataLen:(int)encEnvelopDataLen
          inputStreamForKey:(NSInputStream *)inputStreamForKey
          inputStreamForVal:(NSInputStream *)inputStreamForVal
               outputStream:(NSOutputStream *)outputStream
                    handler:(void(^)(BOOL bSuccess, NSString *message))handler;


/**
 * 打信封 (流式信封)
 * @author  han harvey
 * @param   alg             对称加密算法，SM4:"SM4-ECB"  DES-EDE3:"DES-EDE3-ECB"
 * @param   certB64         证书Base64
 * @param   srcDataLen      数据源的总长度
 * @param   inputStream     输入流
 * @param   outputStream    输出流
 * @remarks 备注         国密只支持SM4:"SM4-ECB"
 如果bSuccess=YES -> 打信封成功
 如果bSuccess=NO  -> 打信封失败 message为错误原因
 */
- (void)EncryptEnvelopStreamEx:(NSString *)alg
                       andCert:(NSString *)certB64
                 andSrcDataLen:(long long)srcDataLen
                   inputStream:(NSInputStream *)inputStream
                  outputStream:(NSOutputStream *)outputStream
                       handler:(void(^)(BOOL bSuccess, NSString *message))handler;


/**
 * 解信封 (流式信封)
 * @author  lvyanlong
 * @param   strSymetricAlg  对称加密算法，SM4:"SM4-ECB"
 * @param   intputStream    输入流(必须为文件流)
 * @param   outputStream    输出流
 * @remarks 备注         国密只支持SM4:"SM4-ECB"
 如果bSuccess=YES -> 解流式信封成功
 如果bSuccess=NO  -> 解流式信封失败 message为错误原因
 */
- (void)DecryptEnvelopStream:(NSString *)strSymetricAlg
                intputStream:(NSInputStream *)intputStream
                outputStream:(NSOutputStream *)outputStream
                     handler:(void (^)(BOOL, NSString *))handler;

/**
 * 解信封，支持SM2 p7解信封
 * @author  han harvey
 * @param   alg             对称加密算法，SM4:"SM4-ECB"(如果用密码模块内的证书还可以是"SM4-CBC")  DES-EDE3:"DES-EDE3-CBC"
 * @param   encValBase64    加密结果Base64
 * @remarks 备注 无
 * @return  返回值           解密原文
 */
- (NSData *)DecryptEnvelopEx:(NSString *)alg
                andEncValStr:(NSString *)encValBase64;


/**
 * 解信封 (RSA)
 * @author  han harvey
 * @param   strEvpDataBase64 打信封结果Base64
 * @remarks 备注 证书只有存储在沙盒中或者iOS KeyChain时，此方法可用
 * @return  返回值            原始数据
 */
- (NSData *)DecryptEnvelop:(NSString *)strEvpDataBase64;

/**
 * 设置摘要算法 不设置默认为SHA1
 * @author  han harvey
 * @param   strDigestAlg 摘要算发名称
 * @remarks 备注 此方法适用于设置rsa算法中的摘要算法，国密只支持SM3，无需自己设置
 * @return  返回值 0:成功 其他:失败
 */
-(int)SetDigestAlg:(NSString *)strDigestAlg;

/**
 * Hash
 * @author  han harvey
 * @param   pSrcData 要做Hash的数据源
 * @return  返回值 16进制数据依次拼接成的字符串
 */
- (NSString *)Digest:(NSData *)pSrcData;


/**
 * Hmac摘要
 * @author  han harvey
 * @param   pSrcData     原数据
 * @param   hmacHash      hmac摘要算法
 * @param   keyData      密钥
 * @remarks 备注 无
 * @return  返回值 hmac数据base64形式
 */
- (NSString *)HmacHashWithSrc:(NSData *)pSrcData
                  withHmacAlg:(MCTK_HmacHash)hmacHash
                  withKeyData:(NSData *)keyData;


/**
 * 设置对称算法 不设置默认为DES3-CBC
 * @author  han harvey
 * @param   strSymmetricAlg 对称加密算法名称
 * @return  返回值   0:成功 其他：失败
 */
-(int)SetSymmetricAlg:(NSString *)strSymmetricAlg;

/**
 * 随机数
 * @author  han harvey
 * @param   nRandomNumberLen 随机数长度
 * @return  返回值  随机数NSData格式数据
 */
- (NSData *)RandomNumber:(int)nRandomNumberLen;


/**
 *生成对称密钥
 *@param  nKeyLength    密钥长度
 *@return 对称密钥
 *
 AES-128-ECB    -> KeyLength:16
 AES-128-CBC    -> KeyLength:16
 DES3-ECB         -> KeyLength:24
 DES3-CBC         -> KeyLength:24
 SM4-ECB           -> KeyLength:16
 */
- (NSData *)GenerateSymmKey:(int)nKeyLength;
/**
 * 对称加密
 * @author  han harvey
 * @param   pSymmetricKey 加密密钥
 * @param   ivData  IV值
 * @param   strSymmAlg  对称算法
 * @param   pSrcData 原始数据
 * @remarks 备注
 目前只支持AES-128-ECB、AES-128-CBC、DES3-ECB、DES3-CBC、SM4-ECB等算法。
 AES-128-ECB    -> KeyLength:16
 AES-128-CBC    -> KeyLength:16  ivLength:16
 DES3-ECB         -> KeyLength:24
 DES3-CBC         -> KeyLength:24  ivLength:8
 SM4-ECB           -> KeyLength:16
 * @return  返回值 对称加密结果Base64
 */
- (NSString *)SymmetricEncrypt:(NSData *)pSymmetricKey
                            iv:(NSData *)ivData
                  symmetricAlg:(NSString *)strSymmAlg
                       srcData:(NSData *)pSrcData;

/**
 * 对称解密
 * @author  han harvey
 * @param   pSymmetricKey           解密密钥
 * @param   ivData  IV值
 * @param   strSymmAlg  对称算法
 * @param   strEncryptDataBase64    加密结果
 * @remarks 备注
 目前只支持AES-128-ECB、AES-128-CBC、DES3-ECB、DES3-CBC、SM4-ECB等算法。
 AES-128-ECB    -> KeyLength:16
 AES-128-CBC    -> KeyLength:16  ivLength:16
 DES3-ECB         -> KeyLength:24
 DES3-CBC         -> KeyLength:24  ivLength:8
 SM4-ECB           -> KeyLength:16
 * @return  返回值 原始数据（解密得到的结果对不对需要和业务里的加密原文作对比）
 */
- (NSData *)SymmetricDecrypt:(NSData *)pSymmetricKey
                          iv:(NSData *)ivData
                symmetricAlg:(NSString *)strSymmAlg
                 encryptData:(NSString *)strEncryptDataBase64;


/**
 * 流式-对称加密
 * @author  han harvey
 * @param   strSymmetricAlg     对称算法
 * @param   pSymmetricKey       对称密钥
 * @param   srcDataLen          加密数据总大小（字节数）
 * @param   inputStream         输入流
 * @param   outputStream        输出流
 * @remarks 备注
 只支持SM4-ECB算法
 如果bSuccess=YES -> 对称加密成功
 如果bSuccess=NO  -> 对称加密失败 message为错误原因
 */
- (void)SymmetricEncryptEx:(NSString *)strSymmetricAlg
              symmetricKey:(NSData *)pSymmetricKey
                srcDataLen:(NSInteger)srcDataLen
               inputStream:(NSInputStream *)inputStream
              outputStream:(NSOutputStream *)outputStream
                   handler:(void(^)(BOOL bSuccess, NSString *message))handler;

/**
 * 流式-对称解密
 * @author  han harvey
 * @param   strSymmetricAlg     对称算法
 * @param   pSymmetricKey       对称密钥
 * @param   encryptDataLen      加密结果数据总大小（字节数）
 * @param   inputStream         输入流
 * @param   outputStream        输出流
 * @remarks 备注
 只支持SM4-ECB
 如果bSuccess=YES -> 对称解密成功
 如果bSuccess=NO  -> 对称解密失败 message为错误原因
 */
- (void)SymmetricDecryptEx:(NSString *)strSymmetricAlg
              symmetricKey:(NSData *)pSymmetricKey
            encryptDataLen:(NSInteger)encryptDataLen
               inputStream:(NSInputStream *)inputStream
              outputStream:(NSOutputStream *)outputStream
                   handler:(void(^)(BOOL bSuccess, NSString *message))handler;

/**
 * 获取做签名业务证书的公钥证书Base64格式
 * @author  han harvey
 * @remarks 备注 证书存储在KeyChain中时使用该方法，其他情况，取出来的值为空串
 * @return  返回值 做业务证书的公钥证书Base64格式
 */
- (NSString *)getSignCertPublicKeyCertBase64;

/**
 * 获取做信封业务证书的公钥证书Base64格式
 * @author  han harvey
 * @remarks 备注 证书存储在KeyChain中时使用该方法，其他情况，取出来的值为空串
 * @return  返回值 做业务证书的公钥证书Base64格式
 */
- (NSString *)getEncCertPublicKeyCertBase64;

/**
 * 获取做业务证书的公钥证书基本信息
 * @author  han harvey
 * @return  返回值 做业务证书的公钥证书基本信息
 */
- (NSDictionary *)getX509InfoDict;

/**
 * 释放资源
 * @author  han harvey
 * @return  返回值 0:成功 其他:失败
 */
- (int)DoFinalize;

/**
 * 做摘要，支持SHA1和SM3
 * @author  han harvey
 * @param   alg         摘要算法，SHA1-－>"SHA1"  SM3-->"SM3"
 * @param   srcData     原始数据
 * @param   hashValB64 【出参】，hash的base64编码
 * @remarks 备注         算法对应关系：SHA1-－>"SHA1"  SM3-->"SM3"
 * @return  返回值       int:错误标识，0:成功，1:失败
 */
- (int)GetHashDataWithAlg:(NSString *)alg
               andSrcData:(NSString *)srcData
         andHashValBase64:(NSString **)hashValB64;

/**
 * 做摘要，支持SHA1和SM3
 * @author  han harvey
 * @param   alg         摘要算法，SHA1-－>"SHA1"  SM3-->"SM3"
 * @param   srcData     原始数据
 * @param   hashValHex 【出参】，16进制的字符串
 * @remarks 备注         算法对应关系：SHA1-－>"SHA1"  SM3-->"SM3"
 * @return  返回值       int:错误标识，0:成功，1:失败
 */
- (int)GetHashDataExWithAlg:(NSString *)alg
                 andSrcData:(NSString *)srcData
              andHashValHex:(NSString **)hashValHex;


/**
 * 对二维码数据做签名或解密业务
 * @author  han harvey
 * @param   certIndex   证书索引
 * @param   strPassword 证书密码
 * @param   strSrc      二维码扫描数据（URL,获取原文地址,时间戳,过期时间）
 * @remarks 备注 调用该方法前，请先调用获取证书列表接口GetCertList
 * @return  返回值
 */
- (int)dealErmaCryptoData:(int)certIndex
              andPassword:(NSString *)strPassword
                   andSrc:(NSString *)strSrc;



/******************** 密码服务 End ************************/


/********************* 其他 Start ************************/

// 获取证书PIN码是否受指纹认证保护
- (BOOL)GetCertIsProtected:(int)arrayIndex;
// 吉安宝专用解绑指纹绑定接口
- (int)JITUnbindSafeModelCertPINCodeProtected;

// 模拟Server端获取证书信息
- (NSDictionary *)getCertInfoWithIP:(NSString *)strServerIP
                               Port:(NSString *)strPort
                        andAuthCode:(NSString *)strAuthCode
                         andP10Data:(NSString *)strP10Data
                        andCertType:(CERT_TYPE_MCTK)certType;

// 模拟Server端更新并获取证书信息
- (NSDictionary *)getUpdateCertInfoWithIP:(NSString *)strServerIP
                                     Port:(NSString *)strPort
                                andP10Str:(NSString *)strP10
                               andP10Type:(NSString *)strP10Type // 1:UPDATE 2:DELAY
                                  andPass:(NSString *)strPassword
                             andCertIndex:(int)certIndex;
/**
 预先设置初始化配置文件
 pathName要和SDK初始化时最后一个参数保持一致
 
 2020.07.27新增加strAdminPIN 和 strUserPIN
 strAdminPIN : 密码模块默认管理员PIN
 strUserPIN : 密码模块默认用户PIN
 如果MCTK第一次初始化需要自己制定默认管理员PIN和默认用户PIN，则在初始化之前需要先调用该方法进行预设置；或者如果修改了默认管理员PIN并且想要修改用户PIN，则在解锁PIN码之前也需要调用该方法进行预设置管理员PIN
 
 注：bundle和pathName一起使用，strAdminPIN 和 strUserPIN可以单独设置
 */
+ (BOOL)SetConfFileWithBundle:(NSBundle *)bundle
                  andCertPath:(NSString *)pathName
        andStrDefaultAdminPIN:(NSString *)strAdminPIN
         andStrDefaultUserPIN:(NSString *)strUserPIN;
/**
 关闭密码模块
 */
- (void)CloseSafeModelHandle;
+ (NSString *)getJitMCTKDeviceId1;
+ (NSString *)base64:(NSData *)data;
+ (NSString *)getRandowWithIP:(NSString *)ipStr
                      andPort:(int)port;
+ (NSString *)ToHexStr:(NSString *)str;
+ (NSString *)getCertSNWithP7bData:(NSData *)p7bData;
// 清理KeyChain中对应cert，keys中所有数据
+ (void)ResetKeychain;
+ (void)unbindCert:(NSString *)certSNStr;

/**
 普通密码模块重置PIN码
 */

// 密码模块解锁PIN码【旧】
- (BOOL)UnlockSafeModelUserPIN:(NSString *)strPIN
                       andCode:(NSString *)strUnlockCode
                     andRandom:(NSString *)strRandom
           andServerUnlockCode:(NSString *)strSerUnlockCode;


/**
协同密码模块重置PIN码
*/

// 协同密码模块重置PIN码第一个接口,获取重置码
// userinfo 用户信息
- (int)GetRestCodeWithUserInfo:(NSString *)strUserInfo;


// 协同密码模块重置PIN码第二个接口,重置PIN码
// strCode 重制码
// strPIN  密码模块新的PIN码
- (int)ResetPINWithCode:(NSString *)strCode
              andNewPIN:(NSString *)strPIN;


/// SM2加密
/// @param pubKey 公钥数据
/// @param pSrcData 原文数据
/// @return 加密结果
- (NSString *)SM2DoEncryp:(NSData *)pubKey withSrc:(NSData *)pSrcData;

/********************** 其他 End *************************/
- (int)GMSSLProxy;
- (void)setGMSSLProxy:(int)value;

@end

// 申请证书-手机类型
@interface JITReqCertAttribute : NSObject

// 设备类型
@property (assign, nonatomic) MCTK_PKITool_PhoneType phoneType;

// 设备ID
@property (strong, nonatomic) NSString *deviceID;

// 其他附加属性
@property (strong, nonatomic) NSDictionary *otherAttr;

@end

// OAuth认证-附加参数
@interface JITOAuthAttribute : NSObject

@property (assign, nonatomic) BOOL reToken;     // 是否强制重新认证
@property (assign, nonatomic) BOOL isPortal;    // 是否为门户
@property (assign, nonatomic) int portalTime;   // 门户时间

@end



