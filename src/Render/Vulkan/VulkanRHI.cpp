#include "VulkanRHI.h"
#include "VulkanGraphicsPipelineBuilder.h"

namespace Sun {

namespace rhi {

    VulkanRHI::VulkanRHI() {
        contextBuilder_.reset(new VulkanInstanceBuilder());
        surfaceBuilder_.reset(new VulkanSurfaceBuilder());
        textureBuilder_.reset(new TextureBuilder());
        graphicsPipelineBuilder_.reset(new VulkanGraphicsPipelineBuilder());
    }

    void VulkanRHI::initEnvironment() {

    }

    DevicesInfo VulkanRHI::getDevicesInfo() {
        return {};
    }

}
    
}