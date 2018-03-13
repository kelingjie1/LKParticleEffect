//
//  LKParticleEffectSystem.m
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectSystem.h"
#include "glcontext.h"
#include "LKParticleEffectShader.h"
#include "LKParticleEffectLogger.h"

#include "LKParticleEffectUtil.h"
#include "PlatformBridge.h"
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/pointer.h"
#include "LKParticleEffectObjectTemplate.h"

using namespace LKKit;
using namespace rapidjson;

LKParticleEffectConfig::LKParticleEffectConfig()
{
    maxObjectCount = 10000;
}

const char* LKParticleEffectSystem::TAG = "LKParticleSystem";

LKParticleEffectSystem::LKParticleEffectSystem(LKParticleEffectConfig config)
{
    this->config = config;
    setupVars();
    
    GLint success;
    GLchar infoLog[512];
    
    const GLchar *const vsStr = LKParticleEffectShader::instance()->vertexShaderStr.c_str();
    const GLchar *const fsStr = LKParticleEffectShader::instance()->fragmentShaderStr.c_str();
    
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLenum glerror = glGetError();
    glShaderSource(vertexShader, 1, &vsStr, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        LKLogError(string("vertexShader compile error")+infoLog);
        glDeleteShader(vertexShader);
    }
    
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsStr, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        LKLogError(string("fragmentShader compile error:")+infoLog);
        glDeleteShader(fragmentShader);
    }
    
    program = glCreateProgram();
    if (!program)
    {
        LKLogError("glprogram create failed");
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    
    GLint linked;
    
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        LKLogError(string("program link error:")+infoLog);
        glDeleteProgram(program);
    }
    
    texturesLocation = glGetUniformLocation(program, "textures");
    frameSizesLocation = glGetUniformLocation(program, "frameSizes");
    vpMatrixLocation = glGetUniformLocation(program, "vpMatrix");
    
    
    
    spriteObjects.resize(config.maxObjectCount);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LKParticleEffectObjectData)*config.maxObjectCount, nullptr, GL_STREAM_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort)*config.maxObjectCount, nullptr, GL_STREAM_DRAW);
    
    setupObjects();
    
    for (int i=0; i<=5; i++)
    {
        glEnableVertexAttribArray(i);
    }
    GLuint offset = 0;
    glVertexAttribPointer(0, 1, GL_INT, false, sizeof(LKParticleEffectObjectData), (const void*)offset);
    offset+=sizeof(GLint);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(LKParticleEffectObjectData), (const void*)offset);
    offset+=sizeof(GLfloat)*3;
    
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(LKParticleEffectObjectData), (const void*)offset);
    offset+=sizeof(GLfloat)*2;
    
    glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(LKParticleEffectObjectData), (const void*)offset);
    offset+=sizeof(GLfloat)*4;
    
    glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(LKParticleEffectObjectData), (const void*)offset);
    offset+=sizeof(GLfloat)*3;
    
    glVertexAttribPointer(5, 1, GL_FLOAT, false, sizeof(LKParticleEffectObjectData), (const void*)offset);
    glBindVertexArray(0);

    LKLogInfo("initialized");
}

void LKParticleEffectSystem::setupVars()
{
    memset(&globalProperty, 0, sizeof(LKParticleEffectGlobalProperty));
    memset(&objectProperty, 0, sizeof(LKParticleEffectObjectProperty));
    vars.push_back(new RVar("totalTime",&globalProperty.totalTime));
    vars.push_back(new RVar("stageTime",&globalProperty.stageTime));
    vars.push_back(new RVar("cameraX",&globalProperty.cameraX));
    vars.push_back(new RVar("cameraY",&globalProperty.cameraY));
    vars.push_back(new RVar("cameraZ",&globalProperty.cameraZ));
    vars.push_back(new RVar("cameraDirX",&globalProperty.cameraDirX));
    vars.push_back(new RVar("cameraDirY",&globalProperty.cameraDirY));
    vars.push_back(new RVar("cameraDirZ",&globalProperty.cameraDirZ));
    
    vars.push_back(new RVar("t",&objectProperty.t));
    for (int i=0; i<10; i++)
    {
        stringstream ss;
        ss<<"rand"<<i;
        vars.push_back(new RVar(ss.str().c_str(),&objectProperty.random[i]));
    }
    vars.push_back(new RVar("last_colorR",&objectProperty.last_colorR));
    vars.push_back(new RVar("last_colorG",&objectProperty.last_colorG));
    vars.push_back(new RVar("last_colorB",&objectProperty.last_colorB));
    vars.push_back(new RVar("last_colorA",&objectProperty.last_colorA));
    vars.push_back(new RVar("last_frameIndex",&objectProperty.last_frameIndex));
    vars.push_back(new RVar("last_width",&objectProperty.last_width));
    vars.push_back(new RVar("last_height",&objectProperty.last_height));
}

