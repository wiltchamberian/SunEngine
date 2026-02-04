#ifndef __SUN_RHI_GRAPHICS_PIPELINE_BUILDER_H
#define __SUN_RHI_GRAPHICS_PIPELINE_BUILDER_H

#include "RHIDefine.h"
#include "ShaderMgr.h"
#include <string>
#include <memory>

namespace Sun {

namespace rhi {

	class GraphicsPipelineBuilder {
	public:
        GraphicsPipelineBuilder() {}

        virtual ~GraphicsPipelineBuilder() = 0;
		virtual HGraphicsPipeline build() = 0;
        virtual void destroy(HGraphicsPipeline) = 0;

        void setDevice(HDevice device) {
            device_ = device;
        }

        GraphicsPipelineBuilder& setShaderModules(std::vector<HShaderModule>& shaderModules) {
            shaders_ = shaderModules;
            return *this;
        }

        GraphicsPipelineBuilder& setVertexLayout(const VertexLayout& layout) {
            vertexLayout_ = layout;
            return *this;
        }

        GraphicsPipelineBuilder& setDrawTopology(const DrawTopology& topo) {
            drawTopology_ = topo;
            return *this;
        }
        GraphicsPipelineBuilder& setViewportState(const ViewportState& state) {
            viewportState_ = state;
            return *this;
        }

        GraphicsPipelineBuilder& setRasterizationState(const RasterizationState& state) {
            rasterizationState_ = state;
            return *this;
        }

        GraphicsPipelineBuilder& setMultiSampleState(const MultiSampleState& state) {
            multiSampleState_ = state;
            return *this;
        }

        GraphicsPipelineBuilder& setDepthStencilState(const PipelineDepthStencilState& state) {
            depthStencilState_ = state;
            return *this;
        }

        GraphicsPipelineBuilder& setColorBlendState(const PipelineColorBlendState& state) {
            colorBlendState_ = state;
            return *this;
        }

        GraphicsPipelineBuilder& setDynamicStates(const DynamicStates& state) {
            dynamicStates_ = state;
            return *this;
        }

        GraphicsPipelineBuilder& setPipelineLayout(const HPipelineLayout& layout) {
            hPipelineLayout_ = layout;
            return *this;
        }

        GraphicsPipelineBuilder& setRenderPass(HRenderPass pass) {
            hRenderPass_ = pass;
            return *this;
        }

    protected:
        
        HDevice                     device_;

        std::vector<HShaderModule>  shaders_;
        VertexLayout                vertexLayout_;
        DrawTopology                drawTopology_;
        ViewportState               viewportState_;
        RasterizationState          rasterizationState_;
        MultiSampleState            multiSampleState_;
        PipelineDepthStencilState   depthStencilState_;
        PipelineColorBlendState     colorBlendState_;
        DynamicStates               dynamicStates_;
        HPipelineLayout             hPipelineLayout_;
        HRenderPass                 hRenderPass_;
        uint32_t                    subPass_;
	};

}

}


#endif