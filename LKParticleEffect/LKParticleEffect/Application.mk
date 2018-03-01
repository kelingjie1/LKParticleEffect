# $Id: Application.mk 212 2015-05-15 10:22:36Z oparviai $
#
# Build library bilaries for all supported architectures
#

APP_ABI := armeabi-v7a # all armeabi
APP_OPTIM := release
APP_STL := c++_shared
APP_CPPFLAGS := -std=c++11 -fexceptions -D __SOFTFP__ -D SOUNDTOUCH_DISABLE_X86_OPTIMIZATIONS
APP_PLATFORM := android-26
NDK_TOOLCHAIN_VERSION := clang
