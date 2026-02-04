#include "ActorComponent.h"
#include "Entity/GameObject.h"

namespace Sun {

#if 1
    void SActorComponent::prepareRender(const RenderInputInfo& inputInfo) {

    }

    void SActorComponent::render(const RenderInputInfo& inputInfo) {

    }

#endif

    void SActorComponent::onAddedToLevel() {
       
    }

    void SActorComponent::setOwn(GameObject* gameobj) {
        owner_ = gameobj;
    }

    Matrix4x4 SActorComponent::getWorldMatrix() {
        if (owner_) {
            return owner_->getWorldMatrix();
        }
        return Matrix4x4();
    }

}