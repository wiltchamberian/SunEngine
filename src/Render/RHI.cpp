#include "RHI.h"

#include "ContextBuilder.h"
#include "SurfaceBuilder.h"

namespace Sun {
namespace rhi {

	std::shared_ptr<GraphicsPipelineBuilder> RHI::getGraphicsPipelineBuilder() {
		return graphicsPipelineBuilder_;
	}

    std::shared_ptr<TextureBuilder> RHI::getTextureBuilder() {
        return textureBuilder_;
    }

    SmartPtr<ContextBuilder> RHI::getContextBuilder() {
        return contextBuilder_;
    }

    SmartPtr<ShaderMgr> RHI::getShaderMgr() {
        return shaderMgr_;
    }

    SmartPtr<SurfaceBuilder> RHI::getSurfaceBuilder() {
        return surfaceBuilder_;
    }

    void RHI::test() {

        // initialize the vk::ApplicationInfo structure
        vk::ApplicationInfo applicationInfo("app", 1, "engine", 1, VK_API_VERSION_1_1);

        // initialize the vk::InstanceCreateInfo
        vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo);

        // create an Instance
        vk::Instance instance = vk::createInstance(instanceCreateInfo);

        // destroy it again
        instance.destroy();



        /* VULKAN_HPP_KEY_END */

        return ;
    }
}
	

}