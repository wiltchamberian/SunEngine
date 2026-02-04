#ifndef __SUN_VULKAN_INSTANCE_H
#define __SUN_VULKAN_INSTANCE_H

#include "vulkan/vulkan_core.h"
#include "Render/ContextBuilder.h"
#include <vector>
#include <string>

namespace Sun {

namespace rhi {
    class VulkanInstanceBuilder : public ContextBuilder
    {
    public:
        VulkanInstanceBuilder();
        HContext build() override;
        void destroy(HContext) override;
    protected:
    };


}
	

}

#endif