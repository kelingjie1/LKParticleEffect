//
//  LKParticleEffectSystem.m
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectSystem.h"
#include <glcontext.h>
#include "LKParticleEffectShader.h"
#include "LKParticleEffectLogger.h"

#include "LKParticleEffectUtil.h"
#include "PlatformBridge.h"
#include <list>
#include <sstream>

#include <glcontext.h>

using namespace LKKit;

LKParticleEffectConfig::LKParticleEffectConfig()
{
    maxObjectCount = 10000;
}

LKParticleEffectSystem::LKParticleEffectSystem(LKParticleEffectConfig config)
{
    GLint success;
    GLchar infoLog[512];
    
    const GLchar *const vsStr = LKParticleEffectShader::instance()->vertexShaderStr.c_str();
    const GLchar *const fsStr = LKParticleEffectShader::instance()->fragmentShaderStr.c_str();
    
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLenum glerror = glGetError();
    glShaderSource(vertexShader, 1, &vsStr, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        LKLogError(string("vertexShader compile error")+infoLog);
        glDeleteShader(vertexShader);
    }
    
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsStr, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
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
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        LKLogError(string("program link error:")+infoLog);
        glDeleteProgram(program);
    }
    
    texturesLocation = glGetUniformLocation(program, "textures");
    frameSizesLocation = glGetUniformLocation(program, "frameSizes");
    vpMatrixLocation = glGetUniformLocation(program, "vpMatrix");
    
    projectMatrix = LKParticleEffectUtil::gluPerspective(3.1415926/2,
                                                         config.viewWidth/(float)config.viewHeight,
                                                         5, 20000);
    
    spriteObjects = new LKParticleEffectSpriteObject*[config.maxObjectCount];
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LKParticleEffectSpriteObjectData)*config.maxObjectCount, NULL, GL_STREAM_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort)*config.maxObjectCount, NULL, GL_STREAM_DRAW);
    
    setupObjects();
    
    for (int i=0; i<=5; i++)
    {
        glEnableVertexAttribArray(i);
    }
    GLuint offset = 0;
    glVertexAttribPointer(0, 1, GL_INT, false, sizeof(LKParticleEffectSpriteObjectData), (const void*)offset);
    offset+=sizeof(GLint);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(LKParticleEffectSpriteObjectData), (const void*)offset);
    offset+=sizeof(GLfloat)*3;
    
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(LKParticleEffectSpriteObjectData), (const void*)offset);
    offset+=sizeof(GLfloat)*2;
    
    glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(LKParticleEffectSpriteObjectData), (const void*)offset);
    offset+=sizeof(GLfloat)*4;
    
    glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(LKParticleEffectSpriteObjectData), (const void*)offset);
    offset+=sizeof(GLfloat)*3;
    
    glVertexAttribPointer(5, 1, GL_FLOAT, false, sizeof(LKParticleEffectSpriteObjectData), (const void*)offset);
    glBindVertexArray(NULL);
}

void LKParticleEffectSystem::load(string path)
{
    LKJSONObject jsonObject = PlatformBridge::JSONObjectFromPath(path);

    LKJSONObject resources = jsonObject.valueMap["resources"];
    LKJSONObject textures = resources.valueMap["textures"];
    for (auto it = textures.valueVector.begin(); it!=textures.valueVector.end(); it++)
    {
        LKParticleEffectTexture texture;
        texture.name = it->valueMap["name"].valueString;
        texture.loadFromPath(path, texture.name);
        texture.frameWidth = it->valueMap["frameWidth"].valueNumber;
        texture.frameHeight = it->valueMap["frameHeight"].valueNumber;
        textureMap[texture.name] = texture;
    }
    LKJSONObject objects = jsonObject.valueMap["objects"];
}

void LKParticleEffectSystem::setupObjects()
{
    glBindVertexArray(vao);
    spriteObjectDatas = (LKParticleEffectSpriteObjectData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(LKParticleEffectSpriteObjectData)*config.maxObjectCount, GL_MAP_WRITE_BIT);
    
    effectIndexes = (GLshort*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLshort)*config.maxObjectCount, GL_MAP_WRITE_BIT);
    
    for (GLuint i=0; i<config.maxObjectCount; i++)
    {
        LKParticleEffectSpriteObject *object = new LKParticleEffectSpriteObject;
        spriteObjects[i] = object;
        object->data = &spriteObjectDatas[i];
        spriteObjectDatas[i].identifier = i;
        spriteObjectDatas[i].colorR = 1;
        spriteObjectDatas[i].colorG = 1;
        spriteObjectDatas[i].colorB = 1;
        spriteObjectDatas[i].colorA = 1;
        
    }
    
    spriteObjectDatas[0].positionX = 0.3;
    spriteObjectDatas[0].positionY = 0.3;
    spriteObjectDatas[0].positionZ = 0;
    spriteObjectDatas[0].width = 200;
    spriteObjectDatas[0].height = 200;
    
    spriteObjectDatas[1].positionX = -0.3;
    spriteObjectDatas[1].positionY = 0;
    spriteObjectDatas[1].positionZ = 0;
    
    spriteObjectDatas[2].positionX = 0;
    spriteObjectDatas[2].positionY = -0.3;
    spriteObjectDatas[2].positionZ = 0;
    
    effectIndexes[0] = 0;
    effectIndexes[1] = 1;
    effectIndexes[2] = 2;
    effectIndexes[3] = 3;
    
    for (GLuint i=0; i<config.maxObjectCount; i++)
    {
        spriteObjects[i]->data = NULL;
    }
    
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void LKParticleEffectSystem::update(double timeDelta)
{
    glBindVertexArray(vao);
    spriteObjectDatas = (LKParticleEffectSpriteObjectData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(LKParticleEffectSpriteObjectData)*config.maxObjectCount, GL_MAP_WRITE_BIT);
    
    effectIndexes = (GLshort*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLshort)*config.maxObjectCount, GL_MAP_WRITE_BIT);
    
    for (GLuint i=0; i<config.maxObjectCount; i++)
    {
        spriteObjects[i]->data = &spriteObjectDatas[i];
    }
    
    
    for (GLuint i=0; i<config.maxObjectCount; i++)
    {
        spriteObjects[i]->data = NULL;
    }
    
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindVertexArray(NULL);
}

void LKParticleEffectSystem::updateElementBuffer()
{
    
}

LKParticleEffectSpriteObject *getUnusedObject()
{
    return NULL;
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
        glBindTexture(GL_TEXTURE_2D, it->second.texture);
        frameSizes[i*2] = it->second.frameWidth/(GLfloat)it->second.width;
        frameSizes[i*2+1] = it->second.frameHeight/(GLfloat)it->second.height;
        i++;
    }
    glUniform2fv(frameSizesLocation, 8, frameSizes);
    
    vector<float> vpMartix = LKParticleEffectUtil::mat4DotMat4(camera.m, projectMatrix);
    glUniformMatrix4fv(vpMatrixLocation, 1, 0, vpMartix.data());
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawElements(GL_POINTS, 1, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(NULL);
    
}

LKParticleEffectSystem::~LKParticleEffectSystem()
{
    if (spriteObjects)
    {
        delete [] spriteObjects;
        spriteObjects = NULL;
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
