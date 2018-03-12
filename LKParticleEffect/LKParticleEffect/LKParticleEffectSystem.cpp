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
    
    projectMatrix = LKParticleEffectUtil::gluPerspective(3.1415926/2,
                                                         config.viewWidth/(float)config.viewHeight,
                                                         5, 20000);
    
    spriteObjects = new LKParticleEffectObject[config.maxObjectCount];
    memset(spriteObjects, 0, sizeof(LKParticleEffectObject)*config.maxObjectCount);
    
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
    vars.push_back(new RVar("totalTime",&globalProperty.totalTime));
    vars.push_back(new RVar("stageTime",&globalProperty.stageTime));
    vars.push_back(new RVar("cameraX",&globalProperty.cameraX));
    vars.push_back(new RVar("cameraY",&globalProperty.cameraY));
    vars.push_back(new RVar("cameraZ",&globalProperty.cameraZ));
    vars.push_back(new RVar("cameraDirX",&globalProperty.cameraDirX));
    vars.push_back(new RVar("cameraDirY",&globalProperty.cameraDirY));
    vars.push_back(new RVar("cameraDirZ",&globalProperty.cameraDirZ));
    
    vars.push_back(new RVar("t",&objectProperty.t));
    vars.push_back(new RVar("rand0",&objectProperty.rand0));
    vars.push_back(new RVar("rand1",&objectProperty.rand1));
    vars.push_back(new RVar("rand2",&objectProperty.rand2));
    vars.push_back(new RVar("rand3",&objectProperty.rand3));
    vars.push_back(new RVar("rand4",&objectProperty.rand4));
    vars.push_back(new RVar("rand5",&objectProperty.rand5));
    vars.push_back(new RVar("rand6",&objectProperty.rand6));
    vars.push_back(new RVar("rand7",&objectProperty.rand7));
    vars.push_back(new RVar("rand8",&objectProperty.rand8));
    vars.push_back(new RVar("rand9",&objectProperty.rand9));
    vars.push_back(new RVar("last_colorR",&objectProperty.last_colorR));
    vars.push_back(new RVar("last_colorG",&objectProperty.last_colorG));
    vars.push_back(new RVar("last_colorB",&objectProperty.last_colorB));
    vars.push_back(new RVar("last_colorA",&objectProperty.last_colorA));
    vars.push_back(new RVar("last_frameIndex",&objectProperty.last_frameIndex));
    vars.push_back(new RVar("last_width",&objectProperty.last_width));
    vars.push_back(new RVar("last_height",&objectProperty.last_height));
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
        LKParticleEffectTexture *texture = new LKParticleEffectTexture();
        texture->name = tex["name"].GetString();
        texture->loadFromPath(path, texture->name);
        texture->frameWidth = tex["frameWidth"].GetDouble();
        texture->frameHeight = tex["frameHeight"].GetDouble();
        texture->index = i;
        textureMap[texture->name] = texture;
    }
    
    const Value &define = document["define"];
    //--------camera--------
    const Value &camera = define["camera"];
    //--------objects--------
    const Value &objects = define["objects"];

    LKLogInfo("%s@%d load objects", __FILE__, __LINE__);
    for (SizeType i = 0; i<objects.Size(); i++)
    {
        LKParticleEffectObjectTemplate *objectTemplate = new LKParticleEffectObjectTemplate(this,objects[i]);
        objectTemplateMap[objectTemplate->name] = objectTemplate;
        objectTemplate->dump();
    }

    // stage section
    const  Value &stages = document["stages"];
    for (SizeType i = 0; i < stages.Size(); ++i)
    {
        LKParticleStage *stage = new LKParticleStage(this, stages[i]);
        stageMap[stage->name] = stage;
        if (i==0)
        {
            changeToStage(stage);
        }
    }
}

void LKParticleEffectSystem::changeToStage(LKParticleStage *stage)
{
    currentStage = stage;
}

void LKParticleEffectSystem::setupObjects()
{
    glBindVertexArray(vao);
    objectDatas = (LKParticleEffectObjectData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(LKParticleEffectObjectData)*config.maxObjectCount, GL_MAP_WRITE_BIT);
    
    effectIndexes = (GLshort*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLshort)*config.maxObjectCount, GL_MAP_WRITE_BIT);
    
    for (GLuint i=0; i<config.maxObjectCount; i++)
    {
        LKParticleEffectObject *object = &spriteObjects[i];
        LKParticleEffectObjectData *data = &objectDatas[i];
        data->index = i;
        data->colorR = 1;
        data->colorG = 1;
        data->colorB = 1;
        data->colorA = 1;
        unusedObjects.insert(object);
    }
    
    auto obj0 = getUnusedObject();
    auto data0 = &objectDatas[obj0->index];
    
    obj0->life = 10;
    data0->positionX = 0.3;
    data0->positionY = 0.3;
    data0->positionZ = 0;
    data0->width = 200;
    data0->height = 200;
    
    effectIndexes[0] = 0;
    
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

