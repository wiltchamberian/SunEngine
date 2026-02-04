#version 430 core

//相机
/* struct Camera{
  vec3 origin; //in world
  vec3 direction; //长度为到阵列的距离
  vec3 up;
  float halfWidth;
  float halfHeight;
}; */
struct Camera{
  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
};

uniform Camera camera;

struct Ray{
  vec3 origin;
  vec3 direction;
};

layout (local_size_x = 1, local_size_y =1) in;

//生成的光线(输出) （按照从左下角先向右，再逐行向上的方式排列)
layout(std430, binding = 1) buffer RayBuf{
   Ray rays[];
};

//相机的像素阵列的宽，高
uniform ivec2 img;

//我们利用计算着色器生成w*h个工作组，w为图像宽，h为图像高；
//然后每个工作组分为N*N个调用，对应均匀采样的N*N条射线

void main(){
    uint W = gl_NumWorkGroups.x;
    uint H = gl_NumWorkGroups.y;
    uint N = gl_WorkGroupSize.x;

    //计算着色器作用于每个调用,获取当前调用的工作组id以及调用id
    uint x = gl_WorkGroupID.x;
    uint y = gl_WorkGroupID.y;

    uint m = gl_LocalInvocationID.x;
    uint n = gl_LocalInvocationID.y;

    //vec3 right = normalize(cross(camera.direction,camera.up));
    //vec3 up = normalize(cross(right,camera.direction));

    float invW = 1.f/float(W);
    float invH = 1.f/(float(H));
    float u = (float(x) + (float(m)+0.5)/float(N)) * invW;
    float v = (float(y) + (float(n)+0.5)/float(N)) * invH;
    //vec3 low_left = camera.origin + camera.direction - right * camera.halfWidth - up * camera.halfHeight;
    vec3 low_left = camera.lower_left_corner;

    //vec3 direction = low_left + right*camera.halfWidth * 2 * u + up* camera.halfHeight*2*v - camera.origin;
    vec3 direction = low_left + camera.horizontal * u + camera.vertical * v - camera.origin;

    uint index = gl_GlobalInvocationID.y * W * N + gl_GlobalInvocationID.x;
    rays[index].origin = camera.origin;
    rays[index].direction = direction;

}