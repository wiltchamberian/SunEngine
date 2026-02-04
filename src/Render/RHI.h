/*****************************************************************************
* @brief : RHI
* Render Hardware Interface
* 渲染硬件接口，统一封装了Vulkan,Opengl（后续可能会加入DirectX3D)为通用接口
* 应用层无需关心渲染底层API,
* opengl,vulkan等渲染API都非常复杂，细节繁多，该RHI目的在于通用性，因此
* 大幅度简化了接口使用，屏蔽掉冗长的细节，易于使用
* (举例来说：opengl绘制三角形需要300余行代码，Vulkan需要1000余行代码，使用该接口,
* 只需10行不到..)
* 该接口经过精心设计，并且有详细注释，可供外部项目使用
* 同时，由于Vulkan和opengl的差异性，RHI是一个覆盖了两端的接口集
* 但是对于特定平台，可能有些接口无法使用，举例来说，某个接口可能只有Vulkan下才
* 可以调用，对于这些接口，都有注释详细说明；如没有说明了，则默认皆可调用;
* 如果调用了不能使用的接口，则不会执行任何操作直接返回
* 
* RHI的调用目前可能做不到完全随意，在demo中详细的调用流程，建议读者按照流程配置
* 如果不按流程执行函数，可能会有异常行为
* @author : acedtang
* @date : 2021/9/29
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/


#ifndef __SUN_RHI_H
#define __SUN_RHI_H

#include <vector>
#include <memory>
#include "GraphicsPipelineBuilder.h"
#include "TextureBuilder.h"
#include "RHIDefine.h"
#include "ShaderModule.h"
#include "ContextBuilder.h"

#include "Vulkan/vulkan.hpp"

namespace Sun {

namespace rhi {

    class SurfaceBuilder;

	class RHI {
	public:
		/*
		* 初始化环境
		* 包括实例或者context构建,创建设备，初始化内存管理系统等
		* 交换链创建
        * 执行RHI的任何操作前必须先initEnvironment，未提高性能，没有加校验
        * 所以，如未执行initEnvironment，行为未定义!
		*/
		virtual void initEnvironment() = 0;

        /*
        * 获取设备信息
        */
        virtual DevicesInfo getDevicesInfo() = 0;


		/*
		* 获取图形管线构造器
		*/
        SmartPtr<GraphicsPipelineBuilder> getGraphicsPipelineBuilder();

        /*
        *  获取纹理构造器
        */
        SmartPtr<TextureBuilder> getTextureBuilder();

        /*
        * 获取Context构造器
        * opengl:context
        * vulkan:instance
        */
        SmartPtr<ContextBuilder> getContextBuilder();

        /*
        * 获取着色器管理器
        */
        SmartPtr<ShaderMgr> getShaderMgr();

        /*
        * 获取Surface构造器
        * opengl:window
        * vulkan:vkSurface
        */
        SmartPtr<SurfaceBuilder> getSurfaceBuilder();

        /*
        * 绑定图形管线
        * opengl : glUseProgram 
        * vulkan:vkCmdBindPipeline
        */
        virtual EResult bindPipeline(HGraphicsPipeline pipeline) = 0;

		/*
		* 向GPU申请buffer;这块buffer未必是一块独立的内存，有可能置于
		* 连续的内存区域上，这一块的复杂逻辑RHI做了封装，简化了用户使用
		*/
		virtual HBuffer applyForBuffer(const char* data, int siz) = 0;

        /*
        * 更新buffer
        */
        virtual void updateBuffer(HBuffer , const char* data,int siz, int offset) = 0;
		
		/* 绑定单个顶点缓冲区buf*/
		virtual void bindVertexBuffer(HBuffer hbuf) = 0;

		/* 绑定多个缓冲区*/
		//Vulkan Only!
		virtual void bindVertexBuffers(HBuffer hbuf ,
			uint32_t firstBinding,
			uint32_t bindingCount,
			const HBuffer* pBuffers,
			const DeviceSize* pOffsets) = 0;

		/*
		* 绑定索引缓冲区
		*/
		virtual void bindIndexBuffer(HBuffer hbuf , IndexType type) = 0;

		/*
		* 绑定索引缓冲区
		* Vulkan Only!
		*/
		virtual void bindIndexBuffer(
			HBuffer                             hbuf,
			DeviceSize							offset, 
			IndexType							indexType) = 0;

		/*
		* 执行底层图形API的drawCall
		*/
		virtual void drawCall(HDrawCall drawCall = HDrawCall()) = 0;

        void test();
	protected:
        SmartPtr<GraphicsPipelineBuilder>   graphicsPipelineBuilder_;
        SmartPtr<TextureBuilder>            textureBuilder_;
        SmartPtr<ContextBuilder>            contextBuilder_;
        SmartPtr<SurfaceBuilder>            surfaceBuilder_;
        SmartPtr<ShaderMgr>                 shaderMgr_;
	};

}
	
}

#endif