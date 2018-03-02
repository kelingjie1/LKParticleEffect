//
// Created by willisdai on 2018/3/2.
//

#ifndef PARTICLESYSTEM_BITMAP_H
#define PARTICLESYSTEM_BITMAP_H

#include <android/bitmap.h>
#include <cstdlib>
#include <cstring>
#include "jnilog.h"

//typedef uint32_t ABsize;//Android Bitmap size
//typedef int32_t ABformat;//Android Bitmap format
#ifdef ARGB_8888
typedef uint32_t  APixel;
    ABformat checkFormat = ANDROID_BITMAP_FORMAT_RGBA_8888;
#elif defined(ARGB_4444)
typedef uint16_t APixel;
    ABformat checkFormat = ANDROID_BITMAP_FORMAT_RGBA_4444;
#elif defined(RGB_565)
typedef uint16_t APixel;
    ABformat checkFormat = ANDROID_BITMAP_FORMAT_RGB_565;
#elif defined(ALPHA_8)
typedef uint8_t APixel;
ABformat checkFormat = ANDROID_BITMAP_FORMAT_A_8;
#else
//typedef uint32_t  APixel;
//int32_t checkFormat = ANDROID_BITMAP_FORMAT_RGBA_8888;
#endif

class Bitmap {

private:
    uint32_t *pixels;
    JNIEnv *jenv;
    _jobject *jbitmap;
    AndroidBitmapInfo info;
    int result;
    uint32_t width;
    uint32_t height;
    int32_t checkFormat = ANDROID_BITMAP_FORMAT_RGBA_8888;


public:
    /*Bitmap(int width, int height) : jenv(NULL), jbitmap(NULL) {
        pixels = (APixel *) malloc(sizeof(APixel) * width * height);
        memset(pixels, 0, width * height);
    }*/

    Bitmap(JNIEnv *env, jobject bitmap);

    ~Bitmap();/* {
        if (jenv && pixels != NULL) {
            AndroidBitmap_unlockPixels(jenv, jbitmap);
        } else {
            free(pixels);
        }
    }*/

    uint32_t *getPixels();/* {
        return pixels;
    }*/

    uint32_t getHeight();/* {
        return jenv ? info.height : height;
    }*/

    uint32_t getWidth();/* {
        return jenv ? info.width : width;
    }*/

    int32_t getType();/* {
        return checkFormat;
    }*/

    int getErrorCode();/* {
        return result;
    }*/

    void release();

    /*operator uint32_t *() {
        return pixels;
    }*/

    /*uint32_t *operator[](int y) {
        if (y >= getHeight()) {
            return NULL;
        }

        return pixels + y * getWidth();
    }*/
};

#endif //PARTICLESYSTEM_BITMAP_H
