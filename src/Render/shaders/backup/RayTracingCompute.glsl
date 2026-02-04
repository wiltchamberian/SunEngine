#version 430 core
layout (local_size_x = 32, local_size_y =16) in;
layout(rgba32f, binding = 0) uniform image2D img_output;


void main()
{
  vec4 pixel = vec4(vec2(gl_WorkGroupID.xy)/gl_WorkGroupSize.x,0,1);
  ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
  //imageStore(img_output, pixel_coords, pixel);

  imageStore(img_output,ivec2(gl_GlobalInvocationID.xy),
  vec4(vec2(gl_LocalInvocationID.xy)/vec2(gl_WorkGroupSize.xy),0.0, 0.0));
}
