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

out vec2 TexCoord0;
out vec3 Normal0; 
out vec3 WorldPos0; 
out vec4 Color0;
const int MAX_BONES = 100;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 gBones[MAX_BONES];

void main()
{ 
    mat4 BoneTransform = gBones[jointIndex[0]] * jointWeight[0];
    BoneTransform += gBones[jointIndex[1]] * jointWeight[1];
    BoneTransform += gBones[jointIndex[2]] * jointWeight[2];
    BoneTransform += gBones[jointIndex[3]] * jointWeight[3];
    vec4 PosL = BoneTransform * vec4(Position, 1.0);
    gl_Position = projection * view * model * PosL;
    TexCoord0 = textures[0];
    vec4 NormalL = BoneTransform * vec4(normal, 0.0);
    Normal0 = (model * NormalL).xyz;
    WorldPos0 = (model * PosL).xyz; 
    Color0 = color;
}