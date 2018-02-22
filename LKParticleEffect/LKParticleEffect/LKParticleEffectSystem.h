//
//  LKParticleEffectSystem.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectEmitter.h"
#include "LKParticleEffectObject.h"
#ifndef LKParticleEffectSystem_h
#define LKParticleEffectSystem_h

class LKParticleEffectConfig
{
public:
    LKParticleEffectConfig();
    unsigned int maxObjectCount;
};

class LKParticleEffectSystem
{
public:
    LKParticleEffectSystem(LKParticleEffectConfig config);
    void update(double timeDelta);
    void render();
    ~LKParticleEffectSystem();
protected:
    LKParticleEffectConfig config;
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;
    LKParticleEffectObject **effectObjects;
    LKParticleEffectObjectData *effectObjectDatas;
    GLshort *effectIndexes;
    void setupObjects();
    void updateElementBuffer();
};
#endif

