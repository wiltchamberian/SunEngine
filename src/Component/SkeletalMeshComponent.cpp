#include "SkeletalMeshComponent.h"

//被迫加上对渲染层的依赖
#include "Render/ShaderManager.h"
#include "Util/Singleton.h"
#include "Assets/AssetsMgr.h"
#include "Common/Common.h"
#include "Render/GraphicsLib.h"
#include "Engine.h"

namespace Sun {

    SkeletalMeshComponent::~SkeletalMeshComponent() {
        releaseGpu();
    }

    void SkeletalMeshComponent::allocGpu(EDrawMode drawType) {
        g_graphicsLib->applyGpuVertexData(gpuVertexDataHandle_, SkeletalMeshVertex::getLayout(), vertices_.data(), vertices_.size() , sizeof(SkeletalMeshVertex),
            indices_.data(), indices_.size() , sizeof(int), drawType);
    }

    void SkeletalMeshComponent::releaseGpu() {
        g_graphicsLib->deleteVertexArray(gpuVertexDataHandle_.vao);
    }

    void SkeletalMeshComponent::bindUniformData(const ShaderProgram& pro) {
        //计算jointMat
        std::shared_ptr<Skeleton2> ptr = skeletonHandle_.lock();
        if (ptr) {
            std::vector<Matrix4x4> mats = ptr->calculateJointTransform();
            if (!mats.empty()) {
                g_graphicsLib->setMat4(pro.getId(), "u_gBones", mats[0].data(), mats.size());
            }
        }
    }

    void SkeletalMeshComponent::unBindUniformData(const ShaderProgram& pro) {

    }

    void SkeletalMeshComponent::setSkeleton(const std::shared_ptr<Skeleton2>& handle) {
        skeletonHandle_ = handle;
    }
#if 0
    void SkeletalMeshComponent::addMesh(const std::shared_ptr<SkeletalMesh>& mesh) {
        mesh_ = mesh;
    }
#endif

    void SkeletalMeshComponent::addAnimation(const std::shared_ptr<Animation>& anim) {
        animationHandle_ = anim;
    }

    void SkeletalMeshComponent::setBeginPlayAnim(bool begin) {
        isPlayingAnim_ = begin;
    }

#if 0
    bool SkeletalMeshComponent::playAnim(float duration) {
        if (animationHandle_.expired()) return false;

        if (skeletonHandle_.expired()) return false;
        
        std::shared_ptr<Animation> anim = animationHandle_.lock();
        std::shared_ptr<Skeleton> skeleton = skeletonHandle_.lock();
        bool allEnd = true;
        for (NodeAnim& nodeAnim : anim->channels) {
            auto it = skeleton->jointMap_.find(nodeAnim.oriNodeIndex_);
            if (it == skeleton->jointMap_.end()) {
                ASSERT(false);
                continue;
            }
            int nodeIndex = it->second;
            Joint& joint = skeleton->jointTree_[nodeIndex];

            //利用duration插值
            double endTime = nodeAnim.endTime();
            if (duration <= endTime) {
                allEnd = false;
            }
            auto tr = nodeAnim.interpolate(duration);
            joint.animTransform_ = Transform3D(std::get<0>(tr).value_, std::get<1>(tr).value_, std::get<2>(tr).value_);

        }
        return allEnd;
    }
#endif

}