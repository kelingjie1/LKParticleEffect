//
//  LKParticleEffectIOSUtil.m
//  LKParticleEffect
//
//  Created by lingtonke on 2018/2/24.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "LKParticleEffectIOSBridge.h"
#import <string>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

using namespace std;
using namespace LKKit;

pair<GLsizei, GLsizei> LKParticleEffectIOSBridge::glTexImage2DFromData(uint8_t *data, int length)
{
    UIImage *image = [UIImage imageWithData:[NSData dataWithBytesNoCopy:data length:length freeWhenDone:NO]];
    GLsizei width = (GLsizei)CGImageGetWidth(image.CGImage);
    GLsizei height = (GLsizei)CGImageGetHeight(image.CGImage);
    CGDataProviderRef dataProvider = CGImageGetDataProvider(image.CGImage);
    CFDataRef cfdata = CGDataProviderCopyData(dataProvider);
    const uint8_t *bitmapdata = CFDataGetBytePtr(cfdata);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmapdata);
    CFRelease(cfdata);
    return pair<GLsizei, GLsizei>(width,height);
}


