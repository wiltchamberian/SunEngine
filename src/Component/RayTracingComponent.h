#ifndef __RAY_TRACING_COMPONENT_H
#define __RAY_TRACING_COMPONENT_H

#include "Render/VertexArray.h"
#include "Math/Vector2D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Component/MeshComponent.h"
#include "Render/GraphicsLib.h"

namespace Sun {

    //虚拟相机，位于(0,0,-1)，像素阵列中心位于(0,0,0)
    struct RCameraOld {
        vec3 origin;
        vec3 direction; //长度为到阵列的距离
        vec3 up;
        float halfWidth;
        float halfHeight;
        RCameraOld() {
            origin = { 0,0,-1 };
            direction = { 0,0,1 };
            up = { 0,1,0 };
            halfHeight = 1;
            halfWidth = 1024. / 768.;
        }
    };

    struct RCamera {
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
    };

    //和glsl对应
    struct Entity {
        int offset; //实体信息数组偏移量
        int len;    //实体信息结构体实际长度
        int type; //实体类型，目前支持球体 0 三角形网格1 ;当前先只处理球体
        int hitFuncIndex; //该实体的hit函数索引
        int material; //材质索引
        vec3 color; //基础颜色，原本应该位于材质，先简化着用一下
    };

    struct RBVHNode {
        int left; //叶子节点为-1
        int right; //叶子节点为-1
        vec3 mins;  //aabb
        vec3 maxs;
        int hitFunc;
        int entity; //如果是叶子节点，指定entity索引，否则是自身索引
    };

    struct RSphere {
       
        /*Vector3D center_;
        float pad0_;
        Vector3D color_;
        float pad1_;
        float radius_;
        vec3 padding_;*/
        vec3 center_;
        vec3 color_;
        float    radius_;
    };

    struct RRay {
        vec3 origin;
        vec3 direction;
    };

    class RayTracingComponent : public MeshComponent {
    public:
        RayTracingComponent() {}
        void prepareRender() override;
        void render() override;

        unsigned int hRayBuf_ = -1;
        const int W_ = 1024;
        const int H_ = 768;
        const int N_ = 6; //N*N sample in one pixel
        unsigned int hImgOutput_ = -1;
        //顶点vao,vbo,ebo
        RenderHandle dh_;
    protected:
        void _render();
        void prepareGenRay();
        void prepareRayTracing();
        void prepareRayShow();
    };
}

#endif