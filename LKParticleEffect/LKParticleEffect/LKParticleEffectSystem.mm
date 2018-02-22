//
//  LKParticleEffectSystem.m
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#import "LKParticleEffectSystem.h"
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "LKParticleEffectUtil.h"
#import "LKParticleEffectPrivate.h"


@implementation LKParticleEffectConfig

+ (instancetype)defaultConfig
{
    LKParticleEffectConfig *config = [[LKParticleEffectConfig alloc] init];
    config.maxObjectCount = 10000;
    return config;
}

@end

@interface LKParticleEffectSystem()
{
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;
    LKParticleEffectObjectData *effectObjectDatas;
    GLshort *effectIndexes;
    NSMutableSet *unusedObject;
    NSMutableArray *effectObjects;
}

@property (nonatomic) LKParticleEffectConfig *config;


@end

@implementation LKParticleEffectSystem

- (instancetype)initWithConfig:(LKParticleEffectConfig *)config
{
    if (self = [self init])
    {
        [self setConfig:config];
    }
    return self;
}

- (void)setup
{
    NSString *vsFile = [[NSBundle LKParticleEffectBundle] pathForResource:@"vertexShader" ofType:@"vs"];
    NSString *fsFile = [[NSBundle LKParticleEffectBundle] pathForResource:@"fragmentShader" ofType:@"fs"];
    NSError *error;
    NSString *vs = [NSString stringWithContentsOfFile:vsFile encoding:NSUTF8StringEncoding error:&error];
    NSString *fs = [NSString stringWithContentsOfFile:fsFile encoding:NSUTF8StringEncoding error:&error];
    
    GLint success;
    GLchar infoLog[512];
    
    const GLchar *const vsStr = [vs cStringUsingEncoding:NSUTF8StringEncoding];
    const GLchar *const fsStr = [fs cStringUsingEncoding:NSUTF8StringEncoding];
    
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLenum glerror = glGetError();
    glShaderSource(vertexShader, 1, &vsStr, nil);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        NSLog(@"vertexShader compile error:%s",infoLog);
        glDeleteShader(vertexShader);
    }
    
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsStr, nil);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        NSLog(@"fragmentShader compile error:%s",infoLog);
        glDeleteShader(fragmentShader);
    }
    
    program = glCreateProgram();
    if (!program)
    {
        NSLog(@"glprogram create failed");
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    
    GLint linked;
    
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        NSLog(@"program link error:%s",infoLog);
        glDeleteProgram(program);
    }
    
    
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LKParticleEffectObjectData)*self.config.maxObjectCount, NULL, GL_STREAM_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort)*self.config.maxObjectCount, NULL, GL_STREAM_DRAW);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    GLuint offset = 0;
    glVertexAttribPointer(0, 1, GL_INT, false, sizeof(LKParticleEffectObjectData), (const void*)offset);
    offset = offset+sizeof(GLint);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(LKParticleEffectObjectData), (const void*)offset);
    glBindVertexArray(NULL);
    
    
    [self updateData];
}

- (void)updateData
{
    glBindVertexArray(vao);
    effectObjectDatas = (LKParticleEffectObjectData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(LKParticleEffectObjectData)*self.config.maxObjectCount, GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
    
    effectIndexes = (GLshort*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLshort)*self.config.maxObjectCount, GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
    
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
    
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindVertexArray(NULL);
}

- (LKParticleEffectObject*)generateObject
{
    LKParticleEffectObject *object = [unusedObject anyObject];
    [unusedObject removeObject:object];
    return object;
}

- (void)render
{
    glUseProgram(program);
    glBindVertexArray(vao);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_POINTS, 3, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(NULL);
    
}



@end
