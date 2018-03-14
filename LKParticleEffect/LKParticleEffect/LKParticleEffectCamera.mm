//
//  LKParticleEffectCamera.cpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/2/26.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectCamera.h"
#include "LKParticleEffectUtil.h"
#include "LKParticleEffectSystem.h"
#include <GLKit/GLKit.h>

using namespace LKKit;

Matrix4f Matrix4MakeLookAt(float eyeX, float eyeY, float eyeZ,
                           float centerX, float centerY, float centerZ,
                           float upX, float upY, float upZ)
{
    Vector3f ev = { eyeX, eyeY, eyeZ };
    Vector3f cv = { centerX, centerY, centerZ };
    Vector3f uv = { upX, upY, upZ };
    Vector3f n = (ev-cv).normalized();
    Vector3f u = (uv.cross(n)).normalized();
    Vector3f v = n.cross(u);
    
    Matrix4f m;
    m<< u[0], v[0], n[0], 0.0f,
    u[1], v[1], n[1], 0.0f,
    u[2], v[2], n[2], 0.0f,
    (-u).dot(ev),(-v).dot(ev),(-n).dot(ev),1.0f;
    return m;
}


LKParticleEffectCamera::LKParticleEffectCamera(const Value &value,LKParticleEffectConfig &config,vector<RVar*> vars)
{
    viewMatrix = Matrix4f::Identity();
    projectionMatrix = Matrix4f::Identity();
}

Matrix4f LKParticleEffectCamera::getVPMatrix()
{
    return viewMatrix*projectionMatrix;
}

void LKParticleEffectCamera::update()
{
    
}

void LKParticleEffect3DCamera::setMotionMatrix(vector<float> motionMatrix)
{
    // 0, 0, -1
#define M(x,y) motionMatrix[x+y*4];
    float backCameraX = -M(2,0);
    float backCameraY = -M(2,1);
    float backCameraZ = -M(2,2);
    // 0, 0, 1
    float frontCameraX = M(2,0);
    float frontCameraY = M(2,1);
    float frontCameraZ = M(2,2);

    // 0, 1, 0
    float cameraUpX = M(1,0);
    float cameraUpY = M(1,1);
    float cameraUpZ = M(1,2);
#undef M
    
    // Position the eye behind the origin.
    float eyeX = 0.f;
    float eyeY = 0.f;
    float eyeZ = 0.f;
    
    // We are looking toward the distance
    float lookX = eyeX + backCameraX;
    float lookY = eyeY + backCameraZ;
    float lookZ = eyeZ - backCameraY;
//    if (self.particleAgent.frontCameraMirror)
//    {
//        lookX = eyeX + frontCameraX;
//        lookY = eyeY + frontCameraZ;
//        lookZ = eyeZ - frontCameraY;
//    }
    
    // Set our up vector. This is where our head would be pointing were we holding the camera.
    float upX = cameraUpX;
    float upY = cameraUpZ;
    float upZ = -cameraUpY;
    
    
    // Set the view matrix. This matrix can be said to represent the camera position.
    // NOTE: In OpenGL 1, a ModelView matrix is used, which is a combination of a model and
    // view matrix. In OpenGL 2, we can keep track of these matrices separately if we choose.
    Matrix<float, 4, 4,RowMajor> m;
    auto l = LKParticleEffectUtil::gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);
    m<<l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14],l[15];
    viewMatrix = m;
}

LKParticleEffect2DCamera::LKParticleEffect2DCamera(const Value &value,LKParticleEffectConfig &config,vector<RVar*> vars):LKParticleEffectCamera(value,config,vars)
{
    
}

LKParticleEffect3DCamera::LKParticleEffect3DCamera(const Value &value,LKParticleEffectConfig &config,vector<RVar*> vars):LKParticleEffectCamera(value,config,vars)
{
    
}



LKParticleEffect3DPerspectiveCamera::LKParticleEffect3DPerspectiveCamera(const Value &value,LKParticleEffectConfig &config,vector<RVar*> vars):LKParticleEffect3DCamera(value,config,vars)
{
    positionX = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["positionX"],vars));
    positionY = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["positionY"],vars));
    positionZ = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["positionZ"],vars));
    lookX = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["lookX"],vars));
    lookY = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["lookY"],vars));
    lookZ = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["lookZ"],vars));
    upX = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["upX"],vars));
    upY = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["upY"],vars));
    upZ = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["upZ"],vars));
    
    
    float aspect = config.viewWidth/(float)config.viewHeight;
    float angle = 3.1415926/4;
    float xScale = cos(angle/2)/sin(angle/2)/aspect;
    float yScale = cos(angle/2)/sin(angle/2);
    float zNear = 5;
    float zFar = 10000;
    projectionMatrix<<
    xScale,0,0,0,
    0,yScale,0,0,
    0,0,-(zFar+zNear)/(zFar-zNear),-1,
    0,0,-2*zNear*zFar/(zFar-zNear),0;
    
    viewMatrix = Matrix4MakeLookAt(0,0,0,0,0,-1,0,1,0);
}

void LKParticleEffect3DPerspectiveCamera::update()
{
    viewMatrix = Matrix4MakeLookAt(positionX->value(),positionY->value(),positionZ->value(),
                                   lookX->value(),lookY->value(),lookZ->value(),
                                   upX->value(),upY->value(),upZ->value());
}


LKParticleEffect3DOrthogonalCamera::LKParticleEffect3DOrthogonalCamera(const Value &value,LKParticleEffectConfig &config,vector<RVar*> vars):LKParticleEffect3DCamera(value,config,vars)
{
    
}


