#include "SurfaceBuilder.h"

namespace Sun {

    namespace rhi {

        void SurfaceBuilder::setContext(HContext context) {
            context_ = context;
        }

        void SurfaceBuilder::setWindowHandle(HWindowHandle windowHandle) {
            windowHandle_ = windowHandle;
        }
    }
}