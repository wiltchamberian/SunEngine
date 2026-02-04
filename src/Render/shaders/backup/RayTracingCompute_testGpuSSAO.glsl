#version 430 compatibility
//#extension GL_ARB_shader_storage_buffer_object : enable;

struct Vertice{
  vec4 pos;
  vec4 color;
};

//球体
struct SphereObj{
  vec3 center;
  float radius;
};


layout (local_size_x = 1, local_size_y =1) in;
//layout(rgba32f, binding = 0) uniform image2D img_output;


layout(std430, binding = 1) buffer VerticeBuf{
    Vertice vertices[];
};

void main()
{
  //vec4 pixel = vec4(vec2(gl_WorkGroupID.xy)/gl_WorkGroupSize.x,0,1);
  //ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
  //imageStore(img_output,ivec2(gl_GlobalInvocationID.xy),
  //vec4(vec2(gl_LocalInvocationID.xy)/vec2(gl_WorkGroupSize.xy),0.0, 0.0));

  uint id = gl_GlobalInvocationID.x;
  vertices[id].pos.x+=0.001;
  vertices[id].pos.y+=0.002;
  vertices[id].pos.z+=0.003;
  
}
