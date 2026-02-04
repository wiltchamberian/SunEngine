/*****************************************************************************
* @brief : BoxPrimitive
* @author : acedtang
* @date : 2021/5/9
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __BOX_PRIMITIVE_H
#define __BOX_PRIMITIVE_H

#include "Math/Vector3D.h"
#include "Math/Ray.h"
#include "Math/Box.h"
#include "PrimitiveDefine.h"
//#include "Physics/HitRecord.h"

namespace Sun {
    
    struct BoxPrimitive : public AABB
    {
        static int type_;
    };

}

#endif