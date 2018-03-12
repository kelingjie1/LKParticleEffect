//
//  LKStageOperation.hpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/3/12.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "rapidjson/document.h"
#include <string>
#include <vector>

#ifndef LKStageOperation_h
#define LKStageOperation_h

namespace LKKit
{
    using namespace std;
    using namespace rapidjson;
    class LKParticleEffectStage;
    class LKParticleEffectStageOperation
    {
    public:
        LKParticleEffectStageOperation(LKParticleEffectStage *stage,const Value &value);
        virtual void process();
        LKParticleEffectStage *stage;
        string name;
    };
    
    class LKParticleEffectStageModifyOperationObject
    {
    public:
        string group;
        string object;
    };
    
    class LKParticleEffectStageModifyOperation:public LKParticleEffectStageOperation
    {
    public:
        LKParticleEffectStageModifyOperation(LKParticleEffectStage *stage,const Value &value);
        vector<LKParticleEffectStageModifyOperationObject> objectList;
    };
    
    class LKParticleEffectStageAddOperation:public LKParticleEffectStageModifyOperation
    {
    public:
        LKParticleEffectStageAddOperation(LKParticleEffectStage *stage,const Value &value);
        virtual void process();
    };
    
    class LKParticleEffectStageRemoveOperation:public LKParticleEffectStageModifyOperation
    {
    public:
        LKParticleEffectStageRemoveOperation(LKParticleEffectStage *stage,const Value &value);
        virtual void process();
    };
}

#endif /* LKStageOperation_h */
