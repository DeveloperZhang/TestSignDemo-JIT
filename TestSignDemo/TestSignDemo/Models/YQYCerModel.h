//
//  YQYCerModel.h
//  TestSignDemo
//
//  Created by ZhangYu on 2020/10/15.
//  Copyright © 2020 JIT. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*
 * 证书Model
 * @author  Zhang Yu
 * @remarks 用于记录证书信息
 */

@interface YQYCerModel : NSObject

@property (nonatomic, strong) NSString *p10Base64String;     //p10
@property (nonatomic, strong) NSString *pkCerBase64String;   //公钥证书
@property (nonatomic, strong) NSString *p7bBase64String;     //p7b
@property (nonatomic, strong) NSString *signSrcStr;           //签名原数据
@property (nonatomic, strong) NSString *signedData;          //签名后数据
@property (nonatomic, strong) NSString *snString;            //证书序列号唯一标识

@end

NS_ASSUME_NONNULL_END
