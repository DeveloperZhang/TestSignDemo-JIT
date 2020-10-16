//
//  JITMiddleWareSDKManager.m
//  TestSignDemo
//
//  Created by ZhangYu on 2020/10/15.
//  Copyright © 2020 JIT. All rights reserved.
//

#import "JITMiddleWareSDKManager.h"
#import "YQYTools.h"
#import "YQYAFHttpManager.h"

static NSString *const kBaseUrl = @"https://39.105.231.49:8015/";

@implementation JITMiddleWareSDKManager

+ (instancetype)sharedSingleton {
    static JITMiddleWareSDKManager *_sharedSingleton = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        //不能再使用alloc方法
        //因为已经重写了allocWithZone方法，所以这里要调用父类的分配空间的方法
        _sharedSingleton = [[super allocWithZone:NULL] init];
    });
    return _sharedSingleton;
}



- (void)configCerType:(CERT_TYPE_YQY)certType {
//    [JITMiddleWareSDKManager sharedSingleton].jitMCTK = [[JITMiddleWareSDK alloc] initWithCertStorageLocation:CERT_STORAGE_LOCATION_SANDBOX andIP:nil andPort:nil andSafeModelPath:nil];
    //密码模块密码必须为Aa111111
    self.jitMCTK = [[JITMiddleWareSDK alloc] initWithCertStorageLocation:CERT_STORAGE_LOCATION_SAFEMODEL andIP:nil andPort:nil andSafeModelPath:nil];
    self.certType = certType;
}


- (BOOL)createP10WithCertCN:(NSString *)strCN
               andP10Base64:(NSString *_Nullable*_Nullable)strP10Base64 {
    int ret = [[JITMiddleWareSDKManager sharedSingleton].jitMCTK createP10WithCertCN:strCN andCertType:(CERT_TYPE_MCTK)self.certType andKeyLength:@"2048" andPassword:@"Aa111111" andP10Base64:strP10Base64];
    if (ret == 0) {//成功
        return YES;
    }
    return NO;
}

- (void)applyCertInfo:(NSString *)p10Str
         successBlock:(void(^)(id  _Nullable responseObject))successBlock {
    if ([NSString isBlankString:p10Str]) {
        [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:@"p10内容为空" callBack:^{
            
        } delayHideSecond:1.];
        return;
    }
    __block NSDictionary *responseDic;
    [[YQYMBProgressHUDManager sharedSingleton] showWaiting];
    NSDictionary *params = @{@"p10":p10Str,@"type":@"userCert",@"duration":@"1",@"dataMap":@{@"userName":@"张宇",@"idNumber":@"220122198908110711"}};
    [[YQYAFHttpManager sharedSingleton] postHttpRequestWithParams:params uriString:@"publicCert/apply" successBlock:^(id  _Nullable responseObject) {
        if (responseObject[@"certInfo"]) {
            responseDic = @{@"cert":responseObject[@"certInfo"][@"cert"],@"sn":responseObject[@"certInfo"][@"serialNumber"]};
            NSLog(@"申请成功");
            [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:@"申请成功" callBack:^{
                if (successBlock) {
                    successBlock(responseDic);
                }
            } delayHideSecond:1.];
        }else {
            [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:responseObject[@"errorMsg"] callBack:^{
            } delayHideSecond:1.];
            NSLog(@"申请失败");
        }
    } failureBlock:^(NSError * _Nonnull error) {
        [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:@"申请失败" callBack:^{
        } delayHideSecond:1.];
        NSLog(@"申请失败");
    }];
}

