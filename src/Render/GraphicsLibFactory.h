#ifndef __GRAPHICS_LIB_FACTORY_H
#define __GRAPHICS_LIB_FACTORY_H

#include "Render/GLGraphicsLib.h"
#include "Render/GraphicsEnums.h"
#include "Render/GraphicsProxy.h"

namespace Sun {

    class GraphicsLibFactory {
    public:
        static GraphicsLib* createGraphicsLib(const GRAPHICS_API_TYPE& type);
        static GraphicsProxy* createGraphicsProxy(const GRAPHICS_API_TYPE& type);
    };


}

#endif