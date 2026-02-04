/*****************************************************************************
* @brief : actor组件
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
#ifndef __SACTOR_COMPONENT_H
#define __SACTOR_COMPONENT_H

#include "Meta/Object.h"
#include "Util/sp.h"
#include "Render/RenderHandle.h"
#include "Common/Common.h"
#include "Interface/Interface.h"

namespace Sun {

    class GameObject;
    class RenderAbleCom;
    class SActorComponent : public InterfaceBase
    {
    public:
        SActorComponent() : comType_(ComponentType::COM_ACTOR) {

        }
        //move to primitive component FIXME
        virtual void prepareRender(const RenderInputInfo& type);
        virtual void render(const RenderInputInfo& type);
        
        virtual void tick(float dtime,const SEvent& ev) {};

        //组件在运行时被添加到关卡中时引擎调用
        virtual void onAddedToLevel();

        void setOwn(GameObject* rotor);

        Matrix4x4 getWorldMatrix();

        //类别
        ComponentType comType_;

        //是否自绑定
        bool isSelfBind_ = false;
    public:
        GameObject* owner_ = nullptr;
        
    };
}


#endif

