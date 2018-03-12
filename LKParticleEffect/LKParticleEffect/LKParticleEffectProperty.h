//
//  LKParticleEffectProperty.hpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/8.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#ifndef LKParticleEffectProperty_h
#define LKParticleEffectProperty_h

namespace LKKit
{
    struct LKParticleEffectGlobalProperty
    {
        double totalTime;
        double stageTime;
        double cameraX;
        double cameraY;
        double cameraZ;
        double cameraDirX;
        double cameraDirY;
        double cameraDirZ;
    };
    
    struct LKParticleEffectObjectProperty
    {
        double t;
        double random[10];
        
        double last_colorR;
        double last_colorG;
        double last_colorB;
        double last_colorA;
        double last_frameIndex;
        double last_width;
        double last_height;
        
        void reset();
    };
}

#endif /* LKParticleEffectProperty_h */
