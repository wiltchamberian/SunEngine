#version 430 core

layout (location = 0) out vec4 color;

//derived from age and read by vertex shader
in float intensity;

void main(void)
{
    //fire rgb: 217,109,79
    //blend between red-hot and cool-blue based on the age of the particle
    //color = mix(vec4(0.0f, 0.2f, 1.0f, 1.0f), vec4(0.2f,0.05f,0.0f,1.0f),intensity);
    color = mix(vec4(0.0f,0.0f,0.0f,1.0f),vec4(217.0/255,109.0/255,79.0/255,1.0f),intensity);
}