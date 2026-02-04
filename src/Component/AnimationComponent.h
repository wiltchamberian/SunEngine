/*****************************************************************************
* @brief : animation
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __ANIMATION_COMPONENT_H
#define __ANIMATION_COMPONENT_H

#include "Component/ActorComponent.h"
#include "Assets/Animation.h"
#include "Assets/Handle.h"

namespace Sun {
    class SAnimationComponent : public SActorComponent {
    public:
        THandle<Animation> animhandle_;
    };
}

#endif