#version 430 core

layout (location = 0) in vec3 vert;

out vec2 TexCoord;

void main()
{
  gl_Position = vec4(vert,1.0); //(-1,1)
  TexCoord = (vec2(vert.x,vert.y)+vec2(1.0,1.0))*0.5; //(0,1)
};