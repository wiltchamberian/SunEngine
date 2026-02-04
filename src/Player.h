/*****************************************************************************
* @brief : 玩家
* 对于玩家角色的抽象，
* @author : acedtang
* @date : 2021/3/20
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
#ifndef __PLAYER_H
#define __PLAYER_H

namespace Sun {
    class SPlayerController;
    class SWorld;

    class SPlayer
    {
    public:
        void setController(SPlayerController* controller);
        SPlayerController* getController(SWorld* world);
    };

}

#endif

