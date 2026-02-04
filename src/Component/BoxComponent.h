/*****************************************************************************
* @brief : box
* 正方体
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __BOX_COMPONENT_H
#define __BOX_COMPONENT_H

#include "Math/Vector3D.h"
#include "Primitive/BoxPrimitive.h"
#include "Primitive/PrimitiveMemoryManager.h"
#include "Physics/HitRecord.h"

namespace Sun {
    class SBoxComponent {
    public:
        //box的半长，半宽，半高(x,y,z)
        //vec3 _boxExtent;

        virtual bool hit(const Ray& ray, float t0, float t1,
            HitRecord& rec) const;
        virtual bool boundingBox(float t0, float t1, AABB& box)const
        {
            BoxPrimitive* bp = g_primitiveMgr.deReference(handle_);
            if (bp) {
                box = AABB(bp->mins_, bp->maxs_);
                return true;
            }    
            return false;
        }

        PHandle<BoxPrimitive> handle_;
    };
}

#endif