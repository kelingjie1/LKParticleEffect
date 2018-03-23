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
        vector<RVar*> vars;
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
        GLuint screenSizeLocation;
        
        LKParticleEffectInputProperty inputProperty;
        LKParticleEffectGlobalProperty globalProperty;
        LKParticleEffectObjectProperty objectProperty;
        vector<RVar*> vars;
        
        map<string,set<LKParticleEffectObject*>> usedObjects;
        map<string,set<LKParticleEffectObject*>> unusedObject;
        
        vector<vector<GLuint>> lines;

        map<string,shared_ptr<LKParticleEffectObjectTemplate>> objectTemplateMap;

        LKParticleEffectSystem(LKParticleEffectConfig config);
        void load(string path);
        void update(double timeDelta);
        void render();
        void triggerEvent(string name,map<string,string> params = map<string,string>());
        
        LKParticleEffectObject *getUnusedObject(string templateName,LKParticleEffectObject *parent=nullptr);
        void removeObject(LKParticleEffectObject *object);
        ~LKParticleEffectSystem();
        void changeToStage(shared_ptr<LKParticleEffectStage> stage);
        void setNextStage(string stageName);
    protected:
        LKParticleEffectConfig config;
        GLuint vbo;
        GLuint vao;
        GLuint ebo;
        int eboIndex;
        GLuint lineVertexCount;
        GLuint pointVertexCount;
        GLuint pointProgram;
        Document document;
        map<string,shared_ptr<LKParticleEffectTexture>> textureMap;
        shared_ptr<LKParticleEffectStage> currentStage;
        shared_ptr<LKParticleEffectStage> nextStage;
        map<string, shared_ptr<LKParticleEffectStage>> stageMap;
        
        vector<LKParticleEffectObject> pointObject;
        LKParticleEffectObjectData *pointObjectDatas;
        
        GLushort *effectIndexes;
        void setupObjects();
        void updateElementBuffer();
        void setupVars();
        void updateGloble(double timeDelta);
        void updateObjects(double timeDelta);
        void updateLines(double timeDelta);
        void updatePoints(double timeDelta);
        void renderLines();
        void renderPoints();
        
        void mapData();
        void unmapData();
    };
}

#endif

