//
//  LKParticleEffectSpriteObject.m
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectObject.h"
using namespace LKKit;
LKParticleEffectObject::LKParticleEffectObject()
{
    property.reset();
}

void LKParticleEffectObject::reset()
{
    emitRestTime = 0;
    emitCount = 0;
}
