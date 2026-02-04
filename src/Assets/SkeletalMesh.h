/*****************************************************************************
* @brief : »ù´¡Íø¸ñ
* @author : acedtang
* @date : 2021/4/14
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __MESH_H
#define __MESH_H

#include <vector>
#include "SkeletalMeshSection.h"

namespace Sun {

    class SkeletalMesh {
        friend class SStaticMeshComponent;
        friend class SSkeletalMeshComponent;
        friend class SkeletalMeshProxy;
    public:
        friend class SRenderHelper;

        void addMeshSection(const SkeletalMeshSection& section);
    public://FIXME
        std::vector<SkeletalMeshSection> meshSections_;
        Guid guid_;
    };

}

#endif