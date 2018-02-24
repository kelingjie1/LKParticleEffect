//
//  LKParticleEffectLogger.cpp
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/22.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectLogger.h"

using namespace LKKit;
LKParticleEffectLogger *LKParticleEffectLogger::instance()
{
    static LKParticleEffectLogger *instance;
    if (!instance)
    {
        instance = new LKParticleEffectLogger();
    }
    return instance;
}

void LKParticleEffectLogger::log(LKParticleEffectLogLevel level,string str)
{
    if (listener)
    {
        listener(level,str.c_str());
    }
}
