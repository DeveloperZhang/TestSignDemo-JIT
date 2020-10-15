//
//  ViewController.m
//  TestSignDemo
//
//  Created by ZhangYu on 2020/10/12.
//  Copyright © 2020 JIT. All rights reserved.
//

#import "ViewController.h"
#include <CommonCrypto/CommonDigest.h>
#import <CommonCrypto/CommonCrypto.h>
#import "YQYTools.h"
#import "YQYCerModel.h"

static NSString *const kBaseUrl = @"https://39.105.231.49:8015/";

@interface ViewController ()

@property (nonatomic, strong) NSArray *dataArray;
@property (nonatomic, strong) NSString *p10Str;
@property (nonatomic, strong) NSString *pkCerStr;
@property (nonatomic, strong) NSString *pkSNStr;
@property (nonatomic, strong) NSString *p7bStr;
@property (nonatomic, strong) YQYCerModel *cerModel;  //证书对象


@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.cerModel = [YQYCerModel new];
    NSLog(@"111");
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 60;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"CustomTableViewCell"];
    cell.textLabel.text = self.dataArray[indexPath.row];
    return cell;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.dataArray.count;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    switch (indexPath.row) {
        case 0: {//产生P10数据
            UIAlertController *alertVc = [UIAlertController alertControllerWithTitle:@"提示" message:@"产生P10数据" preferredStyle:UIAlertControllerStyleAlert];
            UIAlertAction *sureAction = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                NSString *p10Str = @"";
                //密码模块,密码必须为Aa111111
                [[YQYMBProgressHUDManager sharedSingleton] showWaiting];
                BOOL success = [[JITMiddleWareSDKManager sharedSingleton] createP10WithCertCN:@"CN=JIT,C=CN" andP10Base64:&p10Str];
                NSLog(@"p10数据为:%@",p10Str);
                NSLog(@"p10数据为:%@,生成结果为:%@",p10Str,success==YES?@"成功":@"失败");
                self.cerModel.p10Base64String = p10Str;
                [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:[NSString stringWithFormat:@"生成%@",success==YES?@"成功":@"失败"] callBack:^{
                } delayHideSecond:1.];

            }];
            [alertVc addAction:sureAction];
            UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
                
            }];
            [alertVc addAction:cancelAction];
            [self presentViewController:alertVc animated:YES completion:nil];
        }
            break;
        case 1: {//申请证书公钥和序列号
            [[JITMiddleWareSDKManager sharedSingleton] applyCertInfo:self.cerModel.p10Base64String successBlock:^(id  _Nullable responseObject) {
                //cert,sn
                self.cerModel.pkCerBase64String = responseObject[@"cert"];
//                self.cerModel.snString = responseObject[@"sn"];
                NSLog(@"");
            }];
        }
            break;
        case 2: {//cert转换p7b 获取cert base64编码的证书
            [[JITMiddleWareSDKManager sharedSingleton] generateP7bInfo:self.cerModel.pkCerBase64String successBlock:^(id  _Nullable responseObject) {
                self.cerModel.p7bBase64String = responseObject[@"p7b"];
                NSLog(@"");
            }];
        }
            break;
        case 3: {//保存证书
            int ret = [[JITMiddleWareSDKManager sharedSingleton].jitMCTK saveCertWithCertType:@"RSA"
                                                     andCertPassword:@"Aa111111"
                                                           andCertSN:self.cerModel.pkCerBase64String
                                                              andP7b:self.cerModel.p7bBase64String
                                                     andDoubleCertSN:@""
                                                        andDoubleP7b:@""
                                              andDoubleEncSessionKey:@""
                                                    andSessionKeyAlg:@""
                                                 andDleEncPrivateKey:@""];
            NSLog(@"保存证书:%@",ret==0?@"成功":@"失败");
        }
            break;
        case 4: {//设置证书和摘要算法
            //获取证书列表
            NSArray *array = [[JITMiddleWareSDKManager sharedSingleton].jitMCTK GetCertList];
            [[JITMiddleWareSDKManager sharedSingleton].jitMCTK SetCert:[array lastObject][@"UniqueID"] password:@"Aa111111"];
            [[JITMiddleWareSDKManager sharedSingleton].jitMCTK SetDigestAlg:@"SHA1"];
        }
            break;
        case 5: {//签名
            NSData *srcData = [@"123" dataUsingEncoding:NSUTF8StringEncoding];
            NSString *resultStr = [[JITMiddleWareSDKManager sharedSingleton].jitMCTK P1Sign:srcData];
            NSLog(@"resultStr:%@",resultStr);
        }
            break;
        case 6:{//验签
//            NSString *strPCert = [[JITMiddleWareSDKManager sharedSingleton].jitMCTK getSignCertPublicKeyCertBase64];
            NSData *srcData = [@"123" dataUsingEncoding:NSUTF8StringEncoding];
            int state = (int)[[JITMiddleWareSDKManager sharedSingleton].jitMCTK VerifyP1Sign:srcData publicKeyCertBase64:@"MIIC9zCCAd+gAwIBAgIQWphVq9XlBs2MWHrRTTMwPjANBgkqhkiG9w0BAQsFADA2MQswCQYDVQQGEwJDTjENMAsGA1UEChMEU1hDQTEYMBYGA1UEAxMPU2VjdXJpdHlSc2EyMDQ4MB4XDTIwMTAxMzA4MjgwMVoXDTIxMDExMzA4MjgwMVowWzELMAkGA1UEBhMCQ04xCjAIBgNVBAgTAS0xCjAIBgNVBAcTAS0xHTAbBgNVBAsTFDEwMTAxMDEwMTAxMDEwMTAxMDEwMRUwEwYDVQQDDAzmtYvor5Xlhazlj7gwgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAK4szHOa5m4/h/woh7sLg+0MhLlKCAK9YCr9ZNpc4JwAYOp/43+vskkxwwi9RRCBpd5xWHFbiJcXyBB4lHokEwa8iBTQyEcZ38jwSKKyZcbMFUoD5VfipTr8LiUz+a21rrOZ5CDayTnQkcBEfV80aiDU+fw4+sn4GGtTcpekG81pAgMBAAGjYDBeMA4GA1UdDwEB/wQEAwIDyDAdBgNVHQ4EFgQUJekf8g2888m1i3QuSEFlmNHodMUwDAYDVR0TBAUwAwEBADAfBgNVHSMEGDAWgBQFyVcieNtXVquLhKwK696TEtU+mDANBgkqhkiG9w0BAQsFAAOCAQEAHxFo98gUxl9eRCrWe/ISj8GqSQcl7sGXnfNbZlAun6ssMPqxSzwknt7aSQIAzsNdqsK7i5h9CVJ857E5mgbwZhP99/kGjFnKTvIgjoOwwZcXtWla6JlbMnEzVt9VM8zGlRRVdhj1LDDr1z1DEgcCdBlNErzLJW4YZqpd4QFvRFXHRnxAMA+ffujEvUyETo8z7EyN0k/ppNtb8W5xiWYGARMe/ekHgqbab6ENVCjbfIZ+ohJZsW3f2UjpEEAF3gJeOf1qGh/f4Pj8goSsNsr9yG2Q+gusp+xgkA54F8Y4Gd8DDYzcPobRPAZHfPPAcjtSmoA38VTMYFMnmuw/6sdmvA==" SignDataBase64:@"dVEl4Ucz8+1TwN9555p761HdFbvNLRWmmwnjgaEjsY++F7J9AfDlZiyyXRrpc/wERQclsI7D+UKspKZqsMJ0K4NEbt2h1Sy2RQXFfBKRIdNF9oFyL/SWQvR1iB97VRGwwgrvGMUxJ/+Lav6XFLPXOlWqYeyHXAIQ8gqQEUqb+aA="];
            NSLog(@"验签结果:%d",state);

        }
            break;
        case 7: {//p7签名
            NSData *srcData = [@"123" dataUsingEncoding:NSUTF8StringEncoding];
            NSString *resultStr = [[JITMiddleWareSDKManager sharedSingleton].jitMCTK DetachSign:srcData];
            NSLog(@"resultStr:%@",resultStr);
        }
            break;
        case 8: {//p7验签
//            NSString *strPCert = [[JITMiddleWareSDKManager sharedSingleton].jitMCTK getSignCertPublicKeyCertBase64];
            NSData *srcData = [@"123" dataUsingEncoding:NSUTF8StringEncoding];
            int state = (int)[[JITMiddleWareSDKManager sharedSingleton].jitMCTK VerifyDetachSign:srcData SignData:@"MIIEGwYJKoZIhvcNAQcCoIIEDDCCBAgCAQExCTAHBgUrDgMCGjALBgkqhkiG9w0BBwGgggL7MIIC9zCCAd+gAwIBAgIQWphVq9XlBs2MWHrRTTMwPjANBgkqhkiG9w0BAQsFADA2MQswCQYDVQQGEwJDTjENMAsGA1UEChMEU1hDQTEYMBYGA1UEAxMPU2VjdXJpdHlSc2EyMDQ4MB4XDTIwMTAxMzA4MjgwMVoXDTIxMDExMzA4MjgwMVowWzELMAkGA1UEBhMCQ04xCjAIBgNVBAgTAS0xCjAIBgNVBAcTAS0xHTAbBgNVBAsTFDEwMTAxMDEwMTAxMDEwMTAxMDEwMRUwEwYDVQQDDAzmtYvor5Xlhazlj7gwgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAK4szHOa5m4/h/woh7sLg+0MhLlKCAK9YCr9ZNpc4JwAYOp/43+vskkxwwi9RRCBpd5xWHFbiJcXyBB4lHokEwa8iBTQyEcZ38jwSKKyZcbMFUoD5VfipTr8LiUz+a21rrOZ5CDayTnQkcBEfV80aiDU+fw4+sn4GGtTcpekG81pAgMBAAGjYDBeMA4GA1UdDwEB/wQEAwIDyDAdBgNVHQ4EFgQUJekf8g2888m1i3QuSEFlmNHodMUwDAYDVR0TBAUwAwEBADAfBgNVHSMEGDAWgBQFyVcieNtXVquLhKwK696TEtU+mDANBgkqhkiG9w0BAQsFAAOCAQEAHxFo98gUxl9eRCrWe/ISj8GqSQcl7sGXnfNbZlAun6ssMPqxSzwknt7aSQIAzsNdqsK7i5h9CVJ857E5mgbwZhP99/kGjFnKTvIgjoOwwZcXtWla6JlbMnEzVt9VM8zGlRRVdhj1LDDr1z1DEgcCdBlNErzLJW4YZqpd4QFvRFXHRnxAMA+ffujEvUyETo8z7EyN0k/ppNtb8W5xiWYGARMe/ekHgqbab6ENVCjbfIZ+ohJZsW3f2UjpEEAF3gJeOf1qGh/f4Pj8goSsNsr9yG2Q+gusp+xgkA54F8Y4Gd8DDYzcPobRPAZHfPPAcjtSmoA38VTMYFMnmuw/6sdmvDGB6zCB6AIBATBKMDYxCzAJBgNVBAYTAkNOMQ0wCwYDVQQKEwRTWENBMRgwFgYDVQQDEw9TZWN1cml0eVJzYTIwNDgCEFqYVavV5QbNjFh60U0zMD4wBwYFKw4DAhowCwYJKoZIhvcNAQEBBIGAdVEl4Ucz8+1TwN9555p761HdFbvNLRWmmwnjgaEjsY++F7J9AfDlZiyyXRrpc/wERQclsI7D+UKspKZqsMJ0K4NEbt2h1Sy2RQXFfBKRIdNF9oFyL/SWQvR1iB97VRGwwgrvGMUxJ/+Lav6XFLPXOlWqYeyHXAIQ8gqQEUqb+aA="];
            NSLog(@"验签结果:%d",state);
        }
            break;
        case 9: {//网络请求
            AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
            manager.requestSerializer = [AFJSONRequestSerializer serializer];
            [manager.requestSerializer setValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
                    
            [manager POST:@"http://39.106.37.213:8040/system/user/checkUserNameUnique" parameters:@{@"userName":@"a1111111"} headers:@{} progress:^(NSProgress * _Nonnull uploadProgress) {
                
            } success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
                NSLog(@"success");
            } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
                NSLog(@"error");
            }];
        }
            break;
        default:
            break;
    }
}


