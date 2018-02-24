//
//  LKParticleEffectIOSUtil.h
//  LKParticleEffect
//
//  Created by lingtonke on 2018/2/24.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//


#include "LKJSONObject.h"

namespace LKKit
{
    using namespace std;
    class LKParticleEffectIOSBridge
    {
    public:
        static LKJSONObject JSONObjectFromPath(string path);
    };
}


