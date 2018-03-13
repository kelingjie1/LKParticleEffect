//
//  LKParticleEffectLogger.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/22.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#ifndef LKParticleEffectLogger_h
#define LKParticleEffectLogger_h

#define LKLog(level, ...) LKParticleEffectLogger::instance()->log(level,__VA_ARGS__);
#define LKLogInfo(...) LKLog(LKParticleEffectLogLevelInfo,__VA_ARGS__);
#define LKLogWarning(...) LKLog(LKParticleEffectLogLevelWarning,__VA_ARGS__);
#define LKLogError(...) LKLog(LKParticleEffectLogLevelError, __VA_ARGS__);

#include <string>

namespace LKKit
{
    using namespace std;
    enum LKParticleEffectLogLevel
    {
        LKParticleEffectLogLevelInfo,
        LKParticleEffectLogLevelWarning,
        LKParticleEffectLogLevelError,
    };
    
    typedef void LKParticleEffectLoggerListener(LKParticleEffectLogLevel level,const char* str);
    class LKParticleEffectLogger
    {
    public:
        static LKParticleEffectLogger *instance();
        void log(LKParticleEffectLogLevel level, string fmt, ...);
        LKParticleEffectLoggerListener *listener;
    protected:
        LKParticleEffectLogger():listener(nullptr){};
    };

}


#endif /* LKParticleEffectLogger_hpp */
