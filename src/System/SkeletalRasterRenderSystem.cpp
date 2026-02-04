
#include "Assets/AssetsMgr.h"
#include "Render/ShaderManager.h"
#include "Render/GraphicsLib.h"
#include "Engine.h"

namespace Sun {

    SkeletalRasterRenderSystem::SkeletalRasterRenderSystem() {
        //passType_ = RenderPassComType::RP_SKELETAL_MESH;
        renderComType_ = ComponentType::COM_SKELETAL_MESH;
    }

    //游戏开始时调用
    void SkeletalRasterRenderSystem::prepareRender() {

    }

    //setPassCall
#if 0
    void SkeletalRasterRenderSystem::beginRender() {
        sp<SCameraRotor> camera = SUN->getWorld()->getUsingCamera();
        Matrix4x4 view = camera->worldTransform_.toMatrix().getInverse();
        Matrix4x4 proj = camera->calculateProjectionMatrix();
        for (int i = 0; i < components_.size(); ++i) {
            //SkeletalMesh* mesh = mgr.dereference(components_[i]->mesh_);
            std::shared_ptr<SkeletalMesh> mesh = components_[i]->getMeshCom();
            //Matrix4x4 mat = components_[i]->owner_->worldMatrix();
            Matrix4x4 mat;
            Matrix4x4 mvp = proj * view * mat;
            Matrix3x3 normalMat = (view * mat).toMatrix3x3().getInverse().getTranspose();
            //Skeleton2* skt = skMgr.dereference(components_[i]->skeleton2Handle_);
            std::shared_ptr<Skeleton2> skt = components_[i]->skeleton2Handle_.lock();
            if (mesh && g_renderManager) {
                for (SkeletalMeshSection& sec : mesh->meshSections_) {
                    std::vector<SkeletalMeshVertex>& vec = sec.rawMesh_->vertices_;
                    std::vector<int>& indice = sec.rawMesh_->indices_;
                    RenderHandle handle = g_renderManager->setupSkeletalMesh(vec.data(), vec.size() * sizeof(SkeletalMeshVertex), indice.data(), indice.size() * sizeof(int));
                    handles_.push_back(handle);
                    mvps_.push_back(mvp);
                    normalMats_.push_back(normalMat);
                    skeletons_.push_back(skt);
                }
            }
        }
    }
#endif 

    void SkeletalRasterRenderSystem::beginRender() {
        sp<SCameraRotor> camera = SUN->getWorld()->getUsingCamera();
        Matrix4x4 view = camera->worldTransform_.toMatrix().getInverse();
        Matrix4x4 proj = camera->calculateProjectionMatrix();
        Matrix4x4 vp = proj * view;
        for (int i = 0; i < components_.size(); ++i) {
            //SkeletalMesh* mesh = mgr.dereference(components_[i]->mesh_);
            std::shared_ptr<SkeletalMeshComponent> mesh = std::dynamic_pointer_cast<SkeletalMeshComponent>(components_[i]->getMeshCom());
            //Matrix4x4 mat = components_[i]->owner_->worldMatrix();
            Matrix4x4 mat;
            Matrix4x4 mvp = vp * mat;
            Matrix3x3 normalMat = (view * mat).toMatrix3x3().getInverse().getTranspose();
            //Skeleton2* skt = skMgr.dereference(components_[i]->skeleton2Handle_);
            std::shared_ptr<Skeleton2> skt = mesh->skeletonHandle_.lock();
           
            //std::vector<SkeletalMeshVertex>& vec = sec.rawMesh_->vertices_;
            //std::vector<int>& indice = sec.rawMesh_->indices_;
            //RenderHandle handle = g_renderManager->setupSkeletalMesh(vec.data(), vec.size() * sizeof(SkeletalMeshVertex), indice.data(), indice.size() * sizeof(int));
            components_[i]->allocGpu();

            handles_.push_back(components_[i]->getVertexDataHandle());
            mvps_.push_back(mvp);
            normalMats_.push_back(normalMat);
            skeletons_.push_back(skt);
            pros_.push_back(components_[i]->getShaderProgram());
        }
    }

    //drawCall
    void SkeletalRasterRenderSystem::render() {
        if (g_graphicsLib == nullptr) return;

        for (int i = 0; i < handles_.size(); ++i) {
            ShaderProgram pro = pros_[i];

            g_graphicsLib->useProgram(pro);
            g_graphicsLib->setMat4(pro.getId(), "mvp", mvps_[i].data(),1);
            g_graphicsLib->setMat3(pro.getId(), "normalMat", normalMats_[i].data(),1);
            //计算jointMat
            std::shared_ptr<Skeleton2> ptr = skeletons_[i].lock();
            if (ptr) {
                std::vector<Matrix4x4> mats = ptr->calculateJointTransform();
                if (!mats.empty()) {
                    g_graphicsLib->setMat4(pro.getId(), "gBones", mats[0].data(), mats.size());
                }   
            }
            
            g_graphicsLib->drawCall(handles_[i]);
            
        }
    }

    //do some clear 
    void SkeletalRasterRenderSystem::endRender() {
        components_.clear();
        handles_.clear();
        mvps_.clear();
        normalMats_.clear();
        skeletons_.clear();
        pros_.clear();
    }
}