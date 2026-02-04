/*****************************************************************************
* @brief : skeletalMesh
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __SKELETAL_MESH_COMPONENT_H
#define __SKELETAL_MESH_COMPONENT_H

#include "Component/MeshComponent.h"
#include "Common/Events.h"
//#include "Assets/RawMesh.h"
#include "Assets/Skeleton.h"
#include "Assets/Animation.h"
#include "Assets/LODModel.h"
#include "Assets/Handle.h"
#include "Assets/SkeletalMesh.h"
#include "Render/RenderHandle.h"
#include "Render/RenderCommand.h"
#include "Common/Common.h"
#include "Primitive/StaticMeshPrimitive.h"

namespace Sun {
    class SkeletalMeshComponent:public MeshInterface {
        friend class SRenderHelper;
        friend class SkeletalRenderPass;
    public:
        ~SkeletalMeshComponent() override;
        void allocGpu(EDrawMode drawType) override;
        void releaseGpu() override;
        void bindUniformData(const ShaderProgram& pro) override;
        void unBindUniformData(const ShaderProgram& pro) override;

        void setSkeleton(const std::shared_ptr<Skeleton2>& skeleton);
        //void addMesh(const std::shared_ptr<SkeletalMesh>& mesh);
        void addAnimation(const std::shared_ptr<Animation>& anim);

        void setBeginPlayAnim(bool begin);
        //bool playAnim(float duration);

        //std::weak_ptr<SkeletalMesh> mesh_;
        std::vector<SkeletalMeshVertex> vertices_;
        std::vector<int> indices_;
        std::weak_ptr<Skeleton2> skeletonHandle_;
        std::weak_ptr<Animation> animationHandle_;
    protected:
        bool isPlayingAnim_ = false;
        bool isFirstEnterAnim_ = true;
        float animDuration_ = 0;
    };
}

#endif