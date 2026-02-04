#ifndef __SUN_RENDER_BLUE_PRINT_H
#define __SUN_RENDER_BLUE_PRINT_H

#include <vector>
#include "vulkan/vulkan.hpp"

namespace Sun {

    //渲染蓝图类，这个类的设计目的如下，
    /*
    渲染时常有一些全局配置性的参数，在通常的渲染API实现中，都是hardcode，
    为了避免hardCode,可以将它们写入配置文件，但是这样又过于复杂，因此，将
    hardcode的代码统一存放在这里，并封装为接口，这部分代码可以根据实际需求
    调整，或者改为可通过控制台变量或者配置文件，数据库等配置的逻辑。
    
    
    */
    struct DepthFormatInput {
        std::vector<vk::Format> format;
        vk::ImageTiling tiling;
        vk::FormatFeatureFlags features;
    };

    class RenderBluePrint {
    public:
        static DepthFormatInput depthFormatInput;
        static vk::AttachmentDescription depthAttachment;

        static vk::AttachmentDescription colorAttachment;
    };
}

#endif