/*****************************************************************************
* @brief : gltfloader2
* 直接将gltf格式的node映射到内部结构的rotor
* 根节点映射到actor
* 内部数据映射为组件
* 从而与引擎结构更加吻合
* 之前为了和ue4的结构保持对应带来一定不必要的复杂性，现
* 尝试直接用gltf匹配内部格式
* @author : acedtang
* @date : 2021/4/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __GLTF_LOADER_2_H
#define __GLTF_LOADER_2_H

#include <string>
#include <iostream>
//#include "stb_image.h"
#include <vector>

#include "GltfHead.h"
#include "thirdparty/json/json.hpp"
using json = nlohmann::json;
#include "Assets/Assets.h"
#include "Assets/SkeletalMeshSection.h"
#include "Component/SkeletalMeshComponent.h"
#include "Assets/AssetsMgr.h"
#include "Primitive/Image.h"

namespace Sun {

    class SystemBase;

    class GltfLoader2 {
    public:
        void loadScene(const std::string& path);
        std::vector<GameObject*> exportActors();
        std::vector<SystemBase*> exportSystems();
    protected:
        void travelNode(GameObject* rotor, int nodeIndex, Transform3D&);
        bool processElements(GameObject* rotor, int nodeIndex, Transform3D& );
        GameObject* processPrimitive(json& primitive, int skinIndex);

        //获取accessor的完整类型信息
        DataType getDataTypeOfAccessor(const json& accessor);
        //获取数据
        Data getDataOfAccessor(const json& accessor);

        //预处理节点
        void preProcessNodes();
        //解析纹理数组
        void processTextures();
        //解析材质数组
        void processMaterials();
        //解析动画数组
        void processAnimations();
        //预解析骨骼关节
        void preProcessSkeletons();
        //解析骨骼
        void processSkeletons();
        //更新动画rotor
        void sucProcessAnimations();

        //返回skin中所有节点的根节点索引
        int getSkinSkeleton(json& skin);
        Transform3D getNodeTransform(int nodeIndex);
        //解析出该节点的所有子节点（含该节点本身)
        std::vector<int> getAllNodes(int nodeIndex);

        json js_;
        std::string dir_;
        std::string path_;
        std::vector<std::vector<char>> bufs_;
        int numNode_ = 0;
        std::vector<GameObject*> actors_;
        std::vector<SystemBase*> systems_;
        //node oriIndex to rotor (not manager memory) used by skeleton
        std::vector<GameObject*> rotors_;

        //下标为节点索引，对应的值为该节点对应的根节点索引
        std::vector<int> rootMap;

        std::vector<std::shared_ptr<Skeleton2>> skeletons_;
        std::vector<std::shared_ptr<MaterialCom>> materials_;
        //std::vector<std::shared_ptr<Image>> textures_;
        std::vector<Texture> textures_;
    };
}


#endif