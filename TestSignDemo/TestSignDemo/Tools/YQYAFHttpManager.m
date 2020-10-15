//
//  AFHttpManager.m
//  TestSignDemo
//
//  Created by ZhangYu on 2020/10/15.
//  Copyright © 2020 JIT. All rights reserved.
//

#import "YQYAFHttpManager.h"
#import "YQYTools.h"


static NSString *const kBaseUrl = @"https://39.105.231.49:8015/";
static const float kTimeoutFloat = 60.;

@interface YQYAFHttpManager ()

@property(nonatomic,strong) AFHTTPSessionManager *sessisonManager;


@end

@implementation YQYAFHttpManager

+ (instancetype)sharedSingleton {
    static YQYAFHttpManager *_sharedSingleton = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        //不能再使用alloc方法
        //因为已经重写了allocWithZone方法，所以这里要调用父类的分配空间的方法
        _sharedSingleton = [[super allocWithZone:NULL] init];
        [_sharedSingleton configManager];
    });
    return _sharedSingleton;
}

- (void)configManager {
    AFHTTPSessionManager *sessisonManager = [AFHTTPSessionManager manager];
    self.sessisonManager = sessisonManager;
    sessisonManager.requestSerializer.cachePolicy = NSURLRequestReloadIgnoringLocalCacheData;
    sessisonManager.requestSerializer = [AFJSONRequestSerializer serializer];
    [sessisonManager.requestSerializer setValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
    [sessisonManager.requestSerializer setValue:@"application/json" forHTTPHeaderField:@"Accept"];

    sessisonManager.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json", @"text/json", @"text/javascript",@"text/html",@"text/plain", nil];
    sessisonManager.requestSerializer.timeoutInterval = kTimeoutFloat;
    // 2.设置非校验证书模式
    sessisonManager.securityPolicy = [AFSecurityPolicy policyWithPinningMode:AFSSLPinningModeNone];
    sessisonManager.securityPolicy.allowInvalidCertificates = YES;
    [sessisonManager.securityPolicy setValidatesDomainName:NO];
    ((AFJSONResponseSerializer *)self.sessisonManager.responseSerializer).removesKeysWithNullValues = YES;
    ((AFJSONRequestSerializer *)sessisonManager.requestSerializer).writingOptions = NSJSONWritingSortedKeys;
}


- (NSDictionary *)resetHeaderData:(NSDictionary *)params {
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:params options:NSJSONWritingSortedKeys error:0];
    NSString *dataStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    NSString *body = [YQYTools stringToMD5:dataStr];
    NSString *dateTimeStr = [[YQYTools sharedInstance] formateDateWithDate:[NSDate date]];
    NSString *signSrcString = [NSString stringWithFormat:@"af-access-id=accessId0&af-date=%@&body=%@",dateTimeStr,body];
    NSString *shaStr = [YQYTools hmacSha1Encrypt:@"accessKey0" data:signSrcString];
    NSString *authorization = shaStr;
    NSDictionary *headerParams = @{@"af-access-id":@"accessId0",@"af-date":dateTimeStr,@"SignatureMethod":@"HMAC-SHA1",@"Authorization":authorization};
    return headerParams;
}

- (void)postHttpRequestWithParams:(NSDictionary *)params
                        uriString:(NSString *)uriString
                     successBlock:(void(^)(id  _Nullable responseObject))successBlock
                     failureBlock:(void(^)(NSError * _Nonnull error))failureBlock {
    NSString *urlString = [NSString stringWithFormat:@"%@%@",kBaseUrl,uriString];
    [self.sessisonManager POST:urlString
       parameters:params
          headers:[self resetHeaderData:params]
         progress:^(NSProgress * _Nonnull uploadProgress) {
        
    } success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        if (successBlock) {
            successBlock(responseObject);
        }
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        if (failureBlock) {
            failureBlock(error);
        }
    }];
}



@end
