/*****************************************************************************
* @brief : lodmodel
* 细节层次（Levels of Detail，简称LOD）模型
* @author : acedtang
* @date : 2021/4/5
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __LOD_MODEL_H
#define __LOD_MODEL_H

#include <vector>
#include "SkeletalMeshSection.h"

namespace Sun {

    class LodModel {
    public:
        std::vector<SkeletalMeshSection> meshSections_;
    private:
    };


}

#endif