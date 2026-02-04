#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoord;
//layout (location = 4) in vec3 aTangentX;
//layout (location = 5) in vec3 aTangentY;

//instance buffer
layout (location = 6) in vec3 aOffset;


out vec4 ourColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 WorldPos; //world coordinate

//out vec3 FluidPos; //for fluid;

uniform mat4 mvp; // = projection*view*model;
uniform mat3 normal_mat; 
uniform mat4 model; //model

uniform vec3 viewPos;
uniform float fluidHeight;



void main()
{
  vec4 pos = vec4(aPos,1.0);
  //pos = pos + vec4(aOffset,0.0);

  gl_Position = mvp * pos;
  WorldPos = vec3(model* pos);

  //Normal = normal_mat * aNormal;
  //Normal = aNormal;
  mat3 normalMatrix = transpose(inverse(mat3(model)));
  Normal = normalMatrix * aNormal;

  ourColor = aColor;
  TexCoord = aTexCoord;


};

