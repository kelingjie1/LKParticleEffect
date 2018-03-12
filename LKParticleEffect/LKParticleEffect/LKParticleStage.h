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

namespace LKKit {
    using namespace rapidjson;
    using namespace std;

    class LKParticleEffectSystem;

    enum StageActionType {
        StageActionTypeNone = -1,
        StageActionTypeChangeStage,
        StageActionTypeReset,
    };

    typedef struct {
        string type;
        float time;
        string pose;

        StageActionType actionType;
        string actionTarget;
    } stStageEvent;

    typedef struct {
        string group;
        string object;
    } stStageTransformEntity;

    class LKParticleStage {
    public:
        LKParticleStage(LKParticleEffectSystem *system, const Value &stage);
        LKParticleStage();

        string name;
        stStageEvent delayEvent;
        map<string, stStageEvent*> detectEventPoseMap;
        map<string, stStageEvent*> undetectEventPostMap;
//        map<string, LKParticleEffectObjectTemplate*> objectTemplateMap;

        vector<stStageTransformEntity *> enterTransformEntitiesAdded;
        vector<stStageTransformEntity *> enterTransformEntitiesRemoved;
        vector<stStageTransformEntity *> leaveTransformEntitiesAdded;
        vector<stStageTransformEntity *> leaveTransformEntitiesRemoved;

        map<string, LKParticleEffectObjectTemplate*> defineMap;

    private:
        void createEvent(const Value &value);
    };
}


#endif //PARTICLESYSTEM_LKPARTICLESTAGE_H
