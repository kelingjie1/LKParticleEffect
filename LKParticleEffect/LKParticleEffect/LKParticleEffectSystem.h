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
#include "LKParticleEffectEmitter.h"
#include "LKParticleEffectObject.h"
#include "LKParticleEffectTexture.h"
#include "LKParticleEffectCamera.h"
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"

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
    
    struct LKParticleEffectGlobalProperty
    {
        double totalTime;
        double stageTime;
        double cameraX;
        double cameraY;
        double cameraZ;
        double cameraDirX;
        double cameraDirY;
        double cameraDirZ;
    };
    
    class LKParticleEffectSystem
    {
    public:
        static const char* TAG;
        LKParticleEffectCamera camera;
        vector<float> projectMatrix;
        GLuint texturesLocation;
        GLuint frameSizesLocation;
        GLuint vpMatrixLocation;
        
        LKParticleEffectGlobalProperty globalProperty;
        
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
        map<string,LKParticleEffectObject*> objectMap;
        
        set<LKParticleEffectObject*> unusedObjects;
        set<LKParticleEffectObject*> usedObjects;
        LKParticleEffectObject *spriteObjects;
        LKParticleEffectObjectData *spriteObjectDatas;
        GLshort *effectIndexes;
        void setupObjects();
        void updateElementBuffer();
    };
}

#endif

