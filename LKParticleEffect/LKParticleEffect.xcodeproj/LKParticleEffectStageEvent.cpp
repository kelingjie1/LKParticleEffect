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

using namespace LKKit;
LKParticleEffectStageEventAction::LKParticleEffectStageEventAction(LKParticleEffectStage *stage,const Value &value):stage(stage)
{
    
}

void LKParticleEffectStageEventAction::trigger()
{
    
}

LKParticleEffectStageEventChangeStageAction::LKParticleEffectStageEventChangeStageAction(LKParticleEffectStage *stage,const Value &value):LKParticleEffectStageEventAction(stage,value)
{
    target = value["target"].GetString();
}

void LKParticleEffectStageEventChangeStageAction::trigger()
{
    auto system = stage->system;
    system->setNextStage(target);
}


LKParticleEffectStageEvent::LKParticleEffectStageEvent(LKParticleEffectStage *stage,const Value &value):autoCheck(false),stage(stage)
{
    name = value["name"].GetString();
    if (value.HasMember("action"))
    {
        const Value &v = value["action"];
        string type = v["type"].GetString();
        if (type == "change_stage")
        {
            action = shared_ptr<LKParticleEffectStageEventAction>(new LKParticleEffectStageEventChangeStageAction(stage,v));
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
}
