#ifndef __SUN_RHI_SURFACE_BUILDER_H
#define __SUN_RHI_SURFACE_BUILDER_H

#include "RHIDefine.h"

namespace Sun {

namespace rhi {

    class SurfaceBuilder {
    public:
        virtual HSurface build() = 0;
        virtual void destroy(HSurface surface) = 0;

        void setContext(HContext context);
        void setWindowHandle(HWindowHandle windowHandle);

    protected:
        HContext        context_;
        HWindowHandle   windowHandle_;

    };

}
}

#endif