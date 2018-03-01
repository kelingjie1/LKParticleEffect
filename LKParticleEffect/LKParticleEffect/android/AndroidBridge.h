//
// Created by willisdai on 2018/2/28.
//

#ifndef NEWPARTICLE_ANDROIDBRIDGE_H
#define NEWPARTICLE_ANDROIDBRIDGE_H


#include <cstdint>
#include <GLES3/gl3.h>
#include <LKJSONObject.h>

namespace LKKit {
    class AndroidBridge {
    public:
        static LKJSONObject JSONObjectFromPath(string path);
        static pair<GLsizei, GLsizei> glTexImage2DFromData(uint8_t *data, int length);
    };
}


#endif //NEWPARTICLE_ANDROIDBRIDGE_H
