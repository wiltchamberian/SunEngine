#include "SkeletalMeshSection.h"

namespace Sun {

    SkeletalMeshSection::SkeletalMeshSection() {

    }

    SkeletalMeshSection::SkeletalMeshSection(SkeletalMeshData* rawMesh, const SharedPtr<MaterialCom>& materialHandle)
    {
        rawMesh_ = rawMesh;
        materialHanlde_ = materialHandle;
    }

}