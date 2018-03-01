//
// Created by willisdai on 2018/2/28.
//

#include "AndroidBridge.h"

pair<GLsizei, GLsizei> LKKit::AndroidBridge::glTexImage2DFromData(uint8_t *data, int length) {
    GLsizei width, height;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    return pair<GLsizei, GLsizei>(width, height);
}

