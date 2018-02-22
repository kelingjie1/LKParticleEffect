//
//  LKParticleEffectObject.h
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/13.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

typedef struct
{
    GLuint identifier;
    GLfloat positionX;
    GLfloat positionY;
    GLfloat positionZ;
} LKParticleEffectObjectData;

@interface LKParticleEffectObject : NSObject

@property (nonatomic) LKParticleEffectObjectData *data;

@end

