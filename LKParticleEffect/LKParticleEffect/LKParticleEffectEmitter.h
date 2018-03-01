//
//  LKParticleEffectEmitter.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//
#ifndef LKParticleEffectEmitter_h
#define LKParticleEffectEmitter_h
#include <string>
#include "LKParticleEffectValue.h"
namespace LKKit
{
    using namespace std;
    class LKParticleEffectEmitter
    {
    public:
        string name;
        LKParticleEffectValue *emitRate;
    };
}

#endif
