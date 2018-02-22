/* 
  vertexShader.vs
  LKParticleEffect

  Created by 柯灵杰 on 2018/2/13.
  Copyright © 2018年 柯灵杰. All rights reserved.
*/

#version 300 es
layout(location = 0) in uint identifier;
layout(location = 1) in vec3 position;
void main()
{
    gl_PointSize = 100.0;
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
