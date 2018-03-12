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

namespace LKKit
{
    using namespace std;
    using namespace rapidjson;
    class LKParticleEffectCamera
    {
        
    public:
        LKParticleEffectCamera(const Value &value);
        vector<float> m;
        
    };
   
    class LKParticleEffect2DCamera:public LKParticleEffectCamera
    {
    public:
        LKParticleEffect2DCamera(const Value &value);
    };
    
    
    
    class LKParticleEffect3DCamera:public LKParticleEffectCamera
    {
    public:
        LKParticleEffect3DCamera(const Value &value);
        void setMotionMatrix(vector<float> motionMatrix);
    };
    class LKParticleEffect3DPerspectiveCamera:public LKParticleEffect3DCamera
    {
    public:
        LKParticleEffect3DPerspectiveCamera(const Value &value);
        
    };
    class LKParticleEffect3DOrthogonalCamera:public LKParticleEffect3DCamera
    {
    public:
        LKParticleEffect3DOrthogonalCamera(const Value &value);
        
    };
    
}

#endif /* LKParticleEffectCamera_h */
