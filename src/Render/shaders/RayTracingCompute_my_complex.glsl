#version 430 core
//#extension GL_ARB_shader_storage_buffer_object : enable;

//为了避免if else,同时实现c++的多态，这里利用subroutine来实现
//初步实现机制是成员存储一个函数索引，不同函数实现放在一个数组中

#define MAX_flt 3.402823466e+38F

//光追最大反射次数
#define MAX_SCATTER 5

#define A_PI 3.14159265358

/////////////////////////////////////结构体//////////////////////////////////////
//相机
struct Camera{
  vec3 pos; //in world
  vec3 direction;
  vec3 up;
  float halfWidth;
  float halfHeight;
};

uniform Camera camera;

struct Vertice{
  vec4 pos;
  vec4 color;
  vec4 normal;
};

//球体
struct SphereObj{
  vec3 center;
  float radius;
};

//aabbbox  （废弃，等价于三角形网格)
struct AABBObj{
  vec3 min;
  vec3 max;
};

//通用entity  （描述一个gameobject）
struct Entity{
  int offset; //实体信息数组偏移量
  int len;    //实体信息结构体实际长度
  int type; //实体类型，目前支持球体 0 三角形网格1 ;当前先只处理球体
  int hitFuncIndex; //该实体的hit函数索引
  int material; //材质索引
  vec3 color; //基础颜色，原本应该位于材质，先简化着用一下
};

struct BVHNode{
  int left; //叶子节点为-1
  int right; //叶子节点为-1
  vec3 mins;  //aabb
  vec3 maxs;
  int hitFunc; 
  int entity; //如果是叶子节点，指定entity索引，否则是自身索引
};

struct Ray{
  vec3 origin;
  vec3 direction;
};

