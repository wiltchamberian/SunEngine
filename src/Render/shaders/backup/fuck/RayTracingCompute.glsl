#version 430 core

layout (local_size_x = 1, local_size_y = 1) in;
layout (binding = 0, rgba32f) uniform image2D output_image;

void main(void)
{
    vec4 color = vec4(vec2(gl_GlobalInvocationID.xy)/gl_NumWorkGroups.x,0.0,1.0);
    //vec4 color =vec4(1,0,0,1);
    //输出结果到对应像素
    imageStore(output_image, ivec2(gl_GlobalInvocationID.xy),color);
}