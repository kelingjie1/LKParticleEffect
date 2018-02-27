//
//  LKParticleEffectCamera.cpp
//  LKParticleEffect
//
//  Created by lingtonke on 2018/2/26.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectCamera.h"
#include "LKParticleEffectUtil.h"

using namespace LKKit;
LKParticleEffectCamera::LKParticleEffectCamera()
{
    m.resize(16);
}

void LKParticleEffectCamera::setMotionMatrix(vector<float> motionMatrix)
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
    m = LKParticleEffectUtil::gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);
}
