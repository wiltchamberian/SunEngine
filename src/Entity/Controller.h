/*****************************************************************************
* @brief : controller
* 控制器基类
* @author : acedtang
* @date : 2021/3/25
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __CONTROLLER_H
#define __CONTROLLER_H

#include "GameObject.h"
#include "Entity/Pawn.h"

namespace Sun {
    class AController : public GameObject
    {
    public:

    protected:
        //该控制器控制的pawn
        sp<Pawn> pawn_;
    };
}

#endif