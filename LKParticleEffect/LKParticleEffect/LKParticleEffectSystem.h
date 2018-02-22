//
//  LKParticleEffectSystem.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LKParticleEffectEmitter.h"
#import "LKParticleEffectObject.h"

@interface LKParticleEffectConfig : NSObject

@property (nonatomic) NSInteger maxObjectCount;

+ (instancetype)defaultConfig;

@end

@interface LKParticleEffectSystem : NSObject

- (instancetype)initWithConfig:(LKParticleEffectConfig*)config;
- (void)setup;
- (void)update:(double)timeDelta;
- (void)render;

@end
