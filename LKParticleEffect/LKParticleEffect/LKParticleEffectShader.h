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

namespace LKKit
{
    using namespace std;
    class LKParticleEffectShader
    {
    public:
        static LKParticleEffectShader *instance();
        string vertexShaderStr;
        string fragmentShaderStr;
    protected:
        LKParticleEffectShader();
    };
}


#endif /* LKParticleEffectShader_h */
