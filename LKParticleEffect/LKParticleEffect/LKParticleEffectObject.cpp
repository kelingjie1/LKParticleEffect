//
//  LKParticleEffectSpriteObject.m
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectObject.h"
#include "LKParticleEffectSystem.h"
using namespace LKKit;
LKParticleEffectObject::LKParticleEffectObject():rotation(0),positionX(0),positionY(0),positionZ(0),__var_reuse(true)
{
    
}

LKParticleEffectObject::LKParticleEffectObject(LKParticleEffectSystem *system,const Value &value):sprite(NULL),__var_reuse(false)
{
    vars.push_back(new RVar("totalTime",&system->globalProperty.totalTime));
    vars.push_back(new RVar("stageTime",&system->globalProperty.stageTime));
    vars.push_back(new RVar("cameraX",&system->globalProperty.cameraX));
    vars.push_back(new RVar("cameraY",&system->globalProperty.cameraY));
    vars.push_back(new RVar("cameraZ",&system->globalProperty.cameraZ));
    vars.push_back(new RVar("cameraDirX",&system->globalProperty.cameraDirX));
    vars.push_back(new RVar("cameraDirY",&system->globalProperty.cameraDirY));
    vars.push_back(new RVar("cameraDirZ",&system->globalProperty.cameraDirZ));

    vars.push_back(new RVar("t",&objectProperty.t));
    vars.push_back(new RVar("rand0",&objectProperty.rand0));
    vars.push_back(new RVar("rand1",&objectProperty.rand1));
    vars.push_back(new RVar("rand2",&objectProperty.rand2));
    vars.push_back(new RVar("rand3",&objectProperty.rand3));
    vars.push_back(new RVar("rand4",&objectProperty.rand4));
    vars.push_back(new RVar("rand5",&objectProperty.rand5));
    vars.push_back(new RVar("rand6",&objectProperty.rand6));
    vars.push_back(new RVar("rand7",&objectProperty.rand7));
    vars.push_back(new RVar("rand8",&objectProperty.rand8));
    vars.push_back(new RVar("rand9",&objectProperty.rand9));
    vars.push_back(new RVar("last_colorR",&objectProperty.last_colorR));
    vars.push_back(new RVar("last_colorG",&objectProperty.last_colorG));
    vars.push_back(new RVar("last_colorB",&objectProperty.last_colorB));
    vars.push_back(new RVar("last_colorA",&objectProperty.last_colorA));
    vars.push_back(new RVar("last_frameIndex",&objectProperty.last_frameIndex));
    vars.push_back(new RVar("last_width",&objectProperty.last_width));
    vars.push_back(new RVar("last_height",&objectProperty.last_height));

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
            sprite->texture = vsprite["texture"].GetString();
            sprite->frameIndex = LKParticleEffectValue(vsprite["frameIndex"], vars);
            sprite->width = LKParticleEffectValue(vsprite["width"], vars);
            sprite->height = LKParticleEffectValue(vsprite["height"], vars);
        }
    }
    
}

LKParticleEffectObject::~LKParticleEffectObject()
{
    if (sprite)
    {
        delete sprite;
    }

    if (!__var_reuse) {
        for (int i = 0; i < vars.size(); ++i) {
            delete(vars[i]);
        }

    } else {
        vars.clear();
    }

}

LKParticleEffectObject::LKParticleEffectObject(const LKParticleEffectObject &obj) {
    name = obj.name;
    type = obj.type;
    rotation = obj.rotation;
    positionX = obj.positionX;
    positionY = obj.positionY;
    positionZ = obj.positionZ;

    if (obj.sprite != nullptr) {
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

    vars = obj.vars;
    __var_reuse = true;
}
