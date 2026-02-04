/*****************************************************************************
* @brief : ObjectFactory
* @author : acedtang
* @date : 2021/5/14
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __OBJECT_FACTORY_H
#define __OBJECT_FACTORY_H

#include <malloc.h>
#include <utility>

namespace Sun {

    class ObjectFactory {
    public:
        template<class _T, class ..._Args>
        _T* createObj(_Args&&... args) {
#if 0
            void* p = malloc(sizeof(_T));
            memset(p, 0, sizeof(_T));
            new(p)_T(_STD forward<_Args>(args)...);
            return (_T*)(p);
#endif     
            return new _T(_STD forward<_Args>(args)...);
        }

    };
    
    extern ObjectFactory* g_objectFactory;
}

#endif