#ifndef __PAWN_H
#define __PAWN_H

#include "Entity/GameObject.h"
#include "Common/Events.h"
#include "Component/SkeletalMeshComponent.h"
#include "Component/CameraComponent.h"

namespace Sun
{
    SCLASS()
    class Pawn : public GameObject
    {
    public:
        Pawn();
        Pawn(float dz);
        ~Pawn();


        void moveRight(float dis, bool updateDescendants);
        void moveForward(float dis, bool updateDescendants);
        void moveUp(float dis, bool updateDescendants);
          
        SharedPtr<SCameraComponent> getCameraCom();
        float getYaw() const;
        float getPitch() const;

        Matrix4x4 getViewMatrix() const;
        Transform3D getViewTransform() const;
        Matrix4x4 getProjectionMatrix() const;
        vec3 getViewPos() const;
    protected:
        std::shared_ptr<SCameraComponent> cameraCom_;

        void init();
        //标记pawn的模型矩阵为单位矩阵时，其正面指向的方向向量
        //将据此作为移动时的转向依据
        vec3 forwardDirection_;

        float _mouse_x = 0;
        float _mouse_y = 0;
        float _pressed = 0;

    };
}


#endif 

