#ifndef __SUN_SHADER_DLL_H
#define __SUN_SHADER_DLL_H

#define SHADER_DLL_EXPORTS 

#ifdef SHADER_DLL_EXPORTS
#define SHADER_DLL_API __declspec(dllexport)
#else
#define SHADER_DLL_API __declspec(dllimport)
#endif

#include "vulkan_core.h"
#include "Render/RHI2/RHI2Define.h"


extern "C" {
    
    SHADER_DLL_API VkAttachmentDescription createDepthAttachment();

    //获取顶点格式信息
    SHADER_DLL_API void dllGetVertexBindingDescriptions(VkVertexInputBindingDescription* data,int& size);
    SHADER_DLL_API void dllGetAttributeDescriptions(VkVertexInputAttributeDescription* data, int& size);

    //获取绘制原语拓扑  (Ci is a shortCut of CreateInfo for this context)
    SHADER_DLL_API void dllGetAssemblyStateCi(VkPipelineInputAssemblyStateCreateInfo* ci);

    //获取视窗信息
    SHADER_DLL_API void dllGetViewports(VkViewport* port ,int& siz);
    
    //获取裁剪信息
    SHADER_DLL_API void dllGetScissor(VkRect2D* data, int& siz);

    //光栅化配置
    SHADER_DLL_API void dllGetRasterizationStateCi(VkPipelineRasterizationStateCreateInfo* ci);

    //多重采样配置
    SHADER_DLL_API void dllMultisampleStateCi(VkPipelineMultisampleStateCreateInfo* ci);

    //深度和模板测试
    SHADER_DLL_API void dllGetDepthStencilStateCi(VkPipelineDepthStencilStateCreateInfo* ci);

    //颜色混合附件
    SHADER_DLL_API void dllGetColorBlendAttachmentStates(VkPipelineColorBlendAttachmentState* data , int& siz);
    SHADER_DLL_API void dllGetColorBlendStateCi(VkPipelineColorBlendStateCreateInfo* ci);

    SHADER_DLL_API void dllGetDynamicStates(VkDynamicState* states, int& siz);


}

#endif