//HmacSHA1加密
- (NSString *)Base_HmacSha1:(NSString *)key data:(NSString *)data{
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

- (NSArray *)dataArray {
    if (_dataArray == nil) {
        _dataArray = @[@"1.产生P10数据",@"2.申请证书(序列号)",@"3.p10转p7b",@"4.保存证书",@"5.设置证书和摘要算法",@"p1签名",@"p1验签",@"p7签名",@"p7验签",@"网络请求"];
    }
    return _dataArray;
}

- (NSString *)toJSONString:(NSDictionary *)dic {
    NSData *data = [NSJSONSerialization dataWithJSONObject:dic
                                                   options:NSJSONWritingSortedKeys
                                                     error:nil];
     
    if (data == nil) {
        return nil;
    }
     
    NSString *string = [[NSString alloc] initWithData:data
                                             encoding:NSUTF8StringEncoding];
    return string;
}

/*
 * 字符串转字典（NSString转Dictionary）
 *   parameter
 *     turnString : 需要转换的字符串
 */
- (NSDictionary *)turnStringToDictionary:(NSString *)turnString
{
    NSData *turnData = [turnString dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *turnDic = [NSJSONSerialization JSONObjectWithData:turnData options:NSJSONReadingMutableContainers error:nil];
    return turnDic;
}


@end
