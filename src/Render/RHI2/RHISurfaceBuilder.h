#ifndef __RHI_SURFACE_BUILDER_H
#define __RHI_SURFACE_BUILDER_H

#include "vulkan/vulkan.hpp"
#include "glfw/glfw3.h"
#include "RHI2.h"

namespace Sun {

    namespace rhi {

        class ISurfaceBuilder {
        public:
            virtual RHIPtr<ISurface> buildSurface(RHIPtr<IContext> context, GLFWwindow* glfw) = 0;
            virtual void destroySurface(RHIPtr<IContext> context,RHIPtr<ISurface>) = 0;
        };
        
    }
}

#endif