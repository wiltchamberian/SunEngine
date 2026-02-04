#ifndef __cplusplus
#version 460
#extension GL_ARB_separate_shader_objects : enable

#define Uniform uniform
#endif

#ifdef __cplusplus

#include "Math/Vector2D.h"
#include "Math/Vector3D.h"
#include "Math/Matrix4x4.h"
using namespace Sun;

#define layout()
#define uniform
#define in
#define out
#define Uniform struct

extern vec4 gl_Position;

#endif



layout (binding = 0) Uniform UniformBufferObject{
    mat4 model;
    mat4 view;
    mat4 proj;
}ubo;

uniform mat4 mvp;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;


void main() {
    gl_Position = ubo.proj*ubo.view*ubo.model*vec4(inPosition,1.0);
    fragColor = inColor;
    fragTexCoord = aTexCoord;
}