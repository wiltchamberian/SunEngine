#include "Skeleton.h"
#include "Util/exception.h"
#include "Entity/GameObject.h"
#include "Util/sp.h"

namespace Sun {

    JointPointer Skeleton::addJoint(JointPointer parent, const Joint& joint) {
        jointTree_.push_back(joint);
        //Joint* res = &jointTree_.back();
        JointPointer ptr(&jointTree_, jointTree_.size() - 1);
        if (parent) {
            addChild<JointPointer>(parent, ptr);
        }
        jointMap_.insert(std::make_pair(joint.oriIndex, jointTree_.size() - 1));
        return ptr;
    }

    JointPointer Skeleton::addJoint2(JointPointer parent, const Joint& joint) {
        jointTree_.push_back(joint);
        //Joint* res = &jointTree_.back();
        JointPointer ptr(&jointTree_, jointTree_.size() - 1);
        if (parent) {
            addChild<JointPointer>(parent, ptr);
        }
        jointMap2_.insert(std::make_pair(joint.name_, jointTree_.size() - 1));
        return ptr;
    }

    int Skeleton::jointIndex(JointPointer p) {
        if (jointTree_.empty()) return -1;
        int index = p.index_;
        ASSERT(index >= 0 && index < jointTree_.size());
        return index;
    }

    int Skeleton::jointIndex(const std::string& name) {
        if (jointMap2_.find(name) == jointMap2_.end()) {
            return -1;
        }
        return jointMap2_[name];
    }

    int Skeleton::mapIndex(int oriIndex) {
        auto iter = jointMap_.find(oriIndex);
        if (iter == jointMap_.end()) {
            //ASSERT(false);
            return 0;
        }
        return iter->second;
    }

    std::vector<Joint> Skeleton::exportJointTree() {
        return jointTree_;
    }

    const std::vector<Joint>& Skeleton::getJointTree() {
        return jointTree_;
    }

    void Skeleton::calculateJointTransform() {
        //广度优先遍历的方式来计算
        IndexPointer<Joint> indexPointer;
        indexPointer.container_ = &jointTree_;
        indexPointer.index_ = 0;
        travel<IndexPointer<Joint>>(indexPointer, [](IndexPointer<Joint> ptr) {
            if (ptr->parent_) {
                ptr->jointTransform_ = ptr->parent_->jointTransform_ * ptr->animTransform_;

                /*Matrix4x4 mat = ptr->parent_->jointTransform_.toMatrix() * ptr->animTransform_.toMatrix();
                mat.decomposeTransRotScale(ptr->jointTransform_.translation_, ptr->jointTransform_.rotation_,
                    ptr->jointTransform_.scale_);*/
            }
            else {
                ptr->jointTransform_ = ptr->animTransform_;
            }
            });
        travel<JointPointer>(indexPointer, [](JointPointer ptr) {
            ptr->jointTransform_ = ptr->jointTransform_ * ptr->offsetTransform_; 
            /*Matrix4x4 mat = ptr->jointTransform_.toMatrix() * ptr->offsetTransform_.toMatrix();
            mat.decomposeTransRotScale(ptr->jointTransform_.translation_, ptr->jointTransform_.rotation_,
                ptr->jointTransform_.scale_);*/
        });
    }

    //////////////////////////////skeleton2////////////////////////////////////
    std::vector<Matrix4x4> Skeleton2::calculateJointTransform() {
        std::vector<Matrix4x4> res;
        ASSERT(joints_.size() == invBindMatrixes_.size());
        int l = std::min<int>(joints_.size(), invBindMatrixes_.size());
        for (int i = 0; i < l;++i) {
            Matrix4x4 worldMat = joints_[i]->getWorldMatrix();
            Matrix4x4 mat = worldMat * invBindMatrixes_[i];

#if 0
            Transform3D tr;
            invBindMatrixes_[i].decomposeTransRotScale(tr.translation_, tr.rotation_, tr.scale_);
            Transform3D an = joints_[i]->worldTransform_ * tr;
            Matrix4x4 mat2 = an.toMatrix();
#endif

            res.push_back(mat);
        }
        return res;
    }
}