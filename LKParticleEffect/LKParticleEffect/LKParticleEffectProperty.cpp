//
//  LKParticleEffectProperty.cpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/8.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectProperty.h"
#include "LKParticleEffectUtil.h"

using namespace LKKit;


void LKParticleEffectObjectProperty::reset()
{
    memset(this, 0, sizeof(LKParticleEffectObjectProperty));
    for (int i=0; i<10; i++)
    {
        random[i] = rand();
    }
    
}
