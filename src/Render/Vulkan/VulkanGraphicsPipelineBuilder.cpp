#include "VulkanGraphicsPipelineBuilder.h"
#include "vulkan/vulkan_core.h"
#include "Util/FileIO.h"
#include "VulkanHelper.h"

namespace Sun {

namespace rhi {

    VulkanGraphicsPipelineBuilder::VulkanGraphicsPipelineBuilder() {

    }

    VulkanGraphicsPipelineBuilder::~VulkanGraphicsPipelineBuilder() {

    }

    HGraphicsPipeline VulkanGraphicsPipelineBuilder::build() {
        HGraphicsPipeline graphicsPipeline;
        VkPipeline vkGraphicsPipeline = VK_NULL_HANDLE;

        //配置着色器
        std::vector<VkPipelineShaderStageCreateInfo> shaderStages = createVkShaderStages();

        //配置顶点格式
        VertexInputState state = createVkVertexInputState();
        VkPipelineVertexInputStateCreateInfo vertexFormat = state.ci;

        //管线布局
        VkPipelineLayout vkPipelineLayout = VkPipelineLayout(hPipelineLayout_.backend);

        //TODO, 以上三个可能会合并为一个,都属于着色器程序配置

        //配置绘制拓扑
        VkPipelineInputAssemblyStateCreateInfo topoCi = typeCast(drawTopology_);

        //视窗配置
        VkPipelineViewportStateCreateInfo viewportCi = typeCast(viewportState_);
        
        //光栅化配置
        VkPipelineRasterizationStateCreateInfo rasterCi = typeCast(rasterizationState_);

        //多重采样配置
        VkPipelineMultisampleStateCreateInfo multiSampleCi = typeCast(multiSampleState_);

        //深度和模板测试
        VkPipelineDepthStencilStateCreateInfo depthStencilCi = typeCast(depthStencilState_);

        //颜色混合 color blend
        VkPipelineColorBlendStateCreateInfo colorBlendCi = typeCast(colorBlendState_);

        //动态状态（可以运行时修改)
        VkPipelineDynamicStateCreateInfo dynamicStateCi = typeCast(dynamicStates_);

        //管线缓存,hard code
        VkPipelineCache pipelineCache;
        VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
        pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        if (vkCreatePipelineCache((VkDevice)(device_.backend), &pipelineCacheCreateInfo, NULL, &pipelineCache) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        //renderPass
        static_assert(sizeof(VkRenderPass) == sizeof(HRenderPass),"this assert must pass!");
        VkRenderPass pass = (VkRenderPass)hRenderPass_.backend;

        //创建图形管线
        VkGraphicsPipelineCreateInfo pipelineCi = {};
        pipelineCi.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCi.stageCount = shaderStages.size();
        pipelineCi.pStages = shaderStages.data();

        pipelineCi.pVertexInputState = &vertexFormat;
        pipelineCi.pInputAssemblyState = &topoCi;
    
        pipelineCi.pViewportState = &viewportCi;
  
        pipelineCi.pRasterizationState = &rasterCi;
        pipelineCi.pMultisampleState = &multiSampleCi;
        pipelineCi.pDepthStencilState = &depthStencilCi;
        pipelineCi.pColorBlendState = &colorBlendCi;
        pipelineCi.pDynamicState = &dynamicStateCi;
        pipelineCi.pTessellationState = nullptr; //TODO FIXME!
        pipelineCi.layout = vkPipelineLayout;
        pipelineCi.renderPass = pass;
        pipelineCi.subpass = subPass_;
        pipelineCi.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineCi.basePipelineIndex = -1; // Optional

        if (vkCreateGraphicsPipelines((VkDevice)(device_.backend), pipelineCache, 1, &pipelineCi, nullptr, &vkGraphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        graphicsPipeline.backend = (uint64_t)(vkGraphicsPipeline);

        return graphicsPipeline;
    }

    void VulkanGraphicsPipelineBuilder::destroy(HGraphicsPipeline pipeline) {
        vkDestroyPipeline(VkDevice(device_.backend), (VkPipeline)pipeline.backend, nullptr);
    }

    std::vector<VkPipelineShaderStageCreateInfo> VulkanGraphicsPipelineBuilder::createVkShaderStages() {
        std::vector<VkPipelineShaderStageCreateInfo> res;
        for (auto& it : shaders_) {
            VkPipelineShaderStageCreateInfo ci;
            ci.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            ci.pNext = nullptr;
            ci.flags = 0;

            ci.module = (VkShaderModule)it.backend;
            ci.stage = (VkShaderStageFlagBits)it.stage;
            ci.pSpecializationInfo = nullptr; //TODO: unknow
            ci.pName = it.mainFuncName.c_str();

            res.emplace_back(ci);
        }
        return res;
    }

    VulkanGraphicsPipelineBuilder::VertexInputState VulkanGraphicsPipelineBuilder::createVkVertexInputState() {
        VertexInputState output;
        for (auto& it : vertexLayout_.bindings) {
            output.bindings.emplace_back(typeCast(it));
        }
        std::vector<VkVertexInputAttributeDescription> attribs;
        for (auto& it : vertexLayout_.attribs) {
            output.attribs.emplace_back(typeCast(it));
        }
        VkPipelineVertexInputStateCreateInfo ci;
        ci.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        ci.pNext = nullptr;
        ci.flags = 0;
        ci.pVertexAttributeDescriptions = output.attribs.data();
        ci.vertexAttributeDescriptionCount = output.attribs.size();
        ci.pVertexBindingDescriptions = output.bindings.data();
        ci.vertexBindingDescriptionCount = output.bindings.size();
        output.ci = ci;
        
        return output;
    }

    VkPipelineInputAssemblyStateCreateInfo VulkanGraphicsPipelineBuilder::createVkTopology() {
        VkPipelineInputAssemblyStateCreateInfo ci;
        ci.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        ci.pNext = nullptr;
        ci.flags = 0;
        
        ci.primitiveRestartEnable = drawTopology_.primitiveRestartEnable;
        ci.topology = typeCast(drawTopology_.topo);
        
        return ci;
    }

}

}