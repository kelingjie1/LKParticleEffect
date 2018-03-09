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
#include "rapidjson/document.h"
#include "Printable.h"

namespace LKKit
{
    using namespace std;
    using namespace rapidjson;
    class LKParticleEffectValue : public Printable
    {
    public:
        LKParticleEffectValue();
        LKParticleEffectValue(const Value& value);
        LKParticleEffectValue(const Value& value,vector<RVar*> varList);
        LKParticleEffectValue(string expression);
        LKParticleEffectValue(double number);
        
        void setValue(const Value& value);
        void setExpression(string expression);
        void setNumber(double number);
        void setVar(double *var,string name);
        void setVars(vector<RVar*> varList);
        double value();

        string to_string();
        ~LKParticleEffectValue();

        const char *rawStr;
    protected:
        map<string,int> varMap;
        vector<RVar*> vars;
        ROperation *op;
        double num;
        string expr;
    };
}


#endif
