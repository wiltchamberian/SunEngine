#ifndef __SUN_RHI_VULKAN_GRAPHICSPIPELINE_BUILDER_H
#define __SUN_RHI_VULKAN_GRAPHICSPIPELINE_BUILDER_H

#include "Render/GraphicsPipelineBuilder.h"
#include <vector>
#include "vulkan/vulkan_core.h"

namespace Sun {

namespace rhi {

    class VulkanGraphicsPipelineBuilder : public GraphicsPipelineBuilder
    {
    public:
        VulkanGraphicsPipelineBuilder();
        ~VulkanGraphicsPipelineBuilder() override;

        HGraphicsPipeline build() override;
        void destroy(HGraphicsPipeline) override;

    protected:
        std::vector<VkPipelineShaderStageCreateInfo> createVkShaderStages();

        struct VertexInputState {
            std::vector<VkVertexInputBindingDescription> bindings;
            std::vector< VkVertexInputAttributeDescription> attribs;
            VkPipelineVertexInputStateCreateInfo ci;
        };

        VertexInputState createVkVertexInputState();

        VkPipelineInputAssemblyStateCreateInfo createVkTopology();

    };

}

}

#endif