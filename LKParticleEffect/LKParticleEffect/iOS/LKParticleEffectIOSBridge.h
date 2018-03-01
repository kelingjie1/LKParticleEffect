//
//  LKParticleEffectIOSUtil.h
//  LKParticleEffect
//
//  Created by lingtonke on 2018/2/24.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

namespace LKKit
{
    using namespace std;
    class LKParticleEffectIOSBridge
    {
    public:
        static pair<GLsizei, GLsizei> glTexImage2DFromData(uint8_t *data, int length);
    };
}


