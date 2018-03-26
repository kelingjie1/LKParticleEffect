//
//  LKParticleEffectSytemMotionManager.m
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/26.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#import "LKParticleEffectSystemManager.h"
#import "LKParticleEffectSystem.h"

using namespace LKKit;
@implementation LKParticleEffectSystemManager

- (instancetype)initWithSystem:(LKKit::LKParticleEffectSystem *)system
{
    if (self = [super init])
    {
        self.system = system;
        self.moveMultiple = 500;
        self.queue = [NSOperationQueue new];
        self.motionMatrix = GLKMatrix4Identity;
    }
    return self;
}

- (void)updateWithDeviceMotion:(CMDeviceMotion *)deviceMotion
{
    CMRotationMatrix a = deviceMotion.attitude.rotationMatrix;
    GLKMatrix4 matrix = GLKMatrix4Make(a.m11, a.m21, a.m31, 0.0f,
                               a.m12, a.m22, a.m32, 0.0f,
                               a.m13, a.m23, a.m33, 0.0f,
                               0.0f , 0.0f , 0.0f , 1.0f);
    self.motionMatrix = GLKMatrix4Rotate(matrix, M_PI_2, 1, 0, 0);
}

- (void)updateWithARFrame:(ARFrame *)arframe
{
    self.x = -arframe.camera.transform.columns[3][0]*self.moveMultiple;
    self.y = arframe.camera.transform.columns[3][1]*self.moveMultiple;
    self.z = arframe.camera.transform.columns[3][2]*self.moveMultiple;
}

- (void)updateSystemData:(dispatch_block_t)block
{
    NSBlockOperation *op = [NSBlockOperation blockOperationWithBlock:block];
    [self.queue addOperation:op];
}

- (void)updateSystem:(float)timeDelta
{
    [self.queue setSuspended:NO];
    [self.queue waitUntilAllOperationsAreFinished];
    [self.queue setSuspended:YES];
    auto camera = dynamic_cast<LKParticleEffect3DCamera*>(self.system->camera.get());
    if (camera)
    {
        GLKMatrix4 r = self.motionMatrix;
        Matrix4f m;
        m<<
        r.m00,r.m10,r.m20,r.m30,
        r.m01,r.m11,r.m21,r.m31,
        r.m02,r.m12,r.m22,r.m32,
        r.m03,r.m13,r.m23,r.m33;
        camera->motionMatrix = m;
        camera->positionOffsetX = self.x;
        camera->positionOffsetY = self.y;
        camera->positionOffsetZ = self.z;
    }
    self.system->update(timeDelta);
    [self.queue setSuspended:NO];
}

- (void)render
{
    self.system->render();
}

@end
