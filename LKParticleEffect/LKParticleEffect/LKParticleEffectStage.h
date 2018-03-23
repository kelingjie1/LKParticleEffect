//
// Created by willisdai on 2018/3/5.
//

#ifndef PARTICLESYSTEM_LKPARTICLESTAGE_H
#define PARTICLESYSTEM_LKPARTICLESTAGE_H

#include "rapidjson/document.h"
#include "LKParticleEffectObject.h"
#include "LKParticleEffectObjectTemplate.h"
#include <string>
#include <map>
#include "LKParticleEffectStageOperation.h"
#include "LKParticleEffectStageEvent.h"

namespace LKKit {
    using namespace rapidjson;
    using namespace std;

    class LKParticleEffectSystem;

    class LKParticleEffectStage
    {
    public:
        LKParticleEffectSystem *system;
        LKParticleEffectStage(LKParticleEffectSystem *system, const Value &stage);
        LKParticleEffectStage();

        string name;
        map<string, shared_ptr<LKParticleEffectStageEvent>> eventMap;
        
        vector<shared_ptr<LKParticleEffectStageOperation>> enterStageOperations;
        vector<shared_ptr<LKParticleEffectStageOperation>> leaveStageOperations;

        map<string, shared_ptr<LKParticleEffectObjectTemplate>> objectTemplateMap;
        void triggerEvent(string name,map<string,string> params = map<string,string>());
        void checkEvent();
        void enterStage();
        void leaveStage();
    protected:
        void parseStageOperations(vector<shared_ptr<LKParticleEffectStageOperation>> &ops,const Value &objs);
    };
}


#endif //PARTICLESYSTEM_LKPARTICLESTAGE_H
