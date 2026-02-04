#version 430 compatibility
//#extension GL_ARB_shader_storage_buffer_object : enable;

//为了避免if else,同时实现c++的多态，这里利用subroutine来实现
//初步实现机制是成员存储一个函数索引，不同函数实现放在一个数组中

#define MAX_flt 3.402823466e+38F

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
};

struct BVHNode{
  int left; //叶子节点为-1
  int right; //叶子节点为-1
  vec3 min;  //aabb
  vec3 max;
  int hitFunc; 
  int entity; //如果>=0，说明是叶子节点,指定entity索引
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
    int material;
    //标记碰撞点对应物体的点的u,v坐标
    float u;
    float v;
};

//bvh结构(加速结构)
layout(std430, binding =4) buffer BVHNodeBuf{
  BVHNode bvhNodes[];  //nodes[0]为根
};

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
subroutine int Scatter(Ray ray, HitRecord rec, vec3 attenuation, out Ray scattered);
//散射函数选择器
subroutine uniform Scatter scatterFuncArray[10];

//辐射函数原型
//subroutine vec3 Emitted(float u, float v, vec3 p);
//辐射函数选择器
//subroutine uniform Emitted emittedFuncArray[10];



//为去除if else 添加的一些 赋值 子程
subroutine void Assign(in HitRecord r_in,out HitRecord r_out);
subroutine uniform Assign assignFuncArray[3]; //0 :unreal 1:unreal 2:real

subroutine void HitBingo(in HitRecord r_in, out HitRecord r_out);
subroutine uniform HitBingo hitbingoFuncArray[2]; //0:unreal 1:real

//////////////////////////////函数实现////////////////////////////////

//判断球体与光线是否碰撞
subroutine(Hit)
int hitRay_Sphere(int id, Ray ray, float t_min, float t_max, inout HitRecord rec){
  return 1;
}

subroutine(Hit)
int hitRay_BVHNode(int bvhNodeId, Ray ray, float t_min, float t_max, inout HitRecord rec){

  return 1;
}

//Lambertian材质散射
subroutine(Scatter)
int lambertian_scatter(Ray ray, HitRecord rec, vec3 attenuation, out Ray scattered){
  return 1;
}

//金属材质散射
subroutine(Scatter)
int metal_scatter(Ray ray, HitRecord rec, vec3 attenuation, out Ray scattered){
  return 1;
}

//电介质(玻璃)材质散射
subroutine(Scatter)
int dielectrics_scatter(Ray ray, HitRecord rec, vec3 attenuation, out Ray scattered){
  return 1;
}

//光源材质散射
subroutine(Scatter)
int light_scatter(Ray ray, HitRecord rec, vec3 attenuation, out Ray scattered){
  return 1;
}


subroutine(Assign)
void assign_real(in HitRecord r_in, out HitRecord r_out){
  r_out = r_in;
}
subroutine(Assign)
void assign_unreal(in HitRecord r_in, out HitRecord r_out){
  ;
}

subroutine(HitBingo)
void hitBingo_real(in HitRecord r_in, out HitRecord r_out){
  int k = int(sign(r_out.t-r_in.t)+1);
  assignFuncArray[k](r_in,r_out);
  bvhTop++;
  bvhStack[bvhTop%stackLen] = bvhNode.right;
  bvhTop++;
  bvhStack[bvhTop%stackLen] = bvhNode.left;
}

subroutine(HitBingo)
void hitBingo_unreal(in HitRecord r_in, out HitRecord r_out){
}

////////////////////////函数实现--end/////////////////////

///////////////////////////////布局信息//////////////////////////////////////
//生成的光线(输出) （按照从左下角先向右，再逐行向上的方式排列)
layout(std430, binding = 1) buffer RayBuf{
   Ray rays[];
};
uniform Ray curRay; //取出当前处理的射线

layout (local_size_x = 1, local_size_y =1) in;
//layout(rgba32f, binding = 0) uniform image2D img_output;

layout(std430, binding = 0) buffer GameObjBuf{
  Entity entities[];
};

layout(std430, binding =1) buffer SphereObjBuf{
  SphereObj sphereObj[];
};

layout(std430, binding = 2) buffer VerticeBuf{
    Vertice vertices[];
};

layout(std430, binding = 3) buffer IndiceBuf{
  int indices[];
};

///////////////////////////////布局信息-end//////////////////////////////////


//判断bvhnode和光线是否碰撞
int hitRay_BVHNode(Ray ray, float t_min, float t_max, inout HitRecord rec){
    HitRecord rec;
    HitRecord tmpRec;
    int bingo = 0;
    while(bvhTop >= 0 ){
      BVHNode bvhNode = bvhNodes[bvhTop%stackLen];
      bvhTop--;
      Hit hitFunc = hitFuncArray[bvhNode.hitFunc];
      int isHit = hitFunc(ray, 0.001, MAX_flt,tmpRec);
      bingo = bingo | isHit;
      //根据isHit与否，选择将孩子节点放入或者 不放入队列
      hitbingoFuncArray[isHit];
    }
    return bingo;
}

//光线追踪
vec4 rayTracing(in Ray ray, int ){
  HitRecord record;
  
  int bingo = hitRay_BVHNode(ray, 0.001,MAX_flt,record);
  if(bingo){
        //散射（反射或折射 )射线
        Ray scattered;
        //表示颜色（同时也就是衰减的意思，光线原始rgb乘以该系数 体现出颜色吸收,也就是衰减
        vec3 attenuation;
        //辐射体自发光
        //vec3 emitted = rec.material->emitted(rec.u, rec.v, rec.p);
        vec3 emitted = 

        //根据碰撞信息 和 入射法线，计算散射  射线
        //depth是最大 散射次数
        if (depth < 50 && rec.material->scatter(ray, rec, attenuation, scattered)) {
            //散射之后，继续碰撞解算
            return emitted + attenuation * rayTracing(scattered, world, depth + 1);
        }
        else {
            return emitted;
        }
  }
  

  return vec4(0,0,0,1);
}

void main()
{
  //vec4 pixel = vec4(vec2(gl_WorkGroupID.xy)/gl_WorkGroupSize.x,0,1);
  //ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
  //imageStore(img_output,ivec2(gl_GlobalInvocationID.xy),
  //vec4(vec2(gl_LocalInvocationID.xy)/vec2(gl_WorkGroupSize.xy),0.0, 0.0));

  uint id = gl_GlobalInvocationID.x;
  
  
}
