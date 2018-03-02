//
// Created by willisdai on 2018/2/28.
//

#include "AndroidBridge.h"
#include "Bitmap.h"
#include "jnilog.h"

static const char* TAG = "AndroidBridge";

JNIEnv* LKKit::AndroidBridge::env = NULL;

pair<GLsizei, GLsizei> LKKit::AndroidBridge::glTexImage2DFromData(string path, uint8_t *data, int length) {
    pair<GLsizei, GLsizei> size;

    if (env == NULL) {
        return size;
    }

    jclass clz_utils = env->FindClass("com/tencent/willisdai/particlesystem/AndroidUtils");
    jmethodID mid = env->GetStaticMethodID(clz_utils, "decodeBitmapFromFile", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");

    jstring jpath = env->NewStringUTF(path.c_str());
    jobject bitmap = env->CallStaticObjectMethod(clz_utils, mid, jpath);
    env->DeleteLocalRef(jpath);

    Bitmap bmp(env, bitmap);

    void *pixels = bmp.getPixels();
    if (pixels == NULL) {
        LOGE(TAG, "load bitmap %s failed", path.c_str());
        return size;
    }

    size.first = bmp.getWidth();
    size.second = bmp.getHeight();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp.getWidth(), bmp.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    LOGD(TAG, "load texture %s success", path.c_str());

    bmp.release();

    return size;
}