void LKParticleEffectSystem::mapData()
{
    glBindVertexArray(vao);
    objectDatas = (LKParticleEffectObjectData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(LKParticleEffectObjectData)*config.maxObjectCount, GL_MAP_WRITE_BIT|GL_MAP_READ_BIT);
    effectIndexes = (GLshort*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLshort)*config.maxObjectCount, GL_MAP_WRITE_BIT|GL_MAP_READ_BIT);
    
}
void LKParticleEffectSystem::unmapData()
{
    objectDatas = nullptr;
    effectIndexes = nullptr;
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void LKParticleEffectSystem::load(string path)
{
    fstream f(path+"/params.json");
    if (!f.is_open())
    {
        LKLogError("open json failed");
        return;
    }
    IStreamWrapper isw(f);
    document.ParseStream(isw);
    
    if (!document.IsObject())
    {
        LKLogError("document is not Object");
    }

    const Value &resources = document["resources"];
    //--------textures--------
    const Value &textures = resources["textures"];
    for (SizeType i = 0; i<textures.Size(); i++)
    {
        const Value &tex = textures[i];
        shared_ptr<LKParticleEffectTexture> texture = shared_ptr<LKParticleEffectTexture>(new LKParticleEffectTexture());
        texture->name = tex["name"].GetString();
        texture->loadFromPath(path, texture->name);
        texture->frameWidth = tex["frameWidth"].GetDouble();
        texture->frameHeight = tex["frameHeight"].GetDouble();
        texture->index = i;
        textureMap[texture->name] = texture;
    }
    
    const Value &define = document["define"];
    //--------camera--------
    const Value &cameraValue = define["camera"];
    if (cameraValue["type"]=="perspective")
    {
        camera = shared_ptr<LKParticleEffectCamera>(new LKParticleEffect3DPerspectiveCamera(cameraValue,config));
    }
    else if (cameraValue["type"]=="orthogonal")
    {
        camera = shared_ptr<LKParticleEffectCamera>(new LKParticleEffect3DOrthogonalCamera(cameraValue,config));
    }
    else
    {
        camera = shared_ptr<LKParticleEffectCamera>(new LKParticleEffect2DCamera(cameraValue,config));
    }
    //--------objects--------
    const Value &objects = define["objects"];

    LKLogInfo("%s@%d load objects", __FILE__, __LINE__);
    for (SizeType i = 0; i<objects.Size(); i++)
    {
        auto objectTemplate = shared_ptr<LKParticleEffectObjectTemplate>(new LKParticleEffectObjectTemplate(this,objects[i]));
        objectTemplateMap[objectTemplate->name] = objectTemplate;
        objectTemplate->dump();
    }

    // stage section
    const  Value &stages = document["stages"];
    for (SizeType i = 0; i < stages.Size(); ++i)
    {
        auto stage = shared_ptr<LKParticleEffectStage>(new LKParticleEffectStage(this, stages[i]));
        stageMap[stage->name] = stage;
        if (i==0)
        {
            changeToStage(stage);
        }
    }
    
//    //setupTestData
//    mapData();
//    auto obj0 = getUnusedObject();
//    obj0->objectTemplate = objectTemplateMap["flower0"];
//    obj0->life = obj0->objectTemplate->life->value();
//    obj0->positionOffsetX = 0;
//    obj0->positionOffsetY = 0;
//    obj0->positionOffsetZ = 0;
//    obj0->rotationOffset = 0;
//    obj0->emitRestTime = 0;
//    obj0->group = "";
//    unmapData();
}

void LKParticleEffectSystem::changeToStage(shared_ptr<LKKit::LKParticleEffectStage> stage)
{
    if (currentStage)
    {
        currentStage->leaveStage();
    }
    currentStage = stage;
    if (stage)
    {
        stage->enterStage();
    }
    
}

void LKParticleEffectSystem::setupObjects()
{
    mapData();
    for (GLuint i=0; i<config.maxObjectCount; i++)
    {
        LKParticleEffectObject *object = &spriteObjects[i];
        LKParticleEffectObjectData *data = &objectDatas[i];
        object->index = i;
        data->index = i;
        data->colorR = 1;
        data->colorG = 1;
        data->colorB = 1;
        data->colorA = 1;
        unusedObjects.insert(object);
    }
    unmapData();
}

bool compare(LKParticleEffectObject* a,LKParticleEffectObject* b)
{
    return a->distance<b->distance;
}

void LKParticleEffectSystem::update(double timeDelta)
{
    globalProperty.totalTime+=timeDelta;
    globalProperty.stageTime+=timeDelta;
    mapData();
    //emit
    auto objects = usedObjects;
    for (auto it=objects.begin(); it!=objects.end(); it++)
    {
        auto object = *it;
        auto temp = object->objectTemplate;
        auto data = &objectDatas[object->index];
        object->property.t+= timeDelta;
        memcpy(&objectProperty, &object->property, sizeof(LKParticleEffectObjectProperty));
        object->life -= timeDelta;
        if (object->life<=0)
        {
            removeObject(object);
            continue;
        }
        if (!temp)
        {
            continue;
        }
        data->positionX = temp->positionX->value()+object->positionOffsetX;
        data->positionY = temp->positionY->value()+object->positionOffsetY;
        data->positionZ = temp->positionZ->value()+object->positionOffsetZ;
        data->rotation = temp->rotation->value()+object->rotationOffset;
        if (temp->sprite)
        {
            auto sprite = temp->sprite;
            data->colorR = sprite->colorR->value();
            data->colorG = sprite->colorG->value();
            data->colorB = sprite->colorB->value();
            data->colorA = sprite->colorA->value();
            data->width = sprite->width->value();
            data->height = sprite->height->value();
            data->textureIndex = sprite->texture->index;
            int frameIndex = sprite->frameIndex->value();
            pair<int, int> pos = sprite->texture->getPosition(frameIndex);
            data->textureU = pos.first;
            data->textureV = pos.second;
        }
        if (temp->emitter)
        {
            auto emitter = temp->emitter;
            double emitRate = emitter->emitRate->value();
            int emitNum = 0;
            if (emitRate>0)
            {
                double emissionDuration = 1/emitRate;
                emitNum = (timeDelta+object->emitRestTime)/emissionDuration;
                object->emitRestTime = (timeDelta+object->emitRestTime)-emitNum*emissionDuration;
            }
            
            for (int i=0; i<emitNum; i++)
            {
                int tempIndex = rand()%emitter->emitObjects.size();
                string newTempName = emitter->emitObjects[tempIndex];
                LKParticleEffectObject *newObject = getUnusedObject(newTempName,object);
                if (!newObject)
                {
                    break;
                }
            }
        }
    }
    
    vector<LKParticleEffectObject*> objectsList;
    for (auto it=usedObjects.begin(); it!=usedObjects.end(); it++)
    {
        auto object = *it;
        auto temp = object->objectTemplate;
        if (temp->sprite)
        {
            objectsList.push_back(*it);
        }
    }
    sort(objectsList.begin(), objectsList.end(), compare);
    for (int i=0; i<objectsList.size(); i++)
    {
        effectIndexes[i] = objectsList[i]->index;
    }
    
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindVertexArray(0);
}

LKParticleEffectObject *LKParticleEffectSystem::getUnusedObject(string templateName,LKParticleEffectObject *parent)
{
    if (unusedObjects.size()>0)
    {
        LKParticleEffectObject *object = *unusedObjects.begin();
        unusedObjects.erase(object);
        usedObjects.insert(object);
        object->objectTemplate = objectTemplateMap[templateName];
        object->life = object->objectTemplate->life->value();
        if (parent)
        {
            LKParticleEffectObjectData *data = &objectDatas[parent->index];
            object->positionOffsetX = data->positionX;
            object->positionOffsetY = data->positionY;
            object->positionOffsetZ = data->positionZ;
            object->rotationOffset = data->rotation;
        }
        else
        {
            object->positionOffsetX = 0;
            object->positionOffsetY = 0;
            object->positionOffsetZ = 0;
            object->rotationOffset = 0;
        }
        
        object->emitRestTime = 0;
        object->group = object->group;
        return object;
    }
    return nullptr;
}
void LKParticleEffectSystem::removeObject(LKParticleEffectObject *object)
{
    usedObjects.erase(object);
    unusedObjects.insert(object);
}

void LKParticleEffectSystem::render()
{
    
    glUseProgram(program);
    glBindVertexArray(vao);
    glDisable(GL_DEPTH_TEST);
    int i=0;
    GLint textures[8] = {0,1,2,3,4,5,6,7};
    glUniform1iv(texturesLocation, 8, textures);
    
    GLfloat frameSizes[16];
    for (auto it = textureMap.begin(); it!=textureMap.end(); it++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, it->second->texture);
        frameSizes[i*2] = it->second->frameWidth/(GLfloat)it->second->width;
        frameSizes[i*2+1] = it->second->frameHeight/(GLfloat)it->second->height;
        i++;
    }
    glUniform2fv(frameSizesLocation, 8, frameSizes);
    
    //vector<float> vpMartix = LKParticleEffectUtil::mat4DotMat4(camera->m, projectMatrix);
    auto m = camera->getVPMatrix();
    Matrix<float, 4, 4, RowMajor> vpMatrix = m;
    auto da = vpMatrix.data();
    glUniformMatrix4fv(vpMatrixLocation, 1, 0, vpMatrix.data());
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawElements(GL_POINTS, (GLsizei)usedObjects.size(), GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
    
}

LKParticleEffectSystem::~LKParticleEffectSystem()
{
    for (int i = 0; i < vars.size(); ++i) {
        delete(vars[i]);
    }
    vars.clear();
    objectTemplateMap.clear();
    textureMap.clear();
    spriteObjects.clear();
    if (program)
    {
        glDeleteProgram(program);
        program = 0;
    }
    if (vertexShader)
    {
        glDeleteShader(vertexShader);
        vertexShader = 0;
    }
    if (fragmentShader)
    {
        glDeleteShader(fragmentShader);
        fragmentShader = 0;
    }
    if (vao)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    if (vbo)
    {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    if (ebo)
    {
        glDeleteBuffers(1, &ebo);
        ebo = 0;
    }
}
