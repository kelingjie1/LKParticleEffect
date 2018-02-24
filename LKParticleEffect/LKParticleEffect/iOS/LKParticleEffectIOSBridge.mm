//
//  LKParticleEffectIOSUtil.m
//  LKParticleEffect
//
//  Created by lingtonke on 2018/2/24.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKParticleEffectIOSBridge.h"
#import <string>
using namespace std;
using namespace LKKit;

LKJSONObject JSONObjectFromNSObject(NSObject *obj)
{
    __block LKJSONObject jsonObject;
    if ([obj isKindOfClass:[NSNumber class]])
    {
        jsonObject.type = LKJSONObjectTypeNumber;
        jsonObject.valueNumber = [(NSNumber*)obj doubleValue];
    }
    if ([obj isKindOfClass:[NSString class]])
    {
        NSString *str = (NSString*)obj;
        jsonObject.type = LKJSONObjectTypeNumber;
        jsonObject.valueString = [str cStringUsingEncoding:NSUTF8StringEncoding];
    }
    else if ([obj isKindOfClass:[NSArray class]])
    {
        NSArray *array = (NSArray *)obj;
        jsonObject.type = LKJSONObjectTypeVector;
        for (NSObject *value in array)
        {
            jsonObject.valueVector.push_back(JSONObjectFromNSObject(value));
        }
    }
    else if ([obj isKindOfClass:[NSDictionary class]])
    {
        NSDictionary *dic = (NSDictionary *)obj;
        jsonObject.type = LKJSONObjectTypeMap;
        [dic enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
            if ([key isKindOfClass:[NSString class]])
            {
                NSString *str = (NSString*)key;
                string keyStr = [str cStringUsingEncoding:NSUTF8StringEncoding];
                jsonObject.valueMap[keyStr] = JSONObjectFromNSObject(obj);
            }
            
        }];
    }
    return jsonObject;
}

LKJSONObject LKParticleEffectIOSBridge::JSONObjectFromPath(string path)
{
    NSString *pathStr = [NSString stringWithCString:path.c_str() encoding:NSUTF8StringEncoding];
    NSString *jsonPath = [pathStr stringByAppendingPathComponent:@"params.json"];
    NSError *error;
    NSObject *json = [NSJSONSerialization JSONObjectWithData:[NSData dataWithContentsOfFile:jsonPath] options:NSJSONReadingMutableLeaves error:&error];
    return JSONObjectFromNSObject(json);
}


