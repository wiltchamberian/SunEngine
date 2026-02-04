#ifndef __SUN_VULKAN_SURFACE_BUILDER_H
#define __SUN_VULKAN_SURFACE_BUILDER_H

#include "vulkan/vulkan_core.h"
#include "Render/RHIDefine.h"
#include "Render/SurfaceBuilder.h"
#ifdef _WIN32
#include <windows.h>
#include "vulkan/vulkan_win32.h"
#endif

namespace Sun {
    namespace rhi {

        class VulkanSurfaceBuilder : public SurfaceBuilder
        {
        public:
            VulkanSurfaceBuilder();
            HSurface build() override;
            void destroy(HSurface surface) override;

        private:
        };

    }


}

#endif