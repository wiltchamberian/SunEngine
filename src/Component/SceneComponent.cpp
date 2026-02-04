#include "SceneComponent.h"
#include <queue>

namespace Sun {

    void SSceneComponent::attachTo(SSceneComponent* parent)
    {
        if (!parent) return;
        this->attachParent_ = parent;
        parent->attachChildren_.insert(this);

        this->componentToWorld_ = parent->componentToWorld_ * this->relativeTransform_;
        updateDescendantsTransform();
    }

    void SSceneComponent::detach()
    {
        if (attachParent_) {
            attachParent_->attachChildren_.erase(this);
            this->attachParent_ = nullptr;
        }
    }

    void SSceneComponent::moveRight(float dis)
    {
        relativeTransform_.translate(vec3(dis, 0, 0));
    }

    void SSceneComponent::moveForward(float dis)
    {
        relativeTransform_.translate(vec3(0, 0, -dis));
    }

    void SSceneComponent::moveUp(float dis)
    {
        relativeTransform_.translate(vec3(0, dis, 0));
    }

    void SSceneComponent::move(float x, float y, float z)
    {
        relativeTransform_.translate(vec3(x, y, z));
    }

    void SSceneComponent::moveTo(float x, float y, float z)
    {
        relativeTransform_.setTranslation(vec3(x, y, z));
    }

    void SSceneComponent::rotate(const vec3& axis, float radians)
    {
        relativeTransform_.rotate(axis, radians);
    }

    void SSceneComponent::scale(float scale_x, float scale_y, float scale_z)
    {
        relativeTransform_.scale(vec3(scale_x, scale_y, scale_z));
    }

    Matrix4x4 SSceneComponent::componentToWorldMatrix()
    {
        return relativeTransform_.toMatrix();
    }

    const Transform3D& SSceneComponent::componentToWolrdTransform() const
    {
        return componentToWorld_;
    }

    void SSceneComponent::setRelativeTransformation(const Matrix4x4& matrix)
    {
        relativeTransform_.setMatrix(matrix);
    }

    void SSceneComponent::updateDescendantsTransform()
    {
        //广度优先遍历
        std::queue<SSceneComponent*> que;
        for (auto& it : attachChildren_) {
            que.push(it);
        }
        while (!que.empty()) {
            SSceneComponent* top = que.front();
            top->componentToWorld_ = top->attachParent_->componentToWorld_ * top->relativeTransform_;
            que.pop();
            for (auto it = top->attachChildren_.begin(); it != top->attachChildren_.end(); ++it) {
                que.push(*it);
            }
        }
    }

}