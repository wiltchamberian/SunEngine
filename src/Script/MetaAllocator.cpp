#include "MetaAllocator.h"

namespace Sun {

    void MetaAllocator::destroy(MetaData* data) {
        delete data;
    }
}