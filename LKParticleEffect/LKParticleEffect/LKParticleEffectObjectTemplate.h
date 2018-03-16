//
//  LKParticleEffectObjectTemplate.hpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/8.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "glcontext.h"
#include "LKParticleEffectValue.h"
#include "LKParticleEffectTexture.h"
#include "rapidjson/document.h"
#include <string>
#include <memory>

#ifndef LKParticleEffectObjectTemplate_h
#define LKParticleEffectObjectTemplate_h

namespace LKKit
{
    class LKParticleEffectSystem;
    using namespace std;
    class LKParticleEffectSpriteProperty
    {
    public:
        shared_ptr<LKParticleEffectValue> colorR;
        shared_ptr<LKParticleEffectValue> colorG;
        shared_ptr<LKParticleEffectValue> colorB;
        shared_ptr<LKParticleEffectValue> colorA;
        shared_ptr<LKParticleEffectTexture> texture;
        shared_ptr<LKParticleEffectValue> frameIndex;
        shared_ptr<LKParticleEffectValue> width;
        shared_ptr<LKParticleEffectValue> height;
        LKParticleEffectSpriteProperty();
        LKParticleEffectSpriteProperty(LKParticleEffectSpriteProperty &obj);
    };
    class LKParticleEffectEmitterProperty
    {
    public:
        shared_ptr<LKParticleEffectValue> emitRate;
        vector<string> emitObjects;
        shared_ptr<LKParticleEffectValue> emitCount;
        LKParticleEffectEmitterProperty();
        LKParticleEffectEmitterProperty(LKParticleEffectEmitterProperty &obj);
    };
    
    class LKParticleEffectObjectTemplate
    {
    public:
        string name;
        string type;
        
        shared_ptr<LKParticleEffectSpriteProperty> sprite;
        shared_ptr<LKParticleEffectEmitterProperty> emitter;
        shared_ptr<LKParticleEffectValue> life;
        shared_ptr<LKParticleEffectValue> rotation;
        shared_ptr<LKParticleEffectValue> positionX;
        shared_ptr<LKParticleEffectValue> positionY;
        shared_ptr<LKParticleEffectValue> positionZ;
        
        LKParticleEffectObjectTemplate(LKParticleEffectSystem *system,const Value &value);
        LKParticleEffectObjectTemplate(LKParticleEffectObjectTemplate &obj);
        ~LKParticleEffectObjectTemplate();
        void merge(const Value &value);

        void dump();

    protected:
        LKParticleEffectSystem *system;
        vector<RVar*> &vars;
    };
}

#endif /* LKParticleEffectObjectTemplate_h */
