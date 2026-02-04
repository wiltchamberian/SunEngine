#include "VulkanShaderMgr.h"
#include "Util/FileIO.h"
#include "Render/Vulkan/VulkanHelper.h"


namespace Sun {

namespace rhi {

    VulkanShaderMgr::~VulkanShaderMgr() {

    }

    void VulkanShaderMgr::createShaderProgram() {
        shaderModules_.clear();
        std::string key;
        VkShaderModule tmp = VK_NULL_HANDLE;
        for (int i = 0; i < ci_.infos.size(); ++i) {
            key = ci_.infos[i].name;
            auto code = FileIO::readFileBinary(ci_.dir + key);
            std::optional<VkShaderModule> rc = getResource(key);
            if (!rc.has_value()) {
                tmp = createShaderModule(vkDevice_, code);
                addResource(key,tmp);
            }
            else {
                tmp = rc.value();
            }
            HShaderModule shaderModule;
            shaderModule.name = key;
            shaderModule.mainFuncName = ci_.infos[i].mainFuncName;
            shaderModule.backend = tmp;
            shaderModule.stage = ci_.infos[i].stage;

            shaderModules_.emplace_back(shaderModule);
        }
    }

    VkShaderModule VulkanShaderMgr::createShaderModule(VkDevice vkDevice,const std::vector<char>& code) {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();

        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
        VkShaderModule shaderModule;
        if (vkCreateShaderModule(vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
        return shaderModule;
    }
}
}