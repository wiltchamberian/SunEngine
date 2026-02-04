#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec4 ourColor;
out vec3 TexCoord_cube;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model* vec4(aPos.x, aPos.y, aPos.z, 1.0);
  ourColor = aColor;
  TexCoord_cube = vec3(aPos.x,aPos.y,aPos.z);
};

