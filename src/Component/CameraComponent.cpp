#include "CameraComponent.h"

namespace Sun {

    SCameraComponent::SCameraComponent()
        :yaw_(0)
        , pitch_(0)
        ,len_(1)
    {
        fovy_ = 60;
        aspect_ = 4. / 3;
        near_ = 0.01;
        far_ = 500.;
    }

    Matrix4x4 SCameraComponent::getProjectionMatrix() const {
        if(mode_ == ProjMode::ORT){
            return Matrix4x4::getOrtho(-1, 1, -1, 1, near_, far_);
        }
        return Matrix4x4::getPerspective(fovy_, aspect_, near_, far_);
    }

    Matrix4x4 SCameraComponent::getLookAtMatrix(const vec3& target) const{
        Matrix4x4 mat;
        float z = len_ * cos(pitch_) * cos(yaw_);
        float x = len_ * cos(pitch_) * sin(yaw_);
        float y = len_ * sin(pitch_);
        vec3 cameraPos = target +vec3(x, y, z);
        vec3 worldUp = vec3(0, 1, 0);
        mat = Matrix4x4::getLookAt(cameraPos, target, worldUp);

        return mat;
    }

    Transform3D SCameraComponent::getLookAtTransform(const vec3& target) const {
        Transform3D res;
        float z = len_ * cos(pitch_) * cos(yaw_);
        float x = len_ * cos(pitch_) * sin(yaw_);
        float y = len_ * sin(pitch_);
        vec3 cameraPos = target + vec3(x, y, z);
        vec3 worldUp = vec3(0, 1, 0);
        res = Transform3D::getLookAt(cameraPos, target, worldUp);
        return res;
    }

    Matrix4x4 SCameraComponent::getLookAtMatrix() const {
        return getLookAtMatrix(target_);
    }

    void SCameraComponent::setArmLen(float len) {
        len_ = len;
    }

    float SCameraComponent::getArmLen() const{
        return len_;
    }

    float SCameraComponent::addYaw(float phi) {
        float sum = yaw_ + phi;
        float mod = fmodf(sum, A_TWICE_PI);
        if (mod > A_PI)mod -= A_TWICE_PI;
        yaw_ = mod;
        return yaw_;
    }

    float SCameraComponent::getYaw() const {
        return yaw_;
    }

    float SCameraComponent::addPitch(float theta) {
        float sum = pitch_ + theta;
        if (sum > A_HALF_PI - e_) {
            sum = A_HALF_PI - e_;
        }
        if (sum < -A_HALF_PI + e_) {
            sum = -A_HALF_PI + e_;
        }
        pitch_ = sum;
        return sum;
    }

    float SCameraComponent::getPitch() const {
        return pitch_;
    }

    void SCameraComponent::setRelativePosition(const vec3& vec) {
        len_ = vec.getLength();
        yaw_ = atan2(vec.x, vec.z);
        pitch_ = atan2(vec.y, sqrt(vec.x * vec.x + vec.z * vec.z));
    }

    vec3 SCameraComponent::getRelativePosition() const {
        vec3 res;
        res.z = len_ * cos(pitch_) * cos(yaw_);
        res.x = len_ * cos(pitch_) * sin(yaw_);
        res.y = len_ * sin(pitch_);
        return res;
    }

    void SCameraComponent::setTarget(const vec3& target) {
        target_ = target;
    }

    vec3 SCameraComponent::getTarget() const {
        return target_;
    }

    vec3 SCameraComponent::getWorldPosition() const {
        return getTarget() + getRelativePosition();
    }

    void SCameraComponent::setProjectionMode(ProjMode mode) {
        mode_ = mode;
    }

    ProjMode SCameraComponent::getProjectionMode() {
        return mode_;
    }
}