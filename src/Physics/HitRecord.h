#pragma once

#include "Math/Vector3D.h"

namespace Sun {

    class Material;
    struct HitRecord 
    {
        //标记光线碰撞点在光路上的尺度
        float frac = 1.0;
        //光线碰撞点(世界坐标)
        vec3 p;
        //碰撞位置的法线
        vec3 normal;
        
        //碰撞物体的材质
        //Material* material;
        int materailId;

        //碰撞物体的指针
        void* target = nullptr;

        bool isHit = false;
        //碰撞物体的id
        int id;
        //标记碰撞点对应物体的点的u,v坐标
        float u = 0.f;
        float v = 0.f;
    };

}

