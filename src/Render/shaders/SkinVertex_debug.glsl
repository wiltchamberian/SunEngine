#version 330 core
const int MAX_TEXCOORDS = 1;
const int MAX_TOTAL_INFLUENCES =4 ;
layout (location = 0) in vec3 Position; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangentX;
layout (location = 3) in vec3 tangentY;
layout (location = 4) in vec4 color;
layout (location = 5) in vec2 textures[MAX_TEXCOORDS];
layout (location = 6) in int jointIndex[MAX_TOTAL_INFLUENCES];
layout (location = 7) in float jointWeight[MAX_TOTAL_INFLUENCES];
//layout (location = 6) in ivec4 jointIndex;
//layout (location = 7) in vec4 jointWeight;

out vec2 TexCoord0;
out vec3 Normal0; 
out vec3 WorldPos0; 
out vec4 Color0;
const int MAX_BONES = 256;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 gTranslation[MAX_BONES];
uniform vec4 gQuaternion[MAX_BONES];
uniform vec3 gScale[MAX_BONES];

void main()
{ 
    vec3 translation = gTranslation[jointIndex[0]]*jointWeight[0];
    translation +=  gTranslation[jointIndex[1]]*jointWeight[1];
    translation +=  gTranslation[jointIndex[2]]*jointWeight[2];
    translation +=  gTranslation[jointIndex[3]]*jointWeight[3];

    vec3 scale = gScale[jointIndex[0]] * jointWeight[0];
    scale += gScale[jointIndex[1]] * jointWeight[1];
    scale += gScale[jointIndex[2]] * jointWeight[2];
    scale += gScale[jointIndex[3]] * jointWeight[3];

    //四元数插值
    vec4 quaternion;

    vec4 PosL = BoneTransform * vec4(Position, 1.0);
    //vec4 PosL = vec4(Position, 1.0);
    gl_Position = projection * view * model * PosL;
    TexCoord0 = textures[0];
    Color0 = color;
}