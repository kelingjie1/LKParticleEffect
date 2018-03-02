//
//  LKParticleEffectSpriteObject.m
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectObject.h"

using namespace LKKit;
LKParticleEffectGlobalProperty LKParticleEffectObject::globalProperty = LKParticleEffectGlobalProperty();
LKParticleEffectObject::LKParticleEffectObject():rotation(0),positionX(0),positionY(0),positionZ(0)
{
    
}

LKParticleEffectObject::LKParticleEffectObject(const Value &value):sprite(NULL)
{
    vector<RVar*> vars;
    vars.push_back(new RVar("totalTime",&LKParticleEffectObject::globalProperty.totalTime));
    vars.push_back(new RVar("stageTime",&LKParticleEffectObject::globalProperty.stageTime));
    vars.push_back(new RVar("cameraX",&LKParticleEffectObject::globalProperty.cameraX));
    vars.push_back(new RVar("cameraY",&LKParticleEffectObject::globalProperty.cameraY));
    vars.push_back(new RVar("cameraZ",&LKParticleEffectObject::globalProperty.cameraZ));
    vars.push_back(new RVar("cameraDirX",&LKParticleEffectObject::globalProperty.cameraDirX));
    vars.push_back(new RVar("cameraDirY",&LKParticleEffectObject::globalProperty.cameraDirY));
    vars.push_back(new RVar("cameraDirZ",&LKParticleEffectObject::globalProperty.cameraDirZ));
    
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
    if (value.HasMember("sprite"))
    {
        const Value &vsprite = value["sprite"];
        if (vsprite.IsObject())
        {
            sprite = new LKParticleEffectSpriteProperty();
            sprite->colorR = LKParticleEffectValue(vsprite["colorR"]);
        }
    }
    
}

LKParticleEffectObject::~LKParticleEffectObject()
{
    if (sprite)
    {
        delete sprite;
    }
}

