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

using namespace LKKit;

#define TAG "LKParticleEffectObjectTemplate"

LKParticleEffectObjectTemplate::LKParticleEffectObjectTemplate(LKParticleEffectObjectTemplate &obj):vars(obj.vars),sprite(nullptr)
{
    system = obj.system;
    name = obj.name;
    type = obj.type;
    rotation = obj.rotation;
    positionX = obj.positionX;
    positionY = obj.positionY;
    positionZ = obj.positionZ;
    
    if (obj.sprite != nullptr)
    {
        sprite = new LKParticleEffectSpriteProperty();
        sprite->colorR = obj.sprite->colorR;
        sprite->colorG = obj.sprite->colorG;
        sprite->colorB = obj.sprite->colorB;
        sprite->colorA = obj.sprite->colorA;
        sprite->texture = obj.sprite->texture;
        sprite->frameIndex = obj.sprite->frameIndex;
        sprite->width = obj.sprite->width;
        sprite->height = obj.sprite->height;
    }
}

LKParticleEffectObjectTemplate::LKParticleEffectObjectTemplate(LKParticleEffectSystem *system,const Value &value):vars(system->vars),sprite(nullptr)
{
    this->system = system;
    vector<RVar*> &vars = system->vars;
    name = value["name"].GetString();
    type = value["type"].GetString();
    rotation = LKParticleEffectValue(value["rotation"], vars);
    positionX = LKParticleEffectValue(value["positionX"], vars);
    positionY = LKParticleEffectValue(value["positionY"], vars);
    positionZ = LKParticleEffectValue(value["positionZ"], vars);
    if (value.HasMember("sprite"))
    {
        const Value &vsprite = value["sprite"];
        if (vsprite.IsObject())
        {
            sprite = new LKParticleEffectSpriteProperty();
            sprite->colorR = LKParticleEffectValue(vsprite["colorR"], vars);
            sprite->colorG = LKParticleEffectValue(vsprite["colorG"], vars);
            sprite->colorB = LKParticleEffectValue(vsprite["colorB"], vars);
            sprite->colorA = LKParticleEffectValue(vsprite["colorA"], vars);
            string textureName = vsprite["texture"].GetString();
            sprite->texture = system->textureMap[textureName];
            sprite->frameIndex = LKParticleEffectValue(vsprite["frameIndex"], vars);
            sprite->width = LKParticleEffectValue(vsprite["width"], vars);
            sprite->height = LKParticleEffectValue(vsprite["height"], vars);
        }
    }
    if (value.HasMember("emitter"))
    {
        const Value &vemitter = value["emitter"];
        emitter = new LKParticleEffectEmitterProperty();
        emitter->emitRate = LKParticleEffectValue(vemitter["emitRate"], vars);
        const Value &emitObjects = vemitter["emitObjects"];
        for (SizeType i = 0; i<emitObjects.Size(); i++)
        {
            emitter->emitObjects.push_back(emitObjects[i].GetString());
        }
    }
}

void LKParticleEffectObjectTemplate::merge(const Value &value)
{
    if (value.HasMember("rotation")) {
        rotation = LKParticleEffectValue(value["rotation"], vars);
    }
    
    if (value.HasMember("positionX")) {
        positionX = LKParticleEffectValue(value["positionX"], vars);
    }
    
    if (value.HasMember("positionY")) {
        positionY = LKParticleEffectValue(value["positionY"], vars);
    }
    
    if (value.HasMember("positionZ")) {
        positionZ = LKParticleEffectValue(value["positionZ"], vars);
    }
    
    if (value.HasMember("sprite")) {
        const Value &vsprite = value["sprite"];
        if (vsprite.IsObject()) {
            if (vsprite.HasMember("colorR")) {
                sprite->colorR = LKParticleEffectValue(vsprite["colorR"], vars);
            }
            
            if (vsprite.HasMember("colorG")) {
                sprite->colorG = LKParticleEffectValue(vsprite["colorG"], vars);
            }
            
            if (vsprite.HasMember("colorB")) {
                sprite->colorB = LKParticleEffectValue(vsprite["colorB"], vars);
            }
            
            if (vsprite.HasMember("colorA")) {
                sprite->colorA = LKParticleEffectValue(vsprite["colorA"], vars);
            }
            
            if (vsprite.HasMember("texture")) {
                string textureName = vsprite["texture"].GetString();
                sprite->texture = system->textureMap[textureName];
            }
            
            if (vsprite.HasMember("frameIndex")) {
                sprite->frameIndex = LKParticleEffectValue(vsprite["frameIndex"], vars);
            }
            
            if (vsprite.HasMember("width")) {
                sprite->width = LKParticleEffectValue(vsprite["width"], vars);
            }
            
            if (vsprite.HasMember("height")) {
                sprite->height = LKParticleEffectValue(vsprite["height"], vars);
            }
        }
    }
}

LKParticleEffectObjectTemplate::~LKParticleEffectObjectTemplate()
{
    if (sprite)
    {
        delete sprite;
    }
    sprite = nullptr;
    
    for (int i = 0; i < vars.size(); ++i) {
        delete(vars[i]);
    }
    vars.clear();
}

void LKParticleEffectObjectTemplate::dump() {
    LKLogInfo("--------- %s ---------", TAG);
    LKLogInfo("name: %s", name.c_str());
    LKLogInfo("type: %s", type.c_str());
    if (sprite != nullptr) {
        LKLogInfo("sprite.texture: %s", sprite->texture->name.c_str());
        LKLogInfo("sprite.width: %s", sprite->width.to_string().c_str());
        LKLogInfo("sprite.height: %s", sprite->height.to_string().c_str());
        LKLogInfo("sprite.R: %s", sprite->colorR.to_string().c_str());
        LKLogInfo("sprite.G: %s", sprite->colorG.to_string().c_str());
        LKLogInfo("sprite.B: %s", sprite->colorB.to_string().c_str());
        LKLogInfo("sprite.A: %s", sprite->colorA.to_string().c_str());
        LKLogInfo("sprite.frameIndex: %s", sprite->frameIndex.to_string().c_str());
    }
    LKLogInfo("######### %s #########", TAG);
}
