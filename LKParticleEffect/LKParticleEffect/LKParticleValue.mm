//
//  LKParticleValue.m
//  ParticleSystem
//
//  Created by lingtonke on 2017/3/3.
//  Copyright © 2017年 lingtonke. All rights reserved.
//

#import "LKParticleValue.h"
#import "mathexpr.h"
#import <map>
#import <string>
#import <vector>

using namespace std;

@interface LKParticleValue()
{
    map<string,int> varMap;
    vector<RVar*> vars;
}

@property (nonatomic) ROperation *exp;
@property (nonatomic) double num;


@end

@implementation LKParticleValue

-(instancetype)initWithObject:(NSObject *)object
{
    if (self = [super init])
    {
        self.object = object;
    }
    return self;
    
}

-(void)setObject:(NSObject *)object
{
    _object = object;
    if (self.exp)
    {
        delete self.exp;
        self.exp = nil;
    }
    if ([object isKindOfClass:[NSString class]])
    {
        const char *str = [(NSString*)object cStringUsingEncoding:NSUTF8StringEncoding];
        self.exp = new ROperation((char*)str,(int)vars.size(),vars.data());
    }
    else if ([object isKindOfClass:[NSNumber class]])
    {
        self.num = [(NSNumber*)object doubleValue];
    }
}

-(void)dealloc
{
    if (self.exp)
    {
        delete self.exp;
        self.exp = nil;
    }
    
    for (int i=0; i<vars.size(); i++)
    {
        delete vars[i];
    }
    vars.clear();
}

-(double)value
{
    if (self.exp)
    {
        return self.exp->Val();
    }
    else
    {
        return self.num;
    }
}

-(void)setVar:(double *)var name:(NSString *)name
{
    string str = [name cStringUsingEncoding:NSUTF8StringEncoding];
    auto it = varMap.find(str);
    if (it == varMap.end())
    {
        int index = (int)vars.size();
        varMap[str] = index;
        vars.push_back(new RVar(str.c_str(),var));
        if (self.exp)
        {
            self.object = self.object;
        }
        
    }
    else
    {
        int index = it->second;
        vars[index]->pval = var;
    }
}

-(instancetype)clone
{
    LKParticleValue *value = [[[self class] alloc] init];
    value.object = self.object;
    return value;
}

-(NSString *)description
{
    return [NSString stringWithFormat:@"%@",self.object];
}

@end
