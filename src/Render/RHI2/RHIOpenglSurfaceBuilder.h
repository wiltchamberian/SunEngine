#ifndef __SUN_RHI_OPENGL_SURFACE_BUILDER_H
#define __SUN_RHI_OPENGL_SURFACE_BUILDER_H

#include "RHISurfaceBuilder.h"

namespace Sun {

    namespace rhi {

        class OpenglSurface :public ISurface {
            friend class RHIVulkanSurfaceBuilder;
        public:

        protected:
            vk::SurfaceKHR surface_;
        };

        class RHIOpenglSurfaceBuilder : public ISurfaceBuilder {
        public:
            RHIPtr<ISurface> buildSurface(RHIPtr<IContext> context, GLFWwindow* glfw) override;
            void destroySurface(RHIPtr<IContext> context, RHIPtr<ISurface>) override;
        };
    }

}

#endif