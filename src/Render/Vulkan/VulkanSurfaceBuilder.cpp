#include "VulkanSurfaceBuilder.h"
#include <stdexcept>
#include <cassert>
#include "Common/Lib.h"

namespace Sun {

    namespace rhi {
        VulkanSurfaceBuilder::VulkanSurfaceBuilder()
        {
        }

        void VulkanSurfaceBuilder::destroy(HSurface surface) {
            VkSurfaceKHR h = (VkSurfaceKHR)(surface.backend);
            VkInstance instance = (VkInstance)(context_.backend);
            if (h == VK_NULL_HANDLE || instance == VK_NULL_HANDLE) {
                Lib::error("Sun destroy null handle");
            }
            vkDestroySurfaceKHR(instance, h, nullptr);
        }

        HSurface VulkanSurfaceBuilder::build() {
            VkInstance vk = VkInstance(context_.backend);
            if (vk == nullptr) {
                assert(false);
                return HSurface();
            }

            HSurface sf;
            VkSurfaceKHR surface = VK_NULL_HANDLE;
              
#ifdef _WIN32
            VkWin32SurfaceCreateInfoKHR ci;
            ci.flags = 0;
            ci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            ci.hwnd = (HWND)windowHandle_.backend;
            ci.pNext = nullptr;
            ci.hinstance = ::GetModuleHandle(nullptr);
            if (VK_SUCCESS != vkCreateWin32SurfaceKHR(vk, &ci, NULL, &surface))
            {
                throw std::runtime_error(" failed to create window surface .");
            }
#endif
            sf.backend = (uint64_t)surface;
            return sf;
        }

    }
}