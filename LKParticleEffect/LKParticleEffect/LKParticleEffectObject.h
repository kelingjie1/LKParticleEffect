//
//  LKParticleEffectSpriteObject.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "glcontext.h"
#include <string>
#include <memory>
#include "LKParticleEffectObjectTemplate.h"
#include "LKParticleEffectProperty.h"

#ifndef LKParticleEffectSpriteObject_h
#define LKParticleEffectSpriteObject_h
namespace LKKit
{
    using namespace std;
    struct LKParticleEffectObjectData
    {
        GLuint index;
        GLfloat positionX;
        GLfloat positionY;
        GLfloat positionZ;
        GLfloat width;
        GLfloat height;
        GLfloat colorR;
        GLfloat colorG;
        GLfloat colorB;
        GLfloat colorA;
        GLfloat textureU;
        GLfloat textureV;
        GLfloat textureIndex;
        GLfloat rotation;
        GLfloat frameIndex;
    };
    
    class LKParticleEffectObject
    {
    public:
        GLuint index;
        shared_ptr<LKParticleEffectObjectTemplate> objectTemplate;
        LKParticleEffectObjectProperty property;
        double life;
        double positionOffsetX;
        double positionOffsetY;
        double positionOffsetZ;
        double rotationOffset;
        double emitRestTime;
        int emitCount;
        string group;
        GLfloat distance;
        LKParticleEffectObject();
        void reset();
    };
}


#endif


