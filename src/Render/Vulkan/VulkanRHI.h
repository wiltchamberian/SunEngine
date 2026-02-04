#ifndef __SUN_VULKAN_RHI_H
#define __SUN_VULKAN_RHI_H

#include "Render/RHI.h"
#include "Math/Vector3D.h"
#include "Math/Matrix4x4.h"
#include <array>

#define GLFW_INCLUDE_VULKAN
#include "vulkan/vulkan_core.h"

#include "nvvk/swapchain_vk.hpp"
#include "nvvk/resourceallocator_vk.hpp"
#include "VulkanInstanceBuilder.h"
#include "VulkanSurfaceBuilder.h"

namespace Sun {

namespace rhi {

    class VulkanRHI : public RHI{
    public:
        VulkanRHI();
        void initEnvironment() override;
        DevicesInfo getDevicesInfo() override;
    protected:
        // Allocator for buffer, images, acceleration structures
        nvvk::ResourceAllocatorDma alloc_;
    };

}

	

}

#endif