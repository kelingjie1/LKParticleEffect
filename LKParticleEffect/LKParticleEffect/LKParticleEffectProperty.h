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
    class LKParticleEffectInputProperty
    {
    public:
        double touch2DX;//0~1
        double touch2DY;//0~1
    };
    class LKParticleEffectGlobalProperty
    {
    public:
        double totalTime;
        double stageTime;
        double cameraX;
        double cameraY;
        double cameraZ;
        double cameraDirX;
        double cameraDirY;
        double cameraDirZ;
    };
    
    class LKParticleEffectObjectProperty
    {
    public:
        double t;
        double total_t;
        double random[10];
        
        double last_colorR;
        double last_colorG;
        double last_colorB;
        double last_colorA;
        double last_frameIndex;
        double last_width;
        double last_height;
        double last_positionX;
        double last_positionY;
        double last_positionZ;
        
        void reset();
    };
}

#endif /* LKParticleEffectProperty_h */
