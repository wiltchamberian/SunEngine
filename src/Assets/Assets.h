/*****************************************************************************
* @brief : Assets
* @author : acedtang
* @date : 2021/4/2
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __ASSETS_H
#define __ASSETS_H

#include <string>
#include <iostream>
//#include "stb_image.h"
#include <vector>
#include "Assets/StaticMesh.h"
#include "Math/Matrix4x4.h"

namespace Sun {

    class Material;
    class Skeleton;
    class Animation;
    class SkeletalMesh;
    class StaticMesh;

    //the name is not well,FIXME
    //这是一次性导入的相关资产的描述信息
    struct ImportAsset {
        //材质
        std::vector<Material*> materials;
        //物理资产TODO

        //骨骼（作为单独资产解析出来)
        std::vector<Skeleton*> skeletons;

        //动画序列
        std::vector<Animation*> animations;

        //骨骼网格
        std::vector<SkeletalMesh*> skeletalMeshes;
        //骨骼网格对应的model transform
        std::vector<Transform3D> skModelTransform;

        //静态网格
        std::vector<StaticMesh*> staticMeshes;
        //静态网格对应的model transform
        std::vector<Transform3D> stModelTransform;

        //纹理
        std::vector<Texture> textures;
    };

}


#endif 