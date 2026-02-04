#include "SkeletalMesh.h"

namespace Sun {

    void SkeletalMesh::addMeshSection(const SkeletalMeshSection& section) {
        meshSections_.push_back(section);
    }
}