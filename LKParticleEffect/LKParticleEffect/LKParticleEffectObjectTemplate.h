//
//  LKParticleEffectObjectTemplate.hpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/8.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#ifndef LKParticleEffectObjectTemplate_h
#define LKParticleEffectObjectTemplate_h

#include "glcontext.h"
#include "LKParticleEffectValue.h"
#include "rapidjson/document.h"
#include <string>
namespace LKKit
{
    class LKParticleEffectSystem;
    using namespace std;
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
    struct LKParticleEffectEmitterProperty
    {
        LKParticleEffectValue emitRate;
        vector<string> emitObjects;
    };
    
    class LKParticleEffectObjectTemplate
    {
    public:
        string name;
        string type;
        
        LKParticleEffectSpriteProperty *sprite;
        LKParticleEffectEmitterProperty *emitter;
        LKParticleEffectValue life;
        LKParticleEffectValue rotation;
        LKParticleEffectValue positionX;
        LKParticleEffectValue positionY;
        LKParticleEffectValue positionZ;
        
        LKParticleEffectObjectTemplate(vector<RVar*> &vars,const Value &value);
        LKParticleEffectObjectTemplate(LKParticleEffectObjectTemplate &obj);
        ~LKParticleEffectObjectTemplate();
        void merge(const Value &value);

        void dump();

    protected:
        vector<RVar*> &vars;
    };
}

#endif /* LKParticleEffectObjectTemplate_h */
