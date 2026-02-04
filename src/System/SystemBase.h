/*****************************************************************************
* @brief : SystemBase
* @author : acedtang
* @date : 2021/4/20
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SYSTEM_BASE_H
#define __SYSTEM_BASE_H

#include <vector>
#include "Common/Events.h"
#include "Util/Factory.h"

namespace Sun {

    class SystemBase {
    public:
        //游戏循环开始前调用一次
        virtual void prepare() {}
        //每帧调用
        virtual void tick(float dtime, const SEvent& ev) = 0;
        
    };

}

#endif