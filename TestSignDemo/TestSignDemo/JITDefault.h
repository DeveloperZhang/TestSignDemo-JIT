//
//  JITDefault.h
//  TestSignDemo
//
//  Created by ZhangYu on 2020/10/13.
//  Copyright © 2020 JIT. All rights reserved.
//

#ifndef JITDefault_h
#define JITDefault_h

#import "AppDelegate.h"
#import "AFNetworking.h"
#import "JITBase64.h"
#import "JITMiddleWareSDKManager.h"
#import "MBProgressHUD.h"
#import "YQYMBProgressHUDManager.h"
#import "NSString+YQYString.h"

// .h文件
#define SINGLETON_H \
+ (instancetype)sharedInstance;

// .m文件
#define SINGLETON_M \
static id _instance; \
\
+ (instancetype)allocWithZone:(struct _NSZone *)zone \
{ \
static dispatch_once_t onceToken; \
dispatch_once(&onceToken, ^{ \
_instance = [super allocWithZone:zone]; \
}); \
return _instance; \
} \
\
+ (instancetype)sharedInstance \
{ \
static dispatch_once_t onceToken; \
dispatch_once(&onceToken, ^{ \
_instance = [[self alloc] init]; \
}); \
return _instance; \
} \
\
- (id)copyWithZone:(NSZone *)zone \
{ \
return _instance; \
}


#endif /* JITDefault_h */
