#include "ShaderDll.h"
#include "Math/Vector3D.h"
#include "Math/Vector2D.h"
//#include "Render/RHI2/RenderBluePrint.h"

#define IS_RETURN(num) siz = num;\
if(data == nullptr) return;

    struct DllVertex {
        Sun::vec3 pos;
        Sun::vec3 normal;
        Sun::vec4 color;
        Sun::vec2 texture;
    };

    VkAttachmentDescription createDepthAttachment() {
        VkAttachmentDescription res = {
            {},//flags
            VkFormat::VK_FORMAT_UNDEFINED,
            VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
            VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR,
            VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
            VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        };

        return res;
    }

    void dllGetVertexBindingDescriptions(VkVertexInputBindingDescription* data,int& size) {
        if (data == nullptr) {
            size = 1;
            return;
        }
        *data = {};
        data->binding = 0; //
        data->stride = sizeof(DllVertex);  //数组中相邻元素字节间隔
        data->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return;
    }

    void dllGetAttributeDescriptions(VkVertexInputAttributeDescription* data, int& size) {
        size = 4;
        if (data == nullptr) {
            return;
        }
        data[0].binding = 0;
        data[0].location = 0;
        data[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        data[0].offset = offsetof(DllVertex, pos);

        data[1].binding = 0;
        data[1].location = 1; //对应顶点着色器的location
        data[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        data[1].offset = offsetof(DllVertex, normal);

        data[2].binding = 0;
        data[2].location = 2; //对应顶点着色器的location
        data[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        data[2].offset = offsetof(DllVertex, color);

        data[3].binding = 0;
        data[3].location = 3; //对应顶点着色器的location
        data[3].format = VK_FORMAT_R32G32_SFLOAT;
        data[3].offset = offsetof(DllVertex, texture);

        return;
    }

    void dllGetAssemblyStateCi(VkPipelineInputAssemblyStateCreateInfo* ci) {
        if (ci == nullptr) return;
        ci->sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        ci->flags = 0;

        ci->topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        ci->primitiveRestartEnable = VK_FALSE;
        return;
    }

    void dllGetViewports(VkViewport* port,int& siz) {
        siz = 1;
        if (port == nullptr) return;
        port->x = 0.0f;
        port->y = 0.0f;
        port->width = 0.0f; //not set by dll
        port->height = 0.0f; //not set by dll
        port->minDepth = 0.0f;
        port->maxDepth = 1.0f;
    }

    void dllGetScissor(VkRect2D* data, int& siz) {
        siz = 1;
        if (data == nullptr) return;
        data->offset = { 0,0 };
        data->extent = { 0,0 };//not set by dll
    }

    void dllGetRasterizationStateCi(VkPipelineRasterizationStateCreateInfo* ci) {
        *ci = {};
        ci->sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        ci->depthClampEnable = VK_FALSE;
        ci->rasterizerDiscardEnable = VK_FALSE;//开启之后将禁止光栅化
        ci->polygonMode = VK_POLYGON_MODE_FILL;//填充，描边和描点三种
        ci->lineWidth = 1.0f;//描述线宽，大于1.0的需要开启GPU的wideline特性支持
        ci->cullMode = VK_CULL_MODE_BACK_BIT;  //面剔除
        ci->frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;//VK_FRONT_FACE_COUNTER_CLOCKWISE;
        ci->depthBiasEnable = VK_FALSE; //光栅化可以通过添加常量或者基于片元的斜率来更改深度值
        ci->depthBiasConstantFactor = 0.0f; // Optional
        ci->depthBiasClamp = 0.0f; // Optional
        ci->depthBiasSlopeFactor = 0.0f; // Optional
        return;
    }

    void dllMultisampleStateCi(VkPipelineMultisampleStateCreateInfo* ci) {
        *ci = {};
        ci->sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        ci->sampleShadingEnable = VK_FALSE;
        ci->rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        ci->minSampleShading = 1.0f; // Optional
        ci->pSampleMask = nullptr; // Optional
        ci->alphaToCoverageEnable = VK_FALSE; // Optional
        ci->alphaToOneEnable = VK_FALSE; // Optional
        return;
    }

    void dllGetDepthStencilStateCi(VkPipelineDepthStencilStateCreateInfo* ci) {
        *ci = {};
        ci->sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        ci->depthTestEnable = VK_TRUE;
        ci->depthWriteEnable = VK_TRUE;
        ci->depthCompareOp = VK_COMPARE_OP_LESS;
        ci->depthBoundsTestEnable = VK_FALSE;
        ci->minDepthBounds = 0.0f; // Optional
        ci->maxDepthBounds = 1.0f; // Optional
        ci->stencilTestEnable = VK_FALSE;  //模板用
        ci->front = {}; // Optional
        ci->back = {}; // Optional
        return;
    }

    void dllGetColorBlendAttachmentStates(VkPipelineColorBlendAttachmentState* data, int& siz) {
        siz = 1;
        if (data == nullptr) return;
        *data = {};
        data->colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        data->blendEnable = VK_FALSE;
        data->srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        data->dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; 
        data->colorBlendOp = VK_BLEND_OP_ADD; 
        data->srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; 
        data->dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; 
        data->alphaBlendOp = VK_BLEND_OP_ADD; 
        return;
    }

    void dllGetColorBlendStateCi(VkPipelineColorBlendStateCreateInfo* ci) {
        *ci = {};
        ci->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        ci->logicOpEnable = VK_FALSE;
        ci->logicOp = VK_LOGIC_OP_COPY; 
        ci->attachmentCount = 0;
        ci->pAttachments = nullptr;
        ci->blendConstants[0] = 0.0f; 
        ci->blendConstants[1] = 0.0f;
        ci->blendConstants[2] = 0.0f; 
        ci->blendConstants[3] = 0.0f;
        return;
    }

    void dllGetDynamicStates(VkDynamicState* data, int& siz) {
        siz = 2;
        if (data == nullptr) return;
        data[0] = VK_DYNAMIC_STATE_VIEWPORT;
        data[1] = VK_DYNAMIC_STATE_LINE_WIDTH;
        return;
    }


