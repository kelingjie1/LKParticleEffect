//
//  LKParticleValue.h
//  ParticleSystem
//
//  Created by lingtonke on 2017/3/3.
//  Copyright © 2017年 lingtonke. All rights reserved.
//
#include <string>
#include "mathexpr.h"
#include <map>
#include <string>
#include <vector>

#ifndef LKParticleEffectValue_h
#define LKParticleEffectValue_h

class LKParticleEffectValue
{
public:
    void setExpression(std::string expression);
    void setNumber(double number);
    void setVar(double *var,std::string name);
    double value();
    ~LKParticleEffectValue();
protected:
    std::map<std::string,int> varMap;
    std::vector<RVar*> vars;
    ROperation *op;
    double num;
};

#endif
