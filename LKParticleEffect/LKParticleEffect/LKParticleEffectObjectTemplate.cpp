//
//  LKParticleEffectObjectTemplate.cpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/8.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectObjectTemplate.h"
#include "LKParticleEffectSystem.h"
#include "LKParticleEffectLogger.h"
#include "LKParticleEffectUtil.h"

using namespace LKKit;

#define TAG "LKParticleEffectObjectTemplate"
LKParticleEffectLineProperty::LKParticleEffectLineProperty()
{
    
}

LKParticleEffectLineProperty::LKParticleEffectLineProperty(LKParticleEffectLineProperty &obj)
{
    colorR = obj.colorR;
    colorG = obj.colorG;
    colorB = obj.colorB;
    colorA = obj.colorA;
    width = obj.width;
}

LKParticleEffectSpriteProperty::LKParticleEffectSpriteProperty()
{
    
}

LKParticleEffectSpriteProperty::LKParticleEffectSpriteProperty(LKParticleEffectSpriteProperty &obj)
{
    colorR = obj.colorR;
    colorG = obj.colorG;
    colorB = obj.colorB;
    colorA = obj.colorA;
    texture = obj.texture;
    frameIndex = obj.frameIndex;
    width = obj.width;
    height = obj.height;
}
LKParticleEffectEmitterProperty::LKParticleEffectEmitterProperty()
{
    
}


LKParticleEffectEmitterProperty::LKParticleEffectEmitterProperty(LKParticleEffectEmitterProperty &obj)
{
    emitRate = obj.emitRate;
    emitObjects = obj.emitObjects;
    emitCount = obj.emitCount;
}

LKParticleEffectObjectTemplate::LKParticleEffectObjectTemplate(LKParticleEffectObjectTemplate &obj):vars(obj.vars),sprite(nullptr)
{
    system = obj.system;
    name = obj.name;
    type = obj.type;
    life = obj.life;
    rotation = obj.rotation;
    positionX = obj.positionX;
    positionY = obj.positionY;
    positionZ = obj.positionZ;
    
    if (obj.sprite != nullptr)
    {
        sprite = shared_ptr<LKParticleEffectSpriteProperty>(new LKParticleEffectSpriteProperty(*obj.sprite.get()));
    }
    if (obj.emitter != nullptr)
    {
        emitter = shared_ptr<LKParticleEffectEmitterProperty>(new LKParticleEffectEmitterProperty(*obj.emitter.get()));
    }
}

LKParticleEffectObjectTemplate::LKParticleEffectObjectTemplate(LKParticleEffectSystem *system,const Value &value):vars(system->vars),sprite(nullptr)
{
    this->system = system;
    vector<RVar*> &vars = system->vars;
    if (value.HasMember("type"))
    {
        type = value["type"].GetString();
    }
    else
    {
        type = "point";
    }
    if (value.HasMember("life"))
    {
        life = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["life"], vars));
    }
    else
    {
        life = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(99999));
        life->setVars(vars);
    }
    rotation = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["rotation"], vars));
    positionX = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["positionX"], vars));
    positionY = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["positionY"], vars));
    positionZ = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["positionZ"], vars));
    if (value.HasMember("line"))
    {
        const Value &vline = value["line"];
        if (vline.IsObject())
        {
            line = shared_ptr<LKParticleEffectLineProperty>(new LKParticleEffectLineProperty());
            if (vline.HasMember("colorR"))
            {
                line->colorR = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vline["colorR"], vars));
            }
            else
            {
                line->colorR = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(1));
            }
            
            if (vline.HasMember("colorG"))
            {
                line->colorG = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vline["colorG"], vars));
            }
            else
            {
                line->colorG = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(1));
            }
            
            if (vline.HasMember("colorB"))
            {
                line->colorB = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vline["colorB"], vars));
            }
            else
            {
                line->colorB = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(1));
            }
            
            if (vline.HasMember("colorA"))
            {
                line->colorA = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vline["colorA"], vars));
            }
            else
            {
                line->colorA = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(1));
            }
            if (vline.HasMember("width"))
            {
                line->width = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vline["width"], vars));

            }
            else
            {
                line->width = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(10));
            }
        }
    }
    if (value.HasMember("sprite"))
    {
        const Value &vsprite = value["sprite"];
        if (vsprite.IsObject())
        {
            sprite = shared_ptr<LKParticleEffectSpriteProperty>(new LKParticleEffectSpriteProperty());
            if (vsprite.HasMember("colorR"))
            {
                sprite->colorR = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["colorR"], vars));
            }
            else
            {
                sprite->colorR = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(1));
            }
            
            if (vsprite.HasMember("colorG"))
            {
                sprite->colorG = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["colorG"], vars));
            }
            else
            {
                sprite->colorG = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(1));
            }
            
            if (vsprite.HasMember("colorB"))
            {
                sprite->colorB = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["colorB"], vars));
            }
            else
            {
                sprite->colorB = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(1));
            }
            
            if (vsprite.HasMember("colorA"))
            {
                sprite->colorA = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["colorA"], vars));
            }
            else
            {
                sprite->colorA = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(1));
            }
            
            string textureName = vsprite["texture"].GetString();
            sprite->texture = system->textureMap[textureName];
            sprite->frameIndex = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["frameIndex"], vars));
            sprite->width = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["width"], vars));
            sprite->height = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["height"], vars));
        }
    }
    if (value.HasMember("emitter"))
    {
        const Value &vemitter = value["emitter"];
        emitter = shared_ptr<LKParticleEffectEmitterProperty>(new LKParticleEffectEmitterProperty());
        emitter->emitRate = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vemitter["emitRate"], vars));
        const Value &emitObjects = vemitter["emitObjects"];
        for (SizeType i = 0; i<emitObjects.Size(); i++)
        {
            emitter->emitObjects.push_back(emitObjects[i].GetString());
        }
        if (vemitter.HasMember("emitCount"))
        {
            emitter->emitCount = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vemitter["emitCount"], vars));
        }
        else
        {
            emitter->emitCount = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(INT32_MAX));
        }
    }
}

