#include <jni.h>
#include <string>
#include "android/AndroidBridge.h"
#include "LKParticleEffectSystem.h"

using namespace LKKit;
static LKParticleEffectSystem *ps;

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_tencent_willisdai_particlesystem_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
//    LKKit::AndroidBridge::env = env;
//    LKKit::AndroidBridge::glTexImage2DFromData("/sdcard/DCIM/Camera/IMG.jpg", NULL, 0);

    std::string hello = "Hello from C++!!!!";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void

JNICALL
Java_com_tencent_willisdai_particlesystem_ParticleSystemProxy_loadNative(
        JNIEnv *env,
        jobject /* this */) {
//    LKKit::AndroidBridge::env = env;
//    LKKit::AndroidBridge::glTexImage2DFromData("/sdcard/DCIM/Camera/IMG.jpg", NULL, 0);

//    std::string hello = "Hello from C++!!!!";

    LKKit::AndroidBridge::env = env;

    ps = new LKParticleEffectSystem(LKParticleEffectConfig());
    ps->load("/sdcard/particletest");

    LKKit::AndroidBridge::env = NULL;
}

extern "C"
JNIEXPORT void

JNICALL
Java_com_tencent_willisdai_particlesystem_ParticleSystemProxy_renderNative(
        JNIEnv *env,
        jobject /* this */) {
//    LKKit::AndroidBridge::env = env;
//    LKKit::AndroidBridge::glTexImage2DFromData("/sdcard/DCIM/Camera/IMG.jpg", NULL, 0);

//    std::string hello = "Hello from C++!!!!";

    ps->render();
}
