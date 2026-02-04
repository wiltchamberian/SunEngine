#ifndef __SUN_RHI_VULKAN_HELPER_H
#define __SUN_RHI_VULKAN_HELPER_H

//#include "Common/BasicDataType.h"
#include "Render/RHIDefine.h"
#include "vulkan/vulkan_core.h"
#include <cassert>

namespace Sun {

namespace rhi {

#define SWITCH_BEGIN(s) switch(s){

#define SWITCH_CASE(a,b) case a:{\
        return b;\
    }\
    break;

#define SWICH_DEFAULT default:{\
        assert(false); \
    }\
    break;

#define SWITCH_END }

    VkShaderStageFlagBits vulkan_enum_map(EShaderStageFlagBits type) {
        SWITCH_BEGIN(type)
        SWITCH_CASE(EShaderStageFlagBits::VERTEX_BIT, VK_SHADER_STAGE_VERTEX_BIT);
        SWITCH_CASE(EShaderStageFlagBits::FRAGMENT_BIT, VK_SHADER_STAGE_FRAGMENT_BIT);
        SWICH_DEFAULT
        SWITCH_END
        return VK_SHADER_STAGE_VERTEX_BIT;
    }

    inline VkFormat typeCast(EFormat format) {
        return VkFormat(format);
    }

    inline VkVertexInputRate typeCast(EVertexInputRate rate) {
        return VkVertexInputRate(rate);
    }

    inline VkPrimitiveTopology typeCast(EPrimitiveTopology topo) {
        return VkPrimitiveTopology(topo);
    }

    inline VkSampleCountFlagBits typeCast(ESampleCountFlagBits bits) {
        return VkSampleCountFlagBits(bits);
    }

    inline VkCompareOp typeCast(ECompareOp op) {
        return VkCompareOp(op);
    }

    inline VkStencilOp typeCast(EStencilOp op) {
        return VkStencilOp(op);
    }

    inline VkBlendOp typeCast(EBlendOp op) {
        return VkBlendOp(op);
    }

    inline VkColorComponentFlags typeCast(ColorComponentFlags flags) {
        return VkColorComponentFlags(flags);
    }

    inline VkBlendFactor typeCast(EBlendFactor factor) {
        return VkBlendFactor(factor);
    }

    inline VkLogicOp typeCast(ELogicOp op) {
        return VkLogicOp(op);
    }

    inline VkStencilOpState typeCast(const StencilOpState& state) {
        VkStencilOpState res;
        res.compareMask = state.compareMask;
        res.compareOp = typeCast(state.compareOp);
        res.depthFailOp = typeCast(state.depthFailOp);
        res.failOp = typeCast(state.failOp);
        res.passOp = typeCast(state.passOp);
        res.reference = state.reference;
        res.writeMask = state.writeMask;
        return res;
    }

    /// /////////////////////////////////////////////////////////////////////////

    inline VkVertexInputBindingDescription typeCast(const VertexInputBindingDescription& des) {
        VkVertexInputBindingDescription res;
        res.binding = des.binding;
        res.inputRate = typeCast(des.inputRate);
        res.stride = des.stride;
        return res;
    }

    inline VkVertexInputAttributeDescription typeCast(const VertexInputAttributeDescription& attrib) {
        VkVertexInputAttributeDescription res;
        res.binding = attrib.binding;
        res.format = typeCast(attrib.format);
        res.location = attrib.location;
        res.offset = attrib.offset;
        return res;
    }

    inline VkPipelineInputAssemblyStateCreateInfo typeCast(const DrawTopology& topo) {
        VkPipelineInputAssemblyStateCreateInfo res;
        res.pNext = nullptr;
        res.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        res.primitiveRestartEnable = topo.primitiveRestartEnable;
        res.topology = typeCast(topo.topo);
        res.flags = 0;
        return res;
    }

    inline VkPipelineViewportStateCreateInfo typeCast(const ViewportState& viewportState) {
        VkPipelineViewportStateCreateInfo res;
        res.flags = 0;
        res.pNext = nullptr;
        res.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        res.pViewports = (VkViewport*)viewportState.viewports.data();
        res.viewportCount = viewportState.viewports.size();
        res.pScissors = (VkRect2D*)viewportState.scissors.data();
        res.scissorCount = viewportState.scissors.size();
        return res;
    }

    inline VkPipelineRasterizationStateCreateInfo typeCast(const RasterizationState& state) {
        VkPipelineRasterizationStateCreateInfo res;
        res.flags = 0;
        res.cullMode = state.cullMode;
        res.depthBiasClamp = state.depthBiasClamp;
        res.depthBiasConstantFactor = state.depthBiasConstantFactor;
        res.depthBiasEnable = state.depthBiasEnable;
        res.depthBiasSlopeFactor = state.depthBiasSlopeFactor;
        res.depthClampEnable = state.depthClampEnable;
        res.frontFace = (VkFrontFace)state.frontFace;
        res.lineWidth = state.lineWidth;
        return res;
    }

    inline VkPipelineMultisampleStateCreateInfo typeCast(const MultiSampleState& state) {
        VkPipelineMultisampleStateCreateInfo res;
        res.flags = 0;
        res.pNext = nullptr;
        res.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        res.alphaToCoverageEnable = state.alphaToCoverageEnable;
        res.alphaToOneEnable = state.alphaToOneEnable;
        res.minSampleShading = state.minSampleShading;
        res.pSampleMask = &state.SampleMask;  //TODO ,make sure!
        res.rasterizationSamples = typeCast(state.rasterizationSamples);
        res.sampleShadingEnable = state.sampleShadingEnable;
        return res;
    }

    inline VkPipelineDepthStencilStateCreateInfo typeCast(const PipelineDepthStencilState& state) {
        VkPipelineDepthStencilStateCreateInfo res;
        res.flags = 0;
        res.pNext = nullptr;
        res.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

        res.front = typeCast(state.front);
        res.back = typeCast(state.back);
        res.depthBoundsTestEnable = state.depthBoundsTestEnable;
        res.maxDepthBounds = state.maxDepthBounds;
        res.minDepthBounds = state.minDepthBounds;
        res.depthCompareOp = typeCast(state.depthCompareOp);
        res.depthTestEnable = state.depthTestEnable;
        res.depthWriteEnable = state.depthWriteEnable;
        res.stencilTestEnable = state.stencilTestEnable;
        
        return res;
    }

    inline VkPipelineColorBlendAttachmentState typeCast(const PipelineColorBlendAttachmentState& st) {
        VkPipelineColorBlendAttachmentState res;
        res.alphaBlendOp = typeCast(st.alphaBlendOp);
        res.blendEnable = st.blendEnable;
        res.colorBlendOp = typeCast(st.colorBlendOp);
        res.colorWriteMask = typeCast(st.colorWriteMask);
        res.dstAlphaBlendFactor = typeCast(st.dstAlphaBlendFactor);
        res.dstColorBlendFactor = typeCast(st.dstColorBlendFactor);
        res.srcAlphaBlendFactor = typeCast(st.srcAlphaBlendFactor);
        res.srcColorBlendFactor = typeCast(st.srcColorBlendFactor);
        return res;
    }

    inline VkPipelineColorBlendStateCreateInfo typeCast(const PipelineColorBlendState& st) {
        VkPipelineColorBlendStateCreateInfo res;
        res.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        res.flags = 0;
        res.attachmentCount = st.attachments.size();
        res.pAttachments = (const VkPipelineColorBlendAttachmentState*)st.attachments.data();
        res.pNext = nullptr;
        memcpy(res.blendConstants, st.blendConstants ,4 * sizeof(float));
        res.logicOp = typeCast(st.logicOp);
        res.logicOpEnable = st.logicOpEnable;
        return res;
    }

    inline VkPipelineDynamicStateCreateInfo typeCast(const DynamicStates& st) {
        VkPipelineDynamicStateCreateInfo res;
        res.flags = 0;
        res.pNext = nullptr;
        res.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        res.dynamicStateCount = st.dynamicStates.size();
        res.pDynamicStates = (VkDynamicState*)st.dynamicStates.data();
        return res;
    }

    inline VkPipelineLayoutCreateInfo typeCast(const PipelineLayout& layout) {
        VkPipelineLayoutCreateInfo res;
        res.flags = 0;
        res.pNext = nullptr;
        res.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        res.pPushConstantRanges = (VkPushConstantRange*)layout.pushConstantRanges.data();
        res.pushConstantRangeCount = layout.pushConstantRanges.size();
        static_assert(sizeof(VkDescriptorSetLayout) == sizeof(HDescriptorSetLayout));
        res.pSetLayouts = (VkDescriptorSetLayout*)layout.setLayouts.data();
        res.setLayoutCount = layout.setLayouts.size();
        return res;
    }
}

}

#endif