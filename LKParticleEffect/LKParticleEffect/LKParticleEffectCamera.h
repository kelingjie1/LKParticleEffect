//
//  LKParticleEffectCamera.h
//  LKParticleEffect
//
//  Created by lingtonke on 2018/2/26.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#ifndef LKParticleEffectCamera_h
#define LKParticleEffectCamera_h
#include "glcontext.h"
#include <vector>
#include "rapidjson/document.h"
#include "Eigen/Eigen"
#include "LKParticleEffectValue.h"

namespace LKKit
{
    using namespace std;
    using namespace Eigen;
    using namespace rapidjson;
    class LKParticleEffectConfig;
    class LKParticleEffectCamera
    {
        
    public:
        LKParticleEffectCamera(const Value &value,LKParticleEffectConfig &config,vector<RVar*> vars);
        Matrix4f viewMatrix;
        Matrix4f projectionMatrix;
        virtual Matrix4f getVPMatrix();
        virtual void update();
        
    };
   
    class LKParticleEffect2DCamera:public LKParticleEffectCamera
    {
    public:
        LKParticleEffect2DCamera(const Value &value,LKParticleEffectConfig &config,vector<RVar*> vars);
    };
    
    
    
    class LKParticleEffect3DCamera:public LKParticleEffectCamera
    {
    public:
        LKParticleEffect3DCamera(const Value &value,LKParticleEffectConfig &config,vector<RVar*> vars);
        void setMotionMatrix(vector<float> motionMatrix);
        shared_ptr<LKParticleEffectValue> positionX;
        shared_ptr<LKParticleEffectValue> positionY;
        shared_ptr<LKParticleEffectValue> positionZ;
        shared_ptr<LKParticleEffectValue> lookX;
        shared_ptr<LKParticleEffectValue> lookY;
        shared_ptr<LKParticleEffectValue> lookZ;
        shared_ptr<LKParticleEffectValue> upX;
        shared_ptr<LKParticleEffectValue> upY;
        shared_ptr<LKParticleEffectValue> upZ;
    };
    class LKParticleEffect3DPerspectiveCamera:public LKParticleEffect3DCamera
    {
    public:
        LKParticleEffect3DPerspectiveCamera(const Value &value,LKParticleEffectConfig &config,vector<RVar*> vars);
        virtual void update();
    };
    class LKParticleEffect3DOrthogonalCamera:public LKParticleEffect3DCamera
    {
    public:
        LKParticleEffect3DOrthogonalCamera(const Value &value,LKParticleEffectConfig &config,vector<RVar*> vars);
        
    };
    
}

#endif /* LKParticleEffectCamera_h */
