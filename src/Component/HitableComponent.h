/*****************************************************************************
* @brief : HitableComponent
* 光线碰撞组件接口，主要处理射线（光线）与物体碰撞
* 子组件继承该组件实现虚函数以支持光线碰撞
* for ray tracing hit
* @author : acedtang
* @date : 2021/5/9
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __HITABLE_COMPONENT_H
#define __HITABLE_COMPONENT_H

#include "Math/Ray.h"
#include "Math/Box.h"
#include "Physics/HitRecord.h"

namespace Sun {

    class Material;

    //碰撞基类
    class HitableComponent
    {
    public:
        //判断是否碰撞，如果碰撞，碰撞信息输出在HitRecord
        virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const = 0;
        //计算boundingBox
        virtual bool boundingBox(float t0, float t1, AABB& box) const = 0;
        Material* material_;
    };

}

#endif 