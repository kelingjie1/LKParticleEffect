//
// Created by willisdai on 2018/3/1.
//

#ifndef NEWPARTICLE_PLATFORMBRIDGE_H
#define NEWPARTICLE_PLATFORMBRIDGE_H


#include <cstdint>
#include <glcontext.h>
#include <LKJSONObject.h>

namespace LKKit {
    class PlatformBridge {
    public:
        static LKJSONObject JSONObjectFromPath(string path);
        static pair<GLsizei, GLsizei> glTexImage2DFromData(uint8_t *data, int length);
    };
}


#endif //NEWPARTICLE_PLATFORMBRIDGE_H
