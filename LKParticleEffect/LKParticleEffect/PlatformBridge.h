//
// Created by willisdai on 2018/3/1.
//

#ifndef NEWPARTICLE_PLATFORMBRIDGE_H
#define NEWPARTICLE_PLATFORMBRIDGE_H


#include <cstdint>
#include <utility>
#include <string>
#include "glcontext.h"

using namespace std;
namespace LKKit {
    class PlatformBridge {
    public:
        static pair<GLsizei, GLsizei> glTexImage2DFromData(string path, uint8_t *data, int length);
    };
}


#endif //NEWPARTICLE_PLATFORMBRIDGE_H
