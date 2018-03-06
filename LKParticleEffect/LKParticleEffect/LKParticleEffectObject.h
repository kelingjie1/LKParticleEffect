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
    
    struct LKParticleEffectObjectProperty
    {
        double t;
        double rand0;
        double rand1;
        double rand2;
        double rand3;
        double rand4;
        double rand5;
        double rand6;
        double rand7;
        double rand8;
        double rand9;
        
        double last_colorR;
        double last_colorG;
        double last_colorB;
        double last_colorA;
        double last_frameIndex;
        double last_width;
        double last_height;
    };
    
    
    struct LKParticleEffectSpriteProperty
    {
        LKParticleEffectValue colorR;
        LKParticleEffectValue colorG;
        LKParticleEffectValue colorB;
        LKParticleEffectValue colorA;
        string texture;
        LKParticleEffectValue frameIndex;
        LKParticleEffectValue width;
        LKParticleEffectValue height;
    };
    
    class LKParticleEffectObject
    {
    public:
        LKParticleEffectSystem *system;
        LKParticleEffectObjectProperty objectProperty;
        GLshort identifier;
        LKParticleEffectObjectData *data;
        
        string name;
        string type;
        LKParticleEffectSpriteProperty *sprite;
        LKParticleEffectValue rotation;
        LKParticleEffectValue positionX;
        LKParticleEffectValue positionY;
        LKParticleEffectValue positionZ;
        GLfloat distance;
        LKParticleEffectObject(LKParticleEffectSystem *system,const Value &value);
        LKParticleEffectObject(const LKParticleEffectObject &obj);
        LKParticleEffectObject();
        ~LKParticleEffectObject();

        vector<RVar*> vars;
        bool __var_reuse;
    };
}


#endif


