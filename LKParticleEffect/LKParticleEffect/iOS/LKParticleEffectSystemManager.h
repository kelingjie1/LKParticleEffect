//
//  LKParticleEffectSytemMotionManager.h
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/26.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ARKit/ARKit.h>
#import <CoreMotion/CoreMotion.h>

namespace LKKit
{
    class LKParticleEffectSystem;
}

@interface LKParticleEffectSystemManager : NSObject

@property (nonatomic,strong) NSOperationQueue *queue;
@property (nonatomic,assign) LKKit::LKParticleEffectSystem *system;
@property (nonatomic,assign) float moveMultiple;
@property (nonatomic,strong) ARFrame *arframe;
@property (nonatomic,strong) CMDeviceMotion *deviceMotion;
@property (nonatomic,assign) GLKMatrix4 motionMatrix;
@property (nonatomic,assign) float x;
@property (nonatomic,assign) float y;
@property (nonatomic,assign) float z;


- (instancetype)initWithSystem:(LKKit::LKParticleEffectSystem*)system;
- (void)updateWithDeviceMotion:(CMDeviceMotion*)deviceMotion;
- (void)updateWithARFrame:(ARFrame*)arframe;
- (void)updateSystemData:(dispatch_block_t)block;
- (void)updateSystem:(float)timeDelta;
- (void)render;


@end
