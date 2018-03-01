//
//  LKParticleEffectCamera.h
//  LKParticleEffect
//
//  Created by lingtonke on 2018/2/26.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#ifndef LKParticleEffectCamera_h
#define LKParticleEffectCamera_h
#include "glcontext.h"
#include <vector>

namespace LKKit
{
    using namespace std;
    class LKParticleEffectCamera
    {
        
    public:
        LKParticleEffectCamera();
        vector<float> m;
        void setMotionMatrix(vector<float> motionMatrix);
    };
}

#endif /* LKParticleEffectCamera_h */
