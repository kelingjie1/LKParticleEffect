//
//  LKParticleEffectLine.h
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/23.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "glcontext.h"
#include <string>
#include <memory>
#include <vector>

#ifndef LKParticleEffectLine_h
#define LKParticleEffectLine_h

namespace LKKit
{
    using namespace std;
    class LKParticleEffectLine
    {
    public:
        GLfloat width;
        vector<GLuint> indexes;
        
    };
}

#endif /* LKParticleEffectLine_h */
