//
//  LKParticleEffectLogger.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/22.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#ifndef LKParticleEffectLogger_h
#define LKParticleEffectLogger_h

#define LKLog(level,str) LKParticleEffectLogger::instance()->log(level,str);
#define LKLogInfo(str) LKLog(LKParticleEffectLogLevelInfo,str);
#define LKLogWarning(str) LKLog(LKParticleEffectLogLevelWarning,str);
#define LKLogError(str) LKLog(LKParticleEffectLogLevelError,str);

#include <string>
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
    void log(LKParticleEffectLogLevel level,std::string str);
    LKParticleEffectLoggerListener *listener;
protected:
    LKParticleEffectLogger(){};
};

#endif /* LKParticleEffectLogger_hpp */
