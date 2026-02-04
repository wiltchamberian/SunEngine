#include "RHIOpenglSurfaceBuilder.h"

#include "RHI2_Opengl.h"

namespace Sun {

    namespace rhi {

        RHIPtr<ISurface> RHIOpenglSurfaceBuilder::buildSurface(RHIPtr<IContext> context, GLFWwindow* glfw) {
            return nullptr;
        }

        void RHIOpenglSurfaceBuilder::destroySurface(RHIPtr<IContext> context, RHIPtr<ISurface> surface) {
            return;
        }

    }
}