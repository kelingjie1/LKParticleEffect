//
//  LKParticleValue.m
//  ParticleSystem
//
//  Created by lingtonke on 2017/3/3.
//  Copyright © 2017年 lingtonke. All rights reserved.
//

#include "LKParticleEffectValue.h"


using namespace LKKit;
LKParticleEffectValue::LKParticleEffectValue():op(nullptr),num(0)
{
    
}
LKParticleEffectValue::LKParticleEffectValue(const Value& value):op(nullptr),num(0)
{
    setValue(value);
}
LKParticleEffectValue::LKParticleEffectValue(const Value& value,vector<RVar*> varList):op(nullptr),num(0)
{
    setVars(varList);
    setValue(value);
}

LKParticleEffectValue::LKParticleEffectValue(string expression):op(nullptr),num(0)
{
    setExpression(expression);
}
LKParticleEffectValue::LKParticleEffectValue(double number):op(nullptr),num(0)
{
    setNumber(number);
}

void LKParticleEffectValue::setVars(vector<RVar *> varList)
{
    vars = varList;
    varMap.clear();
    for (int i=0; i<vars.size(); i++)
    {
        varMap[vars[i]->name] = i;
    }
    if (op)
    {
        const char *str = op->Expr();
        
        delete op;
        op = nullptr;
        op = new ROperation((char*)str,(int)vars.size(),vars.data());
        
        delete(str);
    }
}


void LKParticleEffectValue::setValue(const Value &value)
{
    if (value.IsString())
    {
        setExpression(value.GetString());
    }
    else
    {
        setNumber(value.GetDouble());
    }
}



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
