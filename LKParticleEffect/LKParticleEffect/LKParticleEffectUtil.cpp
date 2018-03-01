//
//  LKParticleEffectUtil.m
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectUtil.h"
#include <math.h>
#include "glcontext.h"

using namespace LKKit;

vector<float> LKParticleEffectUtil::gluPerspective(GLfloat fovy,GLfloat aspect,GLfloat zNear,GLfloat zFar)
{
    vector<float> mat4x4;
    mat4x4.resize(16);
    float a = cos(fovy/2)/sin(fovy/2);
    mat4x4[0*4+0] = a/aspect;
    mat4x4[1*4+1] = a;
    mat4x4[2*4+2] = -(zFar+zNear)/(zFar-zNear);
    mat4x4[2*4+3] = -2*zFar*zNear;
    mat4x4[3*4+2] = -1;
    return mat4x4;
}

vector<float> LKParticleEffectUtil::gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
                                     GLfloat centerx, GLfloat centery, GLfloat centerz,
                                     GLfloat upx, GLfloat upy, GLfloat upz)
{
    vector<GLfloat> m;
    m.resize(16);
    
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
    
    /* Make rotation matrix */
    
    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {          /* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
    
    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;
    
    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
    
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
    
    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
    
    mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    
    mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
    
#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = -x[0]*eyex-x[1]*eyey+x[2]*eyez;
    M(3, 1) = -y[0]*eyex-y[1]*eyey+y[2]*eyez;
    M(3, 2) = -z[0]*eyex-z[1]*eyey+z[2]*eyez;
    M(3, 3) = 1.0;
#undef M
    return m;
}

vector<float> LKParticleEffectUtil::mat4DotMat4(vector<float> a,vector<float> b)
{
    vector<float> r;
    r.resize(16);
#define A(x,y) a[x+y*4]
#define B(x,y) b[x+y*4]
#define R(x,y) r[x+y*4]
    for (int y=0; y<4; y++)
    {
        for (int x=0; x<4; x++)
        {
            for (int i=0; i<4; i++)
            {
                R(x,y) += A(i,y)*B(x,i);
            }
        }
    }
#undef A
#undef B
#undef R
    return r;
}
