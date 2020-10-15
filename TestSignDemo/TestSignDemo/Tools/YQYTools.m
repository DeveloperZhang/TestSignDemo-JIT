//
//  JITYQYTools.m
//  TestSignDemo
//
//  Created by ZhangYu on 2020/10/15.
//  Copyright © 2020 JIT. All rights reserved.
//

#import "YQYTools.h"
#include <CommonCrypto/CommonDigest.h>
#import <CommonCrypto/CommonCrypto.h>

@interface YQYTools ()

@property (nonatomic, strong) NSDateFormatter *formatter;


@end

@implementation YQYTools

SINGLETON_M

+ (UIWindow *)currentWindow {
    AppDelegate *delegate = ((AppDelegate *)[UIApplication sharedApplication].delegate);
    return [UIApplication sharedApplication].keyWindow;
}

+ (NSString *)stringToMD5:(NSString *)str {
    // 1.首先将字符串转换成UTF-8编码, 因为MD5加密是基于C语言的,所以要先把字符串转化成C语言的字符串
    const char *fooData = [str UTF8String];
    // 2.然后创建一个字符串数组,接收MD5的值
    unsigned char result[CC_MD5_DIGEST_LENGTH];
    // 3.计算MD5的值, 这是官方封装好的加密方法:把我们输入的字符串转换成16进制的32位数,然后存储到result中
    CC_MD5(fooData, (CC_LONG)strlen(fooData), result);
    /*
     第一个参数:要加密的字符串
     第二个参数: 获取要加密字符串的长度
     第三个参数: 接收结果的数组
     */
    // 4.创建一个字符串保存加密结果
    NSMutableString *saveResult = [NSMutableString string];
    // 5.从result 数组中获取加密结果并放到 saveResult中
    for (int i = 0; i < CC_MD5_DIGEST_LENGTH; i++) {
        [saveResult appendFormat:@"%02x", result[i]];
    }
    // x表示十六进制，%02X  意思是不足两位将用0补齐，如果多余两位则不影响
    return saveResult;
    /*
     这里返回的是32位的加密字符串，有时我们需要的是16位的加密字符串，其实仔细观察即可发现，16位的加密字符串就是这个字符串中见的部分。我们只需要截取字符串即可（[saveResult substringWithRange:NSMakeRange(7, 16)]）
     */
}


+ (NSString *)hmacSha1Encrypt:(NSString *)key data:(NSString *)data {
    const char *cKey  = [key cStringUsingEncoding:NSASCIIStringEncoding];
    const char *cData = [data cStringUsingEncoding:NSASCIIStringEncoding];
    //Sha256:
    // unsigned char cHMAC[CC_SHA256_DIGEST_LENGTH];
    //CCHmac(kCCHmacAlgSHA256, cKey, strlen(cKey), cData, strlen(cData), cHMAC);

    //sha1
    unsigned char cHMAC[CC_SHA1_DIGEST_LENGTH];
    CCHmac(kCCHmacAlgSHA1, cKey, strlen(cKey), cData, strlen(cData), cHMAC);

    NSData *HMAC = [[NSData alloc] initWithBytes:cHMAC
                                          length:sizeof(cHMAC)];

    //将加密结果进行一次BASE64编码。
    NSString *hash = [HMAC base64EncodedStringWithOptions:0];
    return hash;
}

- (NSString *)formateDateWithDate:(NSDate *)date {
    [self.formatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    NSString *dateTimeString = [self.formatter stringFromDate:date];
    return dateTimeString;
}

- (NSDateFormatter *)formatter {
    if (_formatter == nil) {
        _formatter = [NSDateFormatter new];
    }
    return _formatter;
}


@end
