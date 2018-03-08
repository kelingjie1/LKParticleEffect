//
//  LKParticleEffectSpriteObject.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "glcontext.h"
#include "LKParticleEffectValue.h"
#include "rapidjson/document.h"
#include "LKParticleEffectProperty.h"
#include "LKParticleEffectObjectTemplate.h"
#ifndef LKParticleEffectSpriteObject_h
#define LKParticleEffectSpriteObject_h
namespace LKKit
{
    class LKParticleEffectSystem;
    using namespace rapidjson;
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
        LKParticleEffectSystem *system;
        LKParticleEffectObjectProperty objectProperty;
        LKParticleEffectObjectTemplate *objectTemplate;
        GLshort identifier;
        LKParticleEffectObjectData *data;
        
        GLfloat distance;
        
        LKParticleEffectObject();
        ~LKParticleEffectObject();

        
    };
}


#endif


