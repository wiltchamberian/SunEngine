/*****************************************************************************
* @brief : meshSection
* 纹理片段，对应gltf2.0中的primitive和UE4中MeshSection
* 表示一个网格中具有相同材质的子集
* @author : acedtang
* @date : 2021/4/5
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __MESH_SECTION_H
#define __MESH_SECTION_H

#include "Primitive/SkeletalMeshPrimitive.h"
#include "Component/MaterialCom.h"
#include "Assets/Handle.h"
#include "Common/Define.h"

namespace Sun {

    class SkeletalMeshSection
    {
    public:
        SkeletalMeshSection();

        SkeletalMeshSection(SkeletalMeshData* rawMesh, const SharedPtr<MaterialCom>& materialHandle);
        
        //RawMesh* rawMesh_;
        SkeletalMeshData* rawMesh_;

        std::weak_ptr<MaterialCom> materialHanlde_;
    }; 


}


#endif