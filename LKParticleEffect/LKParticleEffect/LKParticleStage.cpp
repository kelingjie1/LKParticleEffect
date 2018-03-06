//
// Created by willisdai on 2018/3/5.
//

#include "LKParticleStage.h"
#include "LKParticleEffectSystem.h"

using namespace LKKit;

static inline void mergeObject(LKParticleEffectObject *obj, const Value &value) {
    if (value.HasMember("rotation")) {
        obj->rotation = LKParticleEffectValue(value["rotation"], obj->vars);
    }

    if (value.HasMember("positionX")) {
        obj->positionX = LKParticleEffectValue(value["positionX"], obj->vars);
    }

    if (value.HasMember("positionY")) {
        obj->positionY = LKParticleEffectValue(value["positionY"], obj->vars);
    }

    if (value.HasMember("positionZ")) {
        obj->positionZ = LKParticleEffectValue(value["positionZ"], obj->vars);
    }

    if (value.HasMember("sprite")) {
        const Value &vsprite = value["sprite"];
        if (vsprite.IsObject()) {
            LKParticleEffectSpriteProperty *sprite = obj->sprite;

            if (vsprite.HasMember("colorR")) {
                sprite->colorR = LKParticleEffectValue(vsprite["colorR"], obj->vars);
            }

            if (vsprite.HasMember("colorG")) {
                sprite->colorG = LKParticleEffectValue(vsprite["colorG"], obj->vars);
            }

            if (vsprite.HasMember("colorB")) {
                sprite->colorB = LKParticleEffectValue(vsprite["colorB"], obj->vars);
            }

            if (vsprite.HasMember("colorA")) {
                sprite->colorA = LKParticleEffectValue(vsprite["colorA"], obj->vars);
            }

            if (vsprite.HasMember("texture")) {
                sprite->texture = vsprite["texture"].GetString();
            }

            if (vsprite.HasMember("frameIndex")) {
                sprite->frameIndex = LKParticleEffectValue(vsprite["frameIndex"], obj->vars);
            }

            if (vsprite.HasMember("width")) {
                sprite->width = LKParticleEffectValue(vsprite["width"], obj->vars);
            }

            if (vsprite.HasMember("height")) {
                sprite->height = LKParticleEffectValue(vsprite["height"], obj->vars);
            }
        }
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

    if (stage.HasMember("define")) {
        const Value &defines = stage["define"];
        if (!defines.IsObject()) {
            return;
        }

        for (auto &d : defines.GetObject()) {
            auto iter = system->objectMap.find(d.name.GetString());
            if (iter == system->objectMap.end()) {
                continue;
            }

            LKParticleEffectObject *obj = new LKParticleEffectObject(*(iter->second));
            mergeObject(obj, d.value);
            defineMap[d.name.GetString()] = new LKParticleEffectObject(*(iter->second));
        }
    }

    if (stage.HasMember("objects")) {
        const Value &objs = stage["objects"];
        for (SizeType i = 0; i < objs.Size(); ++i) {
            string name = objs[i].GetString();

            auto iter = system->objectMap.find(name);
            if (iter != system->objectMap.end()) {
                objectMap[name] = new LKParticleEffectObject(*(iter->second));
            }
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
