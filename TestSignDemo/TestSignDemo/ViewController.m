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
    JITMiddleWareSDKManager *sdkManager = [JITMiddleWareSDKManager sharedSingleton];
    [sdkManager configCerType:CERT_TYPE_YQY_RSA];
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
                //cert
                self.cerModel.pkCerBase64String = responseObject[@"cert"];
                self.cerModel.snString = responseObject[@"sn"];
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
            BOOL result = [[JITMiddleWareSDKManager sharedSingleton] saveCertWithCertSN:self.cerModel.pkCerBase64String andP7b:self.cerModel.p7bBase64String];
            NSString *resultStr = [NSString stringWithFormat:@"保存证书:%@",result==YES?@"成功":@"失败"];
            NSLog(@"%@", resultStr);
            [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:resultStr callBack:^{
                
            } delayHideSecond:1.];
        }
            break;
        case 4: {//设置证书和摘要算法
            BOOL result = [[JITMiddleWareSDKManager sharedSingleton] configDigestAlg];
            NSString *resultStr = [NSString stringWithFormat:@"设置证书%@",result==YES ? @"成功" : @"失败"];
            [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:resultStr callBack:^{
                
            } delayHideSecond:1.];
        }
            break;
        case 5: {//签名
            self.cerModel.signSrcStr = @"123";
            NSString *resultStr = [[JITMiddleWareSDKManager sharedSingleton] p1SignWithData:self.cerModel.signSrcStr];
            self.cerModel.signedData = resultStr;
            NSString *resultString = [NSString stringWithFormat:@"签名%@",![NSString isBlankString:resultStr] ? @"成功" : @"失败"];
            [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:resultString callBack:^{
                
            } delayHideSecond:1.];
        }
            break;
        case 6:{//验签
            BOOL result = [[JITMiddleWareSDKManager sharedSingleton] verifyP1Sign:@"123" publicKeyCertBase64:self.cerModel.pkCerBase64String signDataBase64:self.cerModel.signedData];
            NSString *resultStr = [NSString stringWithFormat:@"验签结果:%@",result==YES ? @"成功" : @"失败"];
            NSLog(@"%@", resultStr);
            [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:resultStr callBack:^{
                
            } delayHideSecond:1.];
        }
            break;
        case 7: {//p7签名
            
            self.cerModel.signSrcStr = @"123";
            NSString *resultStr = [[JITMiddleWareSDKManager sharedSingleton] p7SignWithData:self.cerModel.signSrcStr];
            self.cerModel.signedData = resultStr;
            NSLog(@"resultStr:%@",resultStr);
            NSString *resultString = [NSString stringWithFormat:@"签名%@",![NSString isBlankString:resultStr] ? @"成功" : @"失败"];
            [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:resultString callBack:^{
                
            } delayHideSecond:1.];
        }
            break;
        case 8: {//p7验签
            BOOL result = [[JITMiddleWareSDKManager sharedSingleton] verifyP7Sign:@"123" signDataBase64:self.cerModel.signedData];
            NSString *resultStr = [NSString stringWithFormat:@"验签结果:%@",result==YES ? @"成功" : @"失败"];
            NSLog(@"%@", resultStr);
            [[YQYMBProgressHUDManager sharedSingleton] showHudWithText:resultStr callBack:^{
                
            } delayHideSecond:1.];
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
