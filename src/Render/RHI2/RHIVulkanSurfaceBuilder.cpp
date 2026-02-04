#include "RHIVulkanSurfaceBuilder.h"

#include "RHI2_Vulkan.h"

namespace Sun {

    namespace rhi {

        RHIPtr<ISurface> RHIVulkanSurfaceBuilder::buildSurface(RHIPtr<IContext> context, GLFWwindow* glfw) {
            vk::SurfaceKHR     surface;
            VkSurfaceKHR _surface;

            RHIPtr<VulkanContext> ptr = std::dynamic_pointer_cast<VulkanContext>(context);
            glfwCreateWindowSurface(static_cast<VkInstance>(ptr->instance_), glfw, nullptr, &_surface);
            surface = vk::SurfaceKHR(_surface);

            RHIPtr<VulkanSurface> res(new VulkanSurface);
            res->surface_ = surface;

            return res;
        }

        void RHIVulkanSurfaceBuilder::destroySurface(RHIPtr<IContext> context,RHIPtr<ISurface> surface) {
            RHIPtr<VulkanSurface> ptr = std::dynamic_pointer_cast<VulkanSurface>(surface);
            RHIPtr<VulkanContext> ct = std::dynamic_pointer_cast<VulkanContext>(context);
            ct->instance_.destroySurfaceKHR(ptr->surface_);
            return;
        }

    }
}