#version 430 core

#define MAX_METRIC 100000000//3.402823466e+38F

//光追最大反射次数
#define MAX_SCATTER 5

#define A_PI 3.14159265358

//2d噪声纹理
uniform sampler2D Noise; //100*100

int g_index = 0;

//手动实现伪随机采样器
vec3 randomSample(){
  int ct = g_index % 10000;
  int n = ct/100;
  int m = ct - n * 100;
  float x = float(m)/100;
  float y = float(n)/100;
  vec3 v = texture(Noise,vec2(x,y)).xyz;
  g_index += 4987;  // should be a prime
  return v;
}

struct Camera{
  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
};

uniform Camera camera;

//球体
struct Sphere{
  //vec3 center;
  //vec3 color;
  //vec3 radius;  //为了glsl该死的字节对齐，第一分量表示真实半径
  //float radius;
  //vec2 padding; //for alignment

  float center_x;
  float center_y;
  float center_z;
  float color_x;
  float color_y;
  float color_z;
  float radius;
};

struct Item3D{
  int x;
  int y;
  int z;
  float c;
};

struct Ray{
  vec3 origin;
  vec3 direction;
};

//碰撞信息结构体
struct HitRecord {
    //标记光线碰撞点在光路上的尺度
    float t;
    //光线碰撞点
    vec3 p;
    //碰撞位置的法线
    vec3 normal;
    //碰撞物体的材质索引
    int material;  //可能废弃,glsl中材质信息数据放在物体中，直接存物体索引
    //碰撞物体索引
    int id;
    //标记碰撞点对应物体的点的u,v坐标
    float u;
    float v;
};

////////////////////////////////////布局信息//////////////////////////////////////
layout (local_size_x = 1, local_size_y =1) in;
layout(rgba32f, binding = 0) uniform image2D output_image;

//生成的光线(输出) （按照从左下角先向右，再逐行向上的方式排列)
layout(std430, binding = 1) buffer RayBuf{
   Ray rays[];
};

layout(std430, binding =2) buffer SphereBuf{
  Sphere spheres[];
};

const int numItem = 20;
layout(std430, binding =3) buffer Items{
  Item3D items[];
};

//输出的颜色信息
layout(std430, binding = 10) buffer ColorBuf{
  vec3 colors[];
};

//layout(binding = 4) uniform sampler2D Noise;

void get_sphere_uv(vec3 p, inout float u, inout float v) {
    float phi = atan(p.z, p.x); //how to make sure input not all zero?
    float theta = asin(p.y);
    u = 1 - (phi + A_PI) / (2 * A_PI);
    v = (theta + A_PI / 2) / A_PI;
}

int hitRay_Sphere(int id, Ray ray, float t_min, float t_max, inout HitRecord rec){
    Sphere sphere = spheres[id];

    vec3 sphere_center = vec3(sphere.center_x,sphere.center_y,sphere.center_z);
    float radius = sphere.radius;
    vec3 oc = ray.origin - sphere_center;
    
    float A = dot(ray.direction,ray.direction);
    float B = dot(ray.direction,oc);
    float C = dot(oc,oc) - radius* radius;
    float discriminant = B * B -  A * C;
    if (discriminant > 0) {
        float sqr = sqrt(discriminant);
        float t = (-B - sqr) / A;
        if (t<t_max && t>t_min) {
            rec.t = t;
            rec.p = ray.origin + ray.direction * rec.t;
            rec.normal = (rec.p - sphere_center) / radius;
            rec.id = id;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            return 1;
        }
        t = (-B + sqr) / A;
        if (t<t_max && t>t_min) {
            rec.t = t;
            rec.p = ray.origin + ray.direction * rec.t;
            rec.normal = (rec.p - sphere_center) / radius;
            rec.id = id;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            return 1;
        }
    }
    return 0;
}

//int hit_Surface(int id, Ray ray, float t_min, float t_max,inout HitRecord rec){
 // int k = id * numItem;
 // Item3D array[20];
//}

