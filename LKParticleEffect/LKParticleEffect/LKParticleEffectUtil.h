//
//  LKParticleEffectUtil.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "glcontext.h"
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <memory>

#define LKDeletePointer(p) if(p){delete p;}p=nullptr;
#define LKDeleteArray(p) if(p){delete[] p;}p=nullptr;

namespace LKKit
{
    using namespace std;
    class LKParticleEffectUtil
    {
    public:
        //视角，宽高比，近裁面，远裁面
        static vector<float> gluPerspective(GLfloat fovy,GLfloat aspect,GLfloat zNear,GLfloat zFar);
        static vector<float> gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
                                       GLfloat centerx, GLfloat centery, GLfloat centerz,
                                       GLfloat upx, GLfloat upy, GLfloat upz);
        static vector<float> mat4DotMat4(vector<float> a,vector<float> b);
        static vector<float> mat4Identify();
    };
}

