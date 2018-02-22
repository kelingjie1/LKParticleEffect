//
//  LKParticleEffectSystem.m
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectSystem.h"
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include "LKParticleEffectUtil.h"
#include "LKParticleEffectShader.h"
#include "LKParticleEffectLogger.h"
#include <list>

using namespace std;

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
    
    effectObjects = new LKParticleEffectObject*[config.maxObjectCount];
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LKParticleEffectObjectData)*config.maxObjectCount, NULL, GL_STREAM_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort)*config.maxObjectCount, NULL, GL_STREAM_DRAW);
    
    setupObjects();
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    GLuint offset = 0;
    glVertexAttribPointer(0, 1, GL_INT, false, sizeof(LKParticleEffectObjectData), (const void*)offset);
    offset = offset+sizeof(GLint);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(LKParticleEffectObjectData), (const void*)offset);
    glBindVertexArray(NULL);
}

void LKParticleEffectSystem::setupObjects()
{
    glBindVertexArray(vao);
    effectObjectDatas = (LKParticleEffectObjectData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(LKParticleEffectObjectData)*config.maxObjectCount, GL_MAP_WRITE_BIT);
    
    effectIndexes = (GLshort*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLshort)*config.maxObjectCount, GL_MAP_WRITE_BIT);
    
    for (GLuint i=0; i<config.maxObjectCount; i++)
    {
        LKParticleEffectObject *object = new LKParticleEffectObject;
        effectObjects[i] = object;
        object->data = &effectObjectDatas[i];
        effectObjectDatas[i].identifier = i;
    }
    
    effectObjectDatas[0].positionX = 0.3;
    effectObjectDatas[0].positionY = 0.3;
    effectObjectDatas[0].positionZ = 0;
    
    effectObjectDatas[1].positionX = 0.3;
    effectObjectDatas[1].positionY = 0;
    effectObjectDatas[1].positionZ = 0;
    
    effectObjectDatas[2].positionX = 0;
    effectObjectDatas[2].positionY = 0.3;
    effectObjectDatas[2].positionZ = 0;
    
    effectIndexes[0] = 0;
    effectIndexes[1] = 1;
    effectIndexes[2] = 2;
    effectIndexes[3] = 3;
    
    for (GLuint i=0; i<config.maxObjectCount; i++)
    {
        effectObjects[i]->data = NULL;
    }
    
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void LKParticleEffectSystem::update(double timeDelta)
{
    glBindVertexArray(vao);
    effectObjectDatas = (LKParticleEffectObjectData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(LKParticleEffectObjectData)*config.maxObjectCount, GL_MAP_WRITE_BIT);
    
    effectIndexes = (GLshort*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLshort)*config.maxObjectCount, GL_MAP_WRITE_BIT);
    
    for (GLuint i=0; i<config.maxObjectCount; i++)
    {
        effectObjects[i]->data = &effectObjectDatas[i];
    }
    
    
    for (GLuint i=0; i<config.maxObjectCount; i++)
    {
        effectObjects[i]->data = NULL;
    }
    
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindVertexArray(NULL);
}

void LKParticleEffectSystem::updateElementBuffer()
{
    
}

LKParticleEffectObject *getUnusedObject()
{
    return NULL;
}

void LKParticleEffectSystem::render()
{
    glUseProgram(program);
    glBindVertexArray(vao);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_POINTS, 3, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(NULL);
    
}

LKParticleEffectSystem::~LKParticleEffectSystem()
{
    if (effectObjects)
    {
        delete [] effectObjects;
        effectObjects = NULL;
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
