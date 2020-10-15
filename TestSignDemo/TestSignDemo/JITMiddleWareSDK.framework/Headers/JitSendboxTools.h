//
//  JitSendboxTools.h
//  JITSearchDemo
//
//  Created by han harvey on 2016/12/6.
//  Copyright © 2016年 han harvey. All rights reserved.
//
#import <Foundation/Foundation.h>

@interface JitSendboxTools : NSObject

/**
 * 获取沙盒根目录 当前app家(沙盒)根目录
 * @author han harvey
 * @return 沙盒根目录
 */
+(NSString *)GetUserSendboxRootPath;

/**
 * 获取沙盒Document路径
 * @author han harvey
 * @return Document路径
 */
+(NSString *)GetUserDocumentPath;

/**
 * 获取沙盒Library路径
 * @author han harvey
 * @return Library路径
 */
+(NSString *)GetUserLibraryPath;

/**
 * 获取沙盒Library/Caches路径
 * @author han harvey
 * @return Library/Caches路径
 */
+(NSString *)GetUserCachesPath;

/**
 * 获取沙盒Library/Preferences路径
 * @author han harvey
 * @return Library/Preferences路径
 */
+(NSString *)GetUserPreferencesPath;

/**
 * 获取UserDefaults
 * 支持的数据格式有：NSNumber、NSData、NSDate、NSString、NSArray、NSDictionary、BOOL
 * @author han harvey
 * @return UserDefaults
 */
+(id)GetUserDefaults;

/**
 * 根据key获取UserDefault里面的数据
 * @author han harvey
 * @param key 保存数据时的key
 * @return 取出来的数据
 */
+(id)getSaveDateWithKey:(NSString *)key;

/**
 * 沙盒Document路径下下创建文件
 * @author han harvey
 * @param fileName 文件名
 * @return 文件的完整路径
 */
+(NSString *)createFileWithFileName:(NSString *)fileName;

/**
 * 沙盒下创建目录
 * @author han harvey
 * @param directoryName 目录名称
 * @return 创建目录状态 nil 创建成功
 */
+(NSError *)createDirectoryWithDirectoryName:(NSString *)directoryName;

/**
 * 沙盒写入文件
 * @author han harvey
 * @param filePath 文件完整路径
 * @param data 需要写入文件的数据
 * @return 写入文件的状态 NO 写入失败
 */
+(BOOL)writeDataToPath:(NSString *)filePath andData:(NSData *)data;

/**
 * 从沙盒删除文件
 * @author han harvey
 * @param filePath 需要删除文件的完整路径
 * @return 删除文件的状态 NO 删除失败
 */
+(BOOL)deleteFileWithPath:(NSString *)filePath;

/**
 * 获取目录下的所有文件名
 * @author han harvey
 * @param directoryPath 获取文件名所在的目录
 * @return 该目录下的所有文件名
 */
+(NSArray *)GetAllFileNameWithPath:(NSString *)directoryPath;

/**
 * 沙盒读取文件
 * @author han harvey
 * @param filePath 文件完整目录
 * @return 文件里面的数据
 */
+(NSData *)GetDataFromFilePath:(NSString *)filePath;

/**
 * 根据类别和Key获取值
 * @author  han harvey
 * @param   strType     大类别
 * @param   strKey      Key
 * @param   strDefault  默认值（如果取不到值则负值该值）
 * @param   strFilePath 配置文件的路径
 * @param   strValue   【出参】根据Key取到的值
 * @remarks 备注 无
 * @return  0:成功 -1:参数错误 -2:路径不对，或者文件不存在 -3:其他错误
 */
+ (int)GetInfoFromConfigWithType:(NSString *)strType
                          andKey:(NSString *)strKey
                      andDefault:(NSString *)strDefault
                     andFilePath:(NSString *)strFilePath
                        andValue:(NSString **)strValue;

// 工程中文件是否存在
+ (BOOL)FileBeExist:(NSString * )fileName andType:(NSString *)type;

// 沙盒中文件是否存在
+ (BOOL)FileBeExistAtSendbox:(NSString *)strPath;

@end
