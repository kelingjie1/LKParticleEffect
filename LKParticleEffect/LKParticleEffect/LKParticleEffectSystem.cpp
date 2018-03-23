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
    
    GLuint vertexShader;
    GLuint fragmentShader;
    
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
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
    
    pointProgram = glCreateProgram();
    if (!pointProgram)
    {
        LKLogError("glprogram create failed");
    }
    glAttachShader(pointProgram, vertexShader);
    glAttachShader(pointProgram, fragmentShader);
    
    GLint linked;
    
    glLinkProgram(pointProgram);
    glGetProgramiv(pointProgram, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        glGetProgramInfoLog(pointProgram, 512, nullptr, infoLog);
        LKLogError(string("program link error:")+infoLog);
        glDeleteProgram(pointProgram);
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
    
    texturesLocation = glGetUniformLocation(pointProgram, "textures");
    frameSizesLocation = glGetUniformLocation(pointProgram, "frameSizes");
    vpMatrixLocation = glGetUniformLocation(pointProgram, "vpMatrix");
    screenSizeLocation = glGetUniformLocation(pointProgram, "screenSize");
    
    
    pointObject.resize(config.maxObjectCount);
    
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
    offset+=sizeof(GLfloat)*1;
    
    //glVertexAttribPointer(6, 1, GL_FLOAT, false, sizeof(LKParticleEffectObjectData), (const void*)offset);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    LKLogInfo("initialized");
}

void LKParticleEffectSystem::setupVars()
{
    memset(&inputProperty, 0, sizeof(LKParticleEffectInputProperty));
    memset(&globalProperty, 0, sizeof(LKParticleEffectGlobalProperty));
    memset(&objectProperty, 0, sizeof(LKParticleEffectObjectProperty));
    
    vars.push_back(new RVar("touch2DX",&inputProperty.touch2DX));
    vars.push_back(new RVar("touch2DY",&inputProperty.touch2DY));
    
    vars.push_back(new RVar("totalTime",&globalProperty.totalTime));
    vars.push_back(new RVar("stageTime",&globalProperty.stageTime));
    vars.push_back(new RVar("cameraX",&globalProperty.cameraX));
    vars.push_back(new RVar("cameraY",&globalProperty.cameraY));
    vars.push_back(new RVar("cameraZ",&globalProperty.cameraZ));
    vars.push_back(new RVar("cameraDirX",&globalProperty.cameraDirX));
    vars.push_back(new RVar("cameraDirY",&globalProperty.cameraDirY));
    vars.push_back(new RVar("cameraDirZ",&globalProperty.cameraDirZ));
    
    vars.push_back(new RVar("t",&objectProperty.t));
    vars.push_back(new RVar("total_t",&objectProperty.total_t));
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
    vars.push_back(new RVar("last_positionX",&objectProperty.last_positionX));
    vars.push_back(new RVar("last_positionY",&objectProperty.last_positionY));
    vars.push_back(new RVar("last_positionZ",&objectProperty.last_positionZ));
    
    vars.insert(vars.end(), config.vars.begin(),config.vars.end());
}

