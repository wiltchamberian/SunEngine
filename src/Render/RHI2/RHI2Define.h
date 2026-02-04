#ifndef __SUN_RENDER_RHI2_DEFINE_H
#define __SUN_RENDER_RHI2_DEFINE_H

#include "vulkan/vulkan.hpp"
#include "Common/glsl.h"
#include <memory>

namespace Sun {

namespace rhi {

    template<typename _T>
    using RHIPtr = std::shared_ptr<_T>;

    //辅助函数
    template<typename _Target, typename _T>
    inline RHIPtr<_Target> typeCast(RHIPtr<_T> image) {
        return std::dynamic_pointer_cast<_Target>(image);
    }

    class IShaderModule {
    public:
        virtual ~IShaderModule();
    public:
        vk::ShaderStageFlagBits stage;
        std::string mainFuncName;
    };

    class IRenderPass {
    public:
        virtual ~IRenderPass();
    };

    class IPipelineLayout {
    public:
        virtual ~IPipelineLayout();
    };

    class IPipeline {
    public:
        virtual ~IPipeline();
    protected:
    };

    class VertexInputState {
        vk::PipelineVertexInputStateCreateInfo inputState;
        std::vector< vk::VertexInputBindingDescription > bindingDesc;
        std::vector< vk::VertexInputAttributeDescription > attributeDesc;
    };

    struct PipelineDynamicStateCreateInfo {
        std::vector<vk::DynamicState> dynamicStates;
    };

    struct PipelineInputAssemblyStateCreateInfo {
        vk::PrimitiveTopology topology;
        bool primitiveRestartEnable;
    };

    struct PipelineTessellationStateCreateInfo {
        uint32_t patchControlPoints;
        bool use  = false ;
    };

    struct PipelineViewportStateCreateInfo {
        std::vector<vk::Viewport> viewports;
        std::vector<vk::Rect2D> scissors;
    };

    struct PipelineRasterizationStateCreateInfo {
        vk::Bool32                                 depthClampEnable;
        vk::Bool32                                 rasterizerDiscardEnable;
        vk::PolygonMode                            polygonMode;
        vk::CullModeFlags                          cullMode;
        vk::FrontFace                              frontFace;
        vk::Bool32                                 depthBiasEnable;
        float                                      depthBiasConstantFactor;
        float                                      depthBiasClamp;
        float                                      depthBiasSlopeFactor;
        float                                      lineWidth;
    };

    struct PipelineMultisampleStateCreateInfo {
        vk::SampleCountFlagBits     rasterizationSamples;
        vk::Bool32                  sampleShadingEnable;
        float                       minSampleShading;
        vk::SampleMask              sampleMask;
        vk::Bool32                  alphaToCoverageEnable;
        vk::Bool32                  alphaToOneEnable;
    };

    struct PipelineDepthStencilStateCreateInfo {
        vk::Bool32                                  depthTestEnable;
        vk::Bool32                                  depthWriteEnable;
        vk::CompareOp                               depthCompareOp;
        vk::Bool32                                  depthBoundsTestEnable;
        vk::Bool32                                  stencilTestEnable;
        vk::StencilOpState                          front;
        vk::StencilOpState                          back;
        float                                       minDepthBounds;
        float                                       maxDepthBounds;
    };

    struct PipelineColorBlendStateCreateInfo {
        vk::Bool32                                          logicOpEnable;
        vk::LogicOp                                         logicOp;
        std::vector<vk::PipelineColorBlendAttachmentState>  attachments;
        float                                               blendConstants[4];
    };
    
    //图形管线创建参数集
    struct GraphicsPipelineCreateInfo {
        std::vector<IShaderModule>                  stages;
        
        VertexInputState                            vertexInputState;
        PipelineInputAssemblyStateCreateInfo        inputAssemblyState;
        PipelineTessellationStateCreateInfo         tessellationState;
        PipelineViewportStateCreateInfo             viewportState;
        PipelineRasterizationStateCreateInfo        rasterizationState;
        PipelineMultisampleStateCreateInfo          multisampleState;
        PipelineDepthStencilStateCreateInfo         depthStencilState;
        PipelineColorBlendStateCreateInfo           colorBlendState;
        PipelineDynamicStateCreateInfo              dynamicState;
        vk::PipelineLayoutCreateInfo                pipelineLayout;

        RHIPtr<IRenderPass>                         renderPass;
        uint32_t                                    subPass;
        RHIPtr<IPipeline>                           basePipelineHandle;
        int32_t                                     basePipelineIndex;
    };

    //着色器创建
    struct ShaderStageCreateInfo {
        std::string fileName; 
        vk::ShaderStageFlagBits stage;
        std::string funcName = "main";
    };

    struct ShaderProgramCreateInfo {
        std::string dir_;
        std::vector<ShaderStageCreateInfo> infos_;
        //glsl global uniform init
        std::vector<Uniform> uniformInfos_;
        std::string shaderProgramName_; //used for map key
    };
}
}

#endif 