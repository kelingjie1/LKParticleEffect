//
//  LKParticleEffectStageEvent.cpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectStageEvent.h"
#include "LKParticleEffectStage.h"
#include "LKParticleEffectSystem.h"
#include <iostream>

using namespace LKKit;
LKParticleEffectStageEventAction::LKParticleEffectStageEventAction(LKParticleEffectStage *stage,const Value &value):stage(stage)
{
    
}

void LKParticleEffectStageEventAction::trigger()
{
    
}

LKParticleEffectStageCommandAction::LKParticleEffectStageCommandAction(LKParticleEffectStage *stage,const Value &value):LKParticleEffectStageEventAction(stage,value)
{
    command = value["command"].GetString();
}

void LKParticleEffectStageCommandAction::trigger()
{
    auto system = stage->system;
    if (command == "lineIndex++")
    {
        system->lines.push_back(vector<GLuint>());
    }
}

LKParticleEffectStageChangeStageAction::LKParticleEffectStageChangeStageAction(LKParticleEffectStage *stage,const Value &value):LKParticleEffectStageEventAction(stage,value)
{
    target = value["target"].GetString();
}

void LKParticleEffectStageChangeStageAction::trigger()
{
    auto system = stage->system;
    system->setNextStage(target);
}

LKParticleEffectStageDrawLineAction::LKParticleEffectStageDrawLineAction(LKParticleEffectStage *stage,const Value &value):LKParticleEffectStageEventAction(stage,value)
{
    object = value["object"].GetString();
}

void LKParticleEffectStageDrawLineAction::trigger()
{
    auto system = stage->system;
    auto obj = system->getUnusedObject(object);
    auto camera = dynamic_cast<LKParticleEffect3DCamera*>(system->camera.get());
    auto vec = camera->rayCast(system->inputProperty.touch2DX, system->inputProperty.touch2DY);
    obj->positionOffsetX = vec.x()*100;
    obj->positionOffsetY = -vec.y()*100;
    obj->positionOffsetZ = vec.z()*100;
    cout<<vec*100<<endl<<endl;
    system->lines[system->lines.size()-1].push_back(obj->index);
}


LKParticleEffectStageEvent::LKParticleEffectStageEvent(LKParticleEffectStage *stage,const Value &value):autoCheck(false),stage(stage)
{
    if (value.HasMember("action"))
    {
        const Value &v = value["action"];
        string type = v["type"].GetString();
        if (type == "change_stage")
        {
            action = shared_ptr<LKParticleEffectStageEventAction>(new LKParticleEffectStageChangeStageAction(stage,v));
        }
        else if (type == "drawLine")
        {
            action = shared_ptr<LKParticleEffectStageEventAction>(new LKParticleEffectStageDrawLineAction(stage,v));
        }
        else if (type == "command")
        {
            action = shared_ptr<LKParticleEffectStageEventAction>(new LKParticleEffectStageCommandAction(stage,v));
        }
            
    }
    
}

void LKParticleEffectStageEvent::check()
{
    
}

void LKParticleEffectStageEvent::trigger()
{
    if (action)
    {
        action->trigger();
    }
}

LKParticleEffectStageDelayEvent::LKParticleEffectStageDelayEvent(LKParticleEffectStage *stage,const Value &value):LKParticleEffectStageEvent(stage,value)
{
    autoCheck = true;
    time = value["time"].GetDouble();
}

void LKParticleEffectStageDelayEvent::check()
{
    auto system = stage->system;
    if (system->globalProperty.stageTime>time)
    {
        trigger();
    }
}