void LKParticleEffectObjectTemplate::merge(const Value &value)
{
    if (value.HasMember("life"))
    {
        life = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["life"], vars));
    }
    
    if (value.HasMember("type"))
    {
        type = value["type"].GetString();
    }
    
    if (value.HasMember("rotation"))
    {
        rotation = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["rotation"], vars));
    }
    
    if (value.HasMember("positionX"))
    {
        positionX = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["positionX"], vars));
    }
    
    if (value.HasMember("positionY"))
    {
        positionY = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["positionY"], vars));
    }
    
    if (value.HasMember("positionZ"))
    {
        positionZ = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["positionZ"], vars));
    }
    
    if (value.HasMember("sprite"))
    {
        const Value &vsprite = value["sprite"];
        if (vsprite.IsObject())
        {
            if (vsprite.HasMember("colorR"))
            {
                sprite->colorR = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["colorR"], vars));
            }
            
            if (vsprite.HasMember("colorG"))
            {
                sprite->colorG = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["colorG"], vars));
            }
            
            if (vsprite.HasMember("colorB"))
            {
                sprite->colorB = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["colorB"], vars));
            }
            
            if (vsprite.HasMember("colorA"))
            {
                sprite->colorA = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["colorA"], vars));
            }
            
            if (vsprite.HasMember("texture"))
            {
                string textureName = vsprite["texture"].GetString();
                sprite->texture = system->textureMap[textureName];
            }
            
            if (vsprite.HasMember("frameIndex"))
            {
                sprite->frameIndex = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["frameIndex"], vars));
            }
            
            if (vsprite.HasMember("width"))
            {
                sprite->width = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["width"], vars));
            }
            
            if (vsprite.HasMember("height"))
            {
                sprite->height = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vsprite["height"], vars));
            }
        }
    }
    if (value.HasMember("emitter"))
    {
        const Value &vemitter = value["emitter"];
        if (!emitter)
        {
            emitter = shared_ptr<LKParticleEffectEmitterProperty>(new LKParticleEffectEmitterProperty());
        }
        
        if (vemitter.HasMember("emitRate"))
        {
            emitter->emitRate = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vemitter["emitRate"], vars));
        }
        if (vemitter.HasMember("emitObjects"))
        {
            emitter->emitObjects.clear();
            const Value &emitObjects = vemitter["emitObjects"];
            for (SizeType i = 0; i<emitObjects.Size(); i++)
            {
                emitter->emitObjects.push_back(emitObjects[i].GetString());
            }
        }
        if (vemitter.HasMember("emitCount"))
        {
            emitter->emitCount = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(vemitter["emitCount"], vars));
        }
        
    }
}

LKParticleEffectObjectTemplate::~LKParticleEffectObjectTemplate()
{
    
}

void LKParticleEffectObjectTemplate::dump() {
    LKLogInfo("--------- %s ---------", TAG);
    LKLogInfo("name: %s", name.c_str());
    LKLogInfo("type: %s", type.c_str());
    if (sprite != nullptr) {
        LKLogInfo("sprite.texture: %s", sprite->texture->name.c_str());
        LKLogInfo("sprite.width: %s", sprite->width->to_string().c_str());
        LKLogInfo("sprite.height: %s", sprite->height->to_string().c_str());
        LKLogInfo("sprite.R: %s", sprite->colorR->to_string().c_str());
        LKLogInfo("sprite.G: %s", sprite->colorG->to_string().c_str());
        LKLogInfo("sprite.B: %s", sprite->colorB->to_string().c_str());
        LKLogInfo("sprite.A: %s", sprite->colorA->to_string().c_str());
        LKLogInfo("sprite.frameIndex: %s", sprite->frameIndex->to_string().c_str());
    }
    LKLogInfo("######### %s #########", TAG);
}
