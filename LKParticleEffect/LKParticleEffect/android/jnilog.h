//
// Created by willisdai on 17/3/3.
//

#ifndef JNITEST_JNILOG_H
#define JNITEST_JNILOG_H

#include<android/log.h>

#define LOGD(TAG, ...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(TAG, ...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(TAG, ...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(TAG, ...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(TAG, ...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

/*#define LOGD(TAG, ...)
#define LOGI(TAG, ...)
#define LOGW(TAG, ...)
#define LOGE(TAG, ...)
#define LOGF(TAG, ...)*/

#endif //JNITEST_JNILOG_H
