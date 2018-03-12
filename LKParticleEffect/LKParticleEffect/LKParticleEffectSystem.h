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
#include <set>
#include "LKParticleEffectObject.h"
#include "LKParticleEffectTexture.h"
#include "LKParticleEffectCamera.h"
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "LKParticleStage.h"
#include "LKParticleEffectProperty.h"
#include "LKParticleEffectObjectTemplate.h"

namespace LKKit
{
    using namespace std;
    using namespace rapidjson;
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
        friend class LKParticleEffectObjectTemplate;
    public:
        static const char* TAG;
        LKParticleEffectCamera camera;
        vector<float> projectMatrix;
        GLuint texturesLocation;
        GLuint frameSizesLocation;
        GLuint vpMatrixLocation;
        
        LKParticleEffectGlobalProperty globalProperty;
        LKParticleEffectObjectProperty objectProperty;
        vector<RVar*> vars;

        map<string,LKParticleEffectObjectTemplate*> objectTemplateMap;

        LKParticleEffectSystem(LKParticleEffectConfig config);
        void load(string path);
        void update(double timeDelta);
        void render();
        LKParticleEffectObject *getUnusedObject();
        void removeObject(LKParticleEffectObject *object);
        ~LKParticleEffectSystem();
    protected:
        LKParticleEffectConfig config;
        GLuint vbo;
        GLuint vao;
        GLuint ebo;
        GLuint vertexShader;
        GLuint fragmentShader;
        GLuint program;
        Document document;
        map<string,LKParticleEffectTexture*> textureMap;
        LKParticleStage *currentStage;
        map<string, LKParticleStage*> stageMap;
        
        set<LKParticleEffectObject*> unusedObjects;
        set<LKParticleEffectObject*> usedObjects;
        LKParticleEffectObject *spriteObjects;
        LKParticleEffectObjectData *objectDatas;
        GLshort *effectIndexes;
        void setupObjects();
        void updateElementBuffer();
        void setupVars();
        void changeToStage(LKParticleStage *stage);
    };
}

#endif

