/*****************************************************************************
* @brief : scene 场景组件，具备放入场景的能力，
* 主要功能为包含transform 和 支持attach
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __SCENE_COMPONENT_H
#define __SCENE_COMPONENT_H

#include "ActorComponent.h"
#include <map>
#include <set>
#include <memory>
#include "Math/Vector3D.h"
#include "Math/Rotator.h"
#include "Math/Transform3D.h"

namespace Sun {
    class SSceneComponent:public SActorComponent {
    public:
        void attachTo(SSceneComponent* parent);
        void detach();

        //along the x
        void moveRight(float dis);
        //along the -z 
        void moveForward(float dis);
        //along the y
        void moveUp(float dis);
        void move(float x, float y, float z);
        void moveTo(float x, float y, float z);
        void rotate(const vec3& axis, float radians);
        void scale(float scale_x, float scale_y, float scale_z);
        inline Matrix4x4 componentToWorldMatrix();
        const Transform3D& componentToWolrdTransform() const;
        void setRelativeTransformation(const Matrix4x4& matrix);
    protected:
        //更新所有后代的transform信息
        void updateDescendantsTransform();

        vec3 _velocity;

        /*vec3 _relativeLocation;
        Rotator _relativeRotation;
        vec3 _relativeScale3D;*/

        //相对于父节点的变换
        Transform3D relativeTransform_;
    private:
        SSceneComponent* attachParent_;
        std::set<SSceneComponent*> attachChildren_;

        //局部空间到世界空间的变换
        Transform3D componentToWorld_;
    };
}

#endif