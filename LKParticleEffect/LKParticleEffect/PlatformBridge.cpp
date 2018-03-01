//
// Created by willisdai on 2018/3/1.
//

#if IOS
#include "LKParticleEffectIOSBridge.h"
#else
#include <android/AndroidBridge.h>
#endif

#include "PlatformBridge.h"

using namespace std;
pair<GLsizei, GLsizei> LKKit::PlatformBridge::glTexImage2DFromData(uint8_t *data, int length) {
#if IOS
    return LKParticleEffectIOSBridge::glTexImage2DFromData(data, length);
#else
    return AndroidBridge::glTexImage2DFromData(data, length);
#endif
}

