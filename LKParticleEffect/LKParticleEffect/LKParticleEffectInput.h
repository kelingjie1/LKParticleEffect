//
//  LKParticleEffectInput.hpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/1.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#ifndef LKParticleEffectInput_h
#define LKParticleEffectInput_h

namespace LKKit
{
    using namespace std;
    class LKParticleEffectInput
    {
    public:
        float abstime;
        float t;
        float cameraX;
        float cameraY;
        float cameraZ;
        float cameraDirX;
        float cameraDirY;
        float cameraDirZ;
        
        float last_abstime;
        float last_t;
        float last_colorR;
        float last_colorG;
        float last_colorB;
        float last_colorA;
        float last_texture;
        float last_frameIndex;
        float last_width;
        float last_height;
        float last_rotation;
        float last_positionX;
        float last_positionY;
        float last_positionZ;
        
    };
}

#endif /* LKParticleEffectInput_h */
