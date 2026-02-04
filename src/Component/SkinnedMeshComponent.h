/*****************************************************************************
* @brief : skinned
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __SKINNED_MESH_COMPONENT_H
#define __SKINNED_MESH_COMPONENT_H

#include "Assets/Skeleton.h"

namespace Sun {
    class SSkinnedMeshComponent {
    public:
        Skeleton* skeleton;
    };
}

#endif