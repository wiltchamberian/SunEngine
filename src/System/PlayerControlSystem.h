/*****************************************************************************
* @brief : PlayerControlSystem
* 玩家控制系统，处理系统输入输出控制游戏主角运动
* @author : acedtang
* @date : 2021/4/22
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __PLAYER_CONTROL_SYSTEM_H
#define __PLAYER_CONTROL_SYSTEM_H

#include "SystemBase.h"

namespace Sun {
    class SWorld;
    class Character;
    class PlayerControlSystem : public SystemBase
    {
    public:
        PlayerControlSystem(SWorld* world);
        void prepare() override;
        void tick(float dtime, const SEvent& ev) override;
        Character* actor_;
    protected:
        SWorld* world_ = nullptr;
    };

}

#endif 