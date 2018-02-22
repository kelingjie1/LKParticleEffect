//
//  LKParticleEffectShader.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/22.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#ifndef LKParticleEffectShader_h
#define LKParticleEffectShader_h

#include <string>

class LKParticleEffectShader
{
public:
    static LKParticleEffectShader *instance();
    std::string vertexShaderStr;
    std::string fragmentShaderStr;
protected:
    LKParticleEffectShader();
};

#endif /* LKParticleEffectShader_h */
