/*****************************************************************************
* @brief : ¹Ç÷ÀÍø¸ñ²å²Û
* @author : acedtang
* @date : 2021/3/24
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SKELETAL_MESH_SOCKET_H
#define __SKELETAL_MESH_SOCKET_H

#include<string>
#include "Math/Transform3D.h"

namespace Sun {
    class SSkeletalMeshSocket {
    public:
        std::string name_;
        std::string sockeName_;       
        Transform3D transform_;
    };
}

#endif