bool compare(LKParticleEffectObject* a,LKParticleEffectObject* b)
{
    return a->distance<b->distance;
}

void LKParticleEffectSystem::update(double timeDelta)
{
    glBindVertexArray(vao);
    objectDatas = (LKParticleEffectObjectData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(LKParticleEffectObjectData)*config.maxObjectCount, GL_MAP_WRITE_BIT|GL_MAP_READ_BIT);
    
    effectIndexes = (GLshort*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLshort)*config.maxObjectCount, GL_MAP_WRITE_BIT|GL_MAP_READ_BIT);
    
    //emit
    auto objects = usedObjects;
    for (auto it=objects.begin(); it!=objects.end(); it++)
    {
        LKParticleEffectObject *object = *it;
        LKParticleEffectObjectTemplate *temp = object->objectTemplate;
        LKParticleEffectObjectData *data = &objectDatas[object->index];
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
        data->positionX = temp->positionX.value();
        data->positionY = temp->positionY.value();
        data->positionZ = temp->positionZ.value();
        data->rotation = temp->rotation.value();
        if (temp->sprite)
        {
            LKParticleEffectSpriteProperty *sprite = temp->sprite;
            data->colorR = sprite->colorR.value();
            data->colorG = sprite->colorG.value();
            data->colorB = sprite->colorB.value();
            data->colorA = sprite->colorA.value();
            data->width = sprite->width.value();
            data->height = sprite->height.value();
            data->textureIndex = sprite->texture->index;
            int frameIndex = sprite->frameIndex.value();
            pair<int, int> pos = sprite->texture->getPosition(frameIndex);
            data->textureU = pos.first;
            data->textureV = pos.second;
            data->colorR = sprite->colorR.value();
        }
        if (temp->emitter)
        {
            LKParticleEffectEmitterProperty *emitter = temp->emitter;
            double emitRate = emitter->emitRate.value();
            double emissionDuration = 1/emitRate;
            int emitNum = (timeDelta+object->emitRestTime)/emissionDuration;
            object->emitRestTime = (timeDelta+object->emitRestTime)-emitNum*emissionDuration;
            for (int i=0; i<emitNum; i++)
            {
                LKParticleEffectObject *newObject = getUnusedObject();
                if (!newObject)
                {
                    break;
                }
                int tempIndex = rand()%emitter->emitObjects.size();
                string newTempName = emitter->emitObjects[tempIndex];
                newObject->objectTemplate = objectTemplateMap[newTempName];
                newObject->life = newObject->objectTemplate->life.value();
                newObject->positionOffsetX = data->positionX;
                newObject->positionOffsetY = data->positionY;
                newObject->positionOffsetZ = data->positionZ;
                newObject->rotationOffset = data->rotation;
                newObject->emitRestTime = 0;
                newObject->group = object->group;
            }
        }
    }
    
    vector<LKParticleEffectObject*> objectsList;
    for (auto it=usedObjects.begin(); it!=usedObjects.end(); it++)
    {
        objectsList.push_back(*it);
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

LKParticleEffectObject *LKParticleEffectSystem::getUnusedObject()
{
    if (unusedObjects.size()>0)
    {
        LKParticleEffectObject *object = *unusedObjects.begin();
        usedObjects.insert(object);
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
    
    vector<float> vpMartix = LKParticleEffectUtil::mat4DotMat4(camera.m, projectMatrix);
    glUniformMatrix4fv(vpMatrixLocation, 1, 0, vpMartix.data());
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawElements(GL_POINTS, (GLsizei)usedObjects.size(), GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
    
}

LKParticleEffectSystem::~LKParticleEffectSystem()
{
    for (auto it=objectTemplateMap.begin(); it!=objectTemplateMap.end(); it++)
    {
        delete it->second;
    }
    objectTemplateMap.clear();
    for (auto it=textureMap.begin(); it!=textureMap.end(); it++)
    {
        delete it->second;
    }
    textureMap.clear();
    if (spriteObjects)
    {
        delete [] spriteObjects;
        spriteObjects = nullptr;
    }
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
