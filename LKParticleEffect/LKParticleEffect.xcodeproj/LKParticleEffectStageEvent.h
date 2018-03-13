//
//  LKParticleEffectStageEvent.hpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include <string>
#include <memory>
#include "rapidjson/document.h"

#ifndef LKParticleEffectStageEvent_h
#define LKParticleEffectStageEvent_h

namespace LKKit
{
    using namespace std;
    using namespace rapidjson;
    class LKParticleEffectStage;
    class LKParticleEffectStageEventAction
    {
    public:
        LKParticleEffectStage *stage;
        LKParticleEffectStageEventAction(LKParticleEffectStage *stage,const Value &value);
        virtual void trigger();
    };
    
    class LKParticleEffectStageEventChangeStageAction:public LKParticleEffectStageEventAction
    {
    public:
        LKParticleEffectStageEventChangeStageAction(LKParticleEffectStage *stage,const Value &value);
        virtual void trigger();
        
        string target;
    };
    
    class LKParticleEffectStageEvent
    {
    public:
        LKParticleEffectStage *stage;
        LKParticleEffectStageEvent(LKParticleEffectStage *stage,const Value &value);
        
        string name;
        bool autoCheck;
        shared_ptr<LKParticleEffectStageEventAction> action;
        virtual void check();
        virtual void trigger();
    };
    
    class LKParticleEffectStageDelayEvent:public LKParticleEffectStageEvent
    {
    public:
        LKParticleEffectStageDelayEvent(LKParticleEffectStage *stage,const Value &value);
        double time;
    };
    

}

#endif /* LKParticleEffectStageEvent_h */
