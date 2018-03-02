//
//  LKParticleEffectLogger.cpp
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/22.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include <stdarg.h>
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

static char s_buf[1024];

void LKParticleEffectLogger::log(LKParticleEffectLogLevel level, string fmt, ...)
{
    if (listener)
    {
        va_list args;
        va_start(args, fmt);
        snprintf(s_buf, 1024, fmt.c_str(), args);
        va_end(args);

        listener(level, s_buf);
    }
}
