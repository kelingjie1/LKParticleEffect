//
// Created by willisdai on 2018/3/2.
//

#include "Bitmap.h"

static const char* TAG = "ParticleSystemBitmap";

Bitmap::Bitmap(JNIEnv *env, jobject bitmap) : pixels(NULL), jenv(env), jbitmap(bitmap) {
    if ((result = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE(TAG, "bitmap init failed ! error=%d", result);
        return;
    }

    if (info.format != checkFormat) {
        LOGE(TAG, "Bitmap format is not your selection !");
        return;
    }

    if ((result = AndroidBitmap_lockPixels(env, bitmap, (void **) &pixels)) < 0) {
        LOGE(TAG, "bitmap get pixels failed ! error=%d", result);
    }
}

Bitmap::~Bitmap() {
    if (jenv && pixels != NULL) {
        AndroidBitmap_unlockPixels(jenv, jbitmap);

        jenv = NULL;
        pixels = NULL;
    }/* else {
            free(pixels);
        }*/
}

uint32_t* Bitmap::getPixels() {
    return pixels;
}

uint32_t Bitmap::getHeight() {
    return jenv ? info.height : height;
}

uint32_t Bitmap::getWidth() {
    return jenv ? info.width : width;
}

int32_t Bitmap::getType() {
    return checkFormat;
}

int Bitmap::getErrorCode() {
    return result;
}

void Bitmap::release() {
    if (jenv && pixels != NULL) {
        AndroidBitmap_unlockPixels(jenv, jbitmap);

        jenv = NULL;
        pixels = NULL;
    }
}
