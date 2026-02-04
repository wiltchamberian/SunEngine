/*****************************************************************************
* @brief : StaticMeshSection
* @author : acedtang
* @date : 2021/4/16
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __STATIC_MESH_SECTION_H
#define __STATIC_MESH_SECTION_H

#include "Primitive/StaticMeshPrimitive.h"
#include "Component/MaterialCom.h"
#include "Assets/Handle.h"

namespace Sun {

    class StaticMeshSection {
    public:
        //后续改为句柄 FIXME
        //StaticMeshData* staticMeshData_;
        StaticMeshData staticMeshData_;
        EPrimitiveType triangleType_ = EPrimitiveType::TRIANGLES;

        //THandle<Material> materialHandle_;
        std::weak_ptr<MaterialCom> materialHandle_;
    };
}

#endif