- (void)generateP7bInfo:(NSString *)cerString successBlock:(void(^)(id  _Nullable responseObject))successBlock {
    if ([NSString isBlankString:cerString]) {
        [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:@"证书内容为空" callBack:^{
            
        } delayHideSecond:1.];
        return;
    }
    NSDictionary *params = @{@"cert":cerString};
    [[YQYAFHttpManager sharedSingleton] postHttpRequestWithParams:params uriString:@"cloudCert/p7b" successBlock:^(id  _Nullable responseObject) {
        if (responseObject[@"errorMsg"]) {
            [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:responseObject[@"errorMsg"] callBack:^{
            } delayHideSecond:1.];
            NSLog(@"生成失败");
            
        }else {
            NSLog(@"生成成功");
            [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:@"生成成功" callBack:^{
                if (successBlock) {
                    successBlock(responseObject);
                }
            } delayHideSecond:1.];
        }
    } failureBlock:^(NSError * _Nonnull error) {
        [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:@"生成成功" callBack:^{
        } delayHideSecond:1.];
        NSLog(@"生成成功");
    }];
}


- (BOOL)saveCertWithCertSN:(NSString *)strCertSN andP7b:(NSString *)strP7b {
    NSString *certType;
    if ([JITMiddleWareSDKManager sharedSingleton].certType == CERT_TYPE_YQY_RSA) {
        certType = @"RSA";
    }else {
        certType = @"SM2";
    }
    int ret = [[JITMiddleWareSDKManager sharedSingleton].jitMCTK saveCertWithCertType:certType
           andCertPassword:@"Aa111111"
                 andCertSN:strCertSN
                    andP7b:strP7b
           andDoubleCertSN:@""
              andDoubleP7b:@""
    andDoubleEncSessionKey:@""
          andSessionKeyAlg:@""
       andDleEncPrivateKey:@""];
    NSLog(@"保存证书:%@",ret==0?@"成功":@"失败");
    if (ret == 0) {
        [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:@"保存成功" callBack:^{
            
        } delayHideSecond:1.];
        return YES;
    }
    return NO;
}


- (BOOL)configDigestAlg {
    //获取证书列表
    NSArray *array = [[JITMiddleWareSDKManager sharedSingleton].jitMCTK GetCertList];
    if (array.count == 0) {
        NSLog(@"无证书");
        return NO;
    }
    int result = [[JITMiddleWareSDKManager sharedSingleton].jitMCTK SetCert:[array lastObject][@"UniqueID"] password:@"Aa111111"];
    if (result != 0) {
        NSLog(@"设置失败");
        return NO;
    }
    if ([JITMiddleWareSDKManager sharedSingleton].certType == CERT_TYPE_YQY_RSA) {
        [[JITMiddleWareSDKManager sharedSingleton].jitMCTK SetDigestAlg:@"SHA1"];
    }
    return YES;
}

- (NSString *)p1SignWithData:(NSString *)srcStr {
    NSData *srcData = [srcStr dataUsingEncoding:NSUTF8StringEncoding];
    NSString *resultStr = [self.jitMCTK P1Sign:srcData];
    return resultStr;
}

- (BOOL)verifyP1Sign:(NSString *)srcStr publicKeyCertBase64:(NSString *)strPublicKeyCertBase64 signDataBase64:(NSString *)strSignDataBase64 {
    NSData *srcData = [srcStr dataUsingEncoding:NSUTF8StringEncoding];
    int result = [self.jitMCTK VerifyP1Sign:srcData publicKeyCertBase64:strPublicKeyCertBase64 SignDataBase64:strSignDataBase64];
    if (result == 0) {
        return YES;
    }
    return NO;
}

- (NSString *)p7SignWithData:(NSString *)srcStr {
    NSData *srcData = [srcStr dataUsingEncoding:NSUTF8StringEncoding];
    NSString *resultStr = [self.jitMCTK DetachSign:srcData];
    return resultStr;
}

- (BOOL)verifyP7Sign:(NSString *)srcStr signDataBase64:(NSString *)strSignDataBase64 {
    NSData *srcData = [srcStr dataUsingEncoding:NSUTF8StringEncoding];
    int result = [self.jitMCTK VerifyDetachSign:srcData SignData:strSignDataBase64];
    if (result == 0) {
        return YES;
    }
    return NO;
}


@end
