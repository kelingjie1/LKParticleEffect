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

#include "glcontext.h"

#include <string>

namespace LKKit
{
    using namespace std;
    
    class LKParticleEffectTexture
    {
    public:
        string name;
        GLuint index;
        GLuint texture;
        GLsizei width;
        GLsizei height;
        GLsizei frameWidth;
        GLsizei frameHeight;
        int internalFormat;
        bool premultalpha;
        
        void loadFromPath(string path,string name);
        void loadKTXData(uint8_t *data,int length);
        void loadBitmapData(string path, uint8_t *data,int length);
        pair<int, int> getPosition(int frameIndex);
    };
}


#endif /* LKParticleEffectTexture_h */
