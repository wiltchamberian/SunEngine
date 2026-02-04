#include "Pawn.h"
#include "Engine.h"
#include "Component/CameraComponent.h"
#include "Common/Common.h"
#include "Util/Factory.h"

namespace Sun
{
    Pawn::Pawn() {
        init();
    }

    Pawn::Pawn(float dz) {

    }

    Pawn::~Pawn() {

    }

    void Pawn::moveRight(float dis, bool updateDescendants) {
        if (cameraCom_) {
            vec3 relativePos = cameraCom_->getRelativePosition();
            vec3 forward = vec3(-relativePos.x, 0, -relativePos.z).getNormalized();
            vec3 right = vec3(-forward.z, 0, forward.x);

            auto transCom = getComponent<TransformCom>();
            Transform3D relativeTransform = transCom->getRelativeTransform();

            //当前位置
            vec3 curPos = relativeTransform * vec3(0, 0, 0);
            //当前的方向向量(平移对向量无影响,省略了scale的影响,FIXME)
            vec3 direction = relativeTransform.getRotation().rotate(forwardDirection_);

            //修改模型的变换阵使得forwardDirection与forward同向
            vec3 v = forwardDirection_.crossProduct(forward);
            float c = forwardDirection_.dotProduct(forward);
            float radius = acos(c);
            Quaternion qua;
            qua.setRotateAxisAndRadius(v, radius);
            Transform3D trans;
            trans.setTranslation(curPos + right * dis);
            trans.setRotation(qua);

#if 1
            transCom->setRelativeTransform(trans);
            updateWorldTransform();
            if (updateDescendants) {
                updateDescendantsTransform();
            }
#endif 
        }
    }

    void Pawn::moveForward(float dis, bool updateDescendants) {
        if (cameraCom_) {
            vec3 relativePos = cameraCom_->getRelativePosition();
            //表示当前照相机视点的正前方(忽略俯仰角)
            vec3 forward = vec3(-relativePos.x, 0, -relativePos.z).getNormalized();

            //当前位置
            auto transCom = getComponent<TransformCom>();
            vec3 curPos = transCom->getRelativeTransform() * vec3(0, 0, 0);
            //当前的方向向量(平移对向量无影响,省略了scale的影响,FIXME)
            vec3 direction = transCom->getRelativeTransform().getRotation().rotate(forwardDirection_);

            //修改模型的变换阵使得forwardDirection与forward同向
            vec3 v = forwardDirection_.crossProduct(forward);
            float c = forwardDirection_.dotProduct(forward);
            float radius = acos(c);
            Quaternion qua;
            qua.setRotateAxisAndRadius(v, radius);
            Transform3D trans;
            trans.setTranslation(curPos + forward * dis);
            trans.setRotation(qua);
            transCom->setRelativeTransform(trans);
            updateWorldTransform();
            if (updateDescendants) {
                updateDescendantsTransform();
            }
        }
    }

    void Pawn::moveUp(float dis, bool updateDescendants) {
        if (cameraCom_) {
            vec3 relativePos = cameraCom_->getRelativePosition();
            vec3 up = vec3(0.0, 1.0, 0.0);
            vec3 forward = vec3(-relativePos.x, 0, -relativePos.z).getNormalized();

            auto transCom = getComponent<TransformCom>();
            //当前位置
            vec3 curPos = transCom->getRelativeTransform() * vec3(0, 0, 0);
            //当前的方向向量(平移对向量无影响,省略了scale的影响,FIXME)
            vec3 direction = transCom->getRelativeTransform().getRotation().rotate(forwardDirection_);

            //修改模型的变换阵使得forwardDirection与forward同向
            vec3 v = forwardDirection_.crossProduct(forward);
            float c = forwardDirection_.dotProduct(forward);
            float radius = acos(c);
            Quaternion qua;
            qua.setRotateAxisAndRadius(v, radius);
            Transform3D trans;
            trans.setTranslation(curPos + up * dis);
            trans.setRotation(qua);
            transCom->setRelativeTransform(trans);
            updateWorldTransform();
            if (updateDescendants) {
                updateDescendantsTransform();
            }
        }
    }

    SharedPtr<SCameraComponent> Pawn::getCameraCom() {
        return cameraCom_;
    }

    float Pawn::getYaw() const {
        assert(cameraCom_);
        if(cameraCom_)
            return cameraCom_->getYaw();
        return 0.;
    }

    float Pawn::getPitch() const {
        assert(cameraCom_);
        if (cameraCom_)
            return cameraCom_->getPitch();
        return 0.;
    }

    Matrix4x4 Pawn::getViewMatrix() const {
        vec3 zero(0, 0, 0);
        vec3 target = zero;
        if (auto it = getComponent<TransformCom>()) {
            target = it->getWorldTransform() * zero;
        }
        if (cameraCom_) {
            return cameraCom_->getLookAtMatrix(target);
        }
        return Matrix4x4();
    }

    Transform3D Pawn::getViewTransform() const {
        vec3 zero(0, 0, 0);
        vec3 target = zero;
        if (auto it = getComponent<TransformCom>()) {
            target = it->getWorldTransform() * zero;
        }
        if (cameraCom_) {
            return cameraCom_->getLookAtTransform(target);
        }
        return Transform3D();
    }

    Matrix4x4 Pawn::getProjectionMatrix() const {
        if (cameraCom_) {
            return cameraCom_->getProjectionMatrix();
        }
        return Matrix4x4();
    }

    vec3 Pawn::getViewPos() const {
        vec3 zero(0, 0, 0);
        vec3 target;
        if (auto it = getComponent<TransformCom>()) {
            target = it->getWorldTransform() * zero;
        }
            
        if (cameraCom_) {
            return cameraCom_->getRelativePosition() + target;
        }
        return target;
    }

    void Pawn::init()
    {
        cameraCom_ = std::make_shared<SCameraComponent>();

        forwardDirection_ = vec3(0, 0, -1);
    }

 
}

