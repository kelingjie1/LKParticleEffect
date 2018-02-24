//
//  LKParticleEffectSpriteObject.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#ifndef LKParticleEffectSpriteObject_h
#define LKParticleEffectSpriteObject_h
struct LKParticleEffectSpriteObjectData
{
    GLuint identifier;
    GLfloat positionX;
    GLfloat positionY;
    GLfloat positionZ;
};

class LKParticleEffectSpriteObject
{
public:
    LKParticleEffectSpriteObjectData *data;
    GLfloat distance;
};

#endif

