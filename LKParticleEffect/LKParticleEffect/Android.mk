# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# $Id: Android.mk 216 2015-05-18 15:28:41Z oparviai $

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# *** Remember: Change -O0 into -O2 in add-applications.mk ***

LOCAL_MODULE    := LKParticleSystem

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/android/*.cpp)

LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

#LOCAL_SRC_FILES := LKJSONObject.cpp \
#                   LKParticleEffectEmitter.cpp \
#                   LKParticleEffectLogger.cpp \
#                   LKParticleEffectShader.cpp \
#                   LKParticleEffectSpriteObject.cpp \
#                   LKParticleEffectSystem.cpp \
#                   LKParticleEffectTexture.cpp \
#                   LKParticleEffectUtil.cpp \
#                   LKParticleValue.cpp \
#                   mathexpr.cpp \
#                   PlatformBridge.cpp \
#                   android/AndroidBridge.cpp \
#                   LKParticleEffectCamera.cpp \

# for native audio
LOCAL_SHARED_LIBRARIES += -lgcc 
# --whole-archive -lgcc
# for logging
LOCAL_LDLIBS    += -llog
# for native asset manager
#LOCAL_LDLIBS    += -landroid

LOCAL_LDLIBS    += -lGLESv3

# Custom Flags:
# -fvisibility=hidden : don't export all symbols
LOCAL_CFLAGS += -fvisibility=hidden -fdata-sections -ffunction-sections
#LOCAL_CFLAGS += -I/Users/willisdai/dev/android-ndk-r12b/sources/cxx-stl/gnu-libstdc++/4.9/include
#LOCAL_C_INCLUDES += $(LOCAL_PATH)../../include

#LOCAL_C_INCLUDES += O:/Android-lib/app/src/main/jni/include
# OpenMP mode : enable these flags to enable using OpenMP for parallel computation
#LOCAL_CFLAGS += -fopenmp
#LOCAL_LDFLAGS += -fopenmp

APP_ALLOW_MISSING_DEPS :=true

# Use ARM instruction set instead of Thumb for improved calculation performance in ARM CPUs	
LOCAL_ARM_MODE := arm

include $(BUILD_SHARED_LIBRARY)