//材质结构体
struct Material {
  //存储散射函数选择器索引
  int scatterFuncIndex;
  //辐射函数选择器索引
  int emitFuncIndex;
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
/////////////////////////////////////结构体--end//////////////////////////////////////

//2d噪声纹理
uniform sampler2D Noise; //100*100

int g_index = 0;

//手动实现伪随机采样器
vec3 randomSample(){
  
  int t = g_index/100;
  int m = g_index - t*100;
  float x = float(t)/100;
  float y = float(m)/100;
  vec3 v = texture(Noise,vec2(x,y)).xyz;
  g_index += 7;
  return v;
}

////////////////////////////////////布局信息//////////////////////////////////////
//生成的光线(输出) （按照从左下角先向右，再逐行向上的方式排列)
layout(std430, binding = 0) buffer RayBuf{
   Ray rays[];
};

//输出的颜色信息
layout(std430, binding =10) buffer ColorBuf{
  vec3 colors[];
};

layout (local_size_x = 6, local_size_y =6) in;
//layout(rgba32f, binding = 0) uniform image2D img_output;

layout(std430, binding = 1) buffer GameObjBuf{
  Entity entities[];
};

layout(std430, binding =2) buffer SphereObjBuf{
  SphereObj sphereObjs[];
};

layout(std430, binding = 3) buffer VerticeBuf{
    Vertice vertices[];
};

layout(std430, binding = 4) buffer IndiceBuf{
  int indices[];
};

//材质只有有限类型（金属，。。等），材质只映射到函数，具体的参数信息，存放在物体内部
layout(std430, binding = 5) buffer MaterialBuf{
  Material materials[];
};

//bvh结构(加速结构)
layout(std430, binding = 6) buffer BVHNodeBuf{
  BVHNode bvhNodes[];  //nodes[0]为根
};
/////////////////////////////////布局信息-end//////////////////////////////////

//深度递归用栈（采用循环栈)
const int stackLen = 128;
int bvhStack[stackLen];
int bvhTop = 0; //指向栈顶元素
int bvhBottom = -1;  //指向栈底元素
bool isEmpty = false;

////////////////////////////函数原型声明//////////////////////
//由于选择器只能在c++代码访问，我们在程序中用数组静态设置多个选择器并实现配置后，通过索引直接调用.

//碰撞函数原型（类似c++虚函数，或者函数指针，以实现多态，避免if else) true:1 false:0
subroutine int Hit(int id, Ray ray, float t_min, float t_max, inout HitRecord rec);
//碰撞函数选择器 (分别是 bvh_ray, sphere_ray,...)
subroutine uniform Hit hitFuncArray[10];

//散射函数原型
subroutine int Scatter(int id, Ray ray, HitRecord rec, out vec3 attenuation, out Ray scattered);
//散射函数选择器
subroutine uniform Scatter scatterFuncArray[10];

//辐射函数原型
subroutine vec3 Emitted(int id, float u, float v, vec3 p);
//辐射函数选择器
subroutine uniform Emitted emittedFuncArray[2];


//为去除if else 添加的一些 赋值 子程
subroutine void AssignValue(in HitRecord r_in,out HitRecord r_out);
subroutine uniform AssignValue assignFuncArray[2]; //0 :unreal 1:unreal 2:real

subroutine void HitBingo(in HitRecord r_in, out HitRecord r_out ,in BVHNode bvhNode);
subroutine uniform HitBingo hitbingoFuncArray[2]; //0:unreal 1:real

//////////////////////////////函数实现////////////////////////////////

//p should be a normal 
void get_sphere_uv(vec3 p, out float u, out float v) {
    float phi = atan(p.z, p.x); //how to make sure input not all zero?
    float theta = asin(p.y);
    u = 1 - (phi + A_PI) / (2 * A_PI);
    v = (theta + A_PI / 2) / A_PI;
}

//判断球体与光线是否碰撞
subroutine(Hit)
int hitRay_Sphere(int id, Ray ray, float t_min, float t_max, inout HitRecord rec){
    Entity ent = entities[id];
    SphereObj sphere = sphereObjs[ent.offset];

    //to reduce if else, make sure that this will be gurantee by cpu program;
/*
    if (!(sphere.radius < 0 || sphere.radius>0)) { //FIXME
        return 0;
    }
    */
    vec3 oc = ray.origin - sphere.center;
    float A = dot(ray.direction,ray.direction);
    float B = dot(ray.direction,oc);
    float C = dot(oc,oc) - sphere.radius * sphere.radius;
    float discriminant = B * B -  A * C;
    if (discriminant > 0) {
        float sqr = sqrt(discriminant);
        float t = (-B - sqr) / A;
        if (t<t_max && t>t_min) {
            rec.t = t;
            //rec.p = ray.pointAt(rec.t);
            rec.p = ray.origin + ray.direction * rec.t;
            rec.normal = (rec.p - sphere.center) / sphere.radius;
            //rec.material = material_;
            rec.id = id;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            return 1;
        }
        t = (-B + sqr) / A;
        if (t<t_max && t>t_min) {
            rec.t = t;
            rec.p = ray.origin + ray.direction * rec.t;
            rec.normal = (rec.p - sphere.center) / sphere.radius;
            //rec.material = material_;
            rec.id = id;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            return 1;
        }
    }
    return 0;
}

subroutine(Hit)
int hitRay_BVHNode(int bvhNodeId, Ray ray, float t_min, float t_max, inout HitRecord rec){
    BVHNode node = bvhNodes[bvhNodeId];

    float invD = 1.0;
    float d1 = 0.0;
    float d2 = 0.0;
    float t1 = 0.0;
    float t2 = 0.0;
    for (int i = 0; i < 3; ++i) {
        float r = ray.direction[i];
        float p = node.mins[i];
        float q = node.maxs[i];
        invD = 1 / r;
        d1 = (p - r) * invD;
        d2 = (q - r) * invD;
        float t1 = min(d1,d2);
        float t2 = max(d1,d2);

        t_min = max(t_min,t1);
        t_max = min(t_max,t2);

        if (t_max <= t_min)  //FIX ME
            return 0;
    }
    return 1;
  
}

//Lambertian材质散射
subroutine(Scatter)
int lambertian_scatter(int id, Ray ray, HitRecord rec, out vec3 attenuation, out Ray scattered){
    Entity ent = entities[id];
    //ent.material

    //散射球内随机点 (这是原始公式，个人认为可以优化到半球内)
    vec3 randPoint = randomSample();
    vec3 target = rec.p + rec.normal + randPoint;//randomPointInUintSphere();
    //散射射线
    scattered.origin = rec.p;
    scattered.direction = target - rec.p;

    //float u = 0;
    //float v = 0;
    //get_sphere_uv(rec.normal, u, v);
    //attenuation = albedo_->value(rec.u, rec.v, rec.p);
    attenuation = ent.color;

    return 1;
}

//金属材质散射
subroutine(Scatter)
int metal_scatter(int id ,Ray ray, HitRecord rec, out vec3 attenuation, out Ray scattered){
  return 1;
}

//电介质(玻璃)材质散射
subroutine(Scatter)
int dielectrics_scatter(int id ,Ray ray, HitRecord rec, out vec3 attenuation, out Ray scattered){
  return 1;
}

//光源材质散射
subroutine(Scatter)
int light_scatter(int id, Ray ray, HitRecord rec, out vec3 attenuation, out Ray scattered){
  return 1;
}

//光源辐射实现
subroutine(Emitted)
vec3 light_emitted(int id, float u, float v, vec3 p){
  return vec3(1,1,1);
}


subroutine(AssignValue)
void assign_real(in HitRecord r_in, out HitRecord r_out){
  r_out = r_in;
}
subroutine(AssignValue)
void assign_unreal(in HitRecord r_in, out HitRecord r_out){
  ;
}

subroutine(HitBingo)
void hitBingo_real(in HitRecord r_in, out HitRecord r_out, in BVHNode bvhNode){
  int k = int(sign(r_out.t-r_in.t)+1);
  //assignFuncArray[k](r_in,r_out);  //link bug

  if(r_out.t > r_in.t){
    r_out = r_in;
  }

  bvhTop++;
  bvhStack[bvhTop%stackLen] = bvhNode.right;
  bvhTop++;
  bvhStack[bvhTop%stackLen] = bvhNode.left;
}

subroutine(HitBingo)
void hitBingo_unreal(in HitRecord r_in, out HitRecord r_out, in BVHNode bvhNode){
}

////////////////////////函数实现--end/////////////////////




//判断bvhnode和光线是否碰撞
int hitRay_BVHNode(Ray ray, float t_min, float t_max, out HitRecord rec){
    HitRecord tmpRec;
    int bingo = 0;
    while(bvhTop >= 0 ){
      BVHNode bvhNode = bvhNodes[bvhTop%stackLen];
      bvhTop--;
      //Hit hitFunc = hitFuncArray[bvhNode.hitFunc];
      int isHit = hitFuncArray[bvhNode.hitFunc](bvhNode.entity, ray, t_min, t_max,tmpRec);

      bingo = bingo | isHit;
      //根据isHit与否，选择将孩子节点放入或者 不放入队列
      hitbingoFuncArray[isHit](tmpRec,rec,bvhNode);
    }
    return bingo;
    
}

//单次光线追踪结果存储（由于计算最终结果依赖后次追踪结果，只能先存起来)
struct RayTraceInfo{
  vec3 attenuation; //颜色(衰减)
  vec3 emitted;  //辐射
};

RayTraceInfo rayTraceInfoArray[MAX_SCATTER];

//单次光线追踪 子程
subroutine int BingoFunc(in Ray ray, in HitRecord record,int depth);
subroutine(BingoFunc)
int bingoFunc_real(in Ray ray, in HitRecord rec,int depth){
  
  //散射（反射或折射 )射线
  Ray scattered;
  //表示颜色（同时也就是衰减的意思，光线原始rgb乘以该系数 体现出颜色吸收,也就是衰减
  vec3 attenuation;
  //辐射体自发光
  //vec3 emitted = rec.material->emitted(rec.u, rec.v, rec.p);
  int materialId = entities[rec.id].material;
  Material material = materials[materialId];

  //vec3 emitted = emittedFuncArray[material.emitFuncIndex](rec.id, rec.u,rec.v,rec.p);
  vec3 emitted = vec3(0,0,0);
  //int isScatter = scatterFuncArray[material.scatterFuncIndex](ray,rec,attenuation,scattered);

  //由于通过routine报错，我们这里假定只有一种材质，也就是漫反射，且无辐射
  int isScatter = lambertian_scatter(rec.id, ray,rec,attenuation,scattered);

  rayTraceInfoArray[depth].attenuation = attenuation;
  rayTraceInfoArray[depth].emitted = emitted;

  return isScatter;
  
}

subroutine(BingoFunc)
int bingoFunc_unreal(in Ray ray, in HitRecord rec ,int depth){
  rayTraceInfoArray[depth].attenuation = vec3(1,1,1);
  vec3 ud = normalize(ray.direction);
  float t = 0.5 * (ud.y + 1.0);
  rayTraceInfoArray[depth].emitted =   vec3(1., 1., 1) * (1.0 - t) + vec3(0.5, 0.7, 1.0) * t;
  return 0;
}
subroutine uniform BingoFunc bingoFuncArray[2]; //0 unreal 1real

//单次光线追踪
int rayTracingOneHit(in Ray ray, int depth){
  
  HitRecord record;
  int bingo = hitRay_BVHNode(ray, 0.001,MAX_flt,record);
  
  int res = bingoFuncArray[bingo](ray, record ,depth);
  return 0;
  
}

//光线追踪 也需要递归实现
vec3 rayTracing(in Ray ray){
  
  int i = 0;
  int depth = 0;
  
  for(i = 0; i< MAX_SCATTER ; ++i){
    int ok = rayTracingOneHit(ray, depth);
    ++depth;
    if(ok ==0) {
      ++i;
      break;
    }
  }
  
  //累加结果
  vec3 color = vec3(0.,0.,0);
  for(int k=i-1;k>=0;--k){
    //rayTraceInfoArray[k];
    color = rayTraceInfoArray[k].emitted + color * rayTraceInfoArray[k].attenuation;
  }
  color = color / float(i);
  return color;
  
}

void main()
{

  uint id = gl_GlobalInvocationID.x;
  uint W = gl_NumWorkGroups.x;
  uint H = gl_NumWorkGroups.y;
  uint N = gl_WorkGroupSize.x;

  uint index = gl_GlobalInvocationID.y * W * N + gl_GlobalInvocationID.x;
  Ray ray = rays[index];
  vec3 color = rayTracing(ray);
  
  //输出数组 结构与输入不同，为了保证在同一个像素的数据连续存放在一起，我们修改索引
  uint outIndex = (gl_WorkGroupID.y*W + gl_WorkGroupID.x)*(N*N) + gl_LocalInvocationID.y* N+gl_LocalInvocationID.x;

  colors[outIndex] = color;
}

//vec4 pixel = vec4(vec2(gl_WorkGroupID.xy)/gl_WorkGroupSize.x,0,1);
//ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
//imageStore(img_output,ivec2(gl_GlobalInvocationID.xy),
//vec4(vec2(gl_LocalInvocationID.xy)/vec2(gl_WorkGroupSize.xy),0.0, 0.0));
