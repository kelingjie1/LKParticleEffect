//
// Created by willisdai on 2018/2/28.
//

#ifndef NEWPARTICLE_ANDROIDBRIDGE_H
#define NEWPARTICLE_ANDROIDBRIDGE_H


#include <cstdint>
#include <GLES3/gl3.h>
#include <utility>
#include <jni.h>
#include <string>

using namespace std;
namespace LKKit {
    class AndroidBridge {
    public:
        static JNIEnv *env;
        static pair<GLsizei, GLsizei> glTexImage2DFromData(string path, uint8_t *data, int length);
    };
}


#endif //NEWPARTICLE_ANDROIDBRIDGE_H
