#include "GraphicsLibFactory.h"
#include "Util/exception.h"
#include "Render/GLGraphicsProxy.h"

namespace Sun {

    GraphicsLib* GraphicsLibFactory::createGraphicsLib(const GRAPHICS_API_TYPE& type) {
        GraphicsLib* res = nullptr;
        switch (type) {
            case GRAPHICS_API_TYPE::G_OPENGL: 
            {
                res = new GLGraphicsLib();
            }
            break;
            default:
            {
                ASSERT(false);
                break;
            }
        }
        return res;
    }

    GraphicsProxy* GraphicsLibFactory::createGraphicsProxy(const GRAPHICS_API_TYPE& type) {
        GraphicsProxy* res = nullptr;
        switch (type) {
        case GRAPHICS_API_TYPE::G_OPENGL:
        {
            //res = new GLGraphicsProxy();
        }
        break;
        default:
        {
            ASSERT(false);
            break;
        }
        }
        return res;
    }

}