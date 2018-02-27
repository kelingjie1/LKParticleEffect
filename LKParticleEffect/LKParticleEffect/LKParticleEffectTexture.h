//
//  LKParticleEffectTexture.hpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/2/24.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#ifndef LKParticleEffectTexture_h
#define LKParticleEffectTexture_h

#include <stdio.h>
#include <inttypes.h>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include <string>

namespace LKKit
{
    using namespace std;
    
    class LKParticleEffectTexture
    {
    public:
        string name;
        GLuint texture;
        GLsizei width;
        GLsizei height;
        GLsizei frameWidth;
        GLsizei frameHeight;
        int internalFormat;
        bool premultalpha;
        
        void loadFromPath(string path,string name);
        void loadKTXData(uint8_t *data,int length);
        void loadBitmapData(uint8_t *data,int length);
    };
}


#endif /* LKParticleEffectTexture_h */
