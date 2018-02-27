//
//  LKParticleEffectSystem.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#ifndef LKParticleEffectSystem_h
#define LKParticleEffectSystem_h

#include <string>
#include "LKParticleEffectEmitter.h"
#include "LKParticleEffectSpriteObject.h"
#include "LKParticleEffectTexture.h"
#include "LKJSONObject.h"
#include "LKParticleEffectCamera.h"

namespace LKKit
{
    using namespace std;
    class LKParticleEffectConfig
    {
    public:
        LKParticleEffectConfig();
        unsigned int maxObjectCount;
        unsigned int viewWidth;
        unsigned int viewHeight;
    };
    
    class LKParticleEffectSystem
    {
    public:
        LKParticleEffectCamera camera;
        vector<float> projectMatrix;
        GLuint texturesLocation;
        GLuint frameSizesLocation;
        GLuint vpMatrixLocation;
        
        LKParticleEffectSystem(LKParticleEffectConfig config);
        void load(string path);
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
        map<string,LKParticleEffectTexture> textureMap;
        LKParticleEffectSpriteObject **spriteObjects;
        LKParticleEffectSpriteObjectData *spriteObjectDatas;
        GLshort *effectIndexes;
        void setupObjects();
        void updateElementBuffer();
    };
}

#endif

