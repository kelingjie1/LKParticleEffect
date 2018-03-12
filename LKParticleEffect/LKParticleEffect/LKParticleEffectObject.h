//
//  LKParticleEffectSpriteObject.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "glcontext.h"
#include <string>
#ifndef LKParticleEffectSpriteObject_h
#define LKParticleEffectSpriteObject_h
namespace LKKit
{
    using namespace std;
    struct LKParticleEffectObjectData
    {
        GLuint identifier;
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
    };
    
    class LKParticleEffectObject
    {
    public:
        LKParticleEffectObjectData *data;
        string group;
        GLfloat distance;
    };
}


#endif


