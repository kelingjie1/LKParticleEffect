//
//  LKStageOperation.cpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/12.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectStageOperation.h"
#include "LKParticleEffectStage.h"
#include "LKParticleEffectSystem.h"
using namespace LKKit;
LKParticleEffectStageOperation::LKParticleEffectStageOperation(LKParticleEffectStage *stage,const Value &value)
{
    this->stage = stage;
}

void LKParticleEffectStageOperation::process()
{
    
}

LKParticleEffectStageModifyOperation::LKParticleEffectStageModifyOperation(LKParticleEffectStage *stage,const Value &value):LKParticleEffectStageOperation(stage,value)
{
    name = "add";
    for (SizeType i=0; i<value.Size(); i++)
    {
        LKParticleEffectStageModifyOperationObject object;
        object.object = value[i]["object"].GetString();
        if (value[i].HasMember("group"))
        {
            object.group = value[i]["group"].GetString();
        }
        else
        {
            object.group = "";
        }
        objectList.push_back(object);
    }
}

LKParticleEffectStageAddOperation::LKParticleEffectStageAddOperation(LKParticleEffectStage *stage,const Value &value):LKParticleEffectStageModifyOperation(stage,value)
{
    
}

void LKParticleEffectStageAddOperation::process()
{
    LKParticleEffectSystem *system = stage->system;
    for (int i=0; i<objectList.size(); i++)
    {
        auto object = system->getUnusedObject(objectList[i].object);
        object->group = objectList[i].group;
    }
}

LKParticleEffectStageRemoveOperation::LKParticleEffectStageRemoveOperation(LKParticleEffectStage *stage,const Value &value):LKParticleEffectStageModifyOperation(stage,value)
{
    
}

void LKParticleEffectStageRemoveOperation::process()
{
    LKParticleEffectSystem *system = stage->system;
    for (auto objsetIt = system->usedObjects.begin();objsetIt!=system->usedObjects.end();objsetIt++)
    {
        auto &objset = objsetIt->second;
        auto objsetcopy = objsetIt->second;
        for (auto it=objsetcopy.begin(); it!=objsetcopy.end(); it++)
        {
            auto obj = *it;
            for (int i=0; i<objectList.size(); i++)
            {
                if (obj->group == objectList[i].group&&
                    obj->objectTemplate->name==objectList[i].object)
                {
                    objset.erase(obj);
                }
            }
            
        }
    }
    

}

LKParticleEffectStageResetObjectsOperation::LKParticleEffectStageResetObjectsOperation(LKParticleEffectStage *stage,const Value &value):LKParticleEffectStageOperation(stage,value)
{
    
}
void LKParticleEffectStageResetObjectsOperation::process()
{
    LKParticleEffectSystem *system = stage->system;
    for (auto objsetIt = system->usedObjects.begin();objsetIt!=system->usedObjects.end();objsetIt++)
    {
        auto &objset = objsetIt->second;
        for (auto &it:objset)
        {
            it->reset();
        }
    }
}
