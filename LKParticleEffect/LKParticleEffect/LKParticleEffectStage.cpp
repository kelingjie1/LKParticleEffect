//
// Created by willisdai on 2018/3/5.
//

#include "LKParticleEffectStage.h"
#include "LKParticleEffectSystem.h"
#include "LKParticleEffectLogger.h"

using namespace LKKit;

LKParticleEffectStage::LKParticleEffectStage(LKParticleEffectSystem *system, const Value &stage)
{
    this->system = system;
    objectTemplateMap = system->objectTemplateMap;
    name = stage["name"].GetString();

    if (stage.HasMember("events")) {
        const Value &events = stage["events"];
        for (SizeType i = 0; i < events.Size(); ++i) {
            createEvent(events[i]);
        }
    }

    LKLogInfo("%s@%d: parse define section", __FILE__, __LINE__);
    if (stage.HasMember("define")) {
        const Value &defines = stage["define"];
        if (!defines.IsObject()) {
            return;
        }

        for (auto &d : defines.GetObject())
        {
            auto iter = system->objectTemplateMap.find(d.name.GetString());
            if (iter == system->objectTemplateMap.end()) {
                continue;
            }

            auto objTemplate = shared_ptr<LKParticleEffectObjectTemplate>(new LKParticleEffectObjectTemplate(*(iter->second)));
            objTemplate->merge(d.value);
            objectTemplateMap[d.name.GetString()] = objTemplate;

            objTemplate->dump();
        }
    }

    if (stage.HasMember("enter_stage"))
    {
        const Value &objs = stage["enter_stage"];
        parseStageOperations(enterStageOperations,objs);
    }

    if (stage.HasMember("leave_stage"))
    {
        const Value &objs = stage["leave_stage"];
        parseStageOperations(leaveStageOperations,objs);
    }
}

void LKParticleEffectStage::parseStageOperations(vector<shared_ptr<LKParticleEffectStageOperation>> &ops,const Value &objs)
{
    if (objs.HasMember("add"))
    {
        auto op = shared_ptr<LKParticleEffectStageOperation>(new LKParticleEffectStageAddOperation(this,objs["add"]));
        ops.push_back(op);
    }
    
    if (objs.HasMember("remove"))
    {
        auto op = shared_ptr<LKParticleEffectStageOperation>(new LKParticleEffectStageRemoveOperation(this,objs["remove"]));
        ops.push_back(op);
    }
}

LKParticleEffectStage::LKParticleEffectStage()
{
    
}

void LKParticleEffectStage::createEvent(const Value &ev)
{
    if (!ev.HasMember("name"))
    {
        return;
    }
    string name = ev["name"].GetString();
    shared_ptr<LKParticleEffectStageEvent> event;
    if (name == "delay")
    {
        event = shared_ptr<LKParticleEffectStageEvent>(new LKParticleEffectStageDelayEvent(this,ev));
    }
    else
    {
        event = shared_ptr<LKParticleEffectStageEvent>(new LKParticleEffectStageEvent(this,ev));
    }
    eventMap[name] = event;
}

void LKParticleEffectStage::triggerEvent(string name,map<string,string> params)
{
    auto it = eventMap.find(name);
    if (it!= eventMap.end())
    {
        it->second->trigger();
    }
}

void LKParticleEffectStage::enterStage()
{
    for (int i=0; i<enterStageOperations.size(); i++)
    {
        enterStageOperations[i]->process();
    }
}

void LKParticleEffectStage::leaveStage()
{
    for (int i=0; i<leaveStageOperations.size(); i++)
    {
        leaveStageOperations[i]->process();
    }
}
