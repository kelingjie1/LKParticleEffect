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


LKParticleEffectCamera::LKParticleEffectCamera(const Value &value,LKParticleEffectConfig &config,LKParticleEffectSystem *system):system(system)
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

LKParticleEffect2DCamera::LKParticleEffect2DCamera(const Value &value,LKParticleEffectConfig &config,LKParticleEffectSystem *system):LKParticleEffectCamera(value,config,system)
{
    
}

LKParticleEffect3DCamera::LKParticleEffect3DCamera(const Value &value,LKParticleEffectConfig &config,LKParticleEffectSystem *system):LKParticleEffectCamera(value,config,system)
{
    motionMatrix = Matrix4f::Identity();
}

Matrix4f LKParticleEffect3DCamera::getVPMatrix()
{
    return motionMatrix*viewMatrix*projectionMatrix;
}

Vector3f LKParticleEffect3DCamera::rayCast(GLfloat touch2DX,GLfloat touch2DY)
{
    Vector4f ray_nds;
    float x = touch2DX*2-1;
    float y = touch2DY*2-1;
    x/=zNear*2;
    y/=zNear*2;
    float z = -1;
    ray_nds<<x,y,z,1.0;//归一化设备坐标系
    
    Vector4f ray_eye = projectionMatrix.inverse()*ray_nds;
    Vector4f ray_world = viewMatrix.inverse()*ray_eye;
    if (ray_world.w()!=0)
    {
        ray_world.x()/=ray_world.w();
        ray_world.y()/=ray_world.w();
        ray_world.z()/=ray_world.w();
    }
    Vector3f ray_dir;
    ray_dir<<ray_world.x(),ray_world.y(),ray_world.z();
    auto &property = system->globalProperty;
    ray_dir.x()-=property.cameraX;
    ray_dir.y()-=property.cameraY;
    ray_dir.z()-=property.cameraZ;
    ray_dir.normalize();
    return ray_dir;
}


LKParticleEffect3DPerspectiveCamera::LKParticleEffect3DPerspectiveCamera(const Value &value,LKParticleEffectConfig &config,LKParticleEffectSystem *system):LKParticleEffect3DCamera(value,config,system)
{
    auto &vars = system->vars;
    positionX = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["positionX"],vars));
    positionY = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["positionY"],vars));
    positionZ = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["positionZ"],vars));
    lookX = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["lookX"],vars));
    lookY = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["lookY"],vars));
    lookZ = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["lookZ"],vars));
    upX = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["upX"],vars));
    upY = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["upY"],vars));
    upZ = shared_ptr<LKParticleEffectValue>(new LKParticleEffectValue(value["upZ"],vars));
    zNear = value["near"].GetDouble();
    zFar = value["far"].GetDouble();
    
    float aspect = config.viewWidth/(float)config.viewHeight;
    float angle = 3.1415926/4;
    float xScale = cos(angle/2)/sin(angle/2)/aspect;
    float yScale = cos(angle/2)/sin(angle/2);
    
    projectionMatrix<<
    xScale,0,0,0,
    0,yScale,0,0,
    0,0,-(zFar+zNear)/(zFar-zNear),-1,
    0,0,-2*zNear*zFar/(zFar-zNear),0;
    
    viewMatrix = Matrix4MakeLookAt(0,0,0,0,0,-1,0,1,0);
}

void LKParticleEffect3DPerspectiveCamera::update()
{
    auto &property = system->globalProperty;
    property.cameraX = positionX->value();
    property.cameraY = positionY->value();
    property.cameraZ = positionZ->value();
    Vector3f look;
    look<<lookX->value(),lookY->value(),lookZ->value();
    look.normalize();
    property.cameraDirX = look.x();
    property.cameraDirY = look.y();
    property.cameraDirZ = look.z();
    viewMatrix = Matrix4MakeLookAt(property.cameraX,property.cameraY,property.cameraZ,
                                   property.cameraDirX,property.cameraDirY,property.cameraDirZ,
                                   upX->value(),upY->value(),upZ->value());
}

LKParticleEffect3DOrthogonalCamera::LKParticleEffect3DOrthogonalCamera(const Value &value,LKParticleEffectConfig &config,LKParticleEffectSystem *system):LKParticleEffect3DCamera(value,config,system)
{
    
}