int lambertian_scatter(int id, Ray ray, HitRecord rec, inout vec3 attenuation, inout Ray scattered){
    //散射球内随机点 (这是原始公式，个人认为可以优化到半球内)
    //vec3 randPoint = randomSample();
    vec3 randPoint =vec3(0,0,0);
    vec3 target = rec.p + rec.normal + randPoint;//randomPointInUintSphere();
    //散射射线
    scattered.origin = rec.p;
    scattered.direction = target - rec.p;

    //float u = 0;
    //float v = 0;
    //get_sphere_uv(rec.normal, u, v);
    //attenuation = albedo_->value(rec.u, rec.v, rec.p);
    attenuation = vec3(spheres[id].color_x,spheres[id].color_y,spheres[id].color_z);//ent.color;
    //for test
    /* attenuation = vec3(spheres[0].color_x,spheres[0].color_y,spheres[0].color_z);
    attenuation = vec3(0.5,0.5,0.5); */
    return 1;
}

//int hitRay_Sphere(int id, Ray ray, float t_min, float t_max, inout HitRecord rec)
//单次光线追踪  (根据入射光  计算 hitRecord)
int rayTracingOneHit(in Ray ray, inout HitRecord rec){

  float t_min = 0.001;
  float t_max = MAX_METRIC;
  //int siz = 3;
  int siz = spheres.length();
  int res = 0;
  int hit = 0;
  for(int i = 0;i < siz ;++i) {
    HitRecord tmpRec;
    int hit = hitRay_Sphere(i, ray, t_min,t_max, tmpRec);
    res = res|hit;
    if(hit>0 && (tmpRec.t <= rec.t)){
      rec = tmpRec;
    }
  }

  return res;
  
}

//光线追踪
vec3 rayTracing(in Ray ray){
  int i = 0;
  //太阳光
  vec3 outColor = vec3(1,1,1);
  
  Ray outRay;
  HitRecord rec;
  rec.t = MAX_METRIC;
  rec.id = 0;
  Ray inRay = ray;
  for(i = 0; i< MAX_SCATTER ; ++i){
    //将入射光和场景所有物体碰撞（可利用加速结构）找到最近碰撞点信息
    int ok = rayTracingOneHit(inRay, rec);
    if( ok==1){
      //int lambertian_scatter(int id, Ray ray, HitRecord rec, out vec3 attenuation, out Ray scattered)
      vec3 attenuation = vec3(1,1,1); //输出颜色
      Ray scattered;
      //根据输入光线和碰撞信息，计算散射光线
      int isScatter =  lambertian_scatter(rec.id , inRay, rec, attenuation, scattered);
      outColor = outColor * attenuation;
      inRay = scattered;
    }else{
      ++i;
      break;
    }
  }
  
  return outColor;
}

void main(){

  //////////////////////////////光线生成 （着色器内容合并)////////////////////

    float invW = 1.f/float(gl_NumWorkGroups.x);
    float invH = 1.f/float(gl_NumWorkGroups.y);
    float u = (float(gl_WorkGroupID.x) + (float(gl_LocalInvocationID.x)+0.5)/float(gl_WorkGroupSize.x)) * invW;
    float v = (float(gl_WorkGroupID.y) + (float(gl_LocalInvocationID.y)+0.5)/float(gl_WorkGroupSize.y)) * invH;
    vec3 low_left = camera.lower_left_corner;

    vec3 direction = low_left + camera.horizontal * u + camera.vertical * v - camera.origin;

    uint index = gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x + gl_GlobalInvocationID.x;
    //rays[index].origin = camera.origin;
    //rays[index].direction = direction;

    Ray outRay;
    outRay.origin = camera.origin;
    outRay.direction = direction; 

  /////////////////////////////////光线追踪相关//////////////////////////////
    uint id = gl_GlobalInvocationID.x;
    uint W = gl_NumWorkGroups.x;
    uint H = gl_NumWorkGroups.y;
    uint N = gl_WorkGroupSize.x; 

    //uint index = gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x + gl_GlobalInvocationID.x;
    //Ray ray = rays[index];

    vec3 color = rayTracing(outRay);

    //输出数组 结构与输入不同，为了保证在同一个像素的数据连续存放在一起，我们修改索引
    uint outIndex = (gl_WorkGroupID.y*W + gl_WorkGroupID.x)*(N*N) + gl_LocalInvocationID.y* N+gl_LocalInvocationID.x;

    //colors[outIndex] = color;

    imageStore(output_image,ivec2(gl_GlobalInvocationID.xy),
      vec4(color, 1.0));

}