//
//  LKParticleValue.h
//  ParticleSystem
//
//  Created by lingtonke on 2017/3/3.
//  Copyright © 2017年 lingtonke. All rights reserved.
//
#ifndef LKParticleEffectValue_h
#define LKParticleEffectValue_h

#include <string>
#include <map>
#include <string>
#include <vector>
#include "mathexpr.h"

namespace LKKit
{
    using namespace std;
    class LKParticleEffectValue
    {
    public:
        void setExpression(string expression);
        void setNumber(double number);
        void setVar(double *var,string name);
        double value();
        ~LKParticleEffectValue();
    protected:
        map<string,int> varMap;
        vector<RVar*> vars;
        ROperation *op;
        double num;
    };
}


#endif
