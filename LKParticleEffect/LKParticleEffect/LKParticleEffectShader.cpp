//
//  LKParticleEffectShader.cpp
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/22.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectShader.h"
using namespace std;
#define MULTI_LINE_STRING(a) #a

LKParticleEffectShader *LKParticleEffectShader::instance()
{
    static LKParticleEffectShader *instance = NULL;
    if (!instance)
    {
        instance = new LKParticleEffectShader();
    }
    return instance;
}

LKParticleEffectShader::LKParticleEffectShader()
{
    vertexShaderStr = string("#version 300 es\n")+
    MULTI_LINE_STRING
    (
     layout(location = 0) in uint identifier;
     layout(location = 1) in vec3 position;
     void main()
    {
        gl_PointSize = 100.0;
        gl_Position = vec4(position.x, position.y, position.z, 1.0);
    }
     );
    
    fragmentShaderStr = string("#version 300 es\n")+
    MULTI_LINE_STRING
    (
     precision highp float;
     out vec4 color;
     void main()
     {
         color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
     }
     );
}
