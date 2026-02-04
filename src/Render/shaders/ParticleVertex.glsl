#version 430 core

layout (location = 0) in vec4 position;
//layout (location = 1) in vec4 velocity;

uniform mat4 mvp; // = projection*view*model;
out float intensity;

void main(void){
    gl_Position = mvp * vec4(position.xyz, 1.0);
    intensity = position.w;
}