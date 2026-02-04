/*****************************************************************************
* @brief : playerController
* 玩家控制器
* @author : acedtang
* @date : 2021/3/25
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __PLAYER_CONTROLLER_H
#define __PLAYER_CONTROLLER_H

#include "controller.h"

namespace Sun {
    class APlayerController : public AController
    {
    public:
        //处理tick事件
        void tick(double deltaTime);

        //ACamera* camera;
    };
}

#endif 

