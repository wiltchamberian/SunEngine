/*****************************************************************************
* @brief : Factory
* 模板实现工厂,主要用于实例化actor和rotor,实际只是单例内存池
* @author : acedtang
* @date : 2021/4/6
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __FACTORY_H
#define __FACTORY_H

#include "Singleton.h"
#include "QuickMemoryPool.h"
#include "GeneralMemoryPool.h"

namespace Sun {
    template<typename _T>
    using ActorFactory = Singleton<QuickMemoryPool<_T>>;

    template<typename _T>
    using RotorFactory = Singleton<QuickMemoryPool<_T>>;
    
    using GeneralFactory = Singleton<GeneralMemoryPool>;

    extern GeneralMemoryPool* getGlobalComponentMememoryPool();
}

#endif