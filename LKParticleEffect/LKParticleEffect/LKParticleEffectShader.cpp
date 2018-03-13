//
//  LKParticleEffectShader.cpp
//  LKParticleEffect
//
//  Created by 柯灵杰 on 2018/2/22.
//  Copyright © 2018年 柯灵杰. All rights reserved.
//

#include "LKParticleEffectShader.h"
using namespace LKKit;
#define MULTI_LINE_STRING(a) #a

LKParticleEffectShader *LKParticleEffectShader::instance()
{
    static LKParticleEffectShader *instance = nullptr;
    if (!instance)
    {
        instance = new LKParticleEffectShader();
    }
    return instance;
}

LKParticleEffectShader::LKParticleEffectShader()
{
    vertexShaderStr = string("#version 300 es\n")+
    MULTI_LINE_STRING
    (
     layout(location = 0) in uint index;
     layout(location = 1) in vec3 position;
     layout(location = 2) in vec2 pointSize;
     layout(location = 3) in vec4 a_colorFactor;
     layout(location = 4) in vec3 a_texCoord;
     layout(location = 5) in float a_rotation;
     layout(location = 6) in float a_frameIndex;
     
     out vec4 v_colorFactor;
     out vec3 v_texCoord;
     out float v_rotation;
     out vec2 v_pointScale;
     
     uniform mat4 vpMatrix;
     
     
     void main()
    {
        gl_Position = vpMatrix*vec4(position.x, position.y, position.z, 1.0);
        float longEdge = max(pointSize.x,pointSize.y);
        gl_PointSize = longEdge;//*gl_Position.z*1.414*1.0;
        v_pointScale = vec2(pointSize.x/longEdge,pointSize.y/longEdge);
        
        v_colorFactor = a_colorFactor;
        v_texCoord = a_texCoord;
        v_rotation = a_rotation;
    }
     );
    
    fragmentShaderStr = string("#version 300 es\n")+
    MULTI_LINE_STRING
    (
     precision highp float;
     layout(location = 0) out vec4 color;
     uniform sampler2D textures[8];
     uniform int premultiplied[8];
     uniform vec2 frameSizes[8];
     uniform float radian;
     
     in vec3 v_texCoord;
     in vec4 v_colorFactor;
     in float v_rotation;
     in vec2 v_pointScale;
     
     
     
     void main()
     {
         float r = radian + v_rotation;
         vec2 pos = vec2(gl_PointCoord.x-0.5,0.5-gl_PointCoord.y);
         pos = vec2((cos(r)*pos.x+sin(r)*pos.y)*1.414/v_pointScale.x+0.5,
                    (cos(r)*pos.y-sin(r)*pos.x)*1.414/v_pointScale.y+0.5);
         if (pos.x<0.0||pos.x>1.0||pos.y<0.0||pos.y>1.0)
         {
             discard;
         }
         int index = int(v_texCoord.z+0.5);
         vec2 frameSize = frameSizes[index];
         vec4 texColor;
         if (index==0)
         {
             texColor = texture(textures[0], vec2((v_texCoord.x+pos.x)*frameSize.x,
                                                  (v_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==1)
         {
             texColor = texture(textures[1], vec2((v_texCoord.x+pos.x)*frameSize.x,
                                                  (v_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==2)
         {
             texColor = texture(textures[2], vec2((v_texCoord.x+pos.x)*frameSize.x,
                                                  (v_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==3)
         {
             texColor = texture(textures[3], vec2((v_texCoord.x+pos.x)*frameSize.x,
                                                  (v_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==4)
         {
             texColor = texture(textures[4], vec2((v_texCoord.x+pos.x)*frameSize.x,
                                                  (v_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==5)
         {
             texColor = texture(textures[5], vec2((v_texCoord.x+pos.x)*frameSize.x,
                                                  (v_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==6)
         {
             texColor = texture(textures[6], vec2((v_texCoord.x+pos.x)*frameSize.x,
                                                  (v_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==7)
         {
             texColor = texture(textures[7], vec2((v_texCoord.x+pos.x)*frameSize.x,
                                                  (v_texCoord.y+pos.y)*frameSize.y));
         }
         
//         if (premultiplied[index]==1)
//         {
//             texColor.rgb = texColor.rgb * texColor.a;
//         }
//         texColor = vec4(texColor.r * v_colorFactor.r * v_colorFactor.a,
//                         texColor.g * v_colorFactor.g * v_colorFactor.a,
//                         texColor.b * v_colorFactor.b * v_colorFactor.a,
//                         texColor.a * v_colorFactor.a);
         color = texColor;
     }
     );
}
