//
//  LKParticleEffectSystem.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include <string>
#include <set>
#include <memory>
#include <map>
#include "LKParticleEffectObject.h"
#include "LKParticleEffectTexture.h"
#include "LKParticleEffectCamera.h"
#include "rapidjson/document.h"
#include "LKParticleEffectStage.h"
#include "LKParticleEffectProperty.h"
#include "LKParticleEffectObjectTemplate.h"

#ifndef LKParticleEffectSystem_h
#define LKParticleEffectSystem_h


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
        shared_ptr<LKParticleEffectCamera> camera;
        GLuint texturesLocation;
        GLuint frameSizesLocation;
        GLuint vpMatrixLocation;
        
        LKParticleEffectGlobalProperty globalProperty;
        LKParticleEffectObjectProperty objectProperty;
        vector<RVar*> vars;

        map<string,shared_ptr<LKParticleEffectObjectTemplate>> objectTemplateMap;

        LKParticleEffectSystem(LKParticleEffectConfig config);
        void load(string path);
        void update(double timeDelta);
        void render();
        void triggerEvent(string name,map<string,string> params = map<string,string>());
        
        LKParticleEffectObject *getUnusedObject(string templateName,LKParticleEffectObject *parent=nullptr);
        void removeObject(LKParticleEffectObject *object);
        ~LKParticleEffectSystem();
        set<LKParticleEffectObject*> usedObjects;
        void changeToStage(shared_ptr<LKParticleEffectStage> stage);
        void setNextStage(string stageName);
    protected:
        LKParticleEffectConfig config;
        GLuint vbo;
        GLuint vao;
        GLuint ebo;
        GLuint vertexShader;
        GLuint fragmentShader;
        GLuint program;
        Document document;
        map<string,shared_ptr<LKParticleEffectTexture>> textureMap;
        shared_ptr<LKParticleEffectStage> currentStage;
        shared_ptr<LKParticleEffectStage> nextStage;
        map<string, shared_ptr<LKParticleEffectStage>> stageMap;
        
        set<LKParticleEffectObject*> unusedObjects;
        
        vector<LKParticleEffectObject> spriteObjects;
        LKParticleEffectObjectData *objectDatas;
        GLshort *effectIndexes;
        void setupObjects();
        void updateElementBuffer();
        void setupVars();
        
        void mapData();
        void unmapData();
    };
}

#endif