void LKParticleEffectSystem::mapData()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBindVertexArray(vao);
    pointObjectDatas = (LKParticleEffectObjectData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(LKParticleEffectObjectData)*config.maxObjectCount, GL_MAP_WRITE_BIT|GL_MAP_READ_BIT);
    effectIndexes = (GLushort*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort)*config.maxObjectCount, GL_MAP_WRITE_BIT|GL_MAP_READ_BIT);
    
}
void LKParticleEffectSystem::unmapData()
{
    pointObjectDatas = nullptr;
    effectIndexes = nullptr;
    
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LKParticleEffectSystem::load(string path)
{
    fstream f(path+"/params.json",ios::in);
    if (!f.is_open())
    {
        LKLogError("open json failed:%d",errno);
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
        camera = shared_ptr<LKParticleEffectCamera>(new LKParticleEffect3DPerspectiveCamera(cameraValue,config,this));
    }
    else if (cameraValue["type"]=="orthogonal")
    {
        camera = shared_ptr<LKParticleEffectCamera>(new LKParticleEffect3DOrthogonalCamera(cameraValue,config,this));
    }
    else
    {
        camera = shared_ptr<LKParticleEffectCamera>(new LKParticleEffect2DCamera(cameraValue,config,this));
    }
    //--------objects--------
    const Value &objects = define["objects"];
    
    LKLogInfo("%s@%d load objects", __FILE__, __LINE__);
    for (auto &object: objects.GetObject())
    {
        auto objectTemplate = shared_ptr<LKParticleEffectObjectTemplate>(new LKParticleEffectObjectTemplate(this,object.value));
        objectTemplate->name = object.name.GetString();
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
}

void LKParticleEffectSystem::changeToStage(shared_ptr<LKKit::LKParticleEffectStage> stage)
{
    if (currentStage)
    {
        currentStage->leaveStage();
    }
    currentStage = stage;
    globalProperty.stageTime = 0;
    for (auto objsetIt = usedObjects.begin();objsetIt!=usedObjects.end();objsetIt++)
    {
        auto &objset = objsetIt->second;
        for (auto it=objset.begin(); it!=objset.end(); it++)
        {
            auto object = *it;
            auto data = &pointObjectDatas[object->index];
            object->property.t = 0;
            object->property.last_colorR = data->colorR;
            object->property.last_colorG = data->colorG;
            object->property.last_colorB = data->colorB;
            object->property.last_colorA = data->colorA;
            object->property.last_frameIndex = data->frameIndex;
            object->property.last_width = data->width;
            object->property.last_height = data->height;
            object->property.last_positionX = data->positionX-object->positionOffsetX;
            object->property.last_positionY = data->positionY-object->positionOffsetY;
            object->property.last_positionZ = data->positionZ-object->positionOffsetZ;
            
            object->objectTemplate = stage->objectTemplateMap[object->objectTemplate->name];
        }
    }
    
    if (stage)
    {
        stage->enterStage();
    }
    LKLogInfo("EnterStage:%s",stage->name.c_str());
}

void LKParticleEffectSystem::setNextStage(string stageName)
{
    auto stage = stageMap[stageName];
    nextStage = stage;
}

void LKParticleEffectSystem::setupObjects()
{
    for (GLuint i=0; i<config.maxObjectCount; i++)
    {
        LKParticleEffectObject *point = &pointObject[i];
        point->index = i;
        unusedObject["point"].insert(point);
    }
}

bool compare(LKParticleEffectObject* a,LKParticleEffectObject* b)
{
    return a->distance<b->distance;
}

void LKParticleEffectSystem::updateGloble(double timeDelta)
{
    //stage change
    if (nextStage)
    {
        changeToStage(nextStage);
        nextStage = nullptr;
    }
    
    
    currentStage->checkEvent();
    
    camera->update();
}

void LKParticleEffectSystem::updateObjects(double timeDelta)
{
    for (auto objsetIt = usedObjects.begin();objsetIt!=usedObjects.end();objsetIt++)
    {
        auto objset = objsetIt->second;
        for (auto it=objset.begin(); it!=objset.end(); it++)
        {
            auto object = *it;
            auto temp = object->objectTemplate;
            auto data = &pointObjectDatas[object->index];
            data->index = object->index;
            object->property.t+= timeDelta;
            object->property.total_t+= timeDelta;
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
                data->frameIndex = sprite->frameIndex->value();
                pair<int, int> pos = sprite->texture->getPosition(data->frameIndex);
                data->textureU = pos.first;
                data->textureV = pos.second;
            }
            else if (temp->line)
            {
                auto line = temp->line;
                data->colorR = line->colorR->value();
                data->colorG = line->colorG->value();
                data->colorB = line->colorB->value();
                data->colorA = line->colorA->value();
                data->textureIndex = 100;
            }
            else
            {
                data->colorR = 1;
                data->colorG = 1;
                data->colorB = 1;
                data->colorA = 1;
                data->textureIndex = 100;
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
                int emitRestCount = 0;
                if (temp->emitter->emitCount)
                {
                    emitRestCount = temp->emitter->emitCount->value()-object->emitCount;
                }
                if (emitRestCount<0)
                {
                    emitRestCount = 0;
                }
                if (emitNum>emitRestCount)
                {
                    emitNum = emitRestCount;
                }
                object->emitCount+=emitNum;
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
    }
}

void LKParticleEffectSystem::updateLines(double timeDelta)
{
    for (int i=0; i<lines.size(); i++)
    {
        auto &line = lines[i];
        for (int j=0; j<line.size(); j++)
        {
            effectIndexes[eboIndex] = line[j];
            eboIndex++;
        }
    }
    lineVertexCount = eboIndex;
}

void LKParticleEffectSystem::updatePoints(double timeDelta)
{
    vector<LKParticleEffectObject*> objectsList;
    auto pointUsedObjects = usedObjects["point"];
    for (auto it=pointUsedObjects.begin(); it!=pointUsedObjects.end(); it++)
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
        effectIndexes[i+eboIndex] = objectsList[i]->index;
    }
    pointVertexCount = (GLsizei)objectsList.size();
    eboIndex+=objectsList.size();
}

void LKParticleEffectSystem::update(double timeDelta)
{
    globalProperty.totalTime+=timeDelta;
    globalProperty.stageTime+=timeDelta;
    mapData();
    eboIndex = 0;
    updateGloble(timeDelta);
    updateObjects(timeDelta);
    updateLines(timeDelta);
    updatePoints(timeDelta);
    unmapData();
}

LKParticleEffectObject *LKParticleEffectSystem::getUnusedObject(string templateName,LKParticleEffectObject *parent)
{
    auto temp = currentStage->objectTemplateMap[templateName];
    auto &unusedObjectSet = unusedObject[temp->type];
    auto &usedObjectSet = usedObjects[temp->type];
    if (unusedObjectSet.size()>0)
    {
        LKParticleEffectObject *object = *unusedObjectSet.begin();
        unusedObjectSet.erase(object);
        usedObjectSet.insert(object);
        object->objectTemplate = temp;
        object->property.reset();
        object->life = object->objectTemplate->life->value();

        if (parent)
        {
            LKParticleEffectObjectData *data = &pointObjectDatas[parent->index];
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
    string type = object->objectTemplate->type;
    usedObjects[type].erase(object);
    unusedObject[type].insert(object);
}

void LKParticleEffectSystem::render()
{
    renderLines();
    renderPoints();
}

void LKParticleEffectSystem::renderLines()
{
    auto &pointUsedObjects = usedObjects["point"];
    glUseProgram(pointProgram);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
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
    glUniformMatrix4fv(vpMatrixLocation, 1, 0, vpMatrix.data());
    glUniform2f(screenSizeLocation, config.viewWidth, config.viewHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(vao);
    glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    
    GLuint index = 0;
    for (int i=0; i<lines.size(); i++)
    {
        if (lines[i].size()<=0)
        {
            continue;
        }
        auto &obj = pointObject[lines[i].front()];
        if (obj.objectTemplate->line)
        {
            glLineWidth(obj.objectTemplate->line->width->value());
        }
        else
        {
            glLineWidth(10);
        }
        glDrawElements(GL_LINE_STRIP, (GLsizei)lines[i].size(), GL_UNSIGNED_SHORT, (const GLvoid *)index);
        index+=lines[i].size()*sizeof(GLushort);
    }
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void LKParticleEffectSystem::renderPoints()
{
    auto &pointUsedObjects = usedObjects["point"];
    glUseProgram(pointProgram);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(false);
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
    glUniform2f(screenSizeLocation, config.viewWidth, config.viewHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(vao);
    GLuint index = lineVertexCount*sizeof(GLushort);
    glDrawElements(GL_POINTS, pointVertexCount, GL_UNSIGNED_SHORT, (const GLvoid *)index);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void LKParticleEffectSystem::triggerEvent(string name,map<string,string> params)
{
    currentStage->triggerEvent(name,params);
}

LKParticleEffectSystem::~LKParticleEffectSystem()
{
    for (int i = 0; i < vars.size(); ++i) {
        delete(vars[i]);
    }
    vars.clear();
    objectTemplateMap.clear();
    textureMap.clear();
    pointObject.clear();
    if (pointProgram)
    {
        glDeleteProgram(pointProgram);
        pointProgram = 0;
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
