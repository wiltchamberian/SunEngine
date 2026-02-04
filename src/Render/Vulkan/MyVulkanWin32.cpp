#include "MyVulkanWin32.h"
//#include "windef.h"
#include <windows.h>
#include <stdexcept>
#include "vulkan/vulkan_win32.h"

namespace Sun
{
    VkSurfaceKHR createPlatformSurface(void* handle, VkInstance& vkInstance)
    {
        VkSurfaceKHR surface;
        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.hwnd = (HWND)(handle);
        surfaceCreateInfo.hinstance = ::GetModuleHandle(nullptr);

        if (vkCreateWin32SurfaceKHR(vkInstance, &surfaceCreateInfo, NULL, &surface))
        {
            throw std::runtime_error(" failed to create window surface .");
        }

        return surface;
    }
}
