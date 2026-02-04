/*****************************************************************************
* @brief : camera
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __CAMERA_COMPONENT_H
#define __CAMERA_COMPONENT_H

#include "Math/MathConstants.h"
#include "Math/Matrix4x4.h"
#include "Component/SimpleTransformCom.h"

namespace Sun {
    enum ProjMode {
        ORT, //正射
        PER,   //透视
    };

    class SCameraComponent:public SimpleTransformCom
    {
    public:
        SCameraComponent();
        Matrix4x4 getProjectionMatrix() const;
        Matrix4x4 getLookAtMatrix(const vec3& target) const;
        Transform3D getLookAtTransform(const vec3& target) const;
        Matrix4x4 getLookAtMatrix() const;
        void setArmLen(float len);
        float getArmLen() const;
        float addYaw(float phi);
        float getYaw() const;
        float addPitch(float theta);
        float getPitch() const;
        void setRelativePosition(const vec3& vec);
        vec3 getRelativePosition() const;
        void setTarget(const vec3& target);
        vec3 getTarget() const;
        vec3 getWorldPosition() const;

        //设置投影模式 正射投影，透视投影
        void setProjectionMode(ProjMode mode);
        ProjMode getProjectionMode();
    protected:
        ProjMode mode_;

        float fovy_;
        float aspect_;
        float near_;
        float far_;

        float yaw_; //relative to z axis (-pi,pi]
        float pitch_; // [-pi/2+e, pi/2-e]
        float len_;  //>0
        const float e_ = A_PI/180; //1 degree

        vec3 target_;
    };
}

#endif