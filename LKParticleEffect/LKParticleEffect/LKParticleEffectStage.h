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

namespace LKKit {
    using namespace rapidjson;
    using namespace std;

    class LKParticleEffectSystem;

    enum StageActionType {
        StageActionTypeNone = -1,
        StageActionTypeChangeStage,
        StageActionTypeReset,
    };

    typedef struct
    {
        string type;
        float time;
        string pose;

        StageActionType actionType;
        string actionTarget;
    } stStageEvent;


    class LKParticleEffectStage
    {
    public:
        LKParticleEffectSystem *system;
        LKParticleEffectStage(LKParticleEffectSystem *system, const Value &stage);
        LKParticleEffectStage();

        string name;
        stStageEvent delayEvent;
        map<string, stStageEvent*> detectEventPoseMap;
        map<string, stStageEvent*> undetectEventPostMap;
        
        vector<shared_ptr<LKParticleEffectStageOperation>> enterStageOperations;
        vector<shared_ptr<LKParticleEffectStageOperation>> leaveStageOperations;

        map<string, LKParticleEffectObjectTemplate*> defineMap;
        
        void enterStage();
        void leaveStage();
    protected:
        void createEvent(const Value &value);
        void parseStageOperations(vector<shared_ptr<LKParticleEffectStageOperation>> &ops,const Value &objs);
    };
}


#endif //PARTICLESYSTEM_LKPARTICLESTAGE_H
