//
//  LKParticleValue.m
//  ParticleSystem
//
//  Created by lingtonke on 2017/3/3.
//  Copyright © 2017年 lingtonke. All rights reserved.
//

#include "LKParticleEffectValue.h"


using namespace LKKit;

void LKParticleEffectValue::setExpression(string expression)
{
    if (op)
    {
        delete op;
        op = NULL;
    }
    const char *str = expression.c_str();
    op = new ROperation((char*)str,(int)vars.size(),vars.data());
}
void LKParticleEffectValue::setNumber(double number)
{
    if (op)
    {
        delete op;
        op = NULL;
    }
    num = number;
}

LKParticleEffectValue::~LKParticleEffectValue()
{
    if (op)
    {
        delete op;
        op = NULL;
    }
    
    for (int i=0; i<vars.size(); i++)
    {
        delete vars[i];
    }
    vars.clear();
}

void LKParticleEffectValue::setVar(double *var,string name)
{
    auto it = varMap.find(name);
    if (it == varMap.end())
    {
        int index = (int)vars.size();
        varMap[name] = index;
        vars.push_back(new RVar(name.c_str(),var));
        if (op)
        {
            delete op;
            op = NULL;
            const char *str = op->Expr();
            op = new ROperation((char*)str,(int)vars.size(),vars.data());
        }
    }
    else
    {
        int index = it->second;
        vars[index]->pval = var;
    }
}

double LKParticleEffectValue::value()
{
    if (op)
    {
        return op->Val();
    }
    else
    {
        return num;
    }
}