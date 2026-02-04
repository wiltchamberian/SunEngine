/*****************************************************************************
* @brief : AnimationSystem
* @author : acedtang
* @date : 2021/4/20
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __ANIMATION_SYSTEM_H
#define __ANIMATION_SYSTEM_H

#include "Component/ActorComponent.h"
#include "Assets/Animation.h"
#include "Assets/Handle.h"
#include "Util/Factory.h"
#include "System/SystemBase.h"
#include "Entity/GameObject.h"

namespace Sun {

    class AnimationSystem :public SystemBase 
    {
    public:
        void prepare() override;
        void tick(float dtime, const SEvent& ev) override;
        //播放动画,返回值标记是否播放结束
        bool play(float duration);
        void setBeginPlayAnim(bool begin);
        void setHandle(const std::shared_ptr<Animation>& handle);
        GameObject* actor_;
    protected:
        //备份所有rotor的矩阵
        void backupTransforms();
        //归还rotor矩阵
        void setBackTransforms();
        //THandle<Animation> animHandle_;
        std::shared_ptr<Animation> animHandle_;
        bool isPlayingAnim_ = false;
        bool isFirstEnterAnim_ = true;
        float animDuration_ = 0;
        std::vector<Transform3D> rotorTransforms_;
        
    };

}

#endif