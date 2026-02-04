#ifndef __SUN_RHI_VULKAN_SHADER_BUILDER_H
#define __SUN_RHI_VULKAN_SHADER_BUILDER_H

#include "Render/ShaderMgr.h"
#include "vulkan/vulkan_core.h"

namespace Sun {

namespace rhi {

    struct VkShaderModuleDeleter {
        void operator()(VkShaderModule handle) {
            vkDestroyShaderModule(device,handle ,nullptr);
        }
        VkDevice device;
    };

    class VulkanShaderMgr : public ShaderMgr, public TResourceMgr<std::string, VkShaderModule, VkShaderModuleDeleter>
    {
    public:
        ~VulkanShaderMgr() override;
        void createShaderProgram() override;
        
        static VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);

    protected:
        VkDevice vkDevice_ = VK_NULL_HANDLE;
        
    };
}

}

#endif