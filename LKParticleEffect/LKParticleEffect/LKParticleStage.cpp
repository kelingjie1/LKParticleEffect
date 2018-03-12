//
// Created by willisdai on 2018/3/5.
//

#include "LKParticleStage.h"
#include "LKParticleEffectSystem.h"
#include "LKParticleEffectLogger.h"

using namespace LKKit;

static inline void parseStageTransformSection(vector<stStageTransformEntity *> &entities, const Value &section) {
    for (SizeType i = 0; i < section.Size(); ++i) {
        const Value &entity = section[i];

        stStageTransformEntity *stEntity = new stStageTransformEntity;
        if (entity.HasMember("group")) {
            stEntity->group = entity["group"].GetString();
        }

        if (entity.HasMember("object")) {
            stEntity->object = entity["object"].GetString();
        }

        LKLogInfo("entry(%s, %s)", stEntity->group.c_str(), stEntity->object.c_str());

        entities.push_back(stEntity);
    }
}

LKParticleStage::LKParticleStage(LKParticleEffectSystem *system, const Value &stage) {
    delayEvent.type = "delay";
    delayEvent.time = -1.0f;

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

        for (auto &d : defines.GetObject()) {
            auto iter = system->objectTemplateMap.find(d.name.GetString());
            if (iter == system->objectTemplateMap.end()) {
                continue;
            }

            LKParticleEffectObjectTemplate *objTemplate = new LKParticleEffectObjectTemplate(*(iter->second));
            objTemplate->merge(d.value);
            defineMap[d.name.GetString()] = objTemplate;

            objTemplate->dump();
        }
    }

    if (stage.HasMember("enter_stage")) {
        const Value &objs = stage["enter_stage"];
        if (objs.HasMember("add")) {
            LKLogInfo("%s@%d: parse enter_stage [add] section", __FILE__, __LINE__);
            parseStageTransformSection(enterTransformEntitiesAdded, objs["add"]);
        }

        if (objs.HasMember("remove")) {
            LKLogInfo("%s@%d: parse enter_stage [remove] section", __FILE__, __LINE__);
            parseStageTransformSection(enterTransformEntitiesAdded, objs["remove"]);
        }
    }

    if (stage.HasMember("leave_stage")) {
        const Value &objs = stage["leave_stage"];
        if (objs.HasMember("add")) {
            LKLogInfo("%s@%d: parse leave_stage [add] section", __FILE__, __LINE__);
            parseStageTransformSection(leaveTransformEntitiesAdded, objs["add"]);
        }

        if (objs.HasMember("remove")) {
            LKLogInfo("%s@%d: parse leave_stage [remove] section", __FILE__, __LINE__);
            parseStageTransformSection(leaveTransformEntitiesAdded, objs["remove"]);
        }
    }
}

LKParticleStage::LKParticleStage() {
    delayEvent.type = "delay";
    delayEvent.time = -1;
}

static inline void initAction(const Value &ev, stStageEvent *stev) {
    stev->actionType = StageActionTypeNone;

    if (!ev.HasMember("action")) {
        return;
    }

    const Value &ac = ev["action"];
    if (!ac.HasMember("type")) {
        return;
    }

    const char* type = ac["type"].GetString();
    if (strncmp(type, "change_stage", strlen("change_stage")) == 0) {
        stev->actionType = StageActionTypeChangeStage;

    } else if (strncmp(type, "reset", strlen("reset")) == 0) {
        stev->actionType = StageActionTypeReset;
    }

    if (ac.HasMember("target")) {
        stev->actionTarget = ac["target"].GetString();
    }
}

static inline void initDelayEvent(const Value &ev, stStageEvent *stev) {
    if (!ev.HasMember("time") || !ev["time"].IsNumber()) {
        return;
    }

    stev->time = ev["time"].GetFloat();
    initAction(ev, stev);
}

static inline void initPoseEvent(const Value &ev, stStageEvent *stev) {

    stev->pose = ev["pose"].GetString();
    initAction(ev, stev);
}

void LKParticleStage::createEvent(const Value &ev) {
    if (!ev.HasMember("type")) {
        return;
    }

    const char* type = ev["type"].GetString();
    if (strncmp(type, "delay", strlen("delay")) == 0) {
        initDelayEvent(ev, &delayEvent);

    } else if (ev.HasMember("pose")) {
        if (strncmp(type, "detect", strlen("detect")) == 0 || strncmp(type, "undetect", strlen("undetect")) == 0) {
            stStageEvent *stev = new stStageEvent;
            stev->type = type;

            initPoseEvent(ev, stev);

            auto &evmap = strncmp(type, "detect", strlen("detect")) == 0 ? detectEventPoseMap : undetectEventPostMap;
            evmap[stev->pose] = stev;
        }
    }
}
