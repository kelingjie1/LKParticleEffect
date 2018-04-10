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

using namespace LKKit;

Matrix4f Matrix4MakeLookAt(float eyeX, float eyeY, float eyeZ,
                           float targetX, float targetY, float targetZ,
                           float upX, float upY, float upZ)
{
    Vector3f eye = { eyeX, eyeY, eyeZ };
    Vector3f target = { targetX, targetY, targetZ };
    Vector3f up = { upX, upY, upZ };
    Vector3f z = (eye-target).normalized();
    Vector3f x = (z.cross(up)).normalized();
    Vector3f y = x.cross(z);
    
    Matrix4f m;
    m<<
    x[0], y[0], z[0], -x.dot(eye),
    x[1], y[1], z[1], -y.dot(eye),
    x[2], y[2], z[2], -z.dot(eye),
    0,0,0,1.0f;
    return m;
}


LKParticleEffectCamera::LKParticleEffectCamera(const Value &value,LKParticleEffectConfig &config,LKParticleEffectSystem *system):system(system)
{
    viewMatrix = Matrix4f::Identity();
    projectionMatrix = Matrix4f::Identity();
}

Matrix4f LKParticleEffectCamera::getVPMatrix()
{
    return projectionMatrix*viewMatrix;
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

Vector3f LKParticleEffect3DCamera::touchPosition3D(GLfloat touch2DX,GLfloat touch2DY,float distance)
{
    Vector4f ray_nds;
    float x = touch2DX*2-1;
    float y = (1-touch2DY)*2-1;
    float z = 0.95;
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
    return ray_dir;
}

Vector3f LKParticleEffect3DCamera::rayCast(GLfloat touch2DX,GLfloat touch2DY)
{
    Vector4f ray_nds;
    float x = touch2DX*2-1;
    float y = (1-touch2DY)*2-1;
    ray_nds<<x,y,1.0,1.0;
    
    Vector4f ray_eye = projectionMatrix.inverse()*ray_nds;
    ray_eye.z()=-1.0;
    ray_eye.w()=0;
    Vector4f ray_world = viewMatrix.inverse()*ray_eye;
    Vector3f ray_dir;
    ray_dir<<ray_world.x(),ray_world.y(),ray_world.z();
    ray_dir.normalize();
    return ray_dir;
}

void LKParticleEffect3DCamera::yaw(float angle)
{
    Matrix4f m;
    m<<
    cos(angle),0,-sin(angle),0,
    0,1,0,0,
    sin(angle),0,cos(angle),0,
    0,0,0,1;
    viewMatrix = viewMatrix*m;
}

void LKParticleEffect3DCamera::pitch(float angle)
{
    Matrix4f m;
    m<<
    1,0,0,0,
    0,cos(angle),sin(angle),0,
    0,-sin(angle),cos(angle),0,
    0,0,0,1;
    viewMatrix = viewMatrix*m;
}

void LKParticleEffect3DCamera::roll(float angle)
{
    Matrix4f m;
    m<<
    cos(angle),sin(angle),0,0,
    -sin(angle),cos(angle),0,0,
    0,0,1,0,
    0,0,0,1;
    viewMatrix = viewMatrix*m;
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
    
    Matrix4f m;
    m<<
    xScale,0,0,0,
    0,yScale,0,0,
    0,0,-(zFar+zNear)/(zFar-zNear),-2*zFar*zNear/(zFar-zNear),
    0,0,-1,0;
    projectionMatrix = m;
    viewMatrix = Matrix4MakeLookAt(0,0,0,0,0,-1,0,1,0);
}

void LKParticleEffect3DPerspectiveCamera::update()
{
    auto &property = system->globalProperty;
    property.cameraX = positionX->value()+positionOffsetX;
    property.cameraY = positionY->value()+positionOffsetY;
    property.cameraZ = positionZ->value()+positionOffsetZ;
    Vector4f look;

    look<<lookX->value(),lookY->value(),lookZ->value(),1.0;
    look = motionMatrix*look;

    
    viewMatrix = Matrix4MakeLookAt(property.cameraX,property.cameraY,property.cameraZ,
                                   look.x()+positionOffsetX,look.y()+positionOffsetY,look.z()+positionOffsetZ,
                                   upX->value(),upY->value(),upZ->value());
    look.normalize();
    property.cameraDirX = look.x();
    property.cameraDirY = look.y();
    property.cameraDirZ = look.z();
}

LKParticleEffect3DOrthogonalCamera::LKParticleEffect3DOrthogonalCamera(const Value &value,LKParticleEffectConfig &config,LKParticleEffectSystem *system):LKParticleEffect3DCamera(value,config,system)
{
    
}


