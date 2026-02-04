#ifndef __SUN_META_ALLOCATOR_H
#define __SUN_META_ALLOCATOR_H

#include "MetaData.h"

namespace Sun {

    class MetaAllocator {
    public:
        template<typename ... _T>
        MetaData* alloc(_T&&... input) {
            return new MetaData(std::forward<_T>(input)...);
        }

        void destroy(MetaData* data);
    };

}

